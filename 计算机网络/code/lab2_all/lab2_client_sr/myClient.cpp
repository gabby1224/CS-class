// GBN_client.cpp : �������̨Ӧ�ó������ڵ㡣
//
//#include "stdafx.h"
#define PATH "./upload.txt"
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <WinSock2.h>
#include <time.h>
#include <fstream>
#include <iostream>
#include <cmath>
#include <windows.h>
#include <process.h>

using namespace std;

#pragma comment(lib,"ws2_32.lib")

#define SERVER_PORT 12340     //�������ݵĶ˿ں�
#define SERVER_IP "127.0.0.1" // �������� IP ��ַ

const int BUFFER_LENGTH = 1026;
const int SEQ_SIZE = 20;//���ն����кŸ�����Ϊ 1~20
const int SEND_WIND_SIZE = 10; //���ʹ��ڴ�СΪ 10��GBN ��Ӧ���� W + 1 <=N��W Ϊ���ʹ��ڴ�С��N Ϊ���кŸ�����

BOOL havesave[SEQ_SIZE]; //�յ� ack �������Ӧ 0~19 �� ack


//�����Ҫupload��һЩ����
BOOL ack[SEQ_SIZE];//�յ� ack �������Ӧ 0~19 �� ack
int send_base;
int curSeq;						//��ǰ���ݰ��� seq
int curAck;						//��ǰ�ȴ�ȷ�ϵ� ack
int totalSeq;					//�յ��İ�������
int totalPacket;				//��Ҫ���͵İ�����
/****************************************************************/
/* -time �ӷ������˻�ȡ��ǰʱ��
-quit �˳��ͻ���
-testgbn [X] ���� GBN Э��ʵ�ֿɿ����ݴ���
[X] [0,1] ģ�����ݰ���ʧ�ĸ���
[Y] [0,1] ģ�� ACK ��ʧ�ĸ���
*/
/****************************************************************/
void printTips() {
	printf("*****************************************\n");
	printf("| -time to get current time |\n");
	printf("| -quit to exit client |\n");
	printf("| -download [X] [Y] to test the sr_download |\n");
	printf("| -upload [X] [Y] to test the sr_upload |\n");
	printf("*****************************************\n");
}
//************************************
// Method: lossInLossRatio
// FullName: lossInLossRatio
// Access: public
// Returns: BOOL
// Qualifier: ���ݶ�ʧ���������һ�����֣��ж��Ƿ�ʧ,��ʧ�򷵻�TRUE�����򷵻� FALSE
// Parameter: float lossRatio [0,1]
//************************************
BOOL lossInLossRatio(float lossRatio) {
	int lossBound = (int)(lossRatio * 100);
	int r = rand() % 101;
	if (r <= lossBound) {
		return TRUE;
	}
	return FALSE;
}
//************************************
// 
// ��Server����ճ��������
//************************************
// Method: ackHandler
// FullName: ackHandler
// Access: public
// Returns: void
// Qualifier: �յ� ack���ۻ�ȷ�ϣ�ȡ����֡�ĵ�һ���ֽ�
//���ڷ�������ʱ����һ���ֽڣ����кţ�Ϊ 0��ASCII��ʱ����ʧ�ܣ���˼�һ�ˣ��˴���Ҫ��һ��ԭ
// Parameter: char c
//************************************
void SR_ackHandler(char c)
{
	unsigned char index = (unsigned char)c - 1; //���кż�һ
	printf("Recv a ack of %d\n", index);

	if (index < send_base)
	{
		if (send_base < (send_base + SEND_WIND_SIZE - 1) - SEQ_SIZE - 1)
		{	//ack ���������ֵ���ص��� curAck �����
			//still in window
			ack[index] = 1;
		}
	}
	else
	{
		if (index > send_base)
		{
			ack[index] = 1;
		}
		else if (index == send_base)
		{//������ڣ���������
			ack[index] = 1;
			for (int i = 0; i < SEND_WIND_SIZE; i++)
			{
				if (ack[send_base] == 1) {
					send_base = (send_base + 1) % SEQ_SIZE;
				}
				if (send_base == curSeq)  break;

			}

		}
	}
}
//************************************
// Method: timeoutHandler
// FullName: timeoutHandler
// Access: public
// Returns: void
// Qualifier: ��ʱ�ش��������������������ڵ�����֡��Ҫ�ش�
// Ҫ��
//************************************
void timeoutHandler() {
	printf("Timer out error.\n");
	int index;
	for (int i = 0; i < SEND_WIND_SIZE; ++i) {
		index = (i + curAck) % SEQ_SIZE;
		ack[index] = TRUE;
	}
	totalSeq -= SEND_WIND_SIZE;
	curSeq = curAck;
}
//************************************
// Method: seqIsAvailable
// FullName: seqIsAvailable
// Access: public
// Returns: bool
// Qualifier: ��ǰ���к� curSeq �Ƿ����
//************************************
bool SR_seqIsAvailable() {
	int step;
	step = curSeq - send_base;
	step = step >= 0 ? step : step + SEQ_SIZE;
	//���к��Ƿ��ڵ�ǰ���ʹ���֮��
	if (step >= SEND_WIND_SIZE) {
		return false;
	}
	if (ack[curSeq]) {
		return true;
	}
	return false;
}


