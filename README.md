# Safe_Thread_driver
This exercise requires writing a thread safe UART driver in C for a fictitious microcontroller architecture
(simplARM) running a fictitious RTOS (simplOS).

SimplARM: a 32 bit microcontroller which features a simplified version of the Cortex M architecture. simplARM has
one vector table, no interrupt priorities but interrupt nesting. A simplARM interrupt is considered handled when the
associated handler finishes execution. If an interrupt is fired while the handler is executing, the handler will be called
again on exit.

SimplOS: a Realtime Operating System, with minimal support for preemptive scheduling running on a 1 millisecond
systick. Threads can be in the active or suspended state only. No dynamic memory allocation allowed.
============================================================================================================
The driver must be composed of the following API functions:
int UART_Init();
int UART_read(uint8_t *buffer, int len)
int UART_write(uint8_t *buffer, int len)

UART_Init: takes care of configuring the UART Peripheral.
UART_read : reads len bytes from the serial line, copying them in buffer. If no bytes are available, the calling thread is
suspended waiting for incoming bytes.
UART_write : transfers len bytes from buffer to the serial line. If multiple threads are writing concurrently on the UART,
it is not required that each thread's buffer is written sequentially.

a UART peripheral:
The UART of simplARM has fixed baudrate, no hardware flow control, full duplex communication and is controllable
with the following 32 bit registers, accessible from the global structure Uart
Uart->RX //read-only; contains the last byte read from the serial line
Uart->TX //write-only; if a byte is written to TX, the value is trasmitted on the serial line
Uart->TX_READY //read-only; 1 if a write on TX can be done, 0 if TX is busy. After reading TX_READY, it goes
to 0 automatically. If Uart interrupts are enabled, the Uart handler is called
Uart->RX_READY //read-only; 1 if a byte has been read from RX, 0 if the serial line is silent. After reading
RX_READY, it goes to 0 automatically. If Uart interrupts are enabled, the Uart handler is called
Uart->RX_INT //read/write; write 1 to enable Uart interrupt when RX_READY transitions to 1, write 0 to disable
interrupt for RX events.
Uart->TX_INT //read/write; write 1 to enable Uart interrupt when TX_READY transitions to 1, write 0 to disable
interrupt for TX events.
Uart->INTPOS //read-only; contains the position of the Uart interrupt handler in the simplARM vector table
Uart->ENABLE //write-only; write 1 to enable the peripheral, 0 to disable
=========================================================================================================
OS Available primitives:
void OSLock() :Disables simplARM global interrupts and begins a critical section
void OSUnlock() : Enables simplARM global interrupts and exits a critical section. On exit, existing interrupts are
handled
Semaphore OSCreateSemaphore(int n) : creates a counting semaphore initialized at value n
void OSSemaphoreSignal(Semaphore sem) : atomically increments the semaphore value by 1. If the value is non
positive, wakes up a suspended thread
void OSSemaphoreWait(Semaphore sem) : atomically decrease the semaphore value by 1. If the value is negative,
suspends the calling thread
===================================================================================================



