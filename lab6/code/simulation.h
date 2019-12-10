
/*   Universidade Federal do Amazonas - UFAM
#    Disciplina: Programação de Sistemas de Tempo Real - PTR
#    Author: Edson Nascimento Silva Neto - 21350694
#		 Evaldo Patrik dos Santos Cardoso - 21453640

     Arquivo .h que descreve as funções de simulação do controle
     do robô.
*/


#include "matrix.h"
#include <stdio.h>
#include <math.h>
#include "monitors.h"
#include <unistd.h>
#include <time.h>


#ifndef _SIMULATION_H
#define _SIMULATION_H

#define NINT 100 //numero de interações do algoritmo de integração
#define PI 3.14159265358979323846
// TODO: perguntar do prof que raios é alpha
#define ALPHA1 1
#define ALPHA2 2
#define RADIUS 0.3


/* calcula a integral utilizando o método trapezoidal
   com iterações continuas.

   f -> função de xdot
   t -> tempo atual
   prev_t -> tempo anterior
 */
double integral(double (*f)(double t), double t, double prev_t);

/*
   processa a resposta de velocidade do robô de acordo com a
   variável de tempo fornecida. Retorna uma matriz do formato
   u(t) = [v w]^T (transposta de [v w]) em que v é a velocidade linear
   e w a velocidade angular.
*/
Matrix getSpeed(double t);

/*
  computa a matrix x(t) descrita pelo formato
  x(t) = [Xc, Yc, Theta]^t (transposta da matriz) onde
  (Xc, Yc) são as coordenadas cartesianas para a posição
  do centro de massa do robô e Theta é seu ângulo de orientação.

  As integrais servem para calcular os valores de x(t) a
  partir do espaço de estados (xponto(t)) fornecido  pelo enunciado
  do trabalho de laboratório 2.
*/
Matrix getPosition(double t, double prev_t);


/*
  computa a saída do sistema fazendo a multiplicacao
  matricial y(t) = MatrizId*(X(t)*U(t)).

  A saída é no formato y(t) = [Xc Yc Theta]^T (transposta da matriz)
  onde (Xc, Yc) são as coordenadas cartesianas para a posição
  do centro de massa do robô e Theta é seu ângulo de orientação.

*/
Matrix simulate(double t, Matrix u);

/*
  computa parcialmente a função do ponto da frente do robô
  dado pela função Yf(t) = X(t) + [0.5*Dcos(t) 0 0]'*X(t)
*/
Matrix simulate2(double t, double d, Matrix u);


/*

  getSimYdot gera a matrix:
    | cos(wt)   -Rsin(wt) |   | u1 |
    | sin(wt)   Rcos(wt)  | x | u2 |

    onde R é o raio da frente do robô

*/
Matrix getSimYdot(Matrix u, Matrix x, double radius);

/*
  getRef gera uma matrix :
   | Xref(t) |
   | Yref(t) |
  com os valores de referencia para x e y

*/
Matrix getRef(double time);

/*
  getSimYmdot gera uma matrix :
   | Y_dot_mx(t) |
   | Y_dot_my(t) |

*/

Matrix getSimYmdot(Matrix alpha, Matrix ref, double time);

/*
  getSimLimSpeed gera uma matrix v(t):
   | Y_dot_mx + alpha1(ymx - y1) |
   | Y_dot_my + alpha2(ymy - y2) |
  com os valores de velocidade linear do sistema

*/

Matrix getSimV(Matrix ym, Matrix ymdot, Matrix y, Matrix alpha);

/*
  getSimy gera a matriz y(t):
    | 1 0 0 |        | Rcos(x3) |
    | 0 1 0 | x(t) + | Rsin(x3) |

  onde consideramos x3 = w*t (velocidade angular * tempo)

  a matrix y(t) é a saída do sistema.
*/
Matrix getSimY(Matrix x, double radius);

/*
  getSimX gera a matriz x(t):
    | u1*sin(x3)  |
    | u1*cos(x3)  |
    |   w*t       |

  onde consideramos x3 = w*t (velocidade angular * tempo)

  a matrix x(t) é a integral de xponto(t)
*/

Matrix getSimX(Matrix u, double time);



#endif
