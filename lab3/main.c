#include "simulation.h"
#include "matrix.h"
#include <stdio.h>


int main(int argc, char const *argv[]) {

  int time = 20;
  Matrix id = matrix_identity("identidade", 3, 3);

  for(int i=0; i < time; i++){
    printf("tempo de simulação: %d\n", i);
    matrix_print(getPosition(i, i-1));
    matrix_print(getSpeed(i));
    matrix_print(simulate(i, getSpeed(i)));
  }

}
