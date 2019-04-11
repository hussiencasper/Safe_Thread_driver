
/**
  *Safe thread UART
  * Author: Hussien Mohamed Hussien
  * Start Date: 5/4/2019
  */

#ifndef UART_H_
#define UART_H_

#include <stdint.h>

#define DISABLED	 0 
#define ENABLED  	 1
/* return values from the functions to indicate wheither the operation was done ,okay or not , */
#define UART_DONE 	 2 
#define UART_NODONE  3
#define UART_OK   	 4

/*Buffer and Queue maximum size 50 bytes*/
#define MAX_SIZE 50 

int UART_Init(void);
int UART_read(uint8_t *buffer, int len);
int UART_write(uint8_t *buffer, int len);

static int Empty(int front,int EmptyFlag);
static int Full(int rear,int FullFlag);

static void Push(Queue* Pushq);
static uint8_t Pop(Queue* Popq);

typedef struct {
	int Rear=-1;
	int Front=0;
	int Full=1;
	int Empty=0;
	uint8_t BUFF[MAX_SIZE];
}Queue;

#endif /* UART_H_ */