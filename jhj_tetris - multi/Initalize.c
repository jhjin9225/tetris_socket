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


// 게임의 최초화면을 설정한다.
void initMain() {
	srand((unsigned)time(NULL)); // 시간으로 랜덤 설정
	setcursortype(NOCURSOR); //커서 깜박임 제거
	resz(80, 24);	//콘솔의 크기 설정
}

// 게임시작전 필요한 것들을 초기화한다.
void initGame() {
	readyCnt = 0;

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

	if (ISMULTI) {
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
	if (blockQueue == NULL)	blockQueue = createQueue(Q_MAX);

	if (ISSINGLE) {
		for (int i = 0; i < Q_FILL_SIZE; i++) {
			newBlock.element = random(7);	//랜덤으로 블록을 지정해 큐에 저장한다.
			enQueue(blockQueue, newBlock);
		}
	}
	else if (ISSERVER) {
		char msg[1024] = { 0 };
		msg[0] = 'Q';

		for (int i = 0; i < Q_FILL_SIZE; i++) {
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