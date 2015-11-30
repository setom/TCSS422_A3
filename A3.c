/*
	Basic program outline
	
	1 - Create a Ready Queue and populate it with apx 30 processes
	2 - Create 4 empty I/O device queues
	3 - Create 4 empty Service Queues
	4 - Run the outer loop:
 - run the first process in the ready queue
 - For each time the process gets CPU time, quanta++
 - if quantum == total quantums, process is terminated
 break
 - Check the current quanta against the randomly generated i/o values arrays
 if quanta % array value == 0,
 State = blocked
 Enqueue Process pointer to I/O queue of the device that is blocked
 - Check the current quanta against the randomly generated kernel values arrays
 if quanta % array value == 0,
 State = blocked
 enqueue it in the proper kernel queue
 - for each I/O Queue
 randomly terminate the first process (is there a way to do this with timers instead? Needs 2 threads?
 & put it back in the ready queue
 - for each kernel queue
 if mutex is UNLOCKED
 first process LOCKS the mutex
 if mutex is LOCKED
 wait
 Randomly terminate the first process
 Unlock the mutex
 and put it back in the ready queue
 - Randomly add some new processes
 
	*/

#include "A3.h"

int main (int argc, char* argv[]){
    
    //Max size of any queue
#define QUEUE_MAX 100
    //number of IO Queues
#define NUM_IO_DEVICES 4
    //number of kernel queues
#define NUM_KERNEL_SERVICES 4
    
    //counter for processes
    int Proc_ID = 1;
    //counter for loops
    int i, k;
    int s = 0;
    //int for random number generation
    //srand(time(0));
    int r;
    int r2;
    
    
    //1 - Create a Ready Queue and populate it with apx 30 processes
    Queue ReadyQueue = createQueue();
    r = ((rand() % 10) + 25);
    for (i = 0; i < r; i ++){
        //random number of quanta that the process will consume
        r2 = ((rand() % 100) + 2500);
        printf("** R2: %d\n", r2);
        //scanf("%d", &k);
        PCBNode* pcb = createPCBNode(Proc_ID, r2);
        enqueue(pcb, &ReadyQueue);
        Proc_ID++;
    }
    
    //2 - Create 4 empty I/O device queues
    Queue PrinterQueue = createQueue();
    Queue KeyboardQueue = createQueue();
    Queue DiskQueue = createQueue();
    Queue ModemQueue = createQueue();
    
    //3 - Create 4 empty Service Queues
    Queue M1Queue = createQueue();
    Queue M2Queue = createQueue();
    Queue M3Queue = createQueue();
    Queue M4Queue = createQueue();
    
    //4 - Run the outer loop:
    while(ReadyQueue.size > 0 || PrinterQueue.size > 0 || KeyboardQueue.size > 0 || DiskQueue.size > 0 || ModemQueue.size > 0){

		//if something in the readyQueue, dequeue it, check for interrupts and requeue it as appropriate
        if(ReadyQueue.size > 0) {
			//dequeue the first node in the ready queue check if it reached quantum goal
            PCBNode* currentProcess = dequeueAndCheckTermination(&ReadyQueue);
            //printf("Dequeuing node %d from ReadyQueue\n", currentProcess->id);
			
			if(currentProcess != NULL){
				printf("Current CPU Process: %d, count: %d of %d\n", currentProcess->id, currentProcess->count, currentProcess->quanta);
				int interruptSentinel = 0;
				i = 0;

				while(1){
					//check for interrupts on the node
					if(checkIOInterrupt(currentProcess, currentProcess->IO_Printer)){
						printf("Node %d interrupted by IO_Printer at quanta %d, requeueing...\n", currentProcess->id, currentProcess->count);
						currentProcess->state = waiting;
						enqueue(currentProcess, &PrinterQueue);
						interruptSentinel = 1;
						break;
					}
					if(checkIOInterrupt(currentProcess, currentProcess->IO_Keyboard)){
						printf("Node %d interrupted by IO_Keyboard at quanta %d, requeueing...\n", currentProcess->id, currentProcess->count);
						currentProcess->state = waiting;
						enqueue(currentProcess, &KeyboardQueue);
						interruptSentinel = 1;
						break;
					}
					if(checkIOInterrupt(currentProcess, currentProcess->IO_Disk)){
						printf("Node %d interrupted by IO_Disk at quanta %d, requeueing...\n", currentProcess->id, currentProcess->count);
						currentProcess->state = waiting;
						enqueue(currentProcess, &DiskQueue);
						interruptSentinel = 1;
						break;
					}
					if(checkIOInterrupt(currentProcess, currentProcess->IO_Modem)){
						printf("Node %d interrupted by IO_Modem at quanta %d, requeueing...\n", currentProcess->id, currentProcess->count);
						currentProcess->state = waiting;
						enqueue(currentProcess, &ModemQueue);
						interruptSentinel = 1;
						break;
					}
					break;
				}
				
				//if the interrupt sentinel was not changed, there was not an interrupt, simulate its time being done
				if(interruptSentinel == 0){
					currentProcess->state = waiting;
					enqueue(currentProcess, &ReadyQueue);
				}
			} else {
				free(currentProcess);
			}
				
		}
		
		//randomly dequeue the IO request queues
		if(PrinterQueue.size > 0){
			//randomly dequeue the IO Queues
			randomlyDequeue(&PrinterQueue, &ReadyQueue);
		}
		if(KeyboardQueue.size > 0){
			//randomly dequeue the IO Queues
			randomlyDequeue(&KeyboardQueue, &ReadyQueue);
		}
		if(DiskQueue.size > 0){
			//randomly dequeue the IO Queues
			randomlyDequeue(&DiskQueue, &ReadyQueue);
		}
		if(ModemQueue.size > 0){
			//randomly dequeue the IO Queues
			randomlyDequeue(&ModemQueue, &ReadyQueue);
		}
			
		printf("Ready Queue Size: %d\n", ReadyQueue.size);
		printf("Printer Queue Size: %d\n", PrinterQueue.size);
		printf("Keyboard Queue Size: %d\n", KeyboardQueue.size);
		printf("Disk Queue Size: %d\n", DiskQueue.size);
		printf("ModemQueue Size: %d\n", ModemQueue.size);
// 	
// 		printf("M1Queue Size: %d\n", M1Queue.size);
// 		printf("M2Queue Size: %d\n", M2Queue.size);
// 		printf("M3Queue Size: %d\n", M3Queue.size);
// 		printf("M4Queue Size: %d\n\n", M4Queue.size);
		//scanf("%d", &s);
		printf("\n");
    }

}