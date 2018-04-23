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
	//������� ����
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {	// (��û�ϰ��� �ϴ� ������ ����, wsadata����ü ������ �ּҰ� ����) �Ű�����2�� �Լ�ȣ���� ���� �־�дٴµ�....
		printf("ERROR_WSA���¿���..\n");
		return;
	}
	// ���ο� ������ ����
	listen_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (listen_sock == INVALID_SOCKET) {
		printf("ERROR_���� ���� ����\n");
		return;
	}
	// �����ּ� ���� ����
	memset(&listen_addr, 0, sizeof(listen_addr));
	listen_addr.sin_family = AF_INET;	//��Ź�� : TCP/IP
	listen_addr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);	// ���� �ּ� ���� : 127.0.0.1(localhost)
//	listen_addr.sin_addr.S_un.S_addr = htonl(ntohl(inet_addr(GetDefaultMyIP_str())));	// ���� �ּ� ����
	listen_addr.sin_port = htons(PORT);	// ��Ʈ : PORT(1234)inet_ntoa(addr)
										// ���� ���ε�
	if (bind(listen_sock, (struct sockaddr*)&listen_addr, sizeof(listen_addr)) < 0) {
		printf("ERROR_���ε� ����\n");
		return;
	}
	// ���� ����(Ŭ���̾�Ʈ�� ��û�� ��ٸ���...)
	if (listen(listen_sock, clientCount) < 0) {	// �ִ� platerCount���� ������� ��ٸ���.....
		printf("ERROR_���� ���� ����\n");
		return;
	}

	mode = SERVERMODE;

	// Ŭ���̾�Ʈ�� clientCount�� ����
	clrscr();
	printf("������ ��ٸ�����.. ���� ������ ��: 1 / %d", clientCount + 1);
	printf("\n�� ������ �ּ� : %s", GetDefaultMyIP_str());
	sockaddr_in_size = sizeof(connect_addr);

	InitializeCriticalSection(&cs);	// �Ӱ迵�� ����

	for (char i = 0; i < clientCount; i++)
	{
		server_sock[i] = accept(listen_sock, (SOCKADDR*)&connect_addr, &sockaddr_in_size);

		if (server_sock[i] == INVALID_SOCKET)
			printf("accept()_connect_sock[%d] error", i);
		client_num = i;
		serverThread[i] = _beginthreadex(NULL, 0, (_beginthreadex_proc_type)serverRecvThread, NULL, 0, NULL);	// ������ ����
	//	txSock2(i, strcat("I", i));
		putixy(38, 0, i + 2);//�����ο����(Ŭ���̾�Ʈ�� + ����)
		char msg[10] = { 0 }, pt[] = "c";
		txSock(strcat(pt, itoa(10 * playerCnt + i + 2, msg, 10)));
	}

	//************�����͸� ���� �ְ���� �� �ִ�***************

	delay(1000);

	initQueueBlockes();//client�� Q�� ���������� ������ �������� ����
	txPlayers();
	char msg[1024] = { 0 };
	msg[0] = 's';
	for (char cnt = 3; cnt > 0; cnt--) {
		msg[1] = cnt;

		putsxy(20, 5, "����Ϸ�! ������ [ ] �� �Ŀ� �����մϴ�!!");
		putixy(38, 5, msg[1]);

		txSock(msg);
		delay(1000);
	}

	txSock("start");
	isStartGame = TRUE;
	gameStart();



	for (int i = 0; i < clientCount; i++) {
		closesocket(server_sock[i]);	//���ϵ��� �������ְ�
	}
	closesocket(listen_sock);

	WSACleanup();	//���� �ʱ�ȭ

}

void client() {
	clrscr();

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {	// (��û�ϰ��� �ϴ� ������ ����, wsadata����ü ������ �ּҰ� ����) �Ű�����2�� �Լ�ȣ���� ���� �־�дٴµ�....
		printf("ERROR_WSA���¿���..\n");
		return;
	}

	client_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (client_sock == INVALID_SOCKET) {	//���ο� ���� ������ ����
		printf("ERROR_�������Ͽ���\n");
		return;
	}

	//	printf("�������ּҸ� �Է��ϼ��� : ");	scanf("%s", &ip);
	strcpy(ip, "127.0.0.1");
	memset(&client_addr, 0, sizeof(client_addr));
	client_addr.sin_family = AF_INET;
	client_addr.sin_addr.s_addr = inet_addr(ip);
	client_addr.sin_port = htons(PORT);

	if (connect(client_sock, (SOCKADDR*)&client_addr, sizeof(client_addr)) < 0) { //�Է��� �ּҷ� ����
		clrscr(); setColor(RED);	putsxy(30, 10, "�� ���� ����!");	delay(2000);
		putsxy(30, 10, "             ");
		closesocket(client_sock);
		WSACleanup();
		return;
	}
	 
	mode = CLIENTMODE;
	InitializeCriticalSection(&cs);	// �Ӱ迵�� ����

	txSock(strcat("p", myName));
	clientThread = _beginthreadex(NULL, 0, (_beginthreadex_proc_type)clientRecvThread, NULL, 0, NULL);	// ������ ����
	setColor(WHITE); putsxy(0, 0, "������ ��ٸ�����.. ���� ������ ��: 0 / 0");

	while (!isStartGame)	delay(10);

	gameStart();

	closesocket(client_sock);
	WSACleanup();

}

int makeRoom() {
	char clientCount[40] = "�� �� �� ��(�ִ� 4��) : �� 2 ��";
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
			putsxy(50, 7, "��");
			delay(100);
			putsxy(50, 7, "��");
			break;
		case RIGHT:
			pcount = pcount == 4 ? 4 : pcount + 1;
			putsxy(55, 7, "��");
			delay(100);
			putsxy(55, 7, "��");
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

	if (gethostname(localhostname, MAX_PATH) == SOCKET_ERROR)//ȣ��Ʈ �̸� ������
	{
		return "ȣ��Ʈ�̸�����";
	}
	HOSTENT *ptr = gethostbyname(localhostname);//ȣ��Ʈ ��Ʈ�� ������
	while (ptr && ptr->h_name)
	{
		if (ptr->h_addrtype == PF_INET)//IPv4 �ּ� Ÿ���� ��
		{
			memcpy(&addr, ptr->h_addr_list[0], ptr->h_length);//�޸� ����
			break;//�ݺ��� Ż��
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