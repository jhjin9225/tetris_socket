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


// ����� x,y�� ����Ѵ�.
void createBlock(int x, int y, int block, int motion) {
	int i;
	selectColor(block);
	for (i = 0; i < 4; i++) {
		// x,y ��ġ���� block�� ����� block���� motion���� ����� +x +y ��ġ���� ���� 4���� �Ḧ ��� 
		putsxy((x + blocks[block][motion][i][0] * 2), y + blocks[block][motion][i][1], "��"); 
	}
}

// ����� x,y���� �����.
void removeBlock() {
	int i;
	for (i = 0; i < 4; i++) {
		// x,y ��ġ���� block�� ����� block���� motion���� ����� +x +y ��ġ���� ���� 4���� "  "(2byte����)�� ��� 
		putsxy((save_x + blocks[save_block][save_motion][i][0] * 2), save_y + blocks[save_block][save_motion][i][1], "  ");
	}
}

// �������� ���� ������ġ�� ����Ѵ�.
void createPreViewBlock(int x, int y, int block, int motion) {
	int pvY = 1;
	int i;
	while (!checkPreViewWalls(block, motion, x, y, pvY)) { ++pvY; }
	selectColor(block);
	for (i = 0; i < 4; i++) {
		putsxy((x + blocks[block][motion][i][0] * 2), y + pvY - 1 + blocks[block][motion][i][1], "��");
	}
	save_pv_x = x;
	save_pv_y = y + pvY - 1;
}

// �������� ���� ������ġ�� �����.
void removePreViewBlock() {
	int i;
	for (i = 0; i < 4; i++) {
		putsxy((save_pv_x + blocks[save_block][save_motion][i][0] * 2), save_pv_y + blocks[save_block][save_motion][i][1], "  ");
	}
}

// ������ �������� �����Ѵ�.
void saveBlock(int x, int y, int block, int motion) {
	save_x = x;
	save_y = y;
	save_block = block;
	save_motion = motion;
}

// ���� ȸ����Ų��.
void turnBlock(boolean isRight) {
	int next_motion;
	
	if (isRight)	//��ȸ��, ���� ȸ���������
		next_motion = save_motion == 3 ? 0 : save_motion + 1;
	else	//��ȸ��, ���� ȸ���������
		next_motion = save_motion == 0 ? 3 : save_motion - 1;
	
	if (checkWalls(save_block, next_motion, 0, 0)) return;	// ���� ȸ������� ���� �ε�ġ�� ���� ȸ���� ���� �ʴ´�.
	
	if (isPreViewMode || score < 15000) {	//���������̰ų� 15000 ���� �̸� ������ �����Ѵ�.
		removePreViewBlock();
		createPreViewBlock(save_x, save_y, save_block, next_motion);
	}

	removeBlock();	//��µǾ��ִ� ���� ����� ȸ���� ���� ����Ѵ�.
	createBlock(save_x, save_y, save_block, next_motion);
	saveBlock(save_x, save_y, save_block, next_motion);

	if (ISMULTI)
		txMovingBlock(save_x, save_y, save_block, save_motion); // �̵��� ���� ������ ������.

}

