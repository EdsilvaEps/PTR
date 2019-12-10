#include "simulation.h"
#include "matrix.h"
#include "buffer.h"
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <time.h>

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
FILE* thread1_timelog;
FILE* thread2_timelog;


/*
tarefa 1: uma thread que vai rodar a simulação em si, ou seja,
vai contar o tempo (em passos de 30ms) e printar os valores
na tela, além de gerar u(t). Thread produtora-consumidora.
*/
void *task1(  void *args  ){

  Matrix speed;
  Matrix output;

  // criar variaveis como esse em cada thread não é o
  // melhor jeito de se fazer as coisas, mas estou com pressa :D
  struct timespec start, finish;
  double elapsed;

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
    output = outputBuffer.data[(outputBuffer.counter-1)];

    // remove o item do buffer
    if(removeItem(&outputBuffer, &output)){

      fprintf(stderr, "buffer de output: remoção do buffer não permitida\n");

    }

    // libera o acesso ao buffer
    pthread_mutex_unlock(&mutex);

    // sinaliza buffer vazio
    sem_post(&output_empty);

    double v = speed.values[0][0];
    double w = speed.values[1][0];
    double xc = output.values[0][0];
    double yc = output.values[1][0];
    double theta = output.values[2][0];

    // printa tudo na tela
    printf("t: %4.3f | v: %4.3f | w: %4.3f | Xc: %4.3f | Yc: %4.3f | theta: %4.3f \n",t, v, w, xc, yc, theta);

    // printa tudo no arquivo
    fprintf(arquivo, " %f , %f , %f , %f , %f , %f\n",t, v, w, xc, yc, theta);



    //************************************************

    // precisamos medir o tempo que a thread leva para
    // ser novamente agendada, para isso contamos o tempo
    // usando clock_gettime
    clock_gettime(CLOCK_MONOTONIC, &start);

    // usleep suspende a thread por um tempo x em microssegundos
    t += 0.03; // 30 ms
    usleep(SLEEP_TASK_1*1000);

    clock_gettime(CLOCK_MONOTONIC, &finish);
    elapsed = (finish.tv_sec - start.tv_sec);
    elapsed += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;
    fprintf(thread1_timelog, "%f\n", elapsed);


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
  Matrix output = matrix_zeros("pos", 3, 1);

  struct timespec start, finish;
  double elapsed;


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
    output = matrix_add(output, simulate2(t, DIAMETER, speed));
    //output = simulate2(t, DIAMETER, speed);

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


    clock_gettime(CLOCK_MONOTONIC, &start);

    // usleep suspende a thread por um tempo SLEEP_TASK_2 em microssegundos
    t += 0.05; // 50 ms
    usleep(SLEEP_TASK_2*1000);

    clock_gettime(CLOCK_MONOTONIC, &finish);
    elapsed = (finish.tv_sec - start.tv_sec);
    elapsed += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;
    fprintf(thread2_timelog, "%f\n", elapsed);


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

  clock_t start_t, end_t;
  struct timespec start, finish;
  double elapsed;

  // contagem de tempo em segundos e em pulsos de clock
  clock_gettime(CLOCK_MONOTONIC, &start);
  start_t = clock();

  char path[100];

  // se o usuario não colocou um nome de arquivo
  if( argc != 2){

     arquivo = fopen("output.ascii", "w+");

  } else {

    strcpy(path,argv[1]);
    arquivo = fopen(path,"w+");

  }

  thread1_timelog = fopen("task1_time.txt", "w+");
  thread2_timelog = fopen("task2_time.txt","w+");

  initializeAll();

  printf("Inicio do programa, clocks = %ld\n", start_t);

  pthread_create(&task1_thread, NULL, task1, NULL);
  pthread_create(&task2_thread, NULL, task2, NULL);

  //pthread_join(task1_thread, NULL);
  //pthread_join(task2_thread, NULL);



  float t;
  while(t < 20){

    t += 0.05;
    usleep(SLEEP_TASK_2*1000);

  }


  clock_gettime(CLOCK_MONOTONIC, &finish);
  elapsed = (finish.tv_sec - start.tv_sec);
  elapsed += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;


  end_t = clock();
  printf("Fim do programa, clocks = %ld\n", end_t);

  double total_t = (double)(end_t - start_t) / CLOCKS_PER_SEC;
  printf("Tempo total de execução %f\n", elapsed);
  printf("Tempo de CPU: %f\n", total_t);

  fclose(arquivo);
  fclose(thread1_timelog);
  fclose(thread2_timelog);


  return 0;

}
