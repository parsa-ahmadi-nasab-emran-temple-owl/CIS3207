#include <stdio.h>
#include <stdlib.h>
#include "readFile.c"

//GLOBAL VARIABLES
/*Initialize constants from reading config.txt file*/
int SEED = 0; 
int INIT_TIME = 0;
int	FIN_TIME = 20000;
int ARRIVE_MIN = 10;
int ARRIVE_MAX = 30;
float QUIT_PROB = 0;
int CPU_MIN = 20;
int CPU_MAX = 50;
int DISK1_MIN = 100;
int DISK1_MAX = 150;
int DISK2_MIN = 200;
int DISK2_MAX = 300;

#define ARRIVAL 0
#define FINISH 1
#define DISK_ARRIVAL 2
#define DISK_FINISH 3
#define SIMULATION_END 4
#define IDLE 0
#define BUSY 1

/*******************************
DATA STRUCTURE
- 3 FIFO QUEUES as LinkedList
- 1 Priority queue as LinkedList
*******************************/

typedef struct event{
	int time;	//time 
	int jobNo;	//job number, program should have a job counter. each time a event goes back to the cpu for further processing. number of jobs should be incremented by one
	int type;
}event;

typedef struct node{
	struct event job;
	struct node *next;
}node;

/*FIFO QUEUE FUNCTIONS*/
void append(node**,event,int*);
event removeFirst(node**,int*);

/*PRIORTY QUEUE FUNCTIONS*/
void p_append(node**,event,int*);
event p_removeFirst(node**,int*);

event createEvent(int, int, int);
int randomBetween(int, int);

void process_CPU(event);
void process_DISK(event);
//void handleCPUQueue();
event removeJobFromDisk(int);
void recordEvent(char*);

int currentTime = 0, randTime = 0;
node* CPU = NULL, *DISK_1 = NULL, *DISK_2 = NULL; //3 FIFO QUEUES
node* eventQ = NULL; //1 Priority Queue
event task;
int cpuNodes = 0, disk1Nodes = 0, disk2Nodes = 0, eventNodes = 0, numJobs = 1;
int cpuState = IDLE, disk1State = IDLE, disk2State = IDLE;

int main(){

	SEED = atoi(getValue("config.txt","SEED",0,1));
	INIT_TIME = atoi(getValue("config.txt","INIT_TIME",0,1));
	FIN_TIME = atoi(getValue("config.txt","FIN_TIME",0,1));
	ARRIVE_MIN = atoi(getValue("config.txt","ARRIVE_MIN",0,1));
	ARRIVE_MAX = atoi(getValue("config.txt","ARRIVE_MAX",0,1));
	QUIT_PROB = atof(getValue("config.txt","QUIT_PROB",0,1));
	CPU_MIN = atoi(getValue("config.txt","CPU_MIN",0,1));
	CPU_MAX = atoi(getValue("config.txt","CPU_MAX",0,1));
	DISK1_MIN = atoi(getValue("config.txt","DISK1_MIN",0,1));
	DISK1_MAX = atoi(getValue("config.txt","DISK1_MAX",0,1));
	DISK2_MIN = atoi(getValue("config.txt","DISK2_MIN",0,1));
	DISK2_MAX = atoi(getValue("config.txt","DISK2_MAX",0,1));

	//DEBUGGING
	printf("SEED = %d\n",SEED);
	printf("INIT_TIME = %d\n",INIT_TIME);
	printf("FIN_TIME = %d\n",FIN_TIME);
	printf("ARRIVE_MIN = %d\n",ARRIVE_MIN);
	printf("ARRIVE_MAX = %d\n",ARRIVE_MAX);
	printf("QUIT_PROB = %.2f\n",QUIT_PROB);
	printf("CPU_MIN = %d\n",CPU_MIN);
	printf("CPU_MAX = %d\n",CPU_MAX);
	printf("DISK1_MIN = %d\n",DISK1_MIN);
	printf("DISK1_MAX = %d\n",DISK1_MAX);
	printf("DISK2_MIN = %d\n",DISK2_MIN);
	printf("DISK2_MAX = %d\n",DISK2_MAX);
	printf("currentTime = %d\n",currentTime);
	printf("randTime = %d\n",randTime);

	char* value;
	srand(SEED);
	currentTime = INIT_TIME;

	//add two events to priorty queue, job1 arrival and simulation finished
	p_append(&eventQ,createEvent(INIT_TIME,1,ARRIVAL),&eventNodes);
	p_append(&eventQ,createEvent(FIN_TIME,0,SIMULATION_END),&eventNodes);
	//while queue is not empty
	printf("Done\n");
	printf("eventNodes: %d\n", eventNodes);
	while(eventNodes != 0 && currentTime < FIN_TIME){
		//pop events from event queue and place them inside cpu queue
		task = p_removeFirst(&eventQ,&eventNodes);
		currentTime = task.time;
		//Event Handler
		switch(task.type){
			case ARRIVAL: process_CPU(task); break;
			case FINISH: process_CPU(task); break;
			case DISK_ARRIVAL: process_DISK(task); break;
			case DISK_FINISH: process_DISK(task); break;
			case SIMULATION_END: break;
		}
	}
	return 0;
}

void append(node** head_ref, event data, int* total){
	node* new_node = (node*) malloc(sizeof(node));
	new_node->job = data;
	new_node->next = NULL;
	
	//start at beginning of list	
	node *last = *head_ref;
	(*total)++;
	if (*head_ref == NULL){
		*head_ref = new_node;
		return;
	}
	while (last->next != NULL)
		last = last->next;

	last->next = new_node;
	return;
}

event removeFirst(node** head_ref, int* total){
	node *temp = *head_ref;
	*head_ref = (*head_ref)->next;
	event job = temp->job;
	free(temp);
	(*total)--;
	return job;
}

void p_append(node** head_ref, event data, int* total){
	node* temp = (node*) malloc(sizeof(node));
	temp->job = data;
	temp->next = NULL;

	int priority = temp->job.time; //time of event will be the priorty
	printf("Job number: %d, Job Time: %d\n",temp->job.time,temp->job.jobNo);
	//printf("inserting priority: %d\n",priority);
	(*total)++;
	//start at beginning of list	
	node *last = *head_ref;

	if (*head_ref == NULL){ 
		*head_ref = temp;
		return;
	}else if(priority < last->job.time){
		temp->next = last;
		*head_ref = temp; // have the pointer to the linkedlist point to the very start of the node which would be temp
		return;
	}

	//traverse till we get to the end of list OR if we find the correct position where the node belongs
	while (last->next != NULL && priority >= last->next->job.time)
		//printf("%d\n",last->priority);
		last = last->next;
	
	temp->next = last->next;
	last->next = temp;
	return;
}

event p_removeFirst(node** head_ref, int* total){
	node *temp = *head_ref;
	*head_ref = (*head_ref)->next;
	event job = temp->job;
	free(temp);
	(*total)--;
	return job;
}

event createEvent(int time, int jobNo, int type){
	event job;
	job.time = time;
	job.jobNo = jobNo;
	job.type = type;
	return job; 
}

int randomBetween(int min, int max){
	return (rand() % (max - min + 1)) + min; 
}


/**********************************************************************
	If task arrived at the CPU,
	  	- create new event
	  	- append event to event queue
	  	- send task to CPU FIFO
	  		If cpuState is BUSY
	  			do nothing (which means the event will  have to sit in CPU FIFO queue)
			If cpuState is IDLE
			  1. pop task from CPU FIFO
			  2. generate finish time with CPU_MIN & CPU_MAX
			  3. create cpu finish event
			  4. insert event in event queue
			  5. set cpuState to BUSY
	If task is finished at the CPU,
		- set cpuState to IDLE
		- prob(exit,use disk)
			if exit
				do nothing?
			if use disk
				create disk arrival event
				insert event in event queue
	Input arguments:
		struct event task - task popped from event queue 
**********************************************************************/
void process_CPU(event task){
	char str[80];
	if(task.type == ARRIVAL){
		sprintf(str, "At time %d, Job%d arrives.",currentTime,task.jobNo);
		recordEvent(str);
		/*
		1. determine the arrival time for the next job to enter the system and add it as an event to the priority queue 
			- 	To determine the time of the next arrival, we generate a random integer
				between ARRIVE_MIN and ARRIVE_MAX and add it to the current time. This will be the time of the 2nd arrival.*/
		randTime = randomBetween(ARRIVE_MIN,ARRIVE_MAX) + currentTime; numJobs++;
		p_append(&eventQ,createEvent(randTime,numJobs,ARRIVAL),&eventNodes); 
		append(&CPU,task,&cpuNodes); // job sent to the CPU

		/*When jobs reach some component (CPU, disk1, or disk2), if that component is free, the job begins service there immediately.
		 If, on the other hand, that component is busy servicing someone else, the job must wait in that component's queue.*/ 
		if (cpuState == IDLE){
			task = removeFirst(&CPU,&cpuNodes);
			randTime = randomBetween(CPU_MIN,CPU_MAX) + currentTime;
			p_append(&eventQ,createEvent(randTime,task.jobNo,FINISH),&eventNodes); // adds event to priorty queue about when cpu finishes
			cpuState = BUSY;
		}
	}else{
		sprintf(str, "At time %d, Job%d finishes at CPU.",currentTime,task.jobNo);
		recordEvent(str);
		//set CPU state to IDLE
		cpuState = IDLE;
		/*Once a job has finished a round of processing at the CPU, the probability that 
		  it exits the system (instead of doing a disk read and then further computation) is QUIT_PROB.*/
		int quit = rand() <  QUIT_PROB * ((double)RAND_MAX + 1.0);
		//printf("quit is: %d\n",quit);
		if(quit){
			printf("job %d exited exited CPU\n",task.jobNo);
			sprintf(str, "At time %d, Job%d exits.",currentTime,task.jobNo);
			recordEvent(str);
		}else{
			/*When a job needs to do disk I/O, it uses the disk that's the least busy, i.e., the disk whose queue is the shortest. 
			  (This might seem a bit silly, but we can pretend that each disk has the same information.) 
			  If the disk queues are of equal length, choose one of the disks at random.*/
			randTime = randomBetween(ARRIVE_MIN,ARRIVE_MAX) + currentTime;
			p_append(&eventQ,createEvent(randTime,task.jobNo,DISK_ARRIVAL),&eventNodes);
		}
	}	
}

/**********************************************************************
	If task arrived at the DISK,
	  	- select shortest disk. if both disk are equal, randomly pick one to use.
	  	- send event to chosen disk FIFO
		  	If diskState is BUSY
		  		do nothing (which means the event will have to sit in DISK FIFO queue)
		  	If diskState is IDLE
		  		1. pop task from disk FIFO
			  	2. generate finish time with DISK_MIN & DISK_MAX
			  	3. create disk finish event
			  	4. insert event in event queue
			  	5. set diskState to IDLE;
	If task is finished at the DISK,
		- find corresponding job (in disk 1 or 2)
		- create CPU arrival event
		- insert event inside eventQ
		- diskState = IDLE
	Input arguments:
		struct event task - task popped from event queue 
**********************************************************************/
void process_DISK(event task){
	char str[80];
	int sendTo = 0; // holder that determines what disk the event will be sent to
	if (task.type == DISK_ARRIVAL){
		sprintf(str, "At time %d, Job%d arrives at Disk.",currentTime,task.jobNo);
		recordEvent(str);
		if(disk1Nodes < disk2Nodes){
			sendTo = 1;
		}else if(disk1Nodes > disk2Nodes){
			sendTo = 2;
		}else{
			sendTo = (rand() <  0.5 * ((double)RAND_MAX + 1.0)) + 1;
			//printf("sendTo is : %d\n",sendTo);
		}


		if(sendTo == 1){
			append(&DISK_1,task,&disk1Nodes);
			printf("added to disk: %d\n",task.jobNo );
			if(disk1State == IDLE){
				event job = p_removeFirst(&DISK_1,&disk1Nodes);
				printf("popped from disk: %d\n",job.jobNo );
				randTime = randomBetween(DISK1_MIN,DISK1_MAX) + currentTime;
				p_append(&eventQ,createEvent(randTime,job.jobNo,DISK_FINISH),&eventNodes);
				disk1State = BUSY;

			}
		}else if(sendTo == 2){
			append(&DISK_2,task,&disk2Nodes);
			//printf("added to disk2: %d\n",task.jobNo );
			if(disk2State == IDLE){
				event job = p_removeFirst(&DISK_2,&disk2Nodes);
				//printf("popped from disk2: %d\n",job.jobNo );
				randTime = randomBetween(DISK2_MIN,DISK2_MAX) + currentTime;
				p_append(&eventQ,createEvent(randTime,job.jobNo,DISK_FINISH),&eventNodes);
				disk2State = BUSY;
				
			}
		}
	}else{
		printf("disk jobno is %d\n",task.jobNo);
		event job = removeJobFromDisk(task.jobNo);
		randTime = randomBetween(ARRIVE_MIN,ARRIVE_MAX) + currentTime;
		p_append(&eventQ,createEvent(randTime,task.jobNo,ARRIVAL),&eventNodes);
		if(sendTo == 1){
			sprintf(str, "At time %d, Job%d finished I/O at Disk 1.",currentTime,task.jobNo);
			recordEvent(str);
			disk1State = IDLE;
		}else{
			sprintf(str, "At time %d, Job%d finished I/O at Disk 2.",currentTime,task.jobNo);
			recordEvent(str);
			disk1State = IDLE;
		}


		/*event job = findJob(&DISK_1,task.jobNo);
		if(job == NULL)
			job = findJob(&DISK_2,task.jobNo);
		if(job == NULL){
			printf("ERROR FINDING JOB IN DISKS\n");
			exit(0);
		}*/

	}
}
event removeJobFromDisk(int jobNo){
	node **head_ref = &DISK_1;
	node **last = NULL;
	node **temp = NULL;
	int from = 1;

	while(*head_ref != NULL && (*head_ref)->job.jobNo != jobNo){
		last = head_ref;
		(*head_ref) = (*head_ref)->next;
		printf("d job number %d\n",(*head_ref)->job.jobNo);
	}
	if(*head_ref == NULL){
		head_ref = &DISK_2;
		from = 2;
		while(*head_ref != NULL && (*head_ref)->job.jobNo != jobNo){
			last = head_ref;
			*head_ref = (*head_ref)->next;
			printf("d2 job number %d\n",(*head_ref)->job.jobNo);
		}
	}
	if(*head_ref == NULL){
		printf("ERROR FINDING JOB\n");
		exit(0);
	}
	if(*last == NULL){
		if(from == 1){
			*temp = DISK_1;
			DISK_1 = DISK_1->next;
			free(temp);
			disk1Nodes--;
		}else{
			*temp = DISK_2;
			DISK_2 = DISK_2->next;
			free(temp);
			disk2Nodes--;
		}
	}else{
		*last = (*head_ref)->next;
		if(from ==1){
			disk1Nodes--;
		}else{
			disk2Nodes--;
		}

	}
	return (*head_ref)->job;
}
void recordEvent(char *line){ 
	FILE * fp;
	/* open the file for writing*/
	fp = fopen ("log.txt","a");
	fprintf (fp,"%s\n",line);
 
   /* close the file*/  
   fclose (fp);
}
/*event findJob(node** head_ref,int jobNo){
	node *node = *head_ref;
	while(node != NULL && node.jobNo != jobNo){
		node = node->next;
	}
	if(node.jobNo == jobNo)
		return node;
	return NULL;

}*/
