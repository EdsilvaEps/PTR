#include "matrix.h"


#ifndef _BUFFER_H
#define _BUFFER_H

#define MAX_BUFFER_SIZE 10

typedef Matrix BufferItem;

typedef struct{
  BufferItem data[MAX_BUFFER_SIZE];
  int counter;
  int bufferSize;
} Buffer;

// cria um struct buffer
Buffer createBuffer(int bufferSize);

// insere um item no buffer
int insertItem(Buffer *buffer, BufferItem item);

// remove um item do buffer
int removeItem(Buffer *buffer, BufferItem *item);

#endif
