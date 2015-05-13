/*
	Name: kernel_types_que.h
	Desc: Que data structure a methods for it.
	Author: Karel Brezina (xbrezi13)
*/

#ifndef _KERNEL_TYPES_QUE_H_
#define _KERNEL_TYPES_QUE_H_

#define QUE_SIZE 100

typedef struct 
{
	int que[QUE_SIZE];
	int que_start;
	int que_end;
	int que_max;
} TQueI;

void initQueI(TQueI* que)
{
	que->que_start = 0;
	que->que_end = 0;
	que->que_max = QUE_SIZE;
}

void addQueItemI(TQueI* que, int val) 
{
	que->que[que->que_end] = val;
	que->que_end++;
	if (que->que_end >= QUE_SIZE) {
		que->que_end = 0;
	}
}

void removeQueItemI(TQueI* que) {
	if (que->que_start == que->que_end) {
		printf("Nothing to remove!%d\n", que->que_end);
		return;
	}
	que->que_start++;
	if (que->que_start >= QUE_SIZE) {
		que->que_start = 0;
	}
}

int getQueItemI(TQueI* que) {
	return que->que[que->que_start];
}

#endif // _KERNEL_TYPES_QUE_H_