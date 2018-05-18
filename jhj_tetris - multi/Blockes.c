#include "Tetris_Header.h"

extern int mode;
extern int blocks[7][4][4][2];
extern int save_x;
extern int save_block;
extern int save_motion;
extern int save_y;
extern int save_pv_y;
extern int save_pv_x;
extern int score;
extern int readyCnt;
extern int nextBlock[2];
extern int holdBlock;
extern int holdMotion;
extern int stage[22][12];
extern int combo;
extern int attackCnt;
extern boolean isPreViewMode;
extern boolean isBlockCreated;
extern boolean isStartGame;
extern boolean msgState;
extern boolean ishold;
extern CRITICAL_SECTION cs;
extern ArrayQueue *blockQueue;
extern ArrayQueueNode newBlock;


// 블록을 x,y에 출력한다.
void createBlock(int x, int y, int block, int motion) {
	int i;
	selectColor(block);
	for (i = 0; i < 4; i++) {
		// x,y 위치부터 block에 저장된 block값과 motion값의 블록을 +x +y 위치값을 더한 4개의 ■를 출력 
		putsxy((x + blocks[block][motion][i][0] * 2), y + blocks[block][motion][i][1], "■"); 
	}
}

// 블록을 x,y에서 지운다.
void removeBlock() {
	int i;
	for (i = 0; i < 4; i++) {
		// x,y 위치부터 block에 저장된 block값과 motion값의 블록을 +x +y 위치값을 더한 4개의 "  "(2byte공백)을 출력 
		putsxy((save_x + blocks[save_block][save_motion][i][0] * 2), save_y + blocks[save_block][save_motion][i][1], "  ");
	}
}

// 내려오는 블럭의 예상위치를 출력한다.
void createPreViewBlock(int x, int y, int block, int motion) {
	int pvY = 1;
	int i;
	while (!checkPreViewWalls(block, motion, x, y, pvY)) { ++pvY; }
	selectColor(block);
	for (i = 0; i < 4; i++) {
		putsxy((x + blocks[block][motion][i][0] * 2), y + pvY - 1 + blocks[block][motion][i][1], "□");
	}
	save_pv_x = x;
	save_pv_y = y + pvY - 1;
}

// 내려오는 블럭의 예상위치를 지운다.
void removePreViewBlock() {
	int i;
	for (i = 0; i < 4; i++) {
		putsxy((save_pv_x + blocks[save_block][save_motion][i][0] * 2), save_pv_y + blocks[save_block][save_motion][i][1], "  ");
	}
}

// 저장할 블럭정보를 저장한다.
void saveBlock(int x, int y, int block, int motion) {
	save_x = x;
	save_y = y;
	save_block = block;
	save_motion = motion;
}

// 블럭을 회전시킨다.
void turnBlock(boolean isRight) {
	int next_motion;
	
	if (isRight)	//좌회전, 다음 회전모양지정
		next_motion = save_motion == 3 ? 0 : save_motion + 1;
	else	//우회전, 다음 회전모양지정
		next_motion = save_motion == 0 ? 3 : save_motion - 1;
	
	if (checkWalls(save_block, next_motion, 0, 0)) return;	// 다음 회전모양이 벽에 부딛치는 경우는 회전을 하지 않는다.
	
	if (isPreViewMode || score < 15000) {	//프리뷰모드이거나 15000 이하 이면 프리뷰 생성한다.
		removePreViewBlock();
		createPreViewBlock(save_x, save_y, save_block, next_motion);
	}

	removeBlock();	//출력되어있던 블럭을 지우고 회전한 블럭을 출력한다.
	createBlock(save_x, save_y, save_block, next_motion);
	saveBlock(save_x, save_y, save_block, next_motion);

	if (ISMULTI)
		txMovingBlock(save_x, save_y, save_block, save_motion); // 이동한 블럭을 서버에 보낸다.

}

// 새로운 블럭을 만든다.
void createNewBlock() {
	if (isQueueEmpty(blockQueue)) {	// 큐가 비어있다면 Q_REFILL_SIZE만큼 블럭을 큐에 넣어준다.
		char msg[1024] = { 0 };
		msg[0] = 'Q';
		for (int i = 0; i < Q_FILL_SIZE; i++) {
			newBlock.element = random(7);
			enQueue(blockQueue, newBlock);
			if (ISMULTI)	msg[i + 1] = newBlock.element + 0x30;	// 멀티모드일경우(싱글모드가 아닐경우)에는 서버에게 큐 데이터를 보낸다.
		}
		if (ISMULTI)	txSock(msg);
	}

	isBlockCreated = TRUE;

	int newBlock = nextBlock[0];	// 새로운블럭을 화면에 출력한다.
	if (isPreViewMode || score < 15000)
		createPreViewBlock(STARTX, STARTY, newBlock, 0);
	createBlock(STARTX, STARTY, newBlock, 0);
	saveBlock(STARTX, STARTY, newBlock, 0);

	if (ISMULTI)	// 멀티모드일때, 블록이동정보를 서버에 전송한다.
		txMovingBlock(STARTX, STARTY, newBlock, 0);

	nextBlock[0] = nextBlock[1];	// 다음블록을 한칸씩 밀고 큐에서 새로운블록을 꺼낸다.
	nextBlock[1] = deQueue(blockQueue)->element;
	printNextBlock();	//다음블록들을 화면에 출력한다.

	if (checkWalls(save_block, save_motion, 0, 0)) {	//새로꺼낸블럭이 벽에 닫는경우(게임에서 진경우)
		if (ISSINGLE) {
			isStartGame = FALSE;	// 게임 시작변수를 FALSE로 변경한다.
			mode = -1;
			printGameOver();	// 게임오버 화면 출력
		}
		else {
			isStartGame = FALSE;
			delay(50);
			txSock("ILOOSE");	// 내가 졌다는 메세지를 서버에 전송
			printGameOver();	//게임오버 화면 출력
		}
	}
}

