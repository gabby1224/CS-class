//#include "stdafx.h" //���� VS ��Ŀ������Ԥ����ͷ�ļ�
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#define PATH ".//test.txt"
#include <stdlib.h>
#include <time.h>
#include <WinSock2.h>
#include <fstream>
#include <iostream>
#include <cmath>
#include <windows.h>
#include <process.h>

using namespace std;

#pragma comment(lib,"ws2_32.lib")

#define SERVER_PORT 12340       //�˿ں�
#define SERVER_PORT2 8080      //�˿ں�
#define SERVER_IP "0.0.0.0"     //IP ��ַ

const int BUFFER_LENGTH = 1026; //��������С������̫���� UDP ������֡�а�����ӦС�� 1480 �ֽڣ�
const int SEND_WIND_SIZE = 10;//���ʹ��ڴ�СΪ 10�� GBN ��Ӧ���� W + 1 <= N��W Ϊ���ʹ��ڴ�С�� N Ϊ���кŸ�����
						
//����ȡ���к� 0...19 �� 20 ��
//��������ڴ�С��Ϊ 1����Ϊͣ-��Э��
const int SEQ_SIZE = 20; //���кŵĸ������� 0~19 ���� 20 ��
								//���ڷ������ݵ�һ���ֽ����ֵΪ 0�������ݻᷢ��ʧ��
								//��˽��ն����к�Ϊ 1~20���뷢�Ͷ�һһ��Ӧ
BOOL ack[SEQ_SIZE];//�յ� ack �������Ӧ 0~19 �� ack
BOOL havesave[SEQ_SIZE];
int send_base;
int curSeq;						//��ǰ���ݰ��� seq
int curAck;						//��ǰ�ȴ�ȷ�ϵ� ack
int totalSeq;					//�յ��İ�������
int totalPacket;				//��Ҫ���͵İ�����
int timer[SEQ_SIZE];
int u_code;
int recvSeq;
int waitSeq;
int seq;
int b;

//�����׽��ֿ⣨���룩
WORD wVersionRequested;
WSADATA wsaData;
//�׽��ּ���ʱ������ʾ
int err;
SOCKET sockServer;
SOCKET sockServer2;
int iMode;
SOCKADDR_IN addrServer; //��������ַ
SOCKADDR_IN addrServer2; //��������ַ
SOCKADDR_IN addrClient; //�ͻ��˵�ַ
int length = sizeof(SOCKADDR);
char buffer[BUFFER_LENGTH]; //���ݷ��ͽ��ջ�����
char buffer2[BUFFER_LENGTH];
char mydata[1024 * 23];
int recvSize;
int recvSize2;
int stage = 0;
bool runFlag = true;
int stage2 = 0;

