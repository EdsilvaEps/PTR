#include "simulation.h"
#include <stdio.h>
#include <math.h>

/* calcula a integral utilizando o método trapezoidal
   com iterações continuas.

   f -> função de xdot
   t -> tempo atual
   prev_t -> tempo anterior
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
