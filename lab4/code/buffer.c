#include "buffer.h"

Buffer createBuffer(int bufferSize){
  Buffer newBuffer;
  newBuffer.bufferSize = bufferSize;
  newBuffer.counter = 0;

  return newBuffer;
};

// insere um item no buffer
int insertItem(Buffer *buffer, BufferItem item){

  // se buffer não está cheio, adicionar o item
  // e incrementar o contador
  if(buffer->counter < buffer->bufferSize){

    buffer->data[buffer->counter] = item;
    buffer->counter += 1;

    return 0;
  }

  else { // buffer cheio

    return -1;
  }

};

// remove um item do buffer
int removeItem(Buffer *buffer, BufferItem *item){

  // se o ponteiro não estiver vazio, remover o item
  // e decrementar o contador

  if(buffer->counter > 0){

    *item = buffer->data[(buffer->counter-1)];
    buffer->counter = buffer->counter - 1;

    return 0;

  }

  else { // buffer vazio
    return -1;
  }

};
