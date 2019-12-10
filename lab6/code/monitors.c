#include "monitors.h"




Matrix U;
Matrix X;
Matrix Y;
Matrix V;
Matrix Ym;
Matrix Ymdot;
Matrix Ref;


// Mutexes

// create and initialize mutexes
pthread_mutex_t _mutexU = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t _mutexX = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t _mutexY = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t _mutexV = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t _mutexYm = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t _mutexYmdot = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t _mutexRef = PTHREAD_MUTEX_INITIALIZER;

// maintain mutex interface with pointers
static pthread_mutex_t* mutexU = &_mutexU;
static pthread_mutex_t* mutexX = &_mutexX;
static pthread_mutex_t* mutexY = &_mutexY;
static pthread_mutex_t* mutexV = &_mutexV;
static pthread_mutex_t* mutexYm = &_mutexYm;
static pthread_mutex_t* mutexYmdot = &_mutexYmdot;
static pthread_mutex_t* mutexRef = &_mutexRef;


// getters and setters for the different variables
Matrix getU(void){

  Matrix u;

  pthread_mutex_lock(mutexU);
    u = U;
  pthread_mutex_unlock(mutexU);

  return u;

}

void setU(Matrix u){

  pthread_mutex_lock(mutexU);
    U = u;
  pthread_mutex_unlock(mutexU);


}

Matrix getX(void){

  Matrix x;

  pthread_mutex_lock(mutexX);
    x = X;
  pthread_mutex_unlock(mutexX);

  return x;

}

void setX(Matrix x){

  pthread_mutex_lock(mutexX);
    X = x;
  pthread_mutex_unlock(mutexX);


}

Matrix getY(void){

  Matrix y;

  pthread_mutex_lock(mutexY);
    y = Y;
  pthread_mutex_unlock(mutexY);

  return y;

}

void setY(Matrix y){

  pthread_mutex_lock(mutexY);
    Y = y;
  pthread_mutex_unlock(mutexY);


}

Matrix getV(void){

  Matrix v;

  pthread_mutex_lock(mutexV);
    v = V;
  pthread_mutex_unlock(mutexV);

  return v;

}

void setV(Matrix v){

  pthread_mutex_lock(mutexV);
    V = v;
  pthread_mutex_unlock(mutexV);


}

Matrix getYm(void){

  Matrix ym;

  pthread_mutex_lock(mutexYm);
    ym = Ym;
  pthread_mutex_unlock(mutexU);

  return ym;

}

void setYm(Matrix ym){

  pthread_mutex_lock(mutexYm);
    Ym = ym;
  pthread_mutex_unlock(mutexYm);


}

Matrix getYmdot(void){

  Matrix ymdot;

  pthread_mutex_lock(mutexYmdot);
    ymdot = Ymdot;
  pthread_mutex_unlock(mutexYmdot);

  return ymdot;

}

void setYmdot(Matrix ymdot){

  pthread_mutex_lock(mutexYmdot);
    Ymdot = ymdot;
  pthread_mutex_unlock(mutexYmdot);


}

Matrix getRef(void){

  Matrix ref;

  pthread_mutex_lock(mutexRef);
    ref = Ref;
  pthread_mutex_unlock(mutexRef);

  return ref;

}

void setRef(Matrix ref){

  pthread_mutex_lock(mutexRef);
    Ref = ref;
  pthread_mutex_unlock(mutexRef);


}
