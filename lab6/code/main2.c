#include "simulation.h"
#include "matrix.h"
//#include "buffer.h"
#include "monitors.h"
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <time.h>


// TODO: fazer buffers para os valores


// threads
pthread_t modeloRefThread;
pthread_t controleThread;
pthread_t linearizacaoThread;
pthread_t roboThread;

FILE* arquivo;
FILE* threads_data;



int main(int argc, char const *argv[]) {

  clock_t start_t, end_t;
  struct timespec start, finish;
  double elapsed;

  // contagem de tempo em segundos e em pulsos de clock
  clock_gettime(CLOCK_MONOTONIC, &start);
  start_t = clock();

  //threads_data = fopen("threadsData.txt", "w+");
  //thread2_timelog = fopen("task2_time.txt","w+");

  //initializeAll();

  printf("Inicio do programa, clocks = %ld\n", start_t);

  initialize();


  pthread_create(&modeloRefThread, NULL, modeloRefTask, NULL);
  pthread_create(&controleThread, NULL, ControleTask, NULL);
  pthread_create(&linearizacaoThread, NULL, LinearizacaoTask, NULL);
  pthread_create(&roboThread, NULL, RoboTask, NULL);





  float main_delay = 0.12; // 120ms, delay do main
  float t = 0;

  while(t < 20){

    Matrix Ref = getSimRef(t);
    setRef(Ref);

    printf("t: %4.3f  \n",t);


    t += main_delay;
    usleep(120*1000);

  }


  pthread_join(modeloRefThread, NULL);
  pthread_join(controleThread, NULL);
  pthread_join(linearizacaoThread, NULL);
  pthread_join(roboThread, NULL);



  clock_gettime(CLOCK_MONOTONIC, &finish);
  elapsed = (finish.tv_sec - start.tv_sec);
  elapsed += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;


  end_t = clock();
  printf("Fim do programa, clocks = %ld\n", end_t);

  double total_t = (double)(end_t - start_t) / CLOCKS_PER_SEC;
  printf("Tempo total de execução %f\n", elapsed);
  printf("Tempo de CPU: %f\n", total_t);
  printf("%d %d %d %d %d \n",kx, kref, ky, kym, kymdot );

  //printf("%d %d %d \n",kx, kref, ky );
  //printf("%d %d %d %d %d \n",kx, kref, ky, kym, kymdot );


  arquivo = fopen("output.ascii", "w+");
  fprintf(arquivo, " Refx, Refy, xc, yc, Theta, y1, y2 \n");

  for(int i = 0; i < kref; i++){
    //printf("%d\n",i );
    Matrix Ref = bufferRef[i];
    //Matrix Ymdot = bufferYmdot[i];
    //Matrix Ym = bufferYm[i];
    //Matrix V = bufferV[i];
    //Matrix U = bufferU[i];
    Matrix X = bufferX[i];
    Matrix Y = bufferY[i];


    fprintf(arquivo, " %f, %f, %f, %f, %f, %f, %f\n", REF1, REF2, X1 , X2, X3, Y1, Y2);




  }


  fclose(arquivo);
  //fclose(thread1_timelog);
  //fclose(thread2_timelog);


  return 0;

}
