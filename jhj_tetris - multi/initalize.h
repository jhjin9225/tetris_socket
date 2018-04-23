#pragma once
#include "tools.h"

// ���ӽ����� �ʿ��� �͵��� �ʱ�ȭ�Ѵ�.
void initGame() {
	// ���Ӹ�庰 ������ �Ѵ�.
	if (ISSINGLE || mode == -1) {
		isStartGame = TRUE;
		strcpy(myName, "player 1");
	}
	else {
		resz(60 + 27 * clientCount, 24);
	}
	clrscr();	// ȭ�� �ʱ�ȭ

	initBlock();

	// initSingleNextBlock
	if (ISSINGLE)	//!��Ƽ���� �ٸ������� Q�ʱ�ȭ.
		initQueueBlockes();
	nextBlock[0] = deQueue(blockQueue)->element;
	nextBlock[1] = deQueue(blockQueue)->element;

	printStage();	//����̳������� ���� ����Ѵ�.

	if (!ISSINGLE) {
		printOtherPlayer();
		winCnt = 0;
	}

	printBlockSpace();
	printInfo();
	printNextBlock();
	printHoldBlock();

	setColor(WHITE);
	putsxy(28 - (int)(strlen(myName) / 2), 22, myName);	// ������� �̸��� ����Ѵ�.

}

// ���� �ʱ�ȭ�Ѵ�.
void initBlock() {
	stageData[0][0] = 7;
	memcpy(stage, stageData, sizeof(stage));
	save_x = STARTX;
	save_y = STARTY;
	holdBlock = -1;
	save_block = 0;
	save_motion = 0;
	score = 0;
	leveltime = 1000;
	isBlockCreated = FALSE;
}

// ������ ������ Queue�� ��� �ʱ�ȭ�Ѵ�.
void initQueueBlockes() {
	if (ISSINGLE) {
		blockQueue = createQueue(Q_MAX);	

		for (int i = 0; i < Q_MAX; i++) {
			newBlock.element = random(7);	//�������� ����� ������ ť�� �����Ѵ�.
			enQueue(blockQueue, newBlock);
		}
	}
	else if (ISSERVER) {
		char msg[1024] = { 0 };
		msg[0] = 'Q'; 
		blockQueue = createQueue(Q_MAX);

		for (int i = 0; i < Q_MAX; i++) {
			newBlock.element = random(7);	//�������� ����� ������ ť�� �����Ѵ�.
			enQueue(blockQueue, newBlock);
			msg[i + 1] = newBlock.element + 0x30;
		}
		txSock(msg);	// ť�� ������ ����� ������ ������.
	}
	else if (ISCLIENT) {
		while (!isRecvQ) { delay(10); }	// �������� ť�� �Ѱ��ٶ����� ���.
	}
}

