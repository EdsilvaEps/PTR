#include "monitors.h"

// create and initialize mutexes

pthread_mutex_t _mutexU = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t _mutexX = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t _mutexY = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t _mutexV = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t _mutexYm = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t _mutexYmdot = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t _mutexRef = PTHREAD_MUTEX_INITIALIZER;

static pthread_mutex_t* mutexU = &_mutexU;
static pthread_mutex_t* mutexX = &_mutexX;
static pthread_mutex_t* mutexY = &_mutexY;
static pthread_mutex_t* mutexV = &_mutexV;
static pthread_mutex_t* mutexYm = &_mutexYm;
static pthread_mutex_t* mutexYmdot = &_mutexYmdot;
static pthread_mutex_t* mutexRef = &_mutexRef;


// maintain mutex interface with pointers
// Mutexes
pthread_mutex_t _mmutex = PTHREAD_MUTEX_INITIALIZER;

static pthread_mutex_t* mmutex = &_mmutex;



void initialize(){


   U = matrix_zeros("u", 2, 1);
   X = matrix_zeros("x", 3, 1);
   Y = matrix_zeros("y", 2, 1);
   V = matrix_zeros("v", 2, 1);
   Ym = matrix_zeros("ym", 2, 1);
   Ymdot = matrix_zeros("ymdot", 2, 1);
   Ref = matrix_zeros("ref", 2, 1);

   // inicializa os bloqueios mutex
   pthread_mutex_init(mutexU, NULL);
   pthread_mutex_init(mutexX, NULL);
   pthread_mutex_init(mutexY, NULL);
   pthread_mutex_init(mutexV, NULL);
   pthread_mutex_init(mutexYm, NULL);
   pthread_mutex_init(mutexYmdot, NULL);
   pthread_mutex_init(mutexRef, NULL);

   pthread_mutex_init(mmutex, NULL);


   // indices for the buffers
   ku = 0;
   kx = 0;
   ky = 0;
   kv = 0;
   kym = 0;
   kymdot = 0;
   kref = 0;

   // indices for the logging buffers
   kthread1 = 0;
   kthread2 = 0;
   kthread3 = 0;
   kthread4 = 0;

   sem_init(&u_full, 0, 0);
   sem_init(&x_full, 0, 0);
   sem_init(&y_full, 0, 0);
   sem_init(&v_full, 0, 0);
   sem_init(&ym_full, 0, 0);
   sem_init(&ymdot_full, 0, 0);
   sem_init(&ref_full, 0, 0);

   sem_init(&u_empty, 0, 1);
   sem_init(&x_empty, 0, 1);
   sem_init(&y_empty, 0, 1);
   sem_init(&v_empty, 0, 1);
   sem_init(&ym_empty, 0, 1);
   sem_init(&ymdot_empty, 0, 1);
   sem_init(&ref_empty, 0, 1);


}


// getters and setters for the different variables
Matrix getU(void){

  Matrix u;

  //printf("get u \n" );


  //sem_wait(&u_full);
  pthread_mutex_lock(mutexU);
    u = U;
  pthread_mutex_unlock(mutexU);
  //sem_post(&u_empty);
  return u;

}

void setU(Matrix u){

  //printf("set u \n" );


  //sem_wait(&u_empty);
  pthread_mutex_lock(mutexU);
    U = u;
  pthread_mutex_unlock(mutexU);
  //sem_post(&u_full);
  // add to buffer
  bufferU[ku] = u;
  ku++;

}

Matrix getX(void){

  //printf("getx\n" );


  Matrix x;

  //sem_wait(&x_full);
  pthread_mutex_lock(mutexX);
    x = X;
  pthread_mutex_unlock(mutexX);
  //sem_post(&x_empty);

  return x;

}

void setX(Matrix x){

  //printf("setx\n" );

  //sem_wait(&x_empty);
  pthread_mutex_lock(mutexX);
    X = x;
  pthread_mutex_unlock(mutexX);
  //sem_post(&x_full);

  // add to buffer
  bufferX[kx] = x;
  kx++;

}

