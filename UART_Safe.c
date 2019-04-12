
/**
  *Safe thread UART
  * Author: Hussien Mohamed Hussien
  * Start Date: 5/4/2019
  */

#include "UART.h"

/******************************************************************************
*                          Creating Queues and Sempahores                     *
*******************************************************************************/
/*Rx & Tx queues*/
Queue RXqueue,TXqueue;
/*pointer to RXqueue & Txqueue structures*/
const Queue* RXPTR = &RXqueue;
const Queue* TPTR  = &TXqueue;

/*creating semaphore intializeed by 50 same size of queue */
Semaphore TXSEM = OSCreateSemaphore(50); 
Semaphore RXSEM = OSCreateSemaphore(0);
/******************************************************************************
*                          UART APIs Function                                 *
*******************************************************************************/
int UART_Init(void){
/*pointer to UART structure*/	
	const UART *CfgPtr;
	int ReturnVal = UART_NODONE;
	/*Reseting all the registers before starting*/
	/*UART Clock*/ 
     RCGCUART_REG |= ENABLED; 
	 /*Disable UART*/
	(CfgPtr->ENABLE) = DISABLED; 
	/*Clear the Tx interrupt*/
	(CfgPtr->TX_INT) = DISABLED;
	/*Clear the RX interrupt*/
	(CfgPtr->RX_INT) = DISABLED;
	/*installing the handler using its name and position from the INTTPOS*/
    simpl_install_handler((CfgPtr->INTPOS), UART_handler());
	 /*SET the Tx interrupt*/
	(CfgPtr->TX_INT) = ENABLED;	
	/*SET the RX interrupt*/	
	(CfgPtr->RX_INT) = ENABLED;	
	
	/*Enable UART*/
	(CfgPtr->ENABLE) = ENABLED;		
	ReturnVal = UART_OK;
	
	return ReturnVal;
	
 } 

int UART_read(uint8_t *buffer, int len){
	
	const UART *CfgPtr;	
	/*Counter for iterations*/	
	uint8_t RXcounter;	
	/*Return value*/	
	int ReturnVal = UART_NODONE;							
	
	for(RXcounter=0;RXcounter<len;RXcounter++){
	 /*take the semaphore and decrease its value by 1*/
     OSSemaphoreWait(RXSEM);
    /*Critical section*/	 
	 OSLock();
	/*passing the data to be read from Rxqueue to the buffer destination*/	 
	 buffer[RXcounter] = Pop(RXPTR));						
	 /*Critical section ends*/
	 OSUnLock();
	}	
	/*if the length == number of iterations the whole length is read*/
	if(RXcounter == len){
      ReturnVal=UART_DONE;
    }
	else{
	/*Not the whole length is read*/
      ReturnVal=UART_NODONE; 
	}
	return ReturnVal;
 }

int UART_write(uint8_t *buffer, int len){
    
	 const UART *CfgPtr;
	 int ReturnVal = UART_NODONE;
     uint8_t TXcounter;

	for(TXcounter=0;TXcounter<len;TXcounter++){
		/*Semaphore is previously intialized by size of queue */
		OSSemaphoreWait(TXSEM);
		/*critical section start*/
		OSLock();
		/*passing the data to be sent from buffer destination to TXqueue*/
		Push(TXPTR,buffer[TXcounter++]);
		/*Critical section ends*/
		OSUnLock();
	}    
	/*if the length == number of iterations the whole length is passed to the TX queue*/
  if(TXcounter == len){
      ReturnVal=UART_DONE;
    }
	else{
	 /*Not the whole length is passed to the queue*/
      ReturnVal=UART_NODONE;
	}	
	return ReturnVal;
 }
 
void UART_handler(void){
	const UART *CfgPtr;	
	/*TX_READY flag won't be set except if there possibilty to send data*/
   if((CfgPtr->TX_READY) == SEND){
	/*send by getting it from the TXqueue*/
	(CfgPtr->TX) = Pop(TXPTR);
	 /*add semaphore since there is free space in the queue after sending */
	OSSemaphoreSignal(TXSEM);	
	}
	if((CfgPtr->RX_READY) == READ){
	/*Push data to the RXqueue*/
	  Push(RXPTR,(CfgPtr->RX));
	 /*increase semaphore by 1*/
	  OSSemaphoreSignal(RXSEM);
	}
}
/******************************************************************************
*                           Queue Function                                    *
*******************************************************************************/
static int Empty(int front,int EmptyFlag)){
	if(front>(MAX_SIZE-1)){
		EmptyFlag = 1;
	}else{
		EmptyFlag = 0;
	}
	return EmptyFlag;
}

static int Full(int rear,int FullFlag){
	if(rear>=(MAX_SIZE-1)){
		FullFlag = 1;
	}else{
		FullFlag = 0;
	}
	return FullFlag;
}

static void Push(Queue* Pushq,uint8_t data){

	if(!Full((Pushq->Rear),(Pushq->Full))){
	    (Pushq->Rear)++;
		int rear = (Pushq->Rear)
		(Pushq->BUFF[rear])= data;		
	}
}
static uint8_t Pop(Queue* Popq){
	
    uint8_t value;
	int front = (Popq->Front);
	if(!Empty((Popq->Front),(Popq->Empty))){
		
		value= Popq->BUFF[front];
	    (Popq->Front)++;
	}
	return value;
}
