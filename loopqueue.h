#ifndef __LOOP_QUEUE
#define __LOOP_QUEUE
#define LOOP_QUEUE_LENGTH 20
typedef struct _queue{
	char data[LOOP_QUEUE_LENGTH + 1];
	int base;
	int top;
}Queue;
int queue_init(Queue *q);
int queue_offer(Queue *q,char data);
int queue_pull(Queue *q,char *target);
int queue_front(Queue *q);
#endif
