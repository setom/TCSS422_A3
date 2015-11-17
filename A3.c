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
	//counter for processes
	int Proc_ID = 0;
	//counter for loops
	int i;
	//int for random number generation
	srand(time(0));
	int r;
	int r2;
	//create a Ready queue and populate with with apx 30 processes
	Queue ReadyQueue = createQueue();
	r = ((rand() % 10) + 25);
	for (i = 0; i < r; i ++){
		//random number of quanta between 5000-25000
		r2 = ((rand() % 20000) + 5000);
		PCBNode* pcb = createPCBNode(Proc_ID, r2);
		enqueue(pcb, &ReadyQueue);
		printf("Enqueued PCB %d, Quanta: %d\n", pcb->id, pcb->quanta);
		Proc_ID++;
	}
	

}