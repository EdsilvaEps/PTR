#include "simulation.h"
#include <stdio.h>
#include <math.h>

/* calcula a integral utilizando o método trapezoidal
   com iterações continuas.

   f -> função de xdot
   t -> argumento atual
   prev_t -> argumento anterior
 */
double integral(double (*f)(double), double t, double prev_t){


  return ((f(t) + f(prev_t))*(t-prev_t))/2;
}



/*
   processa a resposta de velocidade do robô de acordo com a
   variável de tempo fornecida. Retorna uma matriz do formato
   u(t) = [v w]^T (transposta de [v w]) em que v é a velocidade linear
   e w a velocidade angular.
*/
Matrix getSpeed(double t){
  Matrix speed = matrix_zeros("speed", 2,1);

  if(t >= 0 && t < 10){

    speed.values[0][0] = 1;
    speed.values[1][0] = 0.2*PI;
    return speed;
  }

  else if(t >= 10) {

    speed.values[0][0] = 1;
    speed.values[1][0] = -0.2*PI;
    return speed;
  }

  else{

    return speed;

  }

}

/*
  computa a matrix x(t) descrita pelo formato
  x(t) = [Xc, Yc, Theta]^t (transposta da matriz) onde
  (Xc, Yc) são as coordenadas cartesianas para a posição
  do centro de massa do robô e Theta é seu ângulo de orientação.

  As integrais servem para calcular os valores de x(t) a
  partir do espaço de estados (xponto(t)) fornecido  pelo enunciado
  do trabalho de laboratório 2.
*/
Matrix getPosition(double t, double prev_t){
  Matrix pos = matrix_zeros("position", 3, 2);

  pos.values[0][0] = integral(sin, t , prev_t);
  pos.values[1][0] = integral(cos, t , prev_t);
  pos.values[2][1] = 1;

  return pos;

}

/*
  computa a saída do sistema fazendo a multiplicacao
  matricial y(t) = MatrizId*(X(t)*U(t)).

  A saída é no formato y(t) = [Xc Yc Theta]^T (transposta da matriz)
  onde (Xc, Yc) são as coordenadas cartesianas para a posição
  do centro de massa do robô e Theta é seu ângulo de orientação.

*/
Matrix simulate(double t, Matrix u){

  double curr_angle = u.values[1][1];
  Matrix pos;

  if(t == 10){
    pos = getPosition(curr_angle, curr_angle*(-1));
  } else pos = getPosition(curr_angle, curr_angle);

  Matrix id = matrix_identity("id", 3, 3);

  Matrix res = matrix_mult(id, matrix_mult(pos, u));

  return res;

}

/*
  computa parcialmente a função do ponto da frente do robô
  dado pela função Yf(t) = X(t) + point*X(t)
*/
Matrix simulate2(double t, double d, Matrix u){

  Matrix point = matrix_zeros("point", 3, 3);
  point.values[0][0] = 0.5*d*cos(t);
  point.values[1][1] = 0.5*d*sin(t);
  point.values[2][2] = 1;

  Matrix pos = getPosition(t, t-1);
  Matrix res = matrix_mult(point, matrix_mult(pos, u));

  return res;

}


//---------------- FUNÇÕES LAB 6---------------


/*
  getSimXdot gera a matriz :
    | cos(x3)   0 |
    | sin(x3)   0 | u(t)
    |   0       1 |

  onde consideramos x3 = w*t (velocidade angular * tempo)
*/

Matrix getSimX(Matrix u, double time){

  Matrix x = matrix_zeros("x", 3, 1);

  double v = u.values[0][0]; // velocidade linear
  double w = u.values[1][0]; // velocidade angular

  x.values[0][0] = v * sin(w*time);
  x.values[1][0] = v * sin(w*time);
  x.values[2][0] = w * time;

  return x;

}


Matrix getSimY(Matrix x, double radius){

  Matrix aux = matrix_zeros("aux", 2, 1);

  if(x.m == 3 && x.n == 1){

    double x3 = x.values[3][0];

    /*
      | 1 0 0 |
      | 0 1 0 |
    */
    Matrix y = matrix_identity("y", 2, 3);
    aux.values[0][0] = radius*cos(x3);
    aux.values[1][0] = radius*sin(x3);

    y = matrix_mult(y, x);
    y = matrix_add(y, aux);

    return y;

  } else return aux;

}

/*

  getSimYdot gera a matrix:
    | cos(wt)   -Rsin(wt) |   | u1 |
    | sin(wt)   Rcos(wt)  | x | u2 |

    onde R é o raio da frente do robô

*/

Matrix getSimYdot(Matrix u, Matrix x, double radius){

  Matrix ydot = matrix_zeros("ydot", 2 , 2);
  double x3 = x.values[3][0];


  ydot.values[0][0] = cos(x3);
  ydot.values[1][0] = sin(x3);
  ydot.values[0][1] = (-1)*radius*sin(x3);
  ydot.values[1][1] =  radius*cos(x3);

  ydot = matrix_mult(ydot, u);

  return ydot;

}

/*
  getRef gera uma matrix :
   | Xref(t) |
   | Yref(t) |
  com os valores de referencia para x e y

*/
Matrix getRef(double time){

  Matrix ref = matrix_zeros("ref", 2, 1);
  ref.values[0][0] = (5/PI)*cos(0.2*PI*time);
  ref.values[1][0] = (time < 10) ? ((5/PI)*cos(0.2*PI*time)) : ((-5/PI)*cos(0.2*PI*time));

  return ref;
}

