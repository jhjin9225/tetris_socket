#pragma once
#include "tools.h"


int dx = 27;

// ������ Ŭ���̾�Ʈ�� �޾Ƶ��̴� �������Լ�
void serverRecvThread() {
	int client_index = client_num;	// Ŭ���̾�Ʈ�� �����ϱ����� ���.
	int pre_x[3] = { -1, -1, -1 }, pre_y[3] = { -1, -1, -1 };	// ��Ƽ��� �ٸ�������� �������� ���� �����ϴ� ����
	int pre_block[3] = { -1, -1, -1 }, pre_motion[3] = { -1, -1, -1 };
	char buff[1024] = { 0 };	// ����������� �޵��̴� ����
	char msg[1024] = { 0 };	// ���۸� '\0'�� �����Ͽ� ��ɽ���

	while (1) {
		/*
		 server_sock[client_index]�� ������ ����  buff�� �����Ѵ�.
		 ��ȯ�Ǵ°��� ���� ���ڿ��� �����̴�.
		 buff�� �����Ϳ� ������ ���̿� �ι���('\0')�� ���� ���´�.
		 ex)"������1'\0'������2'\0'������3'\0'...."
		*/
		int len = recv(server_sock[client_index], buff, sizeof(buff), 0);	
		if (len < 0)	continue;											

		int buf_idx = 0;	// buff�� ��� ���ڿ��� ��ù���� ���ڿ��� ����Ŵ.
		while (strlen(buff + buf_idx) != 0) {
			strcpy_s(msg, 1024, buff + buf_idx);

			if (msg[0] == 'b') {
				reflexBlocks(msg, client_index); // Ŭ���̾�Ʈ���� ���� �޼����� �ٸ� Ŭ���̾�Ʈ���� ����

				int x = 60 + (client_index*dx);	// x��ġ�� client �������� �����ȴ�.
				int y = 20;
				int isEmptyCNT = 0;

				EnterCriticalSection(&cs);

				gotoxy(x, y);
				for (int i = 20; i >= 1; i--) {	//stage�� ����Ѵ�.
					for (int j = 1; j <= 10; j++) {
						selectColor(msg[10 * (20 - i) + j] - 0x30);	//����ȣ�� ���� ������ ��µȴ�.
						if (msg[10 * (20 - i) + j] == '8') {
							printf("  ");
							isEmptyCNT++;
							if (isEmptyCNT >= 40) { i = 0; j = 11; }	// ���� 40ĭ ��ĭ�ϰ��� ����
						}
						else {
							printf("��");
							isEmptyCNT = 0;
						}
					}
					gotoxy(x, --y);
				}

				pre_x[client_index] = -1;	// ���� ���� �����Ǹ� ������ ���� ������ϱ⶧����
				pre_y[client_index] = -1;	// �ʱ�ȭ�� ��Ų��.
				pre_block[client_index] = -1;
				pre_motion[client_index] = -1;
				LeaveCriticalSection(&cs);
			}
			else if (msg[0] == 'B') {
				reflexBlocks(msg, client_index);

				for (int i = 0; i < 9; i++)	msg[i] -= 0x30;	// �ƽ�Ű�ڵ尪�� ���ڷ� �ٲ۴�.

				int x = msg[1] * 10 + msg[2] + 42 + (dx * client_index);	// ��µ� ��ġ�� ���� �����Ѵ�.
				int y = msg[3] * 10 + msg[4];
				int block = msg[5] * 10 + msg[6];
				int motion = msg[7] * 10 + msg[8];

				EnterCriticalSection(&cs);
				if (pre_x[client_index] != -1) {
					for (int i = 0; i < 4; i++) {	//�̵��� ��� �����
						putsxy((pre_x[client_index] + blocks[pre_block[client_index]][pre_motion[client_index]][i][0] * 2), pre_y[client_index] + blocks[pre_block[client_index]][pre_motion[client_index]][i][1], "  ");
					}
				}
				selectColor(block);
				for (int i = 0; i < 4; i++) {	//�̵��� ��� �׸���
					putsxy((x + blocks[block][motion][i][0] * 2), y + blocks[block][motion][i][1], "��"); // x,y ��ġ���� block�� ����� block���� motion���� ����� +x +y ��ġ���� ���� 4���� �Ḧ ��� 
				}
				LeaveCriticalSection(&cs);
				pre_x[client_index] = x;	// �̵��� ���� ����
				pre_y[client_index] = y;
				pre_block[client_index] = block;
				pre_motion[client_index] = motion;

			}
			else if (strcmp(msg, "ILOOSE") == 0) {
				winCnt++;	
				reflex(msg, client_index);
				if (winCnt == clientCount) {	// ������ ���ӿ����� �ɶ����� 1�� �����Ͽ� �ٸ��÷��̾ ��� �й��ϸ� �¸��ϰԵȴ�.
					printGameWin();
					isStartGame = FALSE;	// �¸��� ��������
				}
			}
			else if (strcmp(msg, "ready") == 0) {	// ����÷��̾ �����غ� �Ǿ�� ������ �����Ѵ�.
				reflex("ready", client_index);	
				readyCnt++;
			}
			else if (msg[0] == 'q') {	// ���� �Է��� ť�� �������� �Ǹ� ť������ ������ �߰����ش�.
				reflex(msg, client_index);

				if (isStartGame == TRUE) {
					EnterCriticalSection(&cs);
					for (int i = 0; i < Q_REFILL_SIZE; i++) {
						newBlock.element = msg[i + 1] - 0x30;
						enQueue(blockQueue, newBlock);
					}
					LeaveCriticalSection(&cs);
				}
			}
			else if (msg[0] == 'p') {	//�÷��̾��� �̸��� �޾ƿ´�.
				strcpy(playerName[client_index], msg + 1);
			}

			buf_idx = buf_idx + strlen(buff + buf_idx) + 1;	// buf�� ���� ���ڿ��� ù���� ����
		}
		memset(msg, 0, sizeof(msg));	// �ʱ�ȭ
		memset(buff, 0, sizeof(buff));
	}
}
void clientRecvThread() {
	int pre_x[3] = { -1, -1, -1 }, pre_y[3] = { -1, -1, -1 };	// ��Ƽ��� �ٸ�������� �������� ���� �����ϴ� ����
	int pre_block[3] = { -1, -1, -1 }, pre_motion[3] = { -1, -1, -1 };
	char buff[1024] = { 0 };	// ����������� �޵��̴� ����
	char msg[1024] = { 0 };	// ���۸� '\0'�� �����Ͽ� ��ɽ���

	while (1) {
		/*
		client_sock�� ������ ����  buff�� �����Ѵ�.
		��ȯ�Ǵ°��� ���� ���ڿ��� �����̴�.
		buff�� �����Ϳ� ������ ���̿� �ι���('\0')�� ���� ���´�.
		ex)"������1'\0'������2'\0'������3'\0'...."
		*/
		int len = recv(client_sock, buff, sizeof(buff), 0);
		if (len < 0)	continue;

		int buf_idx = 0;	// buff�� ��� ���ڿ��� ��ù���� ���ڿ��� ����Ŵ.
		while (strlen(buff + buf_idx) != 0) {
			strcpy_s(msg, 1024, buff + buf_idx);

			if (msg[0] == 'b') {
				EnterCriticalSection(&cs);

				int client_index = msg[201] == 'R' ? msg[202] - 0x30 : 0;
				int x = 60 + (client_index*dx);	// x��ġ�� client �������� �����ȴ�.
				int y = 20;
				int breakCNT = 0;


				gotoxy(x, y);
				for (int i = 20; i >= 1; i--) {	//stage�� ����Ѵ�.
					for (int j = 1; j <= 10; j++) {
						selectColor(msg[10 * (20 - i) + j] - 0x30);
						if (msg[10 * (20 - i) + j] == '8') {
							printf("  ");
							breakCNT++;
							if (breakCNT >= 40) { i = 0; j = 11; }
						}
						else {
							printf("��");
							breakCNT = 0;
						}
					}
					gotoxy(x, --y);
				}

				pre_x[client_index] = -1;	// ���� ���� �����Ǹ� ������ ���� ������ϱ⶧����
				pre_y[client_index] = -1;	// �ʱ�ȭ�� ��Ų��.
				pre_block[client_index] = -1;
				pre_motion[client_index] = -1;
				LeaveCriticalSection(&cs);
			}
			else if (msg[0] == 'B') {
				int i;
				int client_index = msg[9] == 'R' ? msg[10] - 0x30 : 0;

				EnterCriticalSection(&cs);

				for (i = 0; i < 9; i++)	msg[i] -= 0x30;	// �ƽ�Ű�ڵ尪�� ���ڷ� �ٲ۴�.

				int x = msg[1] * 10 + msg[2] + 42 + (client_index*dx);;	// ��µ� ��ġ�� ���� �����Ѵ�.
				int y = msg[3] * 10 + msg[4];
				int block = msg[5] * 10 + msg[6];
				int motion = msg[7] * 10 + msg[8];

				if (pre_x[client_index] != -1) {
					for (i = 0; i < 4; i++) {//�̵��� ��� �����
						putsxy((pre_x[client_index] + blocks[pre_block[client_index]][pre_motion[client_index]][i][0] * 2), pre_y[client_index] + blocks[pre_block[client_index]][pre_motion[client_index]][i][1], "  ");
					}
				}
				selectColor(block);
				for (i = 0; i < 4; i++) {//�̵��� ��� �׸���
					putsxy((x + blocks[block][motion][i][0] * 2), y + blocks[block][motion][i][1], "��"); // x,y ��ġ���� block�� ����� block���� motion���� ����� +x +y ��ġ���� ���� 4���� �Ḧ ��� 
				}
				LeaveCriticalSection(&cs);
				pre_x[client_index] = x;	// �̵��� ���� ����
				pre_y[client_index] = y;
				pre_block[client_index] = block;
				pre_motion[client_index] = motion;

			}
			else if (strcmp(msg, "start") == 0) { isStartGame = TRUE; }	// �÷��̾ ��� ����Ǹ� ������ �����Ѵ�.
			else if (msg[0] == 'c') {	// �÷��̾ �����ϸ� �ִ��ο��� �������ο��� ǥ���Ѵ�.
				setColor(WHITE);
				putchxy(42, 0, msg[1]);
				putchxy(38, 0, msg[2]);

				clientCount = msg[1] - 0x31;	// �� Ŭ���̾�Ʈ ���� ����Ѵ�.
			}
			else if (msg[0] == 's' && strcmp(msg, "start") != 0) {	// ������ �Ϸ�Ǹ� ī��Ʈ �ٿ��� �Ѵ�.
				setColor(WHITE);
				putsxy(20, 5, "����Ϸ�! ������ [ ] �� �Ŀ� �����մϴ�!!");
				putixy(38, 5, msg[1]);
			}
			else if (msg[0] == 'Q') {	// �������� �� ť�� �޾� �����Ѵ�.

				blockQueue = createQueue(Q_MAX);

				EnterCriticalSection(&cs);
				for (int i = 0; i < Q_MAX; i++) {
					newBlock.element = msg[i + 1] - 0x30;
					enQueue(blockQueue, newBlock);
				}
				LeaveCriticalSection(&cs);

				isRecvQ = TRUE;
			}
			else if (msg[0] == 'q') {	// ������ ť�� �����ϸ� �ٽ� ä���ش�.
				if (isStartGame == TRUE) {
					EnterCriticalSection(&cs);
					for (int i = 0; i < Q_REFILL_SIZE; i++) {
						newBlock.element = msg[i + 1] - 0x30;
						enQueue(blockQueue, newBlock);
					}
					LeaveCriticalSection(&cs);
				}
			}
			else if (strcmp(msg, "ILOOSE") == 0) {	// ������ ���ӿ����� �ɶ����� 1�� �����Ͽ� �ٸ��÷��̾ ��� �й��ϸ� �¸��ϰԵȴ�.
				winCnt++;
				if (winCnt == clientCount) {
					printGameWin();
					isStartGame = FALSE;
				}
			}
			else if (strcmp(msg, "ready") == 0) { readyCnt++; }// ����÷��̾ �����غ� �Ǿ�� ������ �����Ѵ�.
			else if (msg[0] == 'p') {	//�÷��̾��� �̸��� �޾ƿ´�.
				int point = 1, cnt = 0;
			
				int i = 0;
				while (msg[i] != '\0'){
					if (msg[i] == 1) {
						msg[i] = 0;
					}
					i++;
				}
				while (strlen(msg + point) != 0) {
					strcpy(playerName[cnt++], msg + point);
					point = point + strlen(msg + point) + 1;
				}
			}
		
			buf_idx = buf_idx + strlen(buff + buf_idx) + 1;
		}
		memset(msg, 0, sizeof(msg));	// �ʱ�ȭ
		memset(buff, 0, sizeof(buff));
	}
}

//================================================================================================//
void autoDownBlock() {
	while (isStartGame) {
		delay(leveltime);	// ����� �������� �ӵ� ��ŭ ������
		EnterCriticalSection(&cs);
		if (isStartGame && isBlockCreated)
			moveBlock(0, 1);	//����� ��ĭ �Ʒ��� �̵�
		LeaveCriticalSection(&cs);
	}
}
void listenMsg() {
	int count;

	while (isStartGame) {

		if (msgState) {	// ���� �޼����� ������
			msgState = FALSE;	// �ٷ� FALSE�� ������ �ٷ� �޼��� ���
			count = 0;
			while (count != 1000) {	// �� 1�� ���. while�� ���� ������ ����� �ٸ� �޼����� ������ �ֱ⶧��
				delay(1);
				count++;
				if (msgState) {	//����� �޼��� ������ �ٽ� ó������ ���
					count = 0;
					msgState = FALSE;
				}
			}

			EnterCriticalSection(&cs);
			for (int i = 0; i < 6; i++) {
				gotoxy(MSG_X, MSG_Y + i);	printf("              ");	// �޼����� �����.
			}
			LeaveCriticalSection(&cs);
		}
		delay(1);
	}
}