//************************************
// Method: getCurTime
// FullName: getCurTime
// Access: public
// Returns: void
// Qualifier: ��ȡ��ǰϵͳʱ�䣬������� ptime ��
// Parameter: char * ptime
//************************************
void getCurTime(char* ptime) {
	char buffer[128];
	memset(buffer, 0, sizeof(buffer));
	time_t c_time;
	struct tm* p;
	time(&c_time);
	p = localtime(&c_time);
	sprintf_s(buffer, "%d/%d/%d %d:%d:%d",
		p->tm_year + 1900,
		p->tm_mon,
		p->tm_mday,
		p->tm_hour,
		p->tm_min,
		p->tm_sec);
	strcpy_s(ptime, sizeof(buffer), buffer);
}
//************************************
// Method: seqIsAvailable
// FullName: seqIsAvailable
// Access: public
// Returns: bool
// Qualifier: ��ǰ���к� curSeq �Ƿ����
//************************************
bool seqIsAvailable() {
	int step;
	step = curSeq - curAck;
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
//************************************
// Method: timeoutHandler
// FullName: timeoutHandler
// Access: public
// Returns: void
// Qualifier: ��ʱ�ش������������������ڵ�����֡��Ҫ�ش�
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
// Method: ackHandler
// FullName: ackHandler
// Access: public
// Returns: void
// Qualifier: �յ� ack���ۻ�ȷ�ϣ�ȡ����֡�ĵ�һ���ֽ�
//���ڷ�������ʱ����һ���ֽڣ����кţ�Ϊ 0��ASCII��ʱ����ʧ�ܣ���˼�һ�ˣ��˴���Ҫ��һ��ԭ
// Parameter: char c
//************************************
void ackHandler(char c) {
	unsigned char index = (unsigned char)c - 1; //���кż�һ
	printf("Recv a ack of %d\n", index);
	//�ۼ�ȷ�ϵĻ��ƣ��յ���ACK֮ǰ�Ķ�Ĭ�϶����ˣ�ack[]��λ1
	if (curAck <= index) {
		for (int i = curAck; i <= index; ++i) {
			ack[i] = TRUE;
		}
		curAck = (index + 1) % SEQ_SIZE;
	}
	else {
		//ack ���������ֵ���ص��� curAck �����
		for (int i = curAck; i < SEQ_SIZE; ++i) {
			ack[i] = TRUE;
		}
		for (int i = 0; i <= index; ++i) {
			ack[i] = TRUE;
		}
		curAck = index + 1;
	}
}
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

//////////////////////////////////////
void __stdcall Sender()
{
	//���� sr ���Խ׶�
				//���� server��server ���� 0 ״̬���� client ���� 205 ״̬�루server���� 1 ״̬��
				//server �ȴ� client �ظ� 200 ״̬�룬����յ���server ���� 2 ״̬������ʼ�����ļ���������ʱ�ȴ�ֱ����ʱ\
				//���ļ�����׶Σ� server ���ʹ��ڴ�С��Ϊ
	ZeroMemory(buffer, sizeof(buffer));
	int waitCount = 0;
	runFlag = true;
	stage = 0;
	printf("Begain to test SR protocol,please don't abort the process\n");
	printf("Sequence is from 0 to 19......\n");
	//������һ�����ֽ׶�
		//���ȷ�������ͻ��˷���һ�� 205 ��С��״̬�루���Լ�����ģ���ʾ������׼�����ˣ����Է�������
		//�ͻ����յ� 205 ֮��ظ�һ�� 200 ��С��״̬�룬��ʾ�ͻ���׼�����ˣ����Խ���������
		//�������յ� 200 ״̬��֮�󣬾Ϳ�ʼʹ�� GBN ����������
	printf("Shake hands stage\n");

	while (runFlag) {
		switch (stage) {
		case 0://���� 205 �׶�
			buffer[0] = 205;
			sendto(sockServer, buffer, strlen(buffer) + 1, 0, (SOCKADDR*)&addrServer, sizeof(SOCKADDR));
			Sleep(100);
			stage = 1;
			break;
		case 1://�ȴ����� 200 �׶Σ�û���յ��������+1����ʱ������˴Ρ����ӡ����ȴ��ӵ�һ����ʼ
			recvSize =
				recvfrom(sockServer, buffer, BUFFER_LENGTH, 0, ((SOCKADDR*)&addrServer), &length);
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
					printf("File size is %dB, each packet is 1024B and packet total num is % d\n", sizeof(mydata), totalPacket);
					curSeq = 0;
					send_base = 0;
					totalSeq = 0;
					waitCount = 0;
					stage = 2;
				}
			}
			break;
		case 2://���ݴ���׶�
			if (totalSeq >= totalPacket)
			{
				runFlag = false;
				buffer[0] = 21;
				memcpy(&buffer[1], "Transmission done!", 1024);
				printf("Transmission done!\n");
				sendto(sockServer, buffer, BUFFER_LENGTH, 0,
					(SOCKADDR*)&addrServer, sizeof(SOCKADDR));
				
				Sleep(500);
			}
			else
			{
				if (SR_seqIsAvailable()) {
					//���͸��ͻ��˵����кŴ� 1 ��ʼ
					buffer[0] = curSeq + 1;
					ack[curSeq] = FALSE;
					//���ݷ��͵Ĺ�����Ӧ���ж��Ƿ������
					//Ϊ�򻯹��̴˴���δʵ��
					memcpy(&buffer[1], mydata + 1024 * totalSeq, 1024);
					printf("send a packet with a seq of %d\n", curSeq);
					sendto(sockServer, buffer, BUFFER_LENGTH, 0,
						(SOCKADDR*)&addrServer, sizeof(SOCKADDR));
					++curSeq;
					curSeq %= SEQ_SIZE;
					++totalSeq;
					Sleep(500);
				}
				//�ȴ� Ack����û���յ����򷵻�ֵΪ-1��������+1

				//��һ��SEQ_SIZE��С��timer
				//����յ���һ��buffer[0],�������ڼ�ʱ����û�յ�ACK�ģ��ļ�ʱ��+1��
				//��ʱ�ش���ÿ�ζ�ɨ��һ��ȴ������ڵļ�ʱ�������ν�����ʱ�����ش���

				recvSize = recvfrom(sockServer, buffer, BUFFER_LENGTH, 0, ((SOCKADDR*)&addrServer), &length);
				if (recvSize < 0) {
					for (int i = 0; i < SEND_WIND_SIZE; i++)
					{
						if (!ack[send_base + i])
						{
							timer[send_base + i]++;
						}
					}

					for (int i = 0; i < SEND_WIND_SIZE; i++)
					{
						int j = (send_base + i) % SEQ_SIZE;
						if (timer[j] > 20)
						{
							//���ͳ�ʱ�ķ��飬��Ϊack���򵽴��ʱ�Ŀ�����send_base�����,����ÿ����һ��timer
							//ֻ���͵�һ����ʱ����
							printf("time out error,resend a packet with a seq of %d\n", j);
							buffer[0] = j + 1;
							ack[j] = FALSE;
							int over = 0;
							over = ((curSeq - j) >= 0) ? curSeq - j : curSeq - j + SEQ_SIZE;
							memcpy(&buffer[1], mydata + 1024 * (totalSeq - over), 1024);
							sendto(sockServer, buffer, BUFFER_LENGTH, 0, (SOCKADDR*)&addrServer, sizeof(SOCKADDR));
							//������ʱ��
							timer[j] = 0;
							break;
						}
					}
				}
				else {
					//�յ� ack
					SR_ackHandler(buffer[0]);
					timer[buffer[0]] = 0;
				}
			}

		}
	}
}
//////////////////////////////////////

