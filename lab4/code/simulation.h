
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


#ifndef _SIMULATION_H
#define _SIMULATION_H

#define NINT 100 //numero de interações do algoritmo de integração
#define PI 3.14159265358979323846


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


#endif
