

#ifndef _MATRIX_H
#define _MATRIX_H

#define M_MAX 10 // representa o num max de linhas
#define N_MAX 10 // representa o num max de colunas
#define MAX_NAME_SIZE 20 // tam maximo de caracteres no nome

// ADT da Matriz
typedef struct{
  char name[MAX_NAME_SIZE];
  int m; // numero de linhas
  int n; // numero de colunas
  double values[M_MAX][N_MAX]; // valores das linhas e colunas
} Matrix;

// printa os elementos da matriz na tela.
void matrix_print(Matrix mat);

// cria uma matriz de m linhas e n colunas, recebendo como parâmetros
// os elementos da ADT e um array de números (elements) de tamanho m*n
// para serem inseridos na matriz.
Matrix create_matrix(char name[MAX_NAME_SIZE], int m, int n, int elements[N_MAX*M_MAX]);

// cria uma matriz de m linhas e n colunas em que todos os elementos são 0.
Matrix matrix_zeros(char nome[MAX_NAME_SIZE], int m, int n);

// soma duas matrizes, se estas possuirem o mesmo tamanho.
Matrix matrix_add(Matrix mat1, Matrix mat2);

// subtrai duas matrizes, se estas possuirem o mesmo tamanho.
Matrix matrix_sub(Matrix mat1, Matrix mat2);

// multiplica das matrizes, se estas forem de tamanho
// Amxn e Bnxb ,numero de colunas da primeira matriz igual
// ao numero de linhas da segunda matriz.
Matrix matrix_mult(Matrix mat1, Matrix mat2);

// retorna a transposta da matriz recebida.
Matrix matrix_transp(Matrix mat);

// multiplica uma matriz por um escalar.
Matrix matrix_scalar_mult(int scalar, Matrix mat);

// cria uma matriz identidade
Matrix matrix_identity(char name[MAX_NAME_SIZE], int m, int n);




#endif