Matrix getY(void){

  Matrix y;

  //printf("gety\n" );


  //sem_wait(&y_full);
  pthread_mutex_lock(mutexY);
    y = Y;
  pthread_mutex_unlock(mutexY);
  //sem_post(&y_empty);
  return y;

}

void setY(Matrix y){

  //printf("set y\n" );

  //sem_wait(&y_empty);
  pthread_mutex_lock(mutexY);
    Y = y;
  pthread_mutex_unlock(mutexY);
  //sem_post(&y_full);

  // add to buffer
  bufferY[ky] = y;
  ky++;

}

Matrix getV(void){

  Matrix v;

  //printf("getv\n" );

  //sem_wait(&v_full);
  pthread_mutex_lock(mutexV);
    v = V;
  pthread_mutex_unlock(mutexV);
  //sem_post(&v_empty);
  return v;

}

void setV(Matrix v){

  //printf("setv\n" );

  //sem_wait(&v_empty);
  pthread_mutex_lock(mutexV);
    V = v;
  pthread_mutex_unlock(mutexV);
  //sem_post(&v_full);

  // add to buffer
  bufferV[kv] = v;
  kv++;


}

Matrix getYm(void){

  Matrix ym;

  //printf("get ym \n" );

  //sem_wait(&ym_full);
  pthread_mutex_lock(mutexYm);
    ym = Ym;
  pthread_mutex_unlock(mutexYm);
  //sem_post(&ym_empty);
  return ym;

}

void setYm(Matrix ym){
  //printf("ym set\n" );

  //sem_wait(&ym_empty);
  pthread_mutex_lock(mutexYm);
    Ym = ym;
  pthread_mutex_unlock(mutexYm);
  //sem_post(&ym_full);
  // add to buffer
  //printf("ym assignmed\n" );

  bufferYm[kym] = ym;
  kym++;



}

Matrix getYmdot(void){

  Matrix ymdot;

  //sem_wait(&ymdot_full);
  pthread_mutex_lock(mutexYmdot);
    ymdot = Ymdot;
  pthread_mutex_unlock(mutexYmdot);
  //sem_post(&ymdot_empty);

  return ymdot;

}

void setYmdot(Matrix ymdot){

  //printf("ymdot assignment\n" );
  //sem_wait(&ymdot_empty);
  pthread_mutex_lock(mutexYmdot);
    Ymdot = ymdot;
  pthread_mutex_unlock(mutexYmdot);
  //sem_post(&ymdot_full);
  // add to buffer
  bufferYmdot[kymdot] = ymdot;
  kymdot++;


}


Matrix getRef(void){

  //printf("ref gotten\n" );


  Matrix ref;
  //sem_wait(&ref_full);
  pthread_mutex_lock(mutexRef);
    ref = Ref;
  pthread_mutex_unlock(mutexRef);
  //sem_post(&ref_empty);
  return ref;

}

void setRef(Matrix ref){

  //sem_wait(&ref_empty);
  pthread_mutex_lock(mutexRef);
    Ref = ref;
  pthread_mutex_unlock(mutexRef);
  //sem_post(&ref_full);
  // add to buffer
  bufferRef[kref] = ref;
  kref++;

  //printf("ref assignmed\n" );

}

// adiciona valores de periodo das threads para logging
void addPeriodValue(int thread, double period){

  switch (thread) {
    case MODELOREFTHREAD:
      bufferModeloRefThread[kthread1] = period;
      kthread1++;
      break;

    case CONTROLETHREAD:
      bufferControleThread[kthread2] = period;
      kthread2++;
      break;

    case LINEARIZACAOTHREAD:
      bufferLinearizacaoThread[kthread3] = period;
      kthread3++;
      break;

    case ROBOTHREAD:
      bufferRoboThread[kthread4] = period;
      kthread4++;
      break;

    default:
      printf("thread não especificada\n");
      break;

  }


}
