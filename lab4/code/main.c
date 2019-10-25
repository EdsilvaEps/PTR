#include "simulation.h"
#include "matrix.h"
#include "buffer.h"
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

// TODO: mudar a função da frente do robô

#define BUFFER_SIZE 1
#define SLEEP_TASK_1 50
#define SLEEP_TASK_2 30


Buffer outputBuffer;

// bloqueio mutex
pthread_mutex_t mutex;

// semáforos
sem_t full, empty;

// contador de buffer
int counter;

// threads
pthread_t task1_thread;
pthread_t task2_thread;


/*
typedef struct{
  Matrix *speed;
  Matrix *output;
} Args;*/

/*
tarefa 1: uma thread que vai rodar a simulação em si, ou seja,
vai contar o tempo (em passos de 30ms) e printar os valores
na tela. Thread consumidora.
*/
void *task1(  void *args  ){

  //Matrix speed;
  Matrix output;

  float t = 0;
  while(t < 20){

    // espera até o buffer estar cheio;
    sem_wait(&full);

    // bloqueia o acesso ao buffer
    pthread_mutex_lock(&mutex);

    // pega o dado mais atual do buffer
    output = outputBuffer.data[(outputBuffer.counter-1)];

    double xc = output.values[0][0];
    double yc = output.values[1][0];
    double theta = output.values[2][0];

    // printa tudo na tela
    printf("t: %f | Xc: %f | Yc: %f | theta: %f \n", t, xc, yc, theta);

    // remove o item do buffer
    if(removeItem(&outputBuffer, &output)){

      fprintf(stderr, "buffer de output: remoção do buffer não permitida\n");

    }

    // libera o acesso ao buffer
    pthread_mutex_unlock(&mutex);

    // sinaliza buffer vazio
    sem_post(&empty);

    t += 0.03; // 30 ms

    // usleep suspende a thread por um tempo x em microssegundos
    usleep(SLEEP_TASK_1*1000);


  }

  return NULL;

}

/*
tarefa 2: uma thread que vai rodar os calculos a cada unidade de tempo
esta tarefa roda em um intervalo de 50 ms. Thread produtora.
*/
void *task2( void *args  ){

  Matrix speed;
  Matrix output;

  float t = 0;
  while(t < 20){

    // calcula os valores de u(t) e y(t) localmente
    speed = getSpeed(t);
    output = simulate(t, speed);


    // espera até o sinal de vazio
    sem_wait(&empty);

    // bloqueia o acesso ao buffer
    pthread_mutex_lock(&mutex);


    // REGIÃO CRITICA ABAIXO, É PERIGOSO IR SOZINHO

    // insere o valor de output localmente calculado
    // no buffer global
    if(insertItem(&outputBuffer, output)){

      fprintf(stderr, "buffer de output: inserção no buffer não permitida\n");

    } //else printf("inserted on buffer\n");

    // desbloqueia o acesso ao buffer
    pthread_mutex_unlock(&mutex);

    // sinaliza buffer cheio
    sem_post(&full);

    t += 0.05; // 50 ms

    // usleep suspende a thread por um tempo SLEEP_TASK_2 em microssegundos
    usleep(SLEEP_TASK_2*1000);



  }

  return NULL;

}

/**
Esta função faz o óbvio: inicializa todas as variáveis
globais.
*/
void initializeAll(){

  // inicializa o buffer
  outputBuffer = createBuffer(BUFFER_SIZE);

  // inicializa o bloqueio mutex
  pthread_mutex_init(&mutex, NULL);

  // inicializa o semáforo de "cheio" e inicializa p/ 0
  sem_init(&full, 0, 0);

  // inicializa o semáforo de "vazio" e inicializa p/ BUFFER_SIZE
  sem_init(&empty, 0, BUFFER_SIZE);





}

int main(int argc, char const *argv[]) {




  /*Args *args = (Args *)malloc(sizeof(Args));
  args->speed = &speed;
  args->output = &result;*/

  initializeAll();

  pthread_create(&task2_thread, NULL, task2, NULL);
  pthread_create(&task1_thread, NULL, task1, NULL);

  float t;
  while(t < 20){
    //printf("buffer: %d\n", outputBuffer.counter );
    usleep(SLEEP_TASK_1*1000);
    t += 0.03;

  }


  /*char path[100];

  strcpy(path,argv[1]);

  FILE* arquivo = fopen(path,"w+");

  for(double i=0; i < time; i+=0.1){

    position = getPosition(i, i-1);
    speed = getSpeed(i);
    result = simulate(i, getSpeed(i));
    print_time_file(i, arquivo);
    matrix_print_file(position,arquivo);
    matrix_print_file(speed,arquivo);
    matrix_print_file(result,arquivo);
    fprintf(arquivo,"\n");
  }

  fclose(arquivo);*/

  return 0;

}