// 블럭을 x변화량, y변화량 만큼 이동한다.
void moveBlock(int dx, int dy) {
	if (checkWalls(save_block, save_motion, dx, dy)) {	// 벽에 닫는지 확인한다.
		if (dy == 1) changeBlockToWalll();	// 아래로 한칸움직였을때는 땅에 다았기 때문에 블록 --> 벽 으로 바꾼다.
		return;
	}
	else {

		if (isPreViewMode || score < 15000) { // 프리뷰모드이거나 15000점 이하일 때
			removePreViewBlock();		// 이동전 프리뷰블럭을 지우고 이동후 프리뷰 블럭을 출력한다.
			createPreViewBlock(save_x + dx * 2, save_y + dy, save_block, save_motion);
		}

		removeBlock();	//이동전 블럭을 지우고 이동후 블럭을 출력한다.
		createBlock(save_x + dx * 2, save_y + dy, save_block, save_motion);
		saveBlock(save_x + dx * 2, save_y + dy, save_block, save_motion);

		if (ISMULTI)
			txMovingBlock(save_x, save_y, save_block, save_motion);	// 이동한 데이터를 상대방에게 보낸다.

	}
}

// 블럭을 저장한다. 저장한블럭은 사용중인블럭과 교체가능하다.
void holdingBlock() {
	if (holdBlock != -1) {	//저장된 블럭이 있는경우
		removeBlock();	//떨어지고 있는 블럭 제거

		int tmp_block = save_block;	//떨어지고 있는 블럭의 정보 저장
		int tmp_motion = save_motion;

		save_block = holdBlock;	//블럭의 종류와 모양을 저장한다.
		save_motion = holdMotion;
		int checkx = 0;
		while (checkWalls(save_block, save_motion, checkx, 0)) {	// 바꿀 블럭이 벽에 부딛치는경우에는 
			checkx += save_x > STARTX ? -1 : 1;	 //임시 x위치에 부딛치지 않을때까지 1칸씩 이동해서 저장한다.
		}
		moveBlock(checkx, 0); //원래있던 블럭의 위치에서 x위치를 checkx만큼이동후 출력.

		holdBlock = tmp_block;	// 홀드할 블럭정보 변경
		holdMotion = tmp_motion;
		printHoldBlock();	//홀드한 블록을 화면에 출력

		printStage();	//moveBlock(checkx, 0);을 실행할때 화면이 깨지는 부분이 있어 새로 화면출럭.
		moveBlock(0, 0);	// 블럭이 있던자리에 블럭 출력
	}
	else {
		holdBlock = save_block;	// 떨어지는 블럭정보를 홀드블럭에 저장한다.
		holdMotion = save_motion;
		printHoldBlock();	//홀드된 블럭을 화면에 출력한다.

		removeBlock();	// 떨어지던 블럭을 제거하고 새로운블럭을 추가한다.
		removePreViewBlock();
		isBlockCreated = FALSE;
	}
}

// 블럭을 벽으로 바꿔준다.
void changeBlockToWalll() {
	int i;
	for (i = 0; i < 4; i++) {
		stage[(save_y + blocks[save_block][save_motion][i][1])][(save_x / 2 - 8 + blocks[save_block][save_motion][i][0])] = save_block;
	}
	removeBlock();
	ishold = TRUE;	//한번에 홀드를 여러번못하게 하기위해 사용한다.
	printStage();
	checkAllLine();
	isBlockCreated = FALSE;

	if (ISMULTI)
		txStageData();
}

