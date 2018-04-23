#pragma once
#include "tools.h"



// ť�� �����Ѵ�.
ArrayQueue *createQueue(int max) {
	ArrayQueue *Queue = NULL;
	if (max > 0) {
		Queue = (ArrayQueue*)malloc(sizeof(ArrayQueue));
		if (Queue != NULL) { // ť �ʱ�ȭ
			Queue->maxElementCount = max;
			Queue->currentElementCount = 0;
			Queue->rear = -1;
			Queue->front = -1;
		}
		else {
			printf("����!, �޸� �Ҵ����!, createQueue()\n");
			return NULL;
		}
	}
	else {
		printf("����!, ť�� ũ��� 0���� Ŀ�ߵ˴ϴ�, createQueue()\n");
		return NULL;
	}

	Queue->pElement = (ArrayQueueNode*)malloc(sizeof(ArrayQueueNode)*max);
	if (Queue->pElement != NULL) {
		Queue->pElement->element = -1;
	}
	else {
		printf("����!, �޸��Ҵ�2, createQueue()\n");
	}

	return Queue;
}
// ť�� ���� �����´�.
ArrayQueueNode *deQueue(ArrayQueue* Queue) {
	ArrayQueueNode* qNode = NULL;

	if (Queue != NULL) {
		if (!isQueueEmpty(Queue)) {
			qNode = (ArrayQueueNode*)malloc(sizeof(ArrayQueueNode*));
			if (qNode != NULL) {
				Queue->front = (Queue->front + 1) % Queue->maxElementCount;	// ������ ���� ��ġ�� front�� ����Ų��. max�� �ѱ��� �ٽ� ó������ ����.(circle Queue)
				*qNode = Queue->pElement[Queue->front];	// ��ȯ�� ��带 �����Ѵ�.
				Queue->currentElementCount--;	//���� ��尹�� ����
			}
			else {
				printf("����!, �޸��Ҵ� ����, deQueue()\n");
			}
		}
		else {
			printf("����!, ť�� ����ֽ��ϴ�. deQueue()\n");

		}
	}
	return qNode;
}
// ť�� ���� �����Ѵ�.
int enQueue(ArrayQueue* Queue, ArrayQueueNode element) {
	int ret = FALSE;
	int i = 0;

	if (Queue != NULL) {
		if (!isQueueFull(Queue)) {
			Queue->rear = (Queue->rear + 1) % Queue->maxElementCount;// ť�� �ֻ�� ���� ����Ű�� rear ����.
			Queue->pElement[Queue->rear] = element;	// ť�� �ֻ�ܿ� ���� �Է��Ѵ�.
			Queue->currentElementCount++;	//���� ��尹�� ����
		}
		else {
			printf("����, ť�� ����á���ϴ�., enQueue()\n");
		}
	}
	else {
		printf("����, ��ť., enQueue()\n");
	}
}
// ť�� ����á���� Ȯ���Ѵ�.
int isQueueFull(ArrayQueue* Queue) {
	int ret = FALSE;
	if (Queue != NULL) {
		if (Queue->currentElementCount == Queue->maxElementCount) {
			ret = TRUE;
		}
	}
	return ret;
}
// ť�� ����ִ��� Ȯ���Ѵ�.
int isQueueEmpty(ArrayQueue* Queue) {
	int ret = FALSE;
	if (Queue != NULL) {
		if (Queue->currentElementCount == 0) {
			ret = TRUE;
		}
	}
	return ret;
}
// ť�� �����Ѵ�.
void deleteQueue(ArrayQueue* Queue) {
	if (Queue != NULL) {
		if (Queue->pElement != NULL) {
			free(Queue->pElement);
		}
		free(Queue);
	}
}
// ť�� ���ϴ��� ���� �����´�.(ť�� ��� ���� ������ �ʴ´�.)
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