/*
  getRef gera uma matrix :
   | Y_dot_mx(t) |
   | Y_dot_my(t) |

*/

Matrix getSimYmdot(Matrix ym, Matrix ref, double time){

  double np = 2.71828; // constante de euler

  Matrix ymdot = matrix_zeros("ymdot", 2, 1);
  double xref = ref.values[0][0];
  double yref = ref.values[1][0];

  double Ymx = ym.values[0][0];
  double Ymy = ym.values[1][0];

  ymdot.values[0][0] = ALPHA1*(xref - Ymx);
  ymdot.values[1][0] = ALPHA2*(yref - Ymy);

  return ymdot;

}

Matrix getSimYm(double time){

  double np = 2.71828; // constante de euler
  Matrix ym = matrix_zeros("ym", 2, 1);

  // valores de Ymx e Ymy calculados a partir da
  // transformada inversa de G(s) = alpha / (s + alpha);
  double Ymx = ALPHA1*pow(np,(-ALPHA1*time));
  double Ymy = ALPHA2*pow(np,(-ALPHA2*time));

  ym.values[0][0] = Ymx;
  ym.values[1][0] = Ymy;

  return ym;

}

/*
  getSimV gera uma matrix v(t):
   | Y_dot_mx + alpha1(ymx - y1) |
   | Y_dot_my + alpha2(ymy - y2) |
  com os valores de velocidade linear do sistema

*/

Matrix getSimV(Matrix ym, Matrix ymdot, Matrix y){

  Matrix v = matrix_zeros("v", 2, 1);

  double y1 = y.values[0][0];
  double y2 = y.values[1][0];

  double ym1 = ym.values[0][0];
  double ym2 = ym.values[1][0];

  double ymdot1 = ymdot.values[0][0];
  double ymdot2 = ymdot.values[1][0];

  v.values[0][0] = ymdot1 + ALPHA1*(ym1 - y1);
  v.values[1][0] = ymdot2 + ALPHA2*(ym2 - y2);

  return v;

}

// função da thread que calcula Ym e Ymdot

void modeloRefTask( void *args ){

  float delay = 0.05; // 50ms
  Matrix ref, ym, ymdot;
  struct timespec start, finish;
  double elapsed;


  float t = 0;
  while(t < 20){

    // entrada da thread: referencia
    ref = getRef(t);

    // saidas da thread: ym e ymdot
    ym = getSimYm(t);
    setYm(ym);
    setYmdot(getSimYmdot(ym, ref, t));

    // precisamos medir o tempo que a thread leva para
    // ser novamente agendada, para isso contamos o tempo
    // usando clock_gettime
    clock_gettime(CLOCK_MONOTONIC, &start);

    // usleep suspende a thread por um tempo x em microssegundos
    t += delay;
    usleep(delay*1000);

    clock_gettime(CLOCK_MONOTONIC, &finish);
    elapsed = (finish.tv_sec - start.tv_sec);
    elapsed += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;
    //fprintf(thread1_timelog, "%f\n", elapsed);
    // TODO: open a file for this thread's timing


  }

  //return NULL;

}


// função da thread que calcula v(t)

void ControleTask( void *args ){

  float delay = 0.05; // 50ms
  Matrix ym, ymdot, y, v;
  struct timespec start, finish;
  double elapsed;


  float t = 0;
  while(t < 20){

    // entrada do sistema: y, ym e ymdot
    ymdot = getYmdot();
    ym = getYm();
    y = getY();

    // saida do sistema: v
    setV(getSimV(ym, ymdot, y));

    // precisamos medir o tempo que a thread leva para
    // ser novamente agendada, para isso contamos o tempo
    // usando clock_gettime
    clock_gettime(CLOCK_MONOTONIC, &start);

    // usleep suspende a thread por um tempo x em microssegundos
    t += delay;
    usleep(delay*1000);

    clock_gettime(CLOCK_MONOTONIC, &finish);
    elapsed = (finish.tv_sec - start.tv_sec);
    elapsed += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;
    //fprintf(thread1_timelog, "%f\n", elapsed);
    // TODO: open a file for this thread's timing


  }

  //return NULL;

}


// função da thread que calcula u(t)

void LinearizacaoTask( void *args ){

  // TODO: calcular u(t)

}


// função da thread que calcula Y e X, saidas do sistema 

void RoboTask( void *args ){

  float delay = 0.03; // 50ms
  Matrix u, x, y;
  struct timespec start, finish;
  double elapsed;


  float t = 0;
  while(t < 20){

    // entrada do sistema: u
    u = getU();

    // saida do sistema: x e y
    x = getSimX(u, t);
    setY(getSimY(x, RADIUS));
    setX(x);
    // TODO: printa esses dados ou faz alguma coisa

    // precisamos medir o tempo que a thread leva para
    // ser novamente agendada, para isso contamos o tempo
    // usando clock_gettime
    clock_gettime(CLOCK_MONOTONIC, &start);

    // usleep suspende a thread por um tempo x em microssegundos
    t += delay;
    usleep(delay*1000);

    clock_gettime(CLOCK_MONOTONIC, &finish);
    elapsed = (finish.tv_sec - start.tv_sec);
    elapsed += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;
    //fprintf(thread1_timelog, "%f\n", elapsed);
    // TODO: open a file for this thread's timing


  }

  //return NULL;

}