// ���ο� ���� �����.
void createNewBlock() {
	if (isQueueEmpty(blockQueue)) {	// ť�� ����ִٸ� Q_REFILL_SIZE��ŭ ���� ť�� �־��ش�.
		char msg[1024] = { 0 };
		msg[0] = 'Q';
		for (int i = 0; i < Q_FILL_SIZE; i++) {
			newBlock.element = random(7);
			enQueue(blockQueue, newBlock);
			if (ISMULTI)	msg[i + 1] = newBlock.element + 0x30;	// ��Ƽ����ϰ��(�̱۸�尡 �ƴҰ��)���� �������� ť �����͸� ������.
		}
		if (ISMULTI)	txSock(msg);
	}

	isBlockCreated = TRUE;

	int newBlock = nextBlock[0];	// ���ο���� ȭ�鿡 ����Ѵ�.
	if (isPreViewMode || score < 15000)
		createPreViewBlock(STARTX, STARTY, newBlock, 0);
	createBlock(STARTX, STARTY, newBlock, 0);
	saveBlock(STARTX, STARTY, newBlock, 0);

	if (ISMULTI)	// ��Ƽ����϶�, ����̵������� ������ �����Ѵ�.
		txMovingBlock(STARTX, STARTY, newBlock, 0);

	nextBlock[0] = nextBlock[1];	// ��������� ��ĭ�� �а� ť���� ���ο����� ������.
	nextBlock[1] = deQueue(blockQueue)->element;
	printNextBlock();	//������ϵ��� ȭ�鿡 ����Ѵ�.

	if (checkWalls(save_block, save_motion, 0, 0)) {	//���β������� ���� �ݴ°��(���ӿ��� �����)
		if (ISSINGLE) {
			isStartGame = FALSE;	// ���� ���ۺ����� FALSE�� �����Ѵ�.
			mode = -1;
			printGameOver();	// ���ӿ��� ȭ�� ���
		}
		else {
			isStartGame = FALSE;
			delay(50);
			txSock("ILOOSE");	// ���� ���ٴ� �޼����� ������ ����
			printGameOver();	//���ӿ��� ȭ�� ���
		}
	}
}

// ���� x��ȭ��, y��ȭ�� ��ŭ �̵��Ѵ�.
void moveBlock(int dx, int dy) {
	if (checkWalls(save_block, save_motion, dx, dy)) {	// ���� �ݴ��� Ȯ���Ѵ�.
		if (dy == 1) changeBlockToWalll();	// �Ʒ��� ��ĭ������������ ���� �پұ� ������ ��� --> �� ���� �ٲ۴�.
		return;
	}
	else {

		if (isPreViewMode || score < 15000) { // ���������̰ų� 15000�� ������ ��
			removePreViewBlock();		// �̵��� ��������� ����� �̵��� ������ ���� ����Ѵ�.
			createPreViewBlock(save_x + dx * 2, save_y + dy, save_block, save_motion);
		}

		removeBlock();	//�̵��� ���� ����� �̵��� ���� ����Ѵ�.
		createBlock(save_x + dx * 2, save_y + dy, save_block, save_motion);
		saveBlock(save_x + dx * 2, save_y + dy, save_block, save_motion);

		if (ISMULTI)
			txMovingBlock(save_x, save_y, save_block, save_motion);	// �̵��� �����͸� ���濡�� ������.

	}
}

// ���� �����Ѵ�. �����Ѻ��� ������κ��� ��ü�����ϴ�.
void holdingBlock() {
	if (holdBlock != -1) {	//����� ���� �ִ°��
		removeBlock();	//�������� �ִ� �� ����

		int tmp_block = save_block;	//�������� �ִ� ���� ���� ����
		int tmp_motion = save_motion;

		save_block = holdBlock;	//���� ������ ����� �����Ѵ�.
		save_motion = holdMotion;
		int checkx = 0;
		while (checkWalls(save_block, save_motion, checkx, 0)) {	// �ٲ� ���� ���� �ε�ġ�°�쿡�� 
			checkx += save_x > STARTX ? -1 : 1;	 //�ӽ� x��ġ�� �ε�ġ�� ���������� 1ĭ�� �̵��ؼ� �����Ѵ�.
		}
		moveBlock(checkx, 0); //�����ִ� ���� ��ġ���� x��ġ�� checkx��ŭ�̵��� ���.

		holdBlock = tmp_block;	// Ȧ���� ������ ����
		holdMotion = tmp_motion;
		printHoldBlock();	//Ȧ���� ����� ȭ�鿡 ���

		printStage();	//moveBlock(checkx, 0);�� �����Ҷ� ȭ���� ������ �κ��� �־� ���� ȭ���ⷰ.
		moveBlock(0, 0);	// ���� �ִ��ڸ��� �� ���
	}
	else {
		holdBlock = save_block;	// �������� �������� Ȧ����� �����Ѵ�.
		holdMotion = save_motion;
		printHoldBlock();	//Ȧ��� ���� ȭ�鿡 ����Ѵ�.

		removeBlock();	// �������� ���� �����ϰ� ���ο���� �߰��Ѵ�.
		removePreViewBlock();
		isBlockCreated = FALSE;
	}
}

