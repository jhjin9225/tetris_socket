#pragma once
#include "tools.h"



// 큐를 생성한다.
ArrayQueue *createQueue(int max) {
	ArrayQueue *Queue = NULL;
	if (max > 0) {
		Queue = (ArrayQueue*)malloc(sizeof(ArrayQueue));
		if (Queue != NULL) { // 큐 초기화
			Queue->maxElementCount = max;
			Queue->currentElementCount = 0;
			Queue->rear = -1;
			Queue->front = -1;
		}
		else {
			printf("오류!, 메모리 할당오류!, createQueue()\n");
			return NULL;
		}
	}
	else {
		printf("오류!, 큐의 크기는 0보다 커야됩니다, createQueue()\n");
		return NULL;
	}

	Queue->pElement = (ArrayQueueNode*)malloc(sizeof(ArrayQueueNode)*max);
	if (Queue->pElement != NULL) {
		Queue->pElement->element = -1;
	}
	else {
		printf("오류!, 메모리할당2, createQueue()\n");
	}

	return Queue;
}
// 큐의 값을 꺼내온다.
ArrayQueueNode *deQueue(ArrayQueue* Queue) {
	ArrayQueueNode* qNode = NULL;

	if (Queue != NULL) {
		if (!isQueueEmpty(Queue)) {
			qNode = (ArrayQueueNode*)malloc(sizeof(ArrayQueueNode*));
			if (qNode != NULL) {
				Queue->front = (Queue->front + 1) % Queue->maxElementCount;	// 꺼내올 값의 위치를 front가 가르킨다. max를 넘길경우 다시 처음으로 간다.(circle Queue)
				*qNode = Queue->pElement[Queue->front];	// 반환할 노드를 저장한다.
				Queue->currentElementCount--;	//현재 노드갯수 증가
			}
			else {
				printf("오류!, 메모리할당 실패, deQueue()\n");
			}
		}
		else {
			printf("오류!, 큐가 비어있습니다. deQueue()\n");

		}
	}
	return qNode;
}
// 큐에 값을 저장한다.
int enQueue(ArrayQueue* Queue, ArrayQueueNode element) {
	int ret = FALSE;
	int i = 0;

	if (Queue != NULL) {
		if (!isQueueFull(Queue)) {
			Queue->rear = (Queue->rear + 1) % Queue->maxElementCount;// 큐의 최상단 값을 가르키는 rear 증가.
			Queue->pElement[Queue->rear] = element;	// 큐의 최상단에 값을 입력한다.
			Queue->currentElementCount++;	//현재 노드갯수 증가
		}
		else {
			printf("오류, 큐가 가득찼습니다., enQueue()\n");
		}
	}
	else {
		printf("오류, 빈큐., enQueue()\n");
	}
}
// 큐가 가득찼는지 확인한다.
int isQueueFull(ArrayQueue* Queue) {
	int ret = FALSE;
	if (Queue != NULL) {
		if (Queue->currentElementCount == Queue->maxElementCount) {
			ret = TRUE;
		}
	}
	return ret;
}
// 큐가 비어있는지 확인한다.
int isQueueEmpty(ArrayQueue* Queue) {
	int ret = FALSE;
	if (Queue != NULL) {
		if (Queue->currentElementCount == 0) {
			ret = TRUE;
		}
	}
	return ret;
}
// 큐를 삭제한다.
void deleteQueue(ArrayQueue* Queue) {
	if (Queue != NULL) {
		if (Queue->pElement != NULL) {
			free(Queue->pElement);
		}
		free(Queue);
	}
}
// 큐의 최하단의 값을 가져온다.(큐의 노드 수는 변하지 않는다.)
ArrayQueueNode *peek(ArrayQueue *Queue) {
	ArrayQueueNode* qNode = NULL;
	if (Queue != NULL) {
		if (isQueueEmpty(Queue) == FALSE) {
			qNode = (ArrayQueueNode*)malloc(sizeof(ArrayQueueNode));
			*qNode = Queue->pElement[Queue->front + 1];
		}
	}
	return qNode;
}