char cache[SEQ_SIZE * 1024];
void __stdcall Receiver()
{
	printf("into thread 1!\n\n");

	float packetLossRatio = 0.2; //Ĭ�ϰ���ʧ�� 0.0
	float ackLossRatio = 0.2; //Ĭ�� ACK ��ʧ�� 0.0
	

	//��ʱ����Ϊ������ӣ�����ѭ����������
	srand((unsigned)time(NULL));
	//sendto(sockServer, "OK", 3, 0,
	//	(SOCKADDR*)&addrClient, sizeof(SOCKADDR));
	bool entry2 = true;
	stage2 = 0;
	while (entry2)
	{
		ZeroMemory(buffer2, sizeof(buffer2));
		//�ȴ� server �ظ����� UDP Ϊ����ģʽ
		recvSize2 = recvfrom(sockServer2, buffer2, BUFFER_LENGTH, 0, (SOCKADDR*)&addrServer2, &length);
		if (recvSize2 < 0)
		{
			continue;
		}
		switch (stage2) {
		case 0://�ȴ����ֽ׶�
			u_code = (unsigned char)buffer2[0];
			if ((unsigned char)buffer2[0] == 205)
			{
				printf("Ready for file transmission\n");
				//����200��������׼������client������
				buffer2[0] = 200;
				buffer2[1] = '\0';
				sendto(sockServer2, buffer2, 2, 0,
					(SOCKADDR*)&addrServer2, sizeof(SOCKADDR));
				stage2 = 1;
				recvSeq = 0;
				waitSeq = 1;
			}
			break;
		case 1://�ȴ��������ݽ׶�
			seq = (unsigned short)buffer2[0];

			if ((unsigned char)buffer2[0] >= 21)
			{
				entry2 = false;
				printf("\n\n%s\n\n", &buffer2[1]);
				ZeroMemory(buffer2, sizeof(buffer2));
				memcpy(buffer2, "Upload over!!", sizeof("Upload over!!"));
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
				buffer2[0] = seq;

				if (!havesave[seq - 1])//û������ͻ���
				{
					havesave[seq - 1] = 1;
					memcpy(&cache[(seq - 1) * 1024], &buffer[1], 1024 * sizeof(char));
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
							
							ofstream fout;
							fout.open("C_4_S.txt", ios::out | ios::app, _SH_DENYNO);
							if (!fout.is_open())
							{
								printf("cannot open file %s", "download.txt");
							}
							else
							{
								fout.write(&cache[(waitSeq + i - 1)*1024], sizeof(char) * 1024);
								fout.flush();
								//sleep(500);
							}
							waitSeq = (waitSeq + 1) % SEQ_SIZE;
							continue;
						}
						
						break;
					}
					//�������
					printf("%s\n", &buffer2[1]);
				}
				buffer2[1] = '\0';
			}
			else if (waitSeq - SEND_WIND_SIZE <= seq < waitSeq)
			{
				buffer2[0] = seq;
				//recvseq = seq;
				buffer2[1] = '\0';
			}

			b = lossInLossRatio(ackLossRatio);
			if (b) {
				printf("The ack of %d loss\n", (unsigned char)buffer2[0]);
				continue;
			}
			sendto(sockServer2, buffer2, 2, 0, (SOCKADDR*)&addrServer2, sizeof(SOCKADDR));
			printf("send a ack of %d\n", (unsigned char)buffer2[0]);
			break;
		}

	}

}



