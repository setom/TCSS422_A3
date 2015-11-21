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
	//number of processes active
	int numProcesses = 0;
	//counter for loops
	int i, k;
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
		numProcesses++;
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
	printf("readyQueue Size: %d\n", ReadyQueue.size);
	while(numProcesses > 0){
		
		//run the first process in the Ready Queue
			PCBNode* currentProcess = dequeue(&ReadyQueue);
			currentProcess->state = running;
			currentProcess->count++;
			
		//- if quantum == total quantums, process is terminated
		//	restart	
			if (currentProcess->count == currentProcess->quanta){
				currentProcess->state = halted;
				printf("Process %d TERMINATED, Process completed %d quanta of %d total quanta\n", currentProcess->id, currentProcess->count, currentProcess->quanta);
				destroyPCBNode(currentProcess);
				numProcesses--;
				continue;
			} else {
			
				// - Check the current quanta against the randomly generated i/o values arrays
				//	if quanta % array value == 0, 
				//		State = blocked
				//		Enqueue Process pointer to I/O queue of the device that is blocked	
					for(k = 0; k < NODE_ARRAY_SIZE; k++){
						//use i as an interrupted flag
						i = 0;
						if((currentProcess->count % currentProcess->IO_Printer[k]) == 0){
							printf("Process %d, interrupted by I/O Printer Request at Quantum %d\n", currentProcess->id, currentProcess->count);
							currentProcess->state = waiting;
							enqueue(currentProcess, &PrinterQueue);
							i = 1;
							break;
						}
						if((currentProcess->count % currentProcess->IO_Keyboard[k]) == 0){
							printf("Process %d, interrupted by I/O Keyboard Request at Quantum %d\n", currentProcess->id, currentProcess->count);	
							currentProcess->state = waiting;
							enqueue(currentProcess, &KeyboardQueue);
							i = 1;
							break;
						}
						if((currentProcess->count % currentProcess->IO_Disk[k]) == 0){
							printf("Process %d, interrupted by I/O Disk Request at Quantum %d\n", currentProcess->id, currentProcess->count);
							currentProcess->state = waiting;
							enqueue(currentProcess, &DiskQueue);
							i = 1;
							break;
						}
						if((currentProcess->count % currentProcess->IO_Modem[k]) == 0){
							printf("Process %d, interrupted by I/O Modem Request at Quantum %d\n", currentProcess->id, currentProcess->count);
							currentProcess->state = waiting;
							enqueue(currentProcess, &ModemQueue);
							i = 1;
							break;
						}
					}
					//if there was an interrupt, restart the loop
					if(i == 1){
						continue;
					}
			
			
				//- Check the current quanta against the randomly generated kernel values arrays
				//	if quanta % array value == 0,
				//		State = blocked
				//		enqueue it in the proper kernel queue	
					for(k = 0; k < NODE_ARRAY_SIZE; k++){
						//use i as an interrupted flag
						i = 0;
						if((currentProcess->count % currentProcess->M1[k]) == 0){
							printf("Process %d, interrupted by M1 Kernel Request at Quantum %d\n", currentProcess->id, currentProcess->count);
							currentProcess->state = waiting;
							enqueue(currentProcess, &M1Queue);
							i = 1;
							break;
						}
						if((currentProcess->count % currentProcess->M2[k]) == 0){
							printf("Process %d, interrupted by M2 Kernel Request at Quantum %d\n", currentProcess->id, currentProcess->count);	
							currentProcess->state = waiting;
							enqueue(currentProcess, &M2Queue);
							i = 1;
							break;
						}
						if((currentProcess->count % currentProcess->M3[k]) == 0){
							printf("Process %d, interrupted by M3 Kernel Request at Quantum %d\n", currentProcess->id, currentProcess->count);
							currentProcess->state = waiting;
							enqueue(currentProcess, &M3Queue);
							i = 1;
							break;
						}
						if((currentProcess->count % currentProcess->M4[k]) == 0){
							printf("Process %d, interrupted by M4 Kernel Request at Quantum %d\n", currentProcess->id, currentProcess->count);
							currentProcess->state = waiting;
							enqueue(currentProcess, &M4Queue);
							i = 1;
							break;
						}
					}
					if(i == 1) {
						continue;
					}
					
					//put the node back in the ready queue
						currentProcess->state = waiting;
						enqueue(currentProcess, &ReadyQueue);
					
					
					
				//- for each I/O Queue
				//	randomly terminate the first process (is there a way to do this with timers instead? Needs 2 threads?
				//	& put it back in the ready queue		

					//select which queue we are going to randomly work on 
					r = rand() % 4;
					//determine if it terminates or not (0 = no, 1 = yes)
					//move back to ready queue as required
					r2 = rand() % 2;
					switch (r2) {
						case 0 : 
							break;
						case 1 : 
							switch(r) {
								case 0 : 
									if (PrinterQueue.size > 0){
										PCBNode* ioProcess = dequeue(&PrinterQueue);
										printf("Process %d I/O request completed, returning to Ready Queue\n", ioProcess->id);
										enqueue(ioProcess, &ReadyQueue);
									}
									if (M1Queue.size > 0){
										PCBNode* mProcess = dequeue(&M1Queue);
										printf("Process %d M1 Kernel request completed, returning to Ready Queue\n", mProcess->id);
										enqueue(mProcess, &ReadyQueue);
									}
									break;
								case 1 : 
									if (KeyboardQueue.size > 0){
										PCBNode* ioProcess = dequeue(&KeyboardQueue);
										printf("Process %d I/O request completed, returning to Ready Queue\n", ioProcess->id);
										enqueue(ioProcess, &ReadyQueue);
									}
									if (M2Queue.size > 0){
										PCBNode* mProcess = dequeue(&M2Queue);
										printf("Process %d M2 Kernel request completed, returning to Ready Queue\n", mProcess->id);
										enqueue(mProcess, &ReadyQueue);
									}
									break;
								case 2 : 
									if (DiskQueue.size > 0){
										PCBNode* ioProcess = dequeue(&DiskQueue);
										printf("Process %d I/O request completed, returning to Ready Queue\n", ioProcess->id);
										enqueue(ioProcess, &ReadyQueue);
									}
									if (M3Queue.size > 0){
										PCBNode* mProcess = dequeue(&M3Queue);
										printf("Process %d M3 Kernel request completed, returning to Ready Queue\n", mProcess->id);
										enqueue(mProcess, &ReadyQueue);
									}
									break;
								case 3 : 
									if (ModemQueue.size > 0){
										PCBNode* ioProcess = dequeue(&ModemQueue);
										printf("Process %d I/O request completed, returning to Ready Queue\n", ioProcess->id);
										enqueue(ioProcess, &ReadyQueue);
									}
									if (M4Queue.size > 0){
										PCBNode* mProcess = dequeue(&M4Queue);
										printf("Process %d M4 Kernel request completed, returning to Ready Queue\n", mProcess->id);
										enqueue(mProcess, &ReadyQueue);
									}
									break; 
							}
							break;		
					}
								
			}
	}	
	

}