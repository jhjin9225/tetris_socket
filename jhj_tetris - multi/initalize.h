#pragma once
#include "tools.h"

// 게임시작전 필요한 것들을 초기화한다.
void initGame() {
	// 게임모드별 설정을 한다.
	if (ISSINGLE || mode == -1) {
		isStartGame = TRUE;
		strcpy(myName, "player 1");
	}
	else {
		resz(60 + 27 * clientCount, 24);
	}
	clrscr();	// 화면 초기화

	initBlock();

	// initSingleNextBlock
	if (ISSINGLE)	//!멀티모드는 다른곳에서 Q초기화.
		initQueueBlockes();
	nextBlock[0] = deQueue(blockQueue)->element;
	nextBlock[1] = deQueue(blockQueue)->element;

	printStage();	//블록이내려오는 곳을 출력한다.

	if (!ISSINGLE) {
		printOtherPlayer();
		winCnt = 0;
	}

	printBlockSpace();
	printInfo();
	printNextBlock();
	printHoldBlock();

	setColor(WHITE);
	putsxy(28 - (int)(strlen(myName) / 2), 22, myName);	// 사용자의 이름을 출력한다.

}

// 블럭을 초기화한다.
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

// 생성될 블럭들을 Queue에 담아 초기화한다.
void initQueueBlockes() {
	if (ISSINGLE) {
		blockQueue = createQueue(Q_MAX);	

		for (int i = 0; i < Q_MAX; i++) {
			newBlock.element = random(7);	//랜덤으로 블록을 지정해 큐에 저장한다.
			enQueue(blockQueue, newBlock);
		}
	}
	else if (ISSERVER) {
		char msg[1024] = { 0 };
		msg[0] = 'Q'; 
		blockQueue = createQueue(Q_MAX);

		for (int i = 0; i < Q_MAX; i++) {
			newBlock.element = random(7);	//랜덤으로 블록을 지정해 큐에 저장한다.
			enQueue(blockQueue, newBlock);
			msg[i + 1] = newBlock.element + 0x30;
		}
		txSock(msg);	// 큐에 저장할 블록을 서버로 보낸다.
	}
	else if (ISCLIENT) {
		while (!isRecvQ) { delay(10); }	// 서버에서 큐를 넘겨줄때까지 대기.
	}
}

