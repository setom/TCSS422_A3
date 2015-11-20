#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define	NODE_ARRAY_SIZE 4

typedef enum {newPCB, running, waiting, interrupted, halted} State;

//compare function for qsort (Help from Stack Overflow: c-array-sorting-tips)
int compare ( const void* a, const void* b) {
	int int_a = * ((int*)a);
	int int_b = * ((int*)b);
	
	if (int_a == int_b){
		return 0;
	} else if (int_a < int_b) {
		return -1;
	} else {
		return 1;
	}
}

//typedef PCBNode
typedef struct PCBNode {
	int id;
	int quanta;
	int count;
	State state;
	//I/O interrupt array values
	int IO_Printer[NODE_ARRAY_SIZE];
	int IO_Keyboard[NODE_ARRAY_SIZE];
	int IO_Disk[NODE_ARRAY_SIZE];
	int IO_Modem[NODE_ARRAY_SIZE];;
	//Kernel service array values
	int M1[NODE_ARRAY_SIZE];
	int M2[NODE_ARRAY_SIZE];
	int M3[NODE_ARRAY_SIZE];
	int M4[NODE_ARRAY_SIZE];
	struct PCBNode* next;
} PCBNode;

//create a new PCBNode
struct PCBNode* createPCBNode(int theId, int theQuanta) {
	PCBNode* pcb = (PCBNode*) malloc(sizeof(PCBNode));
	pcb->id = theId;
	pcb->quanta = theQuanta;
	pcb->count = 0;
	pcb->state = newPCB;
	//fill the IO and Kernel Arrays with random numbers between 10 - quanta
	//Try to avoid super small numbers because they will fire too many interrupts for the simulation
	int k;
	for (k = 0; k < NODE_ARRAY_SIZE; k++){
		pcb->IO_Printer[k] = (rand() % theQuanta) + 10; 
		pcb->IO_Keyboard[k] = (rand() % theQuanta) + 10; 
		pcb->IO_Disk[k] = (rand() % theQuanta) + 10; 
		pcb->IO_Modem[k] = (rand() % theQuanta) + 10; 
		
		pcb->M1[k] = (rand() % theQuanta) + 10; 
		pcb->M2[k] = (rand() % theQuanta) + 10; 
		pcb->M3[k] = (rand() % theQuanta) + 10; 
		pcb->M4[k] = (rand() % theQuanta) + 10; 
	}
	pcb->next = NULL;
	
	//sort the arrays so that they are in ascending order
	qsort(pcb->IO_Printer, 4, sizeof(int), compare);
	qsort(pcb->IO_Keyboard, 4, sizeof(int), compare);
	qsort(pcb->IO_Disk, 4, sizeof(int), compare);
	qsort(pcb->IO_Modem, 4, sizeof(int), compare);
	
	qsort(pcb->M1, 4, sizeof(int), compare);
	qsort(pcb->M2, 4, sizeof(int), compare);
	qsort(pcb->M3, 4, sizeof(int), compare);
	qsort(pcb->M4, 4, sizeof(int), compare);
	
	
	return pcb;
}

//destroy a PCBNode
void destroyPCBNode(struct PCBNode* pcbNode){
	free(pcbNode);
}

//Typedef of the Queue
typedef struct Queue {
	struct PCBNode* head;
	struct PCBNode* tail;
	int size;
	
	void (*enqueue) (struct PCBNode*, struct Queue*);
	struct PCBNode (*dequeue) (struct Queue*);
	struct PCBNode (*peek) (struct Queue*);

} Queue;

//method definitions

//enqueue takes a queue and some priority and id values
//generates a new pcbnode from those int values and adds it 
//to the queue
void enqueue (PCBNode* pcb, Queue* queue){

	//add it to the queue
	//if the queue doesn't exist, it is the new head
	//else, it is the next of the current tail
	if (queue->head == NULL){
		queue->head = pcb;
	} else {
		queue->tail->next = pcb;
	}
	
	//point the tail at the new node
	queue->tail = pcb;
	//increment the size of the queue
	queue->size++;

}
	
//dequeue takes a queue and dequeues the head of the queue
struct PCBNode* dequeue (Queue* queue){
	//get the head of the queue
	PCBNode* head = queue->head;
	
	//set the new head and decrement size
	queue->head = head->next;
	queue->size--;
	
	//return the head
	return head;
	
	//free the head *** IS THIS CODE REACHED? If not, where do i free it?
	free(head);
}

//peek peeks at the first item in a given queue
struct PCBNode peek (Queue* queue) {
	
	//get the head and return it
	PCBNode* head = queue->head;
	return *head;

}

//create a queue
struct Queue createQueue(){
	Queue queue;
	queue.size = 0;
	queue.head = NULL;
	queue.tail = NULL;
	//queue.enqueue = &enqueue;
	//queue.dequeue = &dequeue;
	//queue.peek = &peek;
	return queue;
}




