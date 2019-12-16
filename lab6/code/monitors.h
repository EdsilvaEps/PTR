#include "matrix.h"
#include <pthread.h>
#include <semaphore.h>


#define U1 U.values[0][0]
#define U2 U.values[1][0]
#define X1 X.values[0][0]
#define X2 X.values[1][0]
#define X3 X.values[1][0]
#define Y1 Y.values[0][0]
#define Y2 Y.values[1][0]
#define V1 V.values[0][0]
#define V2 V.values[1][0]
#define YM1 Ym.values[0][0]
#define YM2 Ym.values[1][0]
#define YMDOT1 Ymdot.values[0][0]
#define YMDOT2 Ymdot.values[1][0]
#define REF1 Ref.values[0][0]
#define REF2 Ref.values[1][0]


Matrix U;
Matrix X;
Matrix Y;
Matrix V;
Matrix Ym;
Matrix Ymdot;
Matrix Ref;



// buffers for logging
Matrix bufferU[700];
Matrix bufferX[700];
Matrix bufferY[700];
Matrix bufferV[700];
Matrix bufferYm[700];
Matrix bufferYmdot[700];
Matrix bufferRef[700];


// indices of those buffers
int ku,kx,ky,kv,kym,kymdot,kref;


// semaphores

sem_t u_full;
sem_t x_full;
sem_t y_full;
sem_t v_full;
sem_t ym_full;
sem_t ymdot_full;
sem_t ref_full;

sem_t x_empty;
sem_t u_empty;
sem_t y_empty;
sem_t v_empty;
sem_t ym_empty;
sem_t ymdot_empty;
sem_t ref_empty;

void initialize();

// getters and setters for the different variables
Matrix getU(void);

void setU(Matrix u);

Matrix getX(void);

void setX(Matrix x);

Matrix getY(void);

void setY(Matrix y);

Matrix getV(void);

void setV(Matrix v);

Matrix getYm(void);

void setYm(Matrix ym);

Matrix getYmdot(void);

void setYmdot(Matrix ymdot);

Matrix getRef(void);

void setRef(Matrix ref);
