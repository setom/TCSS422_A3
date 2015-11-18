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
	//int for random number generation
	srand(time(0));
	int r;
	int r2;
	
	
//1 - Create a Ready Queue and populate it with apx 30 processes
	Queue ReadyQueue = createQueue();
	r = ((rand() % 10) + 25);
	for (i = 0; i < r; i ++){
		//random number of quanta between 5000-25000
		r2 = ((rand() % 20000) + 5000);
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
	printf("readyQueue Size: %d\n", ReadyQueue.size);
	while(ReadyQueue.size > 0){
		
		//run the first process in the Ready Queue
			PCBNode* currentProcess = dequeue(&ReadyQueue);
			currentProcess->state = running;
			currentProcess->count++;
			
		//- if quantum == total quantums, process is terminated
		//	break	
			if (currentProcess->count == currentProcess->quanta){
				currentProcess->state = halted;
				printf("Process %d terminated, Process completed %d quanta of %d total quanta\n", currentProcess->id, currentProcess->count, currentProcess->quanta);
				destroyPCBNode(currentProcess);
				continue;
			} else {
			
				// - Check the current quanta against the randomly generated i/o values arrays
				//	if quanta % array value == 0, 
				//		State = blocked
				//		Enqueue Process pointer to I/O queue of the device that is blocked	
					for(k = 0; k < NODE_ARRAY_SIZE; k++){
						if((currentProcess->count % currentProcess->IO_Printer[k]) == 0){
							printf("Process %d, interrupted by I/O Printer Request at Quantum %d\n", currentProcess->id, currentProcess->count);
						}
						if((currentProcess->count % currentProcess->IO_Keyboard[k]) == 0){
							printf("Process %d, interrupted by I/O Keyboard Request at Quantum %d\n", currentProcess->id, currentProcess->count);	
						}
						if((currentProcess->count % currentProcess->IO_Disk[k]) == 0){
							printf("Process %d, interrupted by I/O Disk Request at Quantum %d\n", currentProcess->id, currentProcess->count);
						}
						if((currentProcess->count % currentProcess->IO_Modem[k]) == 0){
							printf("Process %d, interrupted by I/O Modem Request at Quantum %d\n", currentProcess->id, currentProcess->count);
						}
					}
			
				//- Check the current quanta against the randomly generated kernel values arrays
				//	if quanta % array value == 0,
				//		State = blocked
				//		enqueue it in the proper kernel queue	
					for(k = 0; k < NODE_ARRAY_SIZE; k++){
						if((currentProcess->count % currentProcess->M1[k]) == 0){
							printf("Process %d, interrupted by M1 Kernel Request at Quantum %d\n", currentProcess->id, currentProcess->count);
						}
						if((currentProcess->count % currentProcess->M2[k]) == 0){
							printf("Process %d, interrupted by M2 Kernel Request at Quantum %d\n", currentProcess->id, currentProcess->count);	
						}
						if((currentProcess->count % currentProcess->M3[k]) == 0){
							printf("Process %d, interrupted by M3 Kernel Request at Quantum %d\n", currentProcess->id, currentProcess->count);
						}
						if((currentProcess->count % currentProcess->M4[k]) == 0){
							printf("Process %d, interrupted by M4 Kernel Request at Quantum %d\n", currentProcess->id, currentProcess->count);
						}
					}
			
				//put the node back in the ready queue
					currentProcess->state = waiting;
					enqueue(currentProcess, &ReadyQueue);
			}
	}	
	

}