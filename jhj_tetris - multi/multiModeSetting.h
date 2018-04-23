#pragma once
#include "tools.h"

#define PORT 1234

WSADATA wsaData;
SOCKET listen_sock, server_sock[4], client_sock;
SOCKADDR_IN listen_addr, connect_addr[4], client_addr;
int sockaddr_in_size;
char message[100];
char ip[20];
HANDLE serverThread[3], clientThread;

void server(int playerCnt) {
	if (playerCnt < 2) { putsxy(26, 7, "                               ");	return; }
	clientCount = playerCnt - 1;
	//소켓통신 설정
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {	// (요청하고자 하는 소켓의 버전, wsadata구조체 변수의 주소값 전달) 매개변수2는 함수호출을 위해 넣어둔다는데....
		printf("ERROR_WSA오픈에러..\n");
		return;
	}
	// 새로운 소켓을 생성
	listen_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (listen_sock == INVALID_SOCKET) {
		printf("ERROR_소켓 생성 에러\n");
		return;
	}
	// 소켓주소 정보 설정
	memset(&listen_addr, 0, sizeof(listen_addr));
	listen_addr.sin_family = AF_INET;	//통신방싱 : TCP/IP
	listen_addr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);	// 서버 주소 설정 : 127.0.0.1(localhost)
//	listen_addr.sin_addr.S_un.S_addr = htonl(ntohl(inet_addr(GetDefaultMyIP_str())));	// 서버 주소 설정
	listen_addr.sin_port = htons(PORT);	// 포트 : PORT(1234)inet_ntoa(addr)
										// 소켓 바인드
	if (bind(listen_sock, (struct sockaddr*)&listen_addr, sizeof(listen_addr)) < 0) {
		printf("ERROR_바인드 실패\n");
		return;
	}
	// 소켓 리슨(클라이언트의 요청을 기다린다...)
	if (listen(listen_sock, clientCount) < 0) {	// 최대 platerCount개의 연결까지 기다린다.....
		printf("ERROR_소켓 리슨 실패\n");
		return;
	}

	mode = SERVERMODE;

	// 클라이언트를 clientCount명 받음
	clrscr();
	printf("상대방을 기다리는중.. 현재 접속자 수: 1 / %d", clientCount + 1);
	printf("\n내 아이피 주소 : %s", GetDefaultMyIP_str());
	sockaddr_in_size = sizeof(connect_addr);

	InitializeCriticalSection(&cs);	// 임계영역 설정

	for (char i = 0; i < clientCount; i++)
	{
		server_sock[i] = accept(listen_sock, (SOCKADDR*)&connect_addr, &sockaddr_in_size);

		if (server_sock[i] == INVALID_SOCKET)
			printf("accept()_connect_sock[%d] error", i);
		client_num = i;
		serverThread[i] = _beginthreadex(NULL, 0, (_beginthreadex_proc_type)serverRecvThread, NULL, 0, NULL);	// 쓰레드 설정
	//	txSock2(i, strcat("I", i));
		putixy(38, 0, i + 2);//접속인원출력(클라이언트수 + 서버)
		char msg[10] = { 0 }, pt[] = "c";
		txSock(strcat(pt, itoa(10 * playerCnt + i + 2, msg, 10)));
	}

	//************데이터를 이제 주고받을 수 있다***************

	delay(1000);

	initQueueBlockes();//client에 Q를 보내기위해 게임이 실행전에 전송
	txPlayers();
	char msg[1024] = { 0 };
	msg[0] = 's';
	for (char cnt = 3; cnt > 0; cnt--) {
		msg[1] = cnt;

		putsxy(20, 5, "입장완료! 게임이 [ ] 초 후에 시작합니다!!");
		putixy(38, 5, msg[1]);

		txSock(msg);
		delay(1000);
	}

	txSock("start");
	isStartGame = TRUE;
	gameStart();



	for (int i = 0; i < clientCount; i++) {
		closesocket(server_sock[i]);	//소켓들을 정리해주고
	}
	closesocket(listen_sock);

	WSACleanup();	//윈속 초기화

}

void client() {
	clrscr();

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {	// (요청하고자 하는 소켓의 버전, wsadata구조체 변수의 주소값 전달) 매개변수2는 함수호출을 위해 넣어둔다는데....
		printf("ERROR_WSA오픈에러..\n");
		return;
	}

	client_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (client_sock == INVALID_SOCKET) {	//새로운 소켓 생성후 대입
		printf("ERROR_리슨소켓에러\n");
		return;
	}

	//	printf("아이피주소를 입력하세요 : ");	scanf("%s", &ip);
	strcpy(ip, "127.0.0.1");
	memset(&client_addr, 0, sizeof(client_addr));
	client_addr.sin_family = AF_INET;
	client_addr.sin_addr.s_addr = inet_addr(ip);
	client_addr.sin_port = htons(PORT);

	if (connect(client_sock, (SOCKADDR*)&client_addr, sizeof(client_addr)) < 0) { //입력한 주소로 접속
		clrscr(); setColor(RED);	putsxy(30, 10, "방 접속 실패!");	delay(2000);
		putsxy(30, 10, "             ");
		closesocket(client_sock);
		WSACleanup();
		return;
	}
	 
	mode = CLIENTMODE;
	InitializeCriticalSection(&cs);	// 임계영역 설정

	txSock(strcat("p", myName));
	clientThread = _beginthreadex(NULL, 0, (_beginthreadex_proc_type)clientRecvThread, NULL, 0, NULL);	// 쓰레드 설정
	setColor(WHITE); putsxy(0, 0, "상대방을 기다리는중.. 현재 접속자 수: 0 / 0");

	while (!isStartGame)	delay(10);

	gameStart();

	closesocket(client_sock);
	WSACleanup();

}

