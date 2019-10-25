/*
    Universidade Federal do Amazonas - UFAM
    Disciplina: Programação de Sistemas de Tempo Real - PTR
    Authors: Evaldo Patrik dos Santos Cardoso - 21453640
             Edson Nascimento Silv Neto - 21350694
*/

// imports 
#include  <stdio.h>
#include  <stdlib.h>
#include  <string.h>
#include  "adicao.h"
#include  "subtracao.h"


int  main( int argc, char *argv[]){
  if (argc == 3 ) {
    int num1 = atoi (argv [ 1 ]);
    int num2 = atoi (argv [ 2 ]);
    if (num1 != 0 && num2 != 0 ) {
      printf ( "A soma de (%d + %d) = % d \n \n " ,num1, num2, adicao(num1, num2));
      printf ( "A soma de (%d - %d) = % d \n \n " ,num1, num2, subtracao(num1, num2));
    }else{
      printf ( "Argumentos inválidos \nusage: somasub [int num1] [int num2]\n " );
    }
  }else {
    printf ( "È necessário 2 Argumentos \nusage: somasub [int num1] [int num2]\n" );
  }

  return  0 ;
}