//�����׽��ֿ⣨���룩
WORD wVersionRequested;
WSADATA wsaData;
//�׽��ּ���ʱ������ʾ
int err;
//�汾 2.2
SOCKET socketClient;
SOCKADDR_IN addrServer;
//���ջ�����
char buffer[BUFFER_LENGTH];
int len = sizeof(SOCKADDR);
int ret;
int interval = 1;//�յ����ݰ�֮�󷵻� ack �ļ����Ĭ��Ϊ 1 ��ʾÿ�������� ack�� 0 ���߸�������ʾ���еĶ������� ack
char cmd[128];
float packetLossRatio = 0.2; //Ĭ�ϰ���ʧ�� 0.2
float ackLossRatio = 0.2; //Ĭ�� ACK ��ʧ�� 0.2
//��ʱ����Ϊ������ӣ�����ѭ����������

int main(int argc, char* argv[])
{
	
	wVersionRequested = MAKEWORD(2, 2);
	//���� dll �ļ� Scoket ��
	err = WSAStartup(wVersionRequested, &wsaData);
	if (err != 0) {
		//�Ҳ��� winsock.dll
		printf("WSAStartup failed with error: %d\n", err);
		return 1;
	}
	if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2)
	{
		printf("Could not find a usable version of Winsock.dll\n");
		WSACleanup();
	}
	else {
		printf("The Winsock 2.2 dll was found okay\n");
	}

	//SOCKET socketClient = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	socketClient = socket(AF_INET, SOCK_DGRAM, 0);



	addrServer.sin_addr.S_un.S_addr = inet_addr(SERVER_IP);
	addrServer.sin_family = AF_INET;
	addrServer.sin_port = htons(SERVER_PORT);

	ZeroMemory(buffer, sizeof(buffer));

	//Ϊ�˲���������������ӣ�����ʹ�� -time ����ӷ������˻�õ�ǰʱ��
	//ʹ�� -testgbn [X] [Y] ���� GBN ����[X]��ʾ���ݰ���ʧ����
	// [Y]��ʾ ACK ��������
	printTips();

	srand((unsigned)time(NULL));
	while (true) {
		gets_s(buffer);
		ret = sscanf(buffer, "%s%f%f", &cmd, &packetLossRatio, &ackLossRatio);
		//��ʼ GBN ���ԣ�ʹ�� GBN Э��ʵ�� UDP �ɿ��ļ�����
		if (!strcmp(cmd, "-download")) {
			printf("%s\n", "Begin to test SR protocol, please don't abort the process");
			printf("The loss ratio of packet is %.2f,the loss ratio of ack is % .2f\n", packetLossRatio, ackLossRatio);
			printf("Sequence is from 1 to 20......\n");
			int waitCount = 0;
			int stage = 0;
			BOOL b;

			unsigned char u_code;//״̬��
			unsigned short seq;//�������к�
			unsigned short recvSeq;//���մ��ڴ�СΪ 1����ȷ�ϵ����к�
			unsigned short waitSeq;//�ȴ������к�

			bool entry = true;

			sendto(socketClient, "-download", strlen("-download") + 1, 0,
				(SOCKADDR*)&addrServer, sizeof(SOCKADDR));
			while (entry)
			{
				//�ȴ� server �ظ����� UDP Ϊ����ģʽ
				recvfrom(socketClient, buffer, BUFFER_LENGTH, 0, (SOCKADDR*)&addrServer, &len);
				switch (stage) {
				case 0://�ȴ����ֽ׶�
					u_code = (unsigned char)buffer[0];
					if ((unsigned char)buffer[0] == 205)
					{
						printf("Ready for file transmission\n");
						buffer[0] = 200;
						buffer[1] = '\0';
						sendto(socketClient, buffer, 2, 0,
							(SOCKADDR*)&addrServer, sizeof(SOCKADDR));
						stage = 1;
						recvSeq = 0;
						waitSeq = 1;
					}
					break;
				case 1://�ȴ��������ݽ׶�
					seq = (unsigned short)buffer[0];

					if (buffer[0] == 21)
					{
						entry = false;
						printf("\n\n%s\n\n", &buffer[1]);
						ZeroMemory(havesave, sizeof(havesave));
						memcpy(buffer, "-time", sizeof("-time"));
						break;
					}
					//�����ģ����Ƿ�ʧ
					b = lossInLossRatio(packetLossRatio);
					if (b) {
						printf("The packet with a seq of %d loss\n", seq);
						continue;
					}
					printf("recv a packet with a seq of %d\n", seq);

					//�������[recv_base,recv_base + N - 1]������ACK��û������ͻ��棻
					//���recv_base == ����ack���������ڵ������ķ����֮��
					if (waitSeq <= seq < waitSeq + SEND_WIND_SIZE)
					{
						//����ACK
						////////todo-----done!!!!
						buffer[0] = seq;

						if (!havesave[seq - 1])//û������ͻ���
						{
							havesave[seq - 1] = 1;

							ofstream fout;
							fout.open("S_4_C.txt", ios::out | ios::app, _SH_DENYNO);
							if (!fout.is_open())
							{
								printf("cannot open file %s", "S_4_C.txt");
							}
							else
							{
								fout.write(&buffer[1], sizeof(char) * 1024);
								fout.flush();
								//sleep(500);
							}
						}

						//���waitseq�ͱ��������ȣ����ڻ������ύ��
						if (!(waitSeq - seq)) {
							int count = 0;//��¼���ڻ����˼���
							int i;
							for (i = 0; i < SEND_WIND_SIZE; i++)
							{
								if (havesave[waitSeq + i - 1] == 1)
								{
									havesave[waitSeq + i - 1] = 0;
									waitSeq = (waitSeq + 1) % SEQ_SIZE;
									continue;
								}
								break;
							}
							//�������
							printf("pkg submit...��\n%s\n", &buffer[1]);
						}
						buffer[1] = '\0';
					}
					else if (waitSeq - SEND_WIND_SIZE <= seq < waitSeq)
					{
						buffer[0] = seq;
						//recvseq = seq;
						buffer[1] = '\0';
					}


					b = lossInLossRatio(ackLossRatio);
					if (b) {
						printf("The ack of %d loss\n", (unsigned char)buffer[0]);
						continue;
					}
					sendto(socketClient, buffer, 2, 0, (SOCKADDR*)&addrServer, sizeof(SOCKADDR));
					printf("send a ack of %d\n", (unsigned char)buffer[0]);
					break;
				}
				Sleep(500);
			}
		}
		else if (!strcmp(cmd, "-upload"))
		{
			//Ϊ����ɳ�ʱ��ʱ
			//SOCKET socketClient = socket(AF_INET, SOCK_DGRAM, 0);IPPROTO_UDP
			SOCKET socketClient2 = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

			//�����׽���Ϊ������ģʽ
			int iMode = 1; //1���������� 0������
			ioctlsocket(socketClient2, FIONBIO, (u_long FAR*) & iMode);//����������


			//���������ݶ����ڴ�
			std::ifstream icin;
			icin.open(PATH);
			char data[1024 * 23];
			ZeroMemory(data, sizeof(data));
			icin.read(data, 1024 * 24);
			icin.close();
			totalPacket = sizeof(data) / 1024;
			int recvSize;
			for (int i = 0; i < SEQ_SIZE; ++i) {
				ack[i] = TRUE;
			}
			printf("%s\n", "Begin to test SR protocol, please don't abort the process");
			printf("Sequence is from 0 to 19......\n");
			sendto(socketClient2, "-testupload", strlen("-testupload") + 1, 0,
				(SOCKADDR*)&addrServer, sizeof(SOCKADDR));
			//���� sr ���Խ׶�

			ZeroMemory(buffer, sizeof(buffer));
			int waitCount = 0;

			//������һ�����ֽ׶�

			printf("Shake hands stage\n");
			int stage = 0;
			bool runFlag = true;
			Sleep(500);
			while (runFlag) {
				switch (stage) {
				case 0://���� 205 �׶�
					buffer[0] = 205;
					printf("%d", buffer[0]);
					sendto(socketClient2, buffer, strlen(buffer) + 1, 0, (SOCKADDR*)&addrServer, sizeof(SOCKADDR));
					Sleep(100);
					stage = 1;
					break;
				case 1://�ȴ����� 200 �׶Σ�û���յ��������+1����ʱ������˴Ρ����ӡ����ȴ��ӵ�һ����ʼ
					recvSize =
						recvfrom(socketClient2, buffer, BUFFER_LENGTH, 0, ((SOCKADDR*)&addrServer), &len);
					if (recvSize < 0) {
						++waitCount;
						if (waitCount > 20) {
							//��ʱ
							runFlag = false;
							printf("Timeout error\n");
							break;
						}
						Sleep(500);
						continue;
					}
					else {
						if ((unsigned char)buffer[0] == 200) {
							printf("Begin a file transfer\n");
							printf("File size is %dB, each packet is 1024B and packet total num is % d\n", sizeof(data), totalPacket);
							curSeq = 0;
							send_base = 0;
							totalSeq = 0;
							waitCount = 0;
							stage = 2;
						}
					}
					break;
				case 2://���ݴ���׶�
					//TODO........
					if (totalSeq >= totalPacket)
					{
						runFlag = false;
						buffer[0] = 21;
						memcpy(&buffer[1], "Transmission done!", 1024);
						printf("Transmission done!\n");
						//sendto(socketClient2, buffer, BUFFER_LENGTH, 0,
						//	(SOCKADDR*)&addrServer, sizeof(SOCKADDR));
						closesocket(socketClient2);
					}
					else
					{
						if (SR_seqIsAvailable()) {
							//���͸�Server�����кŴ� 1 ��ʼ
							buffer[0] = curSeq + 1;
							ack[curSeq] = FALSE;
							memcpy(&buffer[1], data + 1024 * totalSeq, 1024);
							printf("send a packet with a seq of %d\n", curSeq);
							sendto(socketClient2, buffer, BUFFER_LENGTH, 0,
								(SOCKADDR*)&addrServer, sizeof(SOCKADDR));

							++curSeq;
							curSeq %= SEQ_SIZE;
							++totalSeq;
							Sleep(500);
						}
						//�ȴ� Ack����û���յ����򷵻�ֵΪ-1��������+1
						//�������
						recvSize = recvfrom(socketClient2, buffer, BUFFER_LENGTH, 0, ((SOCKADDR*)&addrServer), &len);
						if (recvSize < 0) {
							waitCount++;
							//20 �εȴ� ack ��ʱ�ش�
							if (waitCount > 20)
							{
								//��ʱ�������Ҫ�ط�
								printf("time out error,resend a packet with a seq of %d\n", send_base);
								buffer[0] = send_base + 1;
								ack[send_base] = FALSE;
								memcpy(&buffer[1], data + 1024 * (totalSeq - (curSeq - send_base)), 1024);
								sendto(socketClient2, buffer, BUFFER_LENGTH, 0, (SOCKADDR*)&addrServer, sizeof(SOCKADDR));
								//����statrer
								waitCount = 0;
							}
						}
						else {
							//�յ� ack
							SR_ackHandler(buffer[0]);
							waitCount = 0;
						}
					}

					Sleep(500);
					break;
				}
			}

		}
		//�ر��׽���
		sendto(socketClient, buffer, strlen(buffer) + 1, 0,
			(SOCKADDR*)&addrServer, sizeof(SOCKADDR));
		ret = recvfrom(socketClient, buffer, BUFFER_LENGTH, 0, (SOCKADDR*)&addrServer, &len);
		printf("%s\n", buffer);
		if (!strcmp(buffer, "Good bye!")) {
			break;
		}
		printTips();
	}
	

	closesocket(socketClient);
	WSACleanup();
	return 0;
}