// ���� ������ �ٲ��ش�.
void changeBlockToWalll() {
	int i;
	for (i = 0; i < 4; i++) {
		stage[(save_y + blocks[save_block][save_motion][i][1])][(save_x / 2 - 8 + blocks[save_block][save_motion][i][0])] = save_block;
	}
	removeBlock();
	ishold = TRUE;	//�ѹ��� Ȧ�带 ���������ϰ� �ϱ����� ����Ѵ�.
	printStage();
	checkAllLine();
	isBlockCreated = FALSE;

	if (ISMULTI)
		txStageData();
}

void checkAllLine() {
	int i, j;
	int count = 0;
	boolean isLineFull = TRUE;	//���ٿ� ���� ��Ư���ִ��� Ȯ���ϴ� ����.
	for (i = 20; i >= 1; i--) {	// stage�� ��ü �˻縦�Ѵ�.
		for (j = 1; j <= 10; j++) {
			if (stage[i][j] == 8) {
				isLineFull = FALSE;	// 1�ٿ� �Ѱ��� ����� �ִ��� Ȯ���Ѵ�.
				break;
			}
			else isLineFull = TRUE;
		}
		if (isLineFull) {	// ���ٿ� ����� ������
			clearLine(i++);	// ������ ����� ��ĭ�� �Ʒ��� �����Ѵ�.
			count++;
		}
	}
	if (count == 0) {	// ���ٵ� ������� �޺��ʱ�� �� 5���߰�
		combo = 0;
		addScore(5);
		printInfo();
	}
	if (count == 1) combo = combo > 6 ? 6 : combo + 1;	// ��������� �޺������Ѵ�. �ִ��޺� 6.

	if (count > 1) {	//2�� �̻� �����
		combo = combo > 6 ? 6 : combo + 1;
		addScore(count * 100);

		setColor(WHITE);	// ���� �߰�
		EnterCriticalSection(&cs);
		gotoxy(MSG_X, MSG_Y); printf("���κ��ʽ�");
		gotoxy(MSG_X, MSG_Y + 1); printf("+%d", count * 100);

		if (count == 4) {
			setColor(PINK);
			gotoxy(MSG_X, MSG_Y + 4); printf("��Ʈ����!!");
			gotoxy(MSG_X, MSG_Y + 5); printf("+%d", 300);
			addScore(300);
		}
		LeaveCriticalSection(&cs);
		msgState = TRUE;	// ���� ǥ����, �����忡�� �����ð��� ������ ����ǥ�ð� ��������Ѵ�.
		printInfo();
	}
	if (combo > 1) {	//���� �߰�
		int comboBonus = 0;	
		gotoxy(MSG_X, MSG_Y + 2);
		switch (combo) {// ���� ǥ����, �����忡�� �����ð��� ������ ����ǥ�ð� ��������Ѵ�.
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

// ������ ����� ���������� ���� ��ĭ�� �Ʒ��� �̵��Ѵ�.
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

// dx, dy��ŭ �̵������� ���� �ε�ġ���� Ȯ���Ѵ�.
boolean checkWalls(int block, int motion, int dx, int dy) {
	int i;
	boolean result = FALSE;
	for (i = 0; i < 4; i++) {
		//	stage[���� y��ġ + �������� y�����ġ + y�̵���][���� x��ġ + �������� x�����ġ + x�̵���]
		if (stage[save_y + blocks[block][motion][i][1] + dy][save_x / 2 - 8 + (blocks[block][motion][i][0]) + dx] != 8) {
			result = TRUE;
			break;
		}
	}
	return result;
}

// x,y�� ��ġ�� motion����� block�� �����䰡 dy��ŭ �̵������� ���� �ε�ġ���� Ȯ���Ѵ�.
boolean checkPreViewWalls(int block, int motion, int x, int y, int dy) {
	int i;
	boolean result = FALSE;
	for (i = 0; i < 4; i++) {
		//	stage[���� y��ġ + �������� y�����ġ + y�̵���][���� x��ġ + �������� x�����ġ]
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