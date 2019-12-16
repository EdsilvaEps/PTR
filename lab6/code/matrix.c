#include "matrix.h"
#include <stdio.h>
#include <string.h>

// realiza o print da matriz, função roubada do professor
void matrix_print(Matrix mat){

  printf("Matrix: %s\n",mat.name);

  for(int i=0; i < mat.m; i++){
    printf("\n" );
    for(int j=0; j < mat.n; j++){

      if(j == 0){

        printf("| ");
        printf("%4.2f ", mat.values[i][j]);

      }else if(j == mat.n-1){

        printf("%4.2f ", mat.values[i][j]);
        printf("|");

      }else{

        printf("%4.2f ", mat.values[i][j]);

      }
    }
  }
  printf("\n\n");
}


double print_time_file(double time, FILE* arquivo){
  fprintf(arquivo, "%lf,", time);
}

// realiza o print da matriz num arquivo aberto. funcao roubada da funcao acima
void matrix_print_file(Matrix mat, FILE* arquivo){

  for(int i=0; i < mat.m; i++){

    for(int j=0; j < mat.n; j++){

      if(j == 0){
        fprintf(arquivo, "%4.2f,", mat.values[i][j]);

      }else if(j == mat.n-1){

        fprintf(arquivo, "%4.2f,", mat.values[i][j]);

      }else{

        fprintf(arquivo, "%4.2f,", mat.values[i][j]);

      }
    }
  }

}

// cria uma matriz de zeros, outra função desavergonhadamente
// plagiada do prof.
Matrix matrix_zeros(char name[MAX_NAME_SIZE], int m, int n){
  Matrix mat;

  if(m == 0 || n == 0){
    printf("tamanho de matriz inválido\n");
    m = 1;
    n = 1;
  }

  strncpy(mat.name, name, MAX_NAME_SIZE);
  mat.m = m;
  mat.n = n;

  for(int i=0; i < m; i++){
    for(int j=0; j < n; j++){
      mat.values[i][j] = 0;
    }
  }

  return mat;
}

// criação de matrizes
Matrix create_matrix(char name[MAX_NAME_SIZE], int m, int n, int elements[N_MAX*M_MAX]){
  Matrix mat = matrix_zeros(name, m, n);

  int el = 0;
  for(int i=0; i < m; i++){
    for(int j=0; j < n; j++){
      mat.values[i][j] = elements[el];
      el += 1;

    }
  }

  return mat;

}

// adição de matrizes
Matrix matrix_add(Matrix mat1, Matrix mat2){

  Matrix mat3 = matrix_zeros("resultante", mat1.m, mat1.n);

  if(mat1.m != mat2.m  || mat1.n != mat2.n){
    printf("Matrizes tem tamanhos diferentes\n");
    printf("operacao de soma nao pode ser realizada.\n");
    return mat3;
  }else {

    for(int i=0; i < mat1.m; i++){
      for(int j=0; j < mat1.n; j++){
        mat3.values[i][j] = mat1.values[i][j] + mat2.values[i][j];
      }
    }

  }
  return mat3;
}

// subtração de matrizes
Matrix matrix_sub(Matrix mat1, Matrix mat2){
  Matrix mat3 = matrix_zeros("resultante", mat1.m, mat1.n);

  if(mat1.m != mat2.m  || mat1.n != mat2.n){
    printf("Matrizes tem tamanhos diferentes\n");
    printf("operacao de subtracao nao pode ser realizada.\n");
    return mat3;
  }else {

    for(int i=0; i < mat1.m; i++){
      for(int j=0; j < mat1.n; j++){
        mat3.values[i][j] = mat1.values[i][j] - mat2.values[i][j];
      }
    }

  }
  return mat3;

}

// multiplicação de matrizes
Matrix matrix_mult(Matrix mat1, Matrix mat2){
  Matrix mat3 = matrix_zeros("resultante", mat1.m, mat2.n);

  if(mat1.n != mat2.m){
    printf("Matrizes %s e %s tem tamanhos incompativeis de linhas e colunas\n", mat1.name, mat2.name);
    printf("operacao de multiplicacao nao pode ser realizada.\n");
    return mat3;
  }else {

    /*
    na multiplicacao, o numero de colunas da primeira matriz deve ser igual
    ao numero de linhas da segunda. A matriz resultante possuirá o numero de linhas
    da primeria matriz e o numero de colunas da segunda.
    */
    double sum = 0;
    for (int c = 0; c < mat1.m; c++) { // 1o loop - linhas da 1a matriz
      for (int d = 0; d < mat2.n; d++) { // 2o loop - colunas da 2a matriz
        for (int k = 0; k < mat2.m; k++) { // 3o loop - linhas da 2a matriz
          sum = sum + mat1.values[c][k]*mat2.values[k][d];
        }
        mat3.values[c][d] = sum;
        sum = 0;
      }
    }
  }
  return mat3;

}


// calculo da matriz transposta
Matrix matrix_transp(Matrix mat){

  Matrix res = matrix_zeros("resultante", mat.n, mat.m);

  int i,j=0;
  for(i=0;i<mat.m;i++){
      for(j=0;j<mat.n;j++){
           //printf(" val[%d][%d] : %f\n", i,j,mat.values[i][j]);
           res.values[j][i]=mat.values[i][j];
      }
  }

  return res;

}

// multiplicacao escalar x matriz
Matrix matrix_scalar_mult(int scalar, Matrix mat){

  for(int i=0; i < mat.m; i++){
    for(int j=0; i < mat.n; i++){
      mat.values[i][j] = mat.values[i][j]*scalar;
    }
  }

  return mat;

}

// criar uma matriz identidade de m linhas e n colunas
Matrix matrix_identity(char name[MAX_NAME_SIZE], int m, int n){

  Matrix id = matrix_zeros(name, m, n);

  for(int i=0; i < id.m; i++){
    for(int j=0; j < id.n; j++){
      if(i==j) id.values[i][j] = 1;
    }
  }

  return id;

}