int makeRoom() {
	char clientCount[40] = "플 레 이 어(최대 4인) : ◁ 2 ▷";
	clrscr();
	setColor(WHITE);
	putsxy(26, 7, clientCount);
	int pcount = 2;
	boolean isChosePlayer = TRUE;
	while (isChosePlayer) {
		putixy(53, 7, pcount);
		char ch2 = _getch();
		switch (ch2) {
		case LEFT:
			pcount = pcount == 2 ? 2 : pcount - 1;
			putsxy(50, 7, "◀");
			delay(100);
			putsxy(50, 7, "◁");
			break;
		case RIGHT:
			pcount = pcount == 4 ? 4 : pcount + 1;
			putsxy(55, 7, "▶");
			delay(100);
			putsxy(55, 7, "▷");
			break;
		case ENTER:
			return pcount;
			break;
		case ESC:
			return 0;
			break;
		}
	}

}

char* GetDefaultMyIP_str() {
	char localhostname[MAX_PATH];
	IN_ADDR addr = { 0, };

	if (gethostname(localhostname, MAX_PATH) == SOCKET_ERROR)//호스트 이름 얻어오기
	{
		return "호스트이름오류";
	}
	HOSTENT *ptr = gethostbyname(localhostname);//호스트 엔트리 얻어오기
	while (ptr && ptr->h_name)
	{
		if (ptr->h_addrtype == PF_INET)//IPv4 주소 타입일 때
		{
			memcpy(&addr, ptr->h_addr_list[0], ptr->h_length);//메모리 복사
			break;//반복문 탈출
		}
		ptr++;
	}

	return	inet_ntoa(addr);
}

void txPlayers() {
	char msg[1024];
	for (int i = 0; i < clientCount; i++) {
		if (i == 0) {
			memset(msg, 0, sizeof(msg));
			sprintf(msg, "p%s%c%s%c%s", myName, 0x01, playerName[1], 0x01, playerName[2]);
		}
		else if (i == 1) {
			memset(msg, 0, sizeof(msg));
			sprintf(msg, "p%s%c%s%c%s", myName, 0x01, playerName[0], 0x01, playerName[2]);
		}
		else {
			memset(msg, 0, sizeof(msg));
			sprintf(msg, "p%s%c%s%c%s", myName, 0x01, playerName[0], 0x01, playerName[1]);
		}
		
		send(server_sock[i], msg, (int)strlen(msg) + 1, 0);
	}
}
void txSock(char msg[1024]) {
	if (ISCLIENT) {
		send(client_sock, msg, (int)strlen(msg)+1, 0);
	}
	else if (ISSERVER) {
		for (int i = 0; i < clientCount; i++) {
			send(server_sock[i], msg, (int)strlen(msg)+1, 0);
		}
	}
}

void reflexBlocks(char msg[1024], int exception) {

	char reflex[1024] = { 0 };
	char tmp[10] = { 0 };
	int location[2] = { 0 };
	int idx = 0;

	if (exception == 0) {
		location[0] = 1;
		location[1] = 1;
	}
	else if (exception == 1) {
		location[0] = 1;
		location[1] = 2;
	}
	else if (exception == 2) {
		location[0] = 2;
		location[1] = 2;
	}


	for (int i = 0; i < clientCount; i++) {
		if (i == exception)	continue;

		sprintf(reflex, "%s%s%s", msg, "R", itoa(location[idx++], tmp, 10));
		send(server_sock[i], reflex, (int)strlen(reflex) + 1, 0);
	}
}
void reflex(char msg[1024], int from) {
	for (int i = 0; i < clientCount; i++) {
		if (i == from)	continue;
		send(server_sock[i], msg, (int)strlen(msg)+1, 0);
	}
}

void txMovingBlock(int x, int y, int block, int motion) {
	char msg[10] = { 0 };
	msg[0] = 'B';
	msg[1] = (char)(x / 10) + 0x30;
	msg[2] = (char)(x % 10) + 0x30;
	msg[3] = (char)(y / 10) + 0x30;
	msg[4] = (char)(y % 10) + 0x30;
	msg[5] = (char)(block / 10) + 0x30;
	msg[6] = (char)(block % 10) + 0x30;
	msg[7] = (char)(motion / 10) + 0x30;
	msg[8] = (char)(motion % 10) + 0x30;

	txSock(msg);
}
void txStageData() {
	char txStage[1024] = { 0 };
	txStage[0] = 'b';
	for (int i = 20; i >= 1; i--) {
		for (int j = 1; j <= 10; j++) {
			txStage[10 * (20 - i) + j] = stage[i][j] + 0x30;
		}
	}

	txSock(txStage);
}