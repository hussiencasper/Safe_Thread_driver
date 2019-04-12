
/**
  *Safe thread UART
  * Author: Hussien Mohamed Hussien
  * Start Date: 5/4/2019
  */
/******************************************************************************
*                           UART Registers Struct                             *
*******************************************************************************/
/*

 1)Uart->RX //read-only; contains the last byte read from the serial line
 2)Uart->TX //write-only; if a byte is written to TX, the value is trasmitted on the serial line
 
 3)Uart->TX_READY //read-only; 1 if a write on TX can be done, 0 if TX is busy. After reading TX_READY, it goes
to 0 automatically. If Uart interrupts are enabled, the Uart handler is called
 4)Uart->RX_READY //read-only; 1 if a byte has been read from RX, 0 if the serial line is silent. After reading
RX_READY, it goes to 0 automatically. If Uart interrupts are enabled, the Uart handler is called

 5)Uart->RX_INT //read/write; write 1 to enable Uart interrupt when RX_READY transitions to 1, write 0 to disable
interrupt for RX events.
 6)Uart->TX_INT //read/write; write 1 to enable Uart interrupt when TX_READY transitions to 1, write 0 to disable
interrupt for TX events.

7)Uart->INTPOS //read-only; contains the position of the Uart interrupt handler in the simplARM vector table
8)Uart->ENABLE //write-only; write 1 to enable the peripheral, 0 to disable
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

static void Push(Queue* Pushq,uint8_t data);
static uint8_t Pop(Queue* Popq);

typedef struct {
	int Rear=-1;
	int Front=0;
	int Full=1;
	int Empty=0;
	uint8_t BUFF[MAX_SIZE];
}Queue;

#endif /* UART_H_ */