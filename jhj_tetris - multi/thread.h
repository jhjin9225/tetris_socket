#pragma once
#include "tools.h"


int dx = 27;

// 서버가 클라이언트로 받아들이는 쓰레드함수
void serverRecvThread() {
	int client_index = client_num;	// 클라이언트를 구별하기위해 사용.
	int pre_x[3] = { -1, -1, -1 }, pre_y[3] = { -1, -1, -1 };	// 멀티모드 다른사용자의 떨어지는 블럭을 저장하는 변수
	int pre_block[3] = { -1, -1, -1 }, pre_motion[3] = { -1, -1, -1 };
	char buff[1024] = { 0 };	// 소켓통신으로 받들이는 버퍼
	char msg[1024] = { 0 };	// 버퍼를 '\0'로 구분하여 명령실행

	while (1) {
		/*
		 server_sock[client_index]가 보내는 값을  buff에 저장한다.
		 반환되는값은 받은 문자열의 길이이다.
		 buff는 데이터와 데이터 사이에 널문자('\0')가 껴서 들어온다.
		 ex)"데이터1'\0'데이터2'\0'데이터3'\0'...."
		*/
		int len = recv(server_sock[client_index], buff, sizeof(buff), 0);	
		if (len < 0)	continue;											

		int buf_idx = 0;	// buff에 담긴 문자열중 맨첫번재 문자열을 가리킴.
		while (strlen(buff + buf_idx) != 0) {
			strcpy_s(msg, 1024, buff + buf_idx);

			if (msg[0] == 'b') {
				reflexBlocks(msg, client_index); // 클라이언트에서 받은 메세지를 다른 클라이언트에게 전송

				int x = 60 + (client_index*dx);	// x위치는 client 순서별로 지정된다.
				int y = 20;
				int isEmptyCNT = 0;

				EnterCriticalSection(&cs);

				gotoxy(x, y);
				for (int i = 20; i >= 1; i--) {	//stage를 출력한다.
					for (int j = 1; j <= 10; j++) {
						selectColor(msg[10 * (20 - i) + j] - 0x30);	//블럭번호에 따라 색깔이 출력된다.
						if (msg[10 * (20 - i) + j] == '8') {
							printf("  ");
							isEmptyCNT++;
							if (isEmptyCNT >= 40) { i = 0; j = 11; }	// 연속 40칸 빈칸일경우는 멈춤
						}
						else {
							printf("■");
							isEmptyCNT = 0;
						}
					}
					gotoxy(x, --y);
				}

				pre_x[client_index] = -1;	// 블럭이 새로 생성되면 지워질 블럭이 없어야하기때문에
				pre_y[client_index] = -1;	// 초기화를 시킨다.
				pre_block[client_index] = -1;
				pre_motion[client_index] = -1;
				LeaveCriticalSection(&cs);
			}
			else if (msg[0] == 'B') {
				reflexBlocks(msg, client_index);

				for (int i = 0; i < 9; i++)	msg[i] -= 0x30;	// 아스키코드값을 숫자로 바꾼다.

				int x = msg[1] * 10 + msg[2] + 42 + (dx * client_index);	// 출력될 위치와 블럭을 지정한다.
				int y = msg[3] * 10 + msg[4];
				int block = msg[5] * 10 + msg[6];
				int motion = msg[7] * 10 + msg[8];

				EnterCriticalSection(&cs);
				if (pre_x[client_index] != -1) {
					for (int i = 0; i < 4; i++) {	//이동전 블록 지우기
						putsxy((pre_x[client_index] + blocks[pre_block[client_index]][pre_motion[client_index]][i][0] * 2), pre_y[client_index] + blocks[pre_block[client_index]][pre_motion[client_index]][i][1], "  ");
					}
				}
				selectColor(block);
				for (int i = 0; i < 4; i++) {	//이동후 블록 그리기
					putsxy((x + blocks[block][motion][i][0] * 2), y + blocks[block][motion][i][1], "■"); // x,y 위치부터 block에 저장된 block값과 motion값의 블록을 +x +y 위치값을 더한 4개의 ■를 출력 
				}
				LeaveCriticalSection(&cs);
				pre_x[client_index] = x;	// 이동후 값을 저장
				pre_y[client_index] = y;
				pre_block[client_index] = block;
				pre_motion[client_index] = motion;

			}
			else if (strcmp(msg, "ILOOSE") == 0) {
				winCnt++;	
				reflex(msg, client_index);
				if (winCnt == clientCount) {	// 상대방이 게임오버가 될때마다 1씩 증가하여 다른플래이어가 모두 패배하면 승리하게된다.
					printGameWin();
					isStartGame = FALSE;	// 승리후 게임종료
				}
			}
			else if (strcmp(msg, "ready") == 0) {	// 모든플레이어가 시작준비가 되어야 게임을 시작한다.
				reflex("ready", client_index);	
				readyCnt++;
			}
			else if (msg[0] == 'q') {	// 최조 입력한 큐가 떨어지게 되면 큐정보를 보내어 추가해준다.
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
			else if (msg[0] == 'p') {	//플래이어의 이름을 받아온다.
				strcpy(playerName[client_index], msg + 1);
			}

			buf_idx = buf_idx + strlen(buff + buf_idx) + 1;	// buf의 다음 문자열의 첫글자 선택
		}
		memset(msg, 0, sizeof(msg));	// 초기화
		memset(buff, 0, sizeof(buff));
	}
}
void clientRecvThread() {
	int pre_x[3] = { -1, -1, -1 }, pre_y[3] = { -1, -1, -1 };	// 멀티모드 다른사용자의 떨어지는 블럭을 저장하는 변수
	int pre_block[3] = { -1, -1, -1 }, pre_motion[3] = { -1, -1, -1 };
	char buff[1024] = { 0 };	// 소켓통신으로 받들이는 버퍼
	char msg[1024] = { 0 };	// 버퍼를 '\0'로 구분하여 명령실행

	while (1) {
		/*
		client_sock이 보내는 값을  buff에 저장한다.
		반환되는값은 받은 문자열의 길이이다.
		buff는 데이터와 데이터 사이에 널문자('\0')가 껴서 들어온다.
		ex)"데이터1'\0'데이터2'\0'데이터3'\0'...."
		*/
		int len = recv(client_sock, buff, sizeof(buff), 0);
		if (len < 0)	continue;

		int buf_idx = 0;	// buff에 담긴 문자열중 맨첫번재 문자열을 가리킴.
		while (strlen(buff + buf_idx) != 0) {
			strcpy_s(msg, 1024, buff + buf_idx);

			if (msg[0] == 'b') {
				EnterCriticalSection(&cs);

				int client_index = msg[201] == 'R' ? msg[202] - 0x30 : 0;
				int x = 60 + (client_index*dx);	// x위치는 client 순서별로 지정된다.
				int y = 20;
				int breakCNT = 0;


				gotoxy(x, y);
				for (int i = 20; i >= 1; i--) {	//stage를 출력한다.
					for (int j = 1; j <= 10; j++) {
						selectColor(msg[10 * (20 - i) + j] - 0x30);
						if (msg[10 * (20 - i) + j] == '8') {
							printf("  ");
							breakCNT++;
							if (breakCNT >= 40) { i = 0; j = 11; }
						}
						else {
							printf("■");
							breakCNT = 0;
						}
					}
					gotoxy(x, --y);
				}

				pre_x[client_index] = -1;	// 블럭이 새로 생성되면 지워질 블럭이 없어야하기때문에
				pre_y[client_index] = -1;	// 초기화를 시킨다.
				pre_block[client_index] = -1;
				pre_motion[client_index] = -1;
				LeaveCriticalSection(&cs);
			}
			else if (msg[0] == 'B') {
				int i;
				int client_index = msg[9] == 'R' ? msg[10] - 0x30 : 0;

				EnterCriticalSection(&cs);

				for (i = 0; i < 9; i++)	msg[i] -= 0x30;	// 아스키코드값을 숫자로 바꾼다.

				int x = msg[1] * 10 + msg[2] + 42 + (client_index*dx);;	// 출력될 위치와 블럭을 지정한다.
				int y = msg[3] * 10 + msg[4];
				int block = msg[5] * 10 + msg[6];
				int motion = msg[7] * 10 + msg[8];

				if (pre_x[client_index] != -1) {
					for (i = 0; i < 4; i++) {//이동전 블록 지우기
						putsxy((pre_x[client_index] + blocks[pre_block[client_index]][pre_motion[client_index]][i][0] * 2), pre_y[client_index] + blocks[pre_block[client_index]][pre_motion[client_index]][i][1], "  ");
					}
				}
				selectColor(block);
				for (i = 0; i < 4; i++) {//이동후 블록 그리기
					putsxy((x + blocks[block][motion][i][0] * 2), y + blocks[block][motion][i][1], "■"); // x,y 위치부터 block에 저장된 block값과 motion값의 블록을 +x +y 위치값을 더한 4개의 ■를 출력 
				}
				LeaveCriticalSection(&cs);
				pre_x[client_index] = x;	// 이동후 값을 저장
				pre_y[client_index] = y;
				pre_block[client_index] = block;
				pre_motion[client_index] = motion;

			}
			else if (strcmp(msg, "start") == 0) { isStartGame = TRUE; }	// 플레이어가 모두 입장되면 게임을 시작한다.
			else if (msg[0] == 'c') {	// 플레이어가 입장하면 최대인원과 입장한인원을 표시한다.
				setColor(WHITE);
				putchxy(42, 0, msg[1]);
				putchxy(38, 0, msg[2]);

				clientCount = msg[1] - 0x31;	// 총 클라이언트 수를 기억한다.
			}
			else if (msg[0] == 's' && strcmp(msg, "start") != 0) {	// 입장이 완료되면 카운트 다운을 한다.
				setColor(WHITE);
				putsxy(20, 5, "입장완료! 게임이 [ ] 초 후에 시작합니다!!");
				putixy(38, 5, msg[1]);
			}
			else if (msg[0] == 'Q') {	// 서버에서 블럭 큐를 받아 저장한다.

				blockQueue = createQueue(Q_MAX);

				EnterCriticalSection(&cs);
				for (int i = 0; i < Q_MAX; i++) {
					newBlock.element = msg[i + 1] - 0x30;
					enQueue(blockQueue, newBlock);
				}
				LeaveCriticalSection(&cs);

				isRecvQ = TRUE;
			}
			else if (msg[0] == 'q') {	// 상대방이 큐가 부족하면 다시 채워준다.
				if (isStartGame == TRUE) {
					EnterCriticalSection(&cs);
					for (int i = 0; i < Q_REFILL_SIZE; i++) {
						newBlock.element = msg[i + 1] - 0x30;
						enQueue(blockQueue, newBlock);
					}
					LeaveCriticalSection(&cs);
				}
			}
			else if (strcmp(msg, "ILOOSE") == 0) {	// 상대방이 게임오버가 될때마다 1씩 증가하여 다른플래이어가 모두 패배하면 승리하게된다.
				winCnt++;
				if (winCnt == clientCount) {
					printGameWin();
					isStartGame = FALSE;
				}
			}
			else if (strcmp(msg, "ready") == 0) { readyCnt++; }// 모든플레이어가 시작준비가 되어야 게임을 시작한다.
			else if (msg[0] == 'p') {	//플래이어의 이름을 받아온다.
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
		memset(msg, 0, sizeof(msg));	// 초기화
		memset(buff, 0, sizeof(buff));
	}
}

//================================================================================================//
void autoDownBlock() {
	while (isStartGame) {
		delay(leveltime);	// 블록이 내려오는 속도 만큼 딜레이
		EnterCriticalSection(&cs);
		if (isStartGame && isBlockCreated)
			moveBlock(0, 1);	//블록을 한칸 아래로 이동
		LeaveCriticalSection(&cs);
	}
}
void listenMsg() {
	int count;

	while (isStartGame) {

		if (msgState) {	// 상태 메세지가 들어오면
			msgState = FALSE;	// 바로 FALSE로 변경해 바로 메세지 대기
			count = 0;
			while (count != 1000) {	// 약 1초 대기. while을 쓰는 이유는 대기중 다른 메세지를 받을수 있기때문
				delay(1);
				count++;
				if (msgState) {	//대기중 메세지 들어오면 다시 처음부터 대기
					count = 0;
					msgState = FALSE;
				}
			}

			EnterCriticalSection(&cs);
			for (int i = 0; i < 6; i++) {
				gotoxy(MSG_X, MSG_Y + i);	printf("              ");	// 메세지를 지운다.
			}
			LeaveCriticalSection(&cs);
		}
		delay(1);
	}
}

