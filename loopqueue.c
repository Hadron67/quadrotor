#include "loopqueue.h"
#define INCINDEX(a) \
	(a)++; \
	if((a) > LOOP_QUEUE_LENGTH) (a) = 0
//---------------------------------------------------------------------------------------------------
int queue_init(Queue* q){
	q->base = q->top = 0;
	return 0;
}
int queue_offer(Queue* q,char data) {
	//check
	if(q->top < q->base && q->base - q->top <=1) return -1;
	if(q->top > q->base && (q->top - q->base >= LOOP_QUEUE_LENGTH)) return -1;
	q->data[q->top] = data;
	INCINDEX(q->top);
	return 0;
}
int queue_pull(Queue* q,char* target){
	if(q->top == q->base) return -1;
	*target = q->data[q->base];
	INCINDEX(q->base);
	return 0;
}
int queue_front(Queue* q){
	if(q->top == q->base) return -1;
	return q->data[q->base];
}
