#include "Tetris_Header.h"

#define PORT 1234

extern int stage[22][12];
extern int clientCount;
extern int mode;
extern int sockaddr_in_size;
extern char ip[20];
extern char client_num;
extern char playerName[3][50];
extern char myName[50];
extern boolean isStartGame;
extern HANDLE serverThread[3], clientThread;
extern CRITICAL_SECTION cs;
extern WSADATA wsaData;
extern SOCKET listen_sock, server_sock[4], client_sock;
extern SOCKADDR_IN listen_addr, connect_addr[4], client_addr;


void server(int playerCnt) {
	if (playerCnt < 2) { putsxy(26, 7, "                               ");	return; }
	clientCount = playerCnt - 1;
	//소켓통신 설정
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {	// (요청하고자 하는 소켓의 버전, wsadata구조체 변수의 주소값 전달)
		printf("ERROR_WSA오픈에러..\n");
		return;
	}
	// 새로운 소켓을 생성
	listen_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (listen_sock == INVALID_SOCKET) {
		printf("ERROR_소켓 생성 에러\n");
		return;
	}
	// 소켓주소 설정
	memset(&listen_addr, 0, sizeof(listen_addr));
	listen_addr.sin_family = AF_INET;	//통신방싱 : TCP/IP
//	listen_addr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);	// 서버 주소 설정 : 127.0.0.1(localhost)
	listen_addr.sin_addr.S_un.S_addr = htonl(ntohl(inet_addr(GetDefaultMyIP_str())));	// 서버 주소 설정
	listen_addr.sin_port = htons(PORT);	// 포트 : PORT(1234)inet_ntoa(addr)

	// 소켓 바인드
	if (bind(listen_sock, (struct sockaddr*)&listen_addr, sizeof(listen_addr)) < 0) {
		printf("ERROR_바인드 실패\n");
		return;
	}
	// 소켓 리슨
	if (listen(listen_sock, clientCount) < 0) {	
		printf("ERROR_소켓 리슨 실패\n");
		return;
	}

	mode = SERVERMODE;	// 게임모드 : 서버모드

	// 클라이언트를 clientCount명 받음
	clrscr();
	printf("상대방을 기다리는중.. 현재 접속자 수: 1 / %d", clientCount + 1);
	printf("\n내 아이피 주소 : %s", GetDefaultMyIP_str());

	InitializeCriticalSection(&cs);	// 임계영역 설정

	for (char i = 0; i < clientCount; i++)
	{
		sockaddr_in_size = sizeof(connect_addr[i]);
		server_sock[i] = accept(listen_sock, (SOCKADDR*)&connect_addr[i], &sockaddr_in_size);	// 소켓연결이 들어올때까지 대기

		if (server_sock[i] == INVALID_SOCKET)
			printf("accept()_connect_sock[%d] error", i);	// 소켓 연결 실패

		client_num = i;
		serverThread[i] = (HANDLE)_beginthreadex(NULL, 0, (_beginthreadex_proc_type)serverRecvThread, NULL, 0, NULL);	// 쓰레드 설정
		
		putixy(38, 0, i + 2);//접속인원출력(클라이언트수 + 서버)
		
		char msg[1024] = { 0 };	sprintf(msg, "%s%d", "c", 10 * playerCnt + i + 2);
		txSock(msg);

	}

	//************소켓 기본설정 완료***************

	initQueueBlockes();	//client에 Q를 보내기위해 게임이 실행전에 전송
	txPlayers();	// 화면에 출력하기위해 플레이어 이름을 보낸다.

	char msg[1024] = { 0 };
	msg[0] = 's';

	delay(1000);
	for (char cnt = 3; cnt > 0; cnt--) {	// 3초후 게임 시작
		msg[1] = cnt;

		putsxy(20, 5, "입장완료! 게임이 [ ] 초 후에 시작합니다!!");
		putixy(38, 5, msg[1]);

		txSock(msg);
		delay(1000);
	}

	txSock("start");	//게임을 시작한다.
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

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {	// (요청하고자 하는 소켓의 버전, wsadata구조체 변수의 주소값 전달)
		printf("ERROR_WSA오픈에러..\n");
		return;
	}

	client_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (client_sock == INVALID_SOCKET) {	//새로운 소켓 생성후 대입
		printf("ERROR_리슨소켓에러\n");
		return;
	}

	setColor(WHITE);	printf("서버의 아이피주소를 입력하세요 : ");	scanf("%s", &ip);

	memset(&client_addr, 0, sizeof(client_addr));
	client_addr.sin_family = AF_INET;
	client_addr.sin_addr.s_addr = inet_addr(ip);
	client_addr.sin_port = htons(PORT);

	if (connect(client_sock, (SOCKADDR*)&client_addr, sizeof(client_addr)) < 0) { //입력한 주소로 접속
		clrscr(); setColor(RED);	putsxy(30, 10, "방 접속 실패!");	delay(2000);	// 실패시 실패를 알린다.
		putsxy(30, 10, "             "); 
		closesocket(client_sock);
		WSACleanup();
		return;
	}
	 
	mode = CLIENTMODE;	// 게임모드 : 클라이언트 모드
	InitializeCriticalSection(&cs);	// 임계영역 설정

	char msg_tmp[1024];	sprintf(msg_tmp, "%s%s", "p", myName);
	txSock(msg_tmp);

	clientThread = (HANDLE)_beginthreadex(NULL, 0, (_beginthreadex_proc_type)clientRecvThread, NULL, 0, NULL);	// 쓰레드 설정
	putsxy(0, 0, "                                                                     ");
	setColor(WHITE); putsxy(0, 0, "상대방을 기다리는중.. 현재 접속자 수: 0 / 0");

	while (!isStartGame)	delay(10);	// 게임이 시작될때 까지 대기한다.

	gameStart();	//게임 시작

	closesocket(client_sock);	// 소켓을 닫는다.
	WSACleanup();

}

// 멀티모드 게임의 최대 인원을 선택한다. 최대인원을 반환하고 취소할경우 0을 반환한다.
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
	return 0;
}

// 아이피를 문자열로 출력한다.
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

	return	inet_ntoa(addr);	// 주소를 문자열로 변환한다.
}

// 플레이어의 이름을 다른 플레이어에게 보낸다.
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

// 소켓통신으로 데이터를 보낸다.(서버->클라이언트 // 클라이언트 -> 서버)
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

// 서버가 받은 블럭정보를 다른 클라이언트에게 보낸다.
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

		sprintf(reflex, "%s%s%s", msg, "R", _itoa(location[idx++], tmp, 10));
		send(server_sock[i], reflex, (int)strlen(reflex) + 1, 0);
	}
}

// 서버가 받은 정보를 다른 클라이언트에게 보낸다.
void reflex(char msg[1024], int from) {
	for (int i = 0; i < clientCount; i++) {
		if (i != from) {
			send(server_sock[i], msg, (int)strlen(msg) + 1, 0);
		}
	}
}

// 떨어지고 있는 블럭의 정보를 보낸다.
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

// 벽(stage)의 정보를 보낸다. 
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