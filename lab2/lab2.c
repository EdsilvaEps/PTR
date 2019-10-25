#include <stdio.h>
#include "matrix.h"

int main(int argc, char const *argv[]) {


  printf("Criando uma matriz de zeros\n");
  Matrix mat1 = matrix_zeros("matriz 1", 5, 5);
  matrix_print(mat1);
  printf("Criando outras duas matrizes \n");
  int values[] = {1,2,3,4,5,6};
  int values2[] = {6,7,8,9,10,11};
  Matrix mat2 = create_matrix("matriz 2", 3, 2, values);
  Matrix mat3 = create_matrix("matriz 3", 2, 1, values2);
  matrix_print(mat2);
  matrix_print(mat3);
  printf("Somando as duas matrizes\n");
  Matrix mat4 = matrix_add(mat2, mat3);
  matrix_print(mat4);
  printf("Subtraindo as duas matrizes\n");
  mat4 = matrix_sub(mat2, mat3);
  matrix_print(mat4);
  printf("Multiplicando as duas matrizes\n");
  mat4 = matrix_mult(mat2, mat3);
  matrix_print(mat4);
  printf("Multiplicando matriz 2 por escalar 2\n");
  mat4 = matrix_scalar_mult(2,mat2);
  matrix_print(mat4);
  printf("Transposta da matriz 3\n");
  mat4 = matrix_transp(mat3);
  matrix_print(mat4);
  Matrix mat5 = matrix_identity("mat5", 3, 3);
  printf("matriz identidade\n");
  matrix_print(mat5);

  return 0;
}
