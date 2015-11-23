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
    int Proc_ID = 0;
    //counter for loops
    int i, k;
    int s = 0;
    //int for random number generation
    srand(time(0));
    int r;
    int r2;
    
    
    //1 - Create a Ready Queue and populate it with apx 30 processes
    Queue ReadyQueue = createQueue();
    r = ((rand() % 10) + 25);
    for (i = 0; i < r; i ++){
        //random number of quanta between 5000-25000
        r2 = ((rand() % 10) + 50);
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
    while(ReadyQueue.size > 0 || PrinterQueue.size > 0 || KeyboardQueue.size > 0 || DiskQueue.size > 0 || ModemQueue.size > 0 || M1Queue.size > 0 || M2Queue.size > 0 || M3Queue.size > 0 || M4Queue.size > 0){
    	
    	printf("*** NEW LOOP ***\n");	
    	//printf("ReadyQueueSize = %d\n", ReadyQueue.size);
    	if(ReadyQueue.size > 0) {
			//dequeue the first node in the ready queue check if it reached quantum goal
			PCBNode* currentProcess = dequeueAndCheckTermination(&ReadyQueue);
			printf("Dequeuing node %d from ReadyQueue\n", currentProcess->id);	
		
			//if the currentProcess is not null, check for IO and Kernel discontinuties
			if(currentProcess != NULL){
				i = compareIOInterrupt(currentProcess);
				switch(i) {
					case 1:
						currentProcess->state = waiting;
						enqueue(currentProcess, &PrinterQueue);
						break;
					case 2: 
						currentProcess->state = waiting;
						enqueue(currentProcess, &KeyboardQueue);
						break;
					case 3: 
						currentProcess->state = waiting;
						enqueue(currentProcess, &DiskQueue);
						break;
					case 4: 
						currentProcess->state = waiting;
						enqueue(currentProcess, &ModemQueue);
						break;
					case 5: 
						currentProcess->state = waiting;
						enqueue(currentProcess, &M1Queue);
						break;
					case 6: 
						currentProcess->state = waiting;
						enqueue(currentProcess, &M2Queue);
						break;
					case 7: 
						currentProcess->state = waiting;
						enqueue(currentProcess, &M3Queue);
						break;
					case 8: 
						currentProcess->state = waiting;
						enqueue(currentProcess, &M4Queue);
						break;	
					default: 
						break;  		
				}
			
				//if the process is still running, it must not have been interrupted
				//simulate the timer putting it back in the ready queue
				if (currentProcess->state == running){
					currentProcess->state = waiting;
					enqueue(currentProcess, &ReadyQueue);
				}
			}
    	} else {
    		printf("ReadyQueue is Empty! randomly dequeuing IO/Kernel Queues\n");
    	}
    	
    	// randomly complete IO requests (25% chance of clearing IO Queues)
    	r = rand() % 4;
    	if (r == 0){
			if(PrinterQueue.size>0){
				PCBNode* curNode = dequeue(&PrinterQueue);
				enqueue(curNode, &ReadyQueue);
				printf("Process %d IO Printer request completed, returning to ReadyQueue\n", curNode->id);
			}
			if(KeyboardQueue.size>0){
				PCBNode* curNode = dequeue(&KeyboardQueue);
				enqueue(curNode, &ReadyQueue);
				printf("Process %d IO Keyboard request completed, returning to ReadyQueue\n", curNode->id);
			}
			if(DiskQueue.size>0){
				PCBNode* curNode = dequeue(&DiskQueue);
				enqueue(curNode, &ReadyQueue);
				printf("Process %d IO Disk request completed, returning to ReadyQueue\n", curNode->id);
			}
			if(ModemQueue.size>0){
				PCBNode* curNode = dequeue(&ModemQueue);
				enqueue(curNode, &ReadyQueue);
				printf("Process %d IO Modem request completed, returning to ReadyQueue\n", curNode->id);
			}
		}
		
		r = rand() % 4;
    	if (r == 0){
			if(M1Queue.size>0){
				PCBNode* curNode = dequeue(&M1Queue);
				enqueue(curNode, &ReadyQueue);
				printf("Process %d M1 request completed, returning to ReadyQueue\n", curNode->id);
			}
			if(M2Queue.size>0){
				PCBNode* curNode = dequeue(&M2Queue);
				enqueue(curNode, &ReadyQueue);
				printf("Process %d M2request completed, returning to ReadyQueue\n", curNode->id);
			}
			if(M3Queue.size>0){
				PCBNode* curNode = dequeue(&M3Queue);
				enqueue(curNode, &ReadyQueue);
				printf("Process %d M3 request completed, returning to ReadyQueue\n", curNode->id);
			}
			if(M4Queue.size>0){
				PCBNode* curNode = dequeue(&M4Queue);
				enqueue(curNode, &ReadyQueue);
				printf("Process %d M4 request completed, returning to ReadyQueue\n", curNode->id);
			}
		}
		
// 		printf("Ready Queue Size: %d\n", ReadyQueue.size);
// 		printf("Printer Queue Size: %d\n", PrinterQueue.size);
// 		printf("Keyboard Queue Size: %d\n", KeyboardQueue.size);
// 		printf("Disk Queue Size: %d\n", DiskQueue.size);
// 		printf("ModemQueue Size: %d\n", ModemQueue.size);
// 	
// 		printf("M1Queue Size: %d\n", M1Queue.size);
// 		printf("M2Queue Size: %d\n", M2Queue.size);
// 		printf("M3Queue Size: %d\n", M3Queue.size);
// 		printf("M4Queue Size: %d\n", M4Queue.size);
// 		scanf("%d", &s);


    }
    
}