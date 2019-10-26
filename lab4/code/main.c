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
#define SLEEP_TASK_1 30
#define SLEEP_TASK_2 50
#define DIAMETER 1

// buffers de Yf(t)(output) e u(t) (velocidade)
Buffer outputBuffer;
Buffer speedBuffer;

// bloqueio mutex
pthread_mutex_t mutex;

// semáforos
sem_t output_full, output_empty, speed_full, speed_empty;

// threads
pthread_t task1_thread;
pthread_t task2_thread;

FILE* arquivo;


/*
tarefa 1: uma thread que vai rodar a simulação em si, ou seja,
vai contar o tempo (em passos de 30ms) e printar os valores
na tela, além de gerar u(t). Thread produtora-consumidora.
*/
void *task1(  void *args  ){

  Matrix speed;
  Matrix output;

  //FILE* arquivo = (FILE*)args;

  float t = 0;
  while(t < 20){

    speed = getSpeed(t);

    //************** PARTE PRODUTORA *****************
    // espera até o buffer de velocidade estar vazio;
    sem_wait(&speed_empty);

    // bloqueia o semaforo
    pthread_mutex_lock(&mutex);

    // insere valor no buffer de velocidade
    if(insertItem(&speedBuffer, speed)){

      fprintf(stderr, "buffer de velocidade: adição no buffer não permitida\n");

    }

    // desbloqueia acesso
    pthread_mutex_unlock(&mutex);

    // sinaliza buffer de velocidade cheio
    sem_post(&speed_full);

    //************************************************

    //*********** PARTE CONSUMIDORA ******************

    // espera até o buffer de output estar cheio
    sem_wait(&output_full);

    // bloqueia o acesso ao buffer
    pthread_mutex_lock(&mutex);

    // pega o dado mais atual do buffer e soma com X(t) anterior
    output = matrix_add(output, outputBuffer.data[(outputBuffer.counter-1)]);

    double v = speed.values[0][0];
    double w = speed.values[1][0];
    double xc = output.values[0][0];
    double yc = output.values[1][0];
    double theta = output.values[2][0];

    // printa tudo na tela
    printf("t: %4.3f | v: %4.3f | w: %4.3f | Xc: %4.3f | Yc: %4.3f | theta: %4.3f \n",t, v, w, xc, yc, theta);

    // printa tudo no arquivo
    fprintf(arquivo, " %f , %f , %f , %f , %f , %f\n",t, v, w, xc, yc, theta);

    // remove o item do buffer
    if(removeItem(&outputBuffer, &output)){

      fprintf(stderr, "buffer de output: remoção do buffer não permitida\n");

    }

    // libera o acesso ao buffer
    pthread_mutex_unlock(&mutex);

    // sinaliza buffer vazio
    sem_post(&output_empty);

    //************************************************




    // usleep suspende a thread por um tempo x em microssegundos
    usleep(SLEEP_TASK_1*1000);


    t += 0.03; // 30 ms

  }

  return NULL;

}

/*
tarefa 2: uma thread que vai utilizar a velocidade gerada na thread 1 p/
rodar os calculos a cada unidade de tempo esta tarefa
roda em um intervalo de 50 ms. Thread produtora-consumidora.
*/
void *task2( void *args  ){

  Matrix speed;
  Matrix output;


  float t = 0;
  while(t < 20){

    //output = simulate(t, speed);

    //************** PARTE CONSUMIDORA *****************

    // espera até o sinal de cheio para buffer de velocidade
    sem_wait(&speed_full);

    // bloqueia o acesso ao buffer
    pthread_mutex_lock(&mutex);

    // REGIÃO CRITICA ABAIXO, É PERIGOSO IR SOZINHO

    // retira o valor de velocidade mais atual do buffer
    // e calcula o output.

    speed = speedBuffer.data[(speedBuffer.counter-1)];
    output = simulate2(t, DIAMETER, speed);

    if(removeItem(&speedBuffer, &speed)){

      fprintf(stderr, "buffer de velocidada: remoção do buffer não permitida\n");

    }

    // desbloqueia o acesso ao buffer
    pthread_mutex_unlock(&mutex);

    // sinaliza buffer de velocidade vazio
    sem_post(&speed_empty);

    //************************************************

    //************** PARTE PRODUTORA *****************

    // espera até o sinal de vazio para buffer de output
    sem_wait(&output_empty);

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
    sem_post(&output_full);

    //************************************************




    // usleep suspende a thread por um tempo SLEEP_TASK_2 em microssegundos
    usleep(SLEEP_TASK_2*1000);

    t += 0.05; // 50 ms


  }

  return NULL;

}

/**
Esta função faz o óbvio: inicializa todas as variáveis
globais.
*/
void initializeAll(){

  // inicializa o buffer de Yf
  outputBuffer = createBuffer(BUFFER_SIZE);

  // inicializa o buffer de u(t)
  speedBuffer = createBuffer(BUFFER_SIZE);

  // inicializa o bloqueio mutex
  pthread_mutex_init(&mutex, NULL);

  // inicializa o semáforo Yf de "cheio" e inicializa p/ 0
  sem_init(&output_full, 0, 0);

  // inicializa o semáforo de Yf de "vazio" e inicializa p/ BUFFER_SIZE
  sem_init(&output_empty, 0, BUFFER_SIZE);

  // inicializa o semáforo de u(t) de "cheio" e inicializa p/ 0
  sem_init(&speed_full, 0, 0);

  // inicializa o semáforo de u(t) de "vazio" e inicializa p/ BUFFER_SIZE
  sem_init(&speed_empty, 0, BUFFER_SIZE);





}

int main(int argc, char const *argv[]) {

  char path[100];

  //FILE* arquivo;

  // se o usuario não colocou um nome de arquivo
  if( argc != 2){

     arquivo = fopen("output.ascii", "w+");

  } else {

    strcpy(path,argv[1]);
    arquivo = fopen(path,"w+");

  }

  initializeAll();

  pthread_create(&task1_thread, NULL, task1, NULL);
  pthread_create(&task2_thread, NULL, task2, NULL);


  float t;
  while(t < 20){

    t += 0.05;
    usleep(SLEEP_TASK_2*1000);

  }

  fclose(arquivo);


  return 0;

}