void checkAllLine() {
	int i, j;
	int count = 0;
	boolean isLineFull = TRUE;	//한줄에 블럭이 가특차있는지 확인하는 변수.
	for (i = 20; i >= 1; i--) {	// stage를 전체 검사를한다.
		for (j = 1; j <= 10; j++) {
			if (stage[i][j] == 8) {
				isLineFull = FALSE;	// 1줄에 한개라도 빈곳이 있는지 확인한다.
				break;
			}
			else isLineFull = TRUE;
		}
		if (isLineFull) {	// 한줄에 빈곳이 없으면
			clearLine(i++);	// 한줄을 지우고 한칸식 아래로 정렬한다.
			count++;
		}
	}
	if (count == 0) {	// 한줄도 못지우면 콤보초기와 및 5점추가
		combo = 0;
		addScore(5);
		printInfo();
	}
	if (count == 1) combo = combo > 6 ? 6 : combo + 1;	// 한줄지우면 콤보증가한다. 최대콤보 6.

	if (count > 1) {	//2줄 이상 지우면
		combo = combo > 6 ? 6 : combo + 1;
		addScore(count * 100);

		setColor(WHITE);	// 점수 추가
		EnterCriticalSection(&cs);
		gotoxy(MSG_X, MSG_Y); printf("라인보너스");
		gotoxy(MSG_X, MSG_Y + 1); printf("+%d", count * 100);

		if (count == 4) {
			setColor(PINK);
			gotoxy(MSG_X, MSG_Y + 4); printf("테트리스!!");
			gotoxy(MSG_X, MSG_Y + 5); printf("+%d", 300);
			addScore(300);
		}
		LeaveCriticalSection(&cs);
		msgState = TRUE;	// 점수 표시후, 스레드에서 일정시간이 지나면 점수표시가 사라지게한다.
		printInfo();
	}
	if (combo > 1) {	//점수 추가
		int comboBonus = 0;	
		gotoxy(MSG_X, MSG_Y + 2);
		switch (combo) {// 점수 표시후, 스레드에서 일정시간이 지나면 점수표시가 사라지게한다.
		case 2: setColor(WHITE); printf("COMBO"); comboBonus = 200; break;
		case 3: setColor(GREEN); printf("DOUBLE!"); comboBonus = 350; break;
		case 4: setColor(BLUE); printf("TRIPLE!!"); comboBonus = 500; break;
		case 5: setColor(RED); printf("GREAT!!!"); comboBonus = 750; break;
		case 6: setColor(YELLOW);  printf("FANTASTIC!!!!"); comboBonus = 1000; break;
		}
		gotoxy(MSG_X, MSG_Y + 3); printf("+%d", comboBonus);
		addScore(comboBonus);
		msgState = TRUE;
		printInfo();
	}
}

// 한줄을 지우고 지운줄위의 블럭은 한칸씩 아래로 이동한다.
void clearLine(int line) {
	int i, j;
	for (i = line - 1; i >= 1; i--) {
		for (j = 1; j <= 10; j++) {
			stage[i + 1][j] = stage[i][j];
			stage[i][j] = 8;
		}
	}
	addScore(100);
	printInfo();
	printStage();
}

// dx, dy만큼 이동했을때 벽에 부딛치는지 확인한다.
boolean checkWalls(int block, int motion, int dx, int dy) {
	int i;
	boolean result = FALSE;
	for (i = 0; i < 4; i++) {
		//	stage[블럭의 y위치 + 각각블럭의 y상대위치 + y이동량][블럭의 x위치 + 각각블럭의 x상대위치 + x이동량]
		if (stage[save_y + blocks[block][motion][i][1] + dy][save_x / 2 - 8 + (blocks[block][motion][i][0]) + dx] != 8) {
			result = TRUE;
			break;
		}
	}
	return result;
}

// x,y에 위치한 motion모양의 block의 프리뷰가 dy만큼 이동했을때 벽에 부딛치는지 확인한다.
boolean checkPreViewWalls(int block, int motion, int x, int y, int dy) {
	int i;
	boolean result = FALSE;
	for (i = 0; i < 4; i++) {
		//	stage[블럭의 y위치 + 각각블럭의 y상대위치 + y이동량][블럭의 x위치 + 각각블럭의 x상대위치]
		if (stage[y + (blocks[block][motion][i][1] + dy)][x / 2 - 8 + (blocks[block][motion][i][0])] != 8) {
			result = TRUE;
			break;
		}
	}
	return result;
}

void attacked() {
	for (int i = 1; i < 20; i++)
	{
		for (int j = 1; j < 11; j++)
		{
				stage[i][j] = stage[i + 1][j];
		}
	}
	for (int i = 1; i < 11; i++) {
		stage[20][i] = random(7);
	}
	stage[20][random(10) + 1] = 8;
	stage[20][random(10) + 1] = 8;
	stage[20][random(10) + 1] = 8;

	printStage();
	createBlock(save_x, save_y, save_block, save_motion);
	createPreViewBlock(save_x, save_y, save_block, save_motion);
	
	txStageData();
	txMovingBlock(save_x, save_y, save_block, save_motion);
}

void addScore(int sc) {
	int index = score / 300 + 1;

	score += sc;

	if (ISMULTI) {
		//if (score > 300 * index) {
		if (score >= index * 300) {
			attackCnt++;
			putchxy(ATTACK_X + 0, ATTACK_Y, attackCnt / 10 + 0x30);
			putchxy(ATTACK_X + 1, ATTACK_Y, attackCnt % 10 + 0x30);
		}
	}
}