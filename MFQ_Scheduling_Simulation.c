#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>


typedef enum { false, true } bool;
typedef enum {lock,unlock} processor;

#pragma pack(push, 1)

typedef struct {
	int pid;
	int at;
	int iq;
	int ncycle;
	int* cycle;
	int bust;
	int pat, ptt, pwt;
}Pcb;
#pragma pack(pop) //process

typedef struct {
	int front, rear;
	int* pcs;
}Queue; //queue

void InitQueue(Queue* pqueue,int size);
bool IsFull(Queue* pqueue,int size);
bool IsEmpty(Queue* pqueue);
void EnQueue(Queue* pqueue, int size, int pn);
void DeQueue(Queue* pqueue,int size);
int Peek(Queue* pqueue);//queue



int time = 0;
int runp = -1;
int ctq = 0;
int end = 0;
float meantt = 0;
float meanwt = 0;

int main() {
	int n = 0; //
	FILE* fp = fopen("input.txt", "r");
	fscanf(fp, "%d", &n);
	printf("%d\n", n);
	Pcb* pc = malloc(n * (sizeof(Pcb)));

	for (int i = 0; i < n; i++) {
		fscanf(fp,"%d %d %d %d",&pc[i].pid, &pc[i].at, &pc[i].iq, &pc[i].ncycle);
		pc[i].cycle = malloc(sizeof(int) * ((2 * pc[i].ncycle) - 1));
		pc[i].bust = 0;
		pc[i].pwt = 0;
		pc[i].pat = pc[i].at;
		printf("%d %d %d %d ", pc[i].pid, pc[i].at, pc[i].iq, pc[i].ncycle);

		for (int j = 0; j < 2 * pc[i].ncycle - 1; j++) {
			fscanf(fp, "%d", &pc[i].cycle[j]);
			printf("%d ", pc[i].cycle[j]);
			pc[i].pwt = pc[i].pwt - pc[i].cycle[j];
		}
		
		printf("\n"); // malloc: pc, pc[i].cycle
	}

	//
	Queue q[4];
	for (int i = 0; i < 4; i++) {
		InitQueue(q+i,n+1); //
	}
	processor core = unlock;

	//

	//
	printf("\n\n  Gantt Chart \n");
	printf(" --------------\n");
	printf(" | Time | PID |\n");
	printf(" --------------\n");

	while (end < n) {
		for (int i = 0; i < n; i++) {
			if (pc[i].at == time)
				EnQueue(q + pc[i].iq, n+1, pc[i].pid);
		}
		//ready Queue
		if (core == unlock) {
			if (IsEmpty(q)) {
				if (IsEmpty(q + 1)) {
					if (IsEmpty(q + 2)) {
						if (IsEmpty(q + 3));
						else {
							runp = (Peek(q + 3) - 1);
							ctq = -1;
							DeQueue(q + 3, n+1);
							core = lock;
						}
					}
					else {
						runp = (Peek(q + 2) - 1);
						ctq = 8;
						DeQueue(q + 2, n+1);
						core = lock;
					}
				}
				else {
					runp = (Peek(q + 1) - 1);
					ctq = 4;
					DeQueue(q + 1, n+1);
					core = lock;
				}
			}
			else {
				runp = (Peek(q ) - 1);
				ctq = 2;
				DeQueue(q + 0, n+1);
				core = lock;
			}
		}//
		time++;
		if (core == lock) {
			pc[runp].cycle[pc[runp].bust]--;
			ctq--;
			printf(" | % 4d | P%d |", time, pc[runp].pid);
			if (pc[runp].cycle[pc[runp].bust] == 0) {
				if (pc[runp].bust == (2 * (pc[runp].ncycle - 1))) {
					end++;
					pc[runp].at = -1;
					pc[runp].ptt = time - pc[runp].pat;
					pc[runp].pwt = pc[runp].ptt + pc[runp].pwt;
					printf("  out: p%d", runp+1);
				}//
				else {
					pc[runp].at = time + pc[runp].cycle[pc[runp].bust + 1];
					if (ctq >= 0) {
						pc[runp].iq--;
					}
					pc[runp].bust = pc[runp].bust + 2;
				}//
				core = unlock;
			}
			else if (ctq == 0) {
				pc[runp].iq++;
				pc[runp].at = time;
				core = unlock;
			}//
			printf("\n");
		}//cpu burst
		else
			printf(" | % 4d |null|\n", time);

	}
	printf(" --------------\n");

	//
	//
	printf(" -----------------------------------------------------\n");
	printf(" | Process ID : Turnaround Time(TT)  Wating Time (WT)|\n");
	printf(" -----------------------------------------------------\n");
	for (int i = 0; i < n; i++) {
		printf(" | Process %2d :  %15d  %18d |\n",pc[i].pid, pc[i].ptt, pc[i].pwt);
		meantt = meantt + pc[i].ptt;
		meanwt = meanwt + pc[i].pwt;
	}
	printf(" -----------------------------------------------------\n");
	printf(" | mean of TT is %35f |\n", meantt / n);
	printf(" | mean of WT is %35f |\n", meanwt / n);
	printf(" -----------------------------------------------------\n");

	//
	for (int i = 0; i < 4; i++) {
		free(q[i].pcs);
	}
	for (int i = 0; i < n; i++) { 
		free(pc[i].cycle);
	}
	free(pc);
	//
	fclose(fp); //
	return 0;

}


//
void InitQueue(Queue* pqueue,int size) {
	pqueue->front = pqueue->rear = 0;
	pqueue->pcs = malloc(sizeof(int) * size);
}

bool IsFull(Queue* pqueue,int size) {
	return pqueue->front == (pqueue->rear + 1) % size;
}

bool IsEmpty(Queue* pqueue) {
	return pqueue->front == pqueue->rear;
}

void EnQueue(Queue* pqueue,int size,int pn) {
	if (IsFull(pqueue, size))
		exit(1);
	pqueue->pcs[pqueue->rear] = pn;
	pqueue->rear = (pqueue->rear + 1) % size;
}

void DeQueue(Queue* pqueue, int size) {
	if (IsEmpty(pqueue))
		exit(1);
	pqueue->front = (pqueue->front + 1) % size;
}

int Peek(Queue* pqueue) {
	if (IsEmpty(pqueue))
		exit(1);
	return pqueue->pcs[pqueue->front];
}
