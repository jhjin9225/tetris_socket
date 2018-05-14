#include "Tetris_Header.h"

extern int stageData[22][12];
extern int stage[22][12];
extern int holdBlock;
extern int save_x;
extern int save_y;
extern int save_block;
extern int save_motion;
extern int score;
extern int nextBlock[2];
extern int leveltime;
extern int clientCount;
extern int mode;
extern int winCnt;
extern int readyCnt;
extern char myName[50];
extern boolean isBlockCreated;
extern boolean isStartGame;
extern boolean isRecvQ;
extern ArrayQueueNode newBlock;
extern ArrayQueue *blockQueue;


// ������ ����ȭ���� �����Ѵ�.
void initMain() {
	srand((unsigned)time(NULL)); // �ð����� ���� ����
	setcursortype(NOCURSOR); //Ŀ�� ������ ����
	resz(80, 24);	//�ܼ��� ũ�� ����
}

// ���ӽ����� �ʿ��� �͵��� �ʱ�ȭ�Ѵ�.
void initGame() {
	readyCnt = 0;

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

	if (ISMULTI) {
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
	if (blockQueue == NULL)	blockQueue = createQueue(Q_MAX);

	if (ISSINGLE) {
		for (int i = 0; i < Q_FILL_SIZE; i++) {
			newBlock.element = random(7);	//�������� ����� ������ ť�� �����Ѵ�.
			enQueue(blockQueue, newBlock);
		}
	}
	else if (ISSERVER) {
		char msg[1024] = { 0 };
		msg[0] = 'Q';

		for (int i = 0; i < Q_FILL_SIZE; i++) {
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