////////////������////////////////////
int main(int argc, char* argv[])
{

	//�汾 2.2
	wVersionRequested = MAKEWORD(2, 2);
	//���� dll �ļ� Scoket ��
	err = WSAStartup(wVersionRequested, &wsaData);
	if (err != 0) {
		//�Ҳ��� winsock.dll
		printf("WSAStartup failed with error: %d\n", err);
		return -1;
	}
	if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2)
	{
		printf("Could not find a usable version of Winsock.dll\n");
		WSACleanup();
	}
	else {
		printf("The Winsock 2.2 dll was found okay\n");
	}
	sockServer = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	
	//�����׽���Ϊ������ģʽ
	iMode = 1; //1���������� 0������
	ioctlsocket(sockServer, FIONBIO, (u_long FAR*) & iMode);//����������
	
	//addrServer.sin_addr.S_un.S_addr = inet_addr(SERVER_IP);
	addrServer.sin_addr.S_un.S_addr = inet_addr(SERVER_IP);//���߾���
	addrServer.sin_family = AF_INET;
	addrServer.sin_port = 8080;
	err = bind(sockServer, (SOCKADDR*)&addrServer, sizeof(SOCKADDR));

	
	if (err) {
		err = GetLastError();
		printf("Could not bind the port %d for socket.Error code is % d\n",SERVER_PORT,err);
			WSACleanup();
		return -1;
	}
	//����һ�������������ݵ�socket
	sockServer2 = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	//�����׽���Ϊ����ģʽ
	iMode = 0; //1���������� 0������
	ioctlsocket(sockServer2, FIONBIO, (u_long FAR*) & iMode);//��������
	addrServer2.sin_addr.S_un.S_addr = inet_addr(SERVER_IP);//���߾���
	addrServer2.sin_family = AF_INET;
	addrServer2.sin_port = 1234;
	err = bind(sockServer2, (SOCKADDR*)&addrServer2, sizeof(SOCKADDR));
	if (err) {
		err = GetLastError();
		printf("Could not bind the port %d for socket.Error code is % d\n", SERVER_PORT2, err);
		WSACleanup();
		return -1;
	}
	
	ZeroMemory(buffer, sizeof(buffer));
	//���������ݶ����ڴ�
	std::ifstream icin;
	icin.open(PATH);

	ZeroMemory(mydata, sizeof(mydata));
	icin.read(mydata, 1024 * 20);
	icin.close();
	totalPacket = sizeof(mydata) / 1024;

	for (int i = 0; i < SEQ_SIZE; ++i) {
		ack[i] = TRUE;
	}
	
	while (true) {
		//���������գ���û���յ����ݣ�����ֵΪ-1,���ݶ�����buffer��
		recvSize = recvfrom(sockServer, buffer, BUFFER_LENGTH, 0, ((SOCKADDR*)&addrServer), &length);
		if (recvSize < 0) {
			Sleep(200);
			continue;
		}
		printf("recv from client: %s\n", buffer);

		if (strcmp(buffer, "-time") == 0) {
			ZeroMemory(buffer, sizeof(buffer));
			getCurTime(buffer);
		}
		else if (strcmp(buffer, "-quit") == 0) {
			strcpy_s(buffer, strlen("Good bye!") + 1, "Good bye!");
		}
		else if (strcmp(buffer, "-testsr") == 0) {
			
			//���￪�����̣߳�һ����һ���գ�ÿһ���߳����һ��pkg�Ĵ���
			//=======================���������߳�=======================//
			for (int i = 0; i < SEND_WIND_SIZE; i++)
			{
				ack[i] = true;
				havesave[i] = false;
				timer[i] = 0;
			}
			HANDLE handle[2];
			handle[0] = (HANDLE)_beginthreadex(NULL, 0, (_beginthreadex_proc_type)&Sender, NULL, 0, 0);
			handle[1] = (HANDLE)_beginthreadex(NULL, 0, (_beginthreadex_proc_type)&Receiver, NULL, 0, 0);

			if (NULL == handle[0])
			{
				cout << "Create Thread failed !" << endl;
				return -1;
			}
			if (NULL == handle[1])
			{
				cout << "Create Thread failed !" << endl;
				return -1;
			}
			WaitForMultipleObjects(2, handle, TRUE, INFINITE);
			//=======================���д���=======================//
			CloseHandle(handle[0]);
			CloseHandle(handle[1]);
			
			Sleep(500);	
			continue;
		}
		else continue;
		sendto(sockServer, buffer, strlen(buffer) + 1, 0, (SOCKADDR*)&addrServer,
			sizeof(SOCKADDR));
		Sleep(500);
	}
	
	//�ر��׽��֣�ж�ؿ�
	closesocket(sockServer);
	WSACleanup();
	return 0;
}