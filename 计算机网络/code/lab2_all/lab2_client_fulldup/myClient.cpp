// GBN_client.cpp : 定义控制台应用程序的入口点。
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
#include <string>

using namespace std;

#pragma comment(lib,"ws2_32.lib")

#define SERVER_PORT 12340     //接收数据的端口号
#define SERVER_PORT2 8080     //fasong数据的端口号
#define SERVER_IP "127.0.0.1" // 服务器的 IP 地址

const int BUFFER_LENGTH = 1026;
const int SEQ_SIZE = 20;//接收端序列号个数，为 1~20
const int SEND_WIND_SIZE = 10; //发送窗口大小为 10，GBN 中应满足 W + 1 <=N（W 为发送窗口大小，N 为序列号个数）

BOOL havesave[SEQ_SIZE]; //收到 ack 情况，对应 0~19 的 ack


//如果需要upload的一些参数
BOOL ack[SEQ_SIZE];//收到 ack 情况，对应 0~19 的 ack
int send_base;
int curSeq;						//当前数据包的 seq
int curAck;						//当前等待确认的 ack
int totalSeq;					//收到的包的总数
int totalPacket;				//需要发送的包总数

//加载套接字库（必须）
WORD wVersionRequested;
WSADATA wsaData;
//套接字加载时错误提示
int err;
//版本 2.2
SOCKET socketClient, socketClient2;
SOCKADDR_IN addrServer, addrServer2;
//接收缓冲区
char buffer[BUFFER_LENGTH];
char buffer2[BUFFER_LENGTH];
int len = sizeof(SOCKADDR);
int ret, ret2;
int interval = 1;//收到数据包之后返回 ack 的间隔，默认为 1 表示每个都返回 ack， 0 或者负数均表示所有的都不返回 ack
char cmd[128];
float packetLossRatio = 0.2; //默认包丢失率 0.2
float ackLossRatio = 0.2; //默认 ACK 丢失率 0.2


unsigned char u_code;//状态码
unsigned short seq;//包的序列号
unsigned short recvSeq;//接收窗口大小为 1，已确认的序列号
unsigned short waitSeq;//等待的序列号

bool entry = true;
char mydata[1024 * 23];
/****************************************************************/
/* -time 从服务器端获取当前时间
-quit 退出客户端
-testgbn [X] 测试 GBN 协议实现可靠数据传输
[X] [0,1] 模拟数据包丢失的概率
[Y] [0,1] 模拟 ACK 丢失的概率
*/
/****************************************************************/
void printTips() {
	printf("*****************************************\n");
	printf("| -time to get current time |\n");
	printf("| -quit to exit client |\n");
	printf("| -testsr [X] [Y] to test the sr protocol |\n");
	printf("*****************************************\n");
}
//************************************
// Method: lossInLossRatio
// FullName: lossInLossRatio
// Access: public
// Returns: BOOL
// Qualifier: 根据丢失率随机生成一个数字，判断是否丢失,丢失则返回TRUE，否则返回 FALSE
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
// 从Server那里粘贴过来的
//************************************
// Method: ackHandler
// FullName: ackHandler
// Access: public
// Returns: void
// Qualifier: 收到 ack，累积确认，取数据帧的第一个字节
//由于发送数据时，第一个字节（序列号）为 0（ASCII）时发送失败，因此加一了，此处需要减一还原
// Parameter: char c
//************************************
void SR_ackHandler(char c)
{
	unsigned char index = (unsigned char)c - 1; //序列号减一
	printf("Recv a ack of %d\n", index);

	if (index < send_base)
	{
		if (send_base < (send_base + SEND_WIND_SIZE - 1) - SEQ_SIZE - 1)
		{	//ack 超过了最大值，回到了 curAck 的左边
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
		{//如果等于，滑动窗口
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
// Qualifier: 超时重传处理函数，滑动窗口内的数据帧都要重传
// 要改
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
// Qualifier: 当前序列号 curSeq 是否可用
//************************************
bool SR_seqIsAvailable() {
	int step;
	step = curSeq - send_base;
	step = step >= 0 ? step : step + SEQ_SIZE;
	//序列号是否在当前发送窗口之内
	if (step >= SEND_WIND_SIZE) {
		return false;
	}
	if (ack[curSeq]) {
		return true;
	}
	return false;
}
///////////////////////////////////////
void __stdcall Sender()
{
	ZeroMemory(buffer2, sizeof(buffer2));
	int waitCount = 0;
	int recvSize;
	//加入了一个握手阶段

	printf("Shake hands stage\n");
	int stage = 0;
	bool runFlag = true;
	Sleep(500);
	while (runFlag) {
		switch (stage) {
		case 0://发送 205 阶段
			buffer2[0] = 205;
			printf("%d\n", buffer2[0]);
			sendto(socketClient2, buffer2, strlen(buffer2) + 1, 0, (SOCKADDR*)&addrServer2, sizeof(SOCKADDR));
			Sleep(100);
			stage = 1;
			break;
		case 1://等待接收 200 阶段，没有收到则计数器+1，超时则放弃此次“连接”，等待从第一步开始
			recvSize =
				recvfrom(socketClient2, buffer2, BUFFER_LENGTH, 0, ((SOCKADDR*)&addrServer2), &len);
			if (recvSize < 0) {
				++waitCount;
				if (waitCount > 20) {
					//超时
					runFlag = false;
					printf("Timeout error\n");
					break;
				}
				Sleep(500);
				continue;
			}
			else {
				if ((unsigned char)buffer2[0] == 200) {
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
		case 2://数据传输阶段
			//TODO........
			if (totalSeq >= totalPacket)
			{
				runFlag = false;
				buffer2[0] = 21;
				memcpy(&buffer2[1], "Transmission done!", 1024);
				printf("Transmission done!\n");
				sendto(socketClient2, buffer, BUFFER_LENGTH, 0,
					(SOCKADDR*)&addrServer2, sizeof(SOCKADDR));
			}
			else
			{
				if (SR_seqIsAvailable()) {
					//发送给Server的序列号从 1 开始
					buffer2[0] = curSeq + 1;
					ack[curSeq] = FALSE;
					memcpy(&buffer2[1], mydata + 1024 * totalSeq, 1024);
					printf("send a packet with a seq of %d\n", curSeq);
					sendto(socketClient2, buffer2, BUFFER_LENGTH, 0,
						(SOCKADDR*)&addrServer2, sizeof(SOCKADDR));

					++curSeq;
					curSeq %= SEQ_SIZE;
					++totalSeq;
					Sleep(500);
				}
				//等待 Ack，若没有收到，则返回值为-1，计数器+1
				//这个不行
				recvSize = recvfrom(socketClient2, buffer2, BUFFER_LENGTH, 0, ((SOCKADDR*)&addrServer2), &len);
				if (recvSize < 0) {
					waitCount++;
					//20 次等待 ack 则超时重传
					if (waitCount > 20)
					{
						//超时且这里就要重发
						printf("time out error,resend a packet with a seq of %d\n", send_base);
						buffer2[0] = send_base + 1;
						ack[send_base] = FALSE;
						memcpy(&buffer2[1], mydata + 1024 * (totalSeq - (curSeq - send_base)), 1024);
						sendto(socketClient2, buffer2, BUFFER_LENGTH, 0, (SOCKADDR*)&addrServer2, sizeof(SOCKADDR));
						//重启statrer
						waitCount = 0;
					}
				}
				else {
					//收到 ack
					SR_ackHandler(buffer2[0]);
					waitCount = 0;
				}
			}

			Sleep(500);
			break;
		}
	}
}
char cache[SEQ_SIZE * 1024];
//////////////////////////////////////////////
void __stdcall Receiver()
{
	ZeroMemory(buffer, sizeof(buffer));
	int stage2 = 0;
	int countall = 0;
	BOOL b;
	entry = 1;
	int recvs;
	while (entry)
	{
		//等待 server 回复设置 UDP 为阻塞模式
		recvs = recvfrom(socketClient, buffer, BUFFER_LENGTH, 0, (SOCKADDR*)&addrServer, &len);
		switch (stage2) {
		case 0://等待握手阶段
			u_code = (unsigned char)buffer[0];
			if ((unsigned char)buffer[0] == 205)
			{
				printf("Ready for file transmission\n");
				buffer[0] = 200;
				buffer[1] = '\0';
				sendto(socketClient, buffer, 2, 0,
					(SOCKADDR*)&addrServer, sizeof(SOCKADDR));
				stage2 = 1;
				recvSeq = 0;
				waitSeq = 1;
			}
			break;
		case 1://等待接收数据阶段
			seq = (unsigned short)buffer[0];
			//如果服务器端给出发送完成的信号
			//这里设置为21，则服务器端接收一个time，返回传输完成时间
			if (buffer[0] == SEQ_SIZE + 1)
			{
				entry = false;
				printf("\n\n%s\n\n", &buffer[1]);
				ZeroMemory(havesave, sizeof(havesave));
				memcpy(buffer, "-time", sizeof("-time"));
				break;
			}
			//随机法模拟包是否丢失
			b = lossInLossRatio(packetLossRatio);
			if (b) {
				printf("The packet with a seq of %d loss\n", seq);
				continue;
			}
			printf("client recv a packet with a seq of %d\n", seq);
			countall++;
			//如果是在[recv_base,recv_base + N - 1]，发送ACK，没缓存过就缓存；
			//如果recv_base == 缓存ack，滑动窗口到连续的分组号之后
			if (waitSeq <= seq < waitSeq + SEND_WIND_SIZE)
			{
				//发送ACK
				////////todo-----done!!!!
				buffer[0] = seq;

				if (!havesave[seq - 1])//没缓存过就缓存
				{
					havesave[seq - 1] = 1;
					memcpy(&cache[(seq - 1) * 1024], &buffer[1], 1024 * sizeof(char));
					
				}

				//如果waitseq和报文序号相等，窗口滑动，提交；
				if (!(waitSeq - seq)) {
					int count = 0;//记录窗口滑动了几次
					int i;
					for (i = 0; i < SEND_WIND_SIZE; i++)
					{
						if (havesave[waitSeq + i - 1] == 1)
						{
							
							havesave[waitSeq + i - 1] = 0;
							ofstream fout;
							fout.open("S_4_C.txt", ios::out | ios::app, _SH_DENYNO);
							if (!fout.is_open())
							{
								printf("cannot open file %s", "download.txt");
							}
							else
							{
								fout.write(&cache[(waitSeq + i - 1) * 1024], sizeof(char) * 1024);
								fout.flush();
								//sleep(500);
							}
							waitSeq = (waitSeq + 1) % SEQ_SIZE;
							continue;
						}
						break;
					}
					//输出数据
					printf("pkg submit...：\n%s\n", &buffer[1]);
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




int main(int argc, char* argv[])
{
	
	wVersionRequested = MAKEWORD(2, 2);
	//加载 dll 文件 Scoket 库
	err = WSAStartup(wVersionRequested, &wsaData);
	if (err != 0) {
		//找不到 winsock.dll
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
	socketClient = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);//1用作接收

	socketClient2 = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);//2用作发送，设置为非阻塞，处理超时
	//设置套接字为非阻塞模式
	int iMode = 1; //1：非阻塞， 0：阻塞
	ioctlsocket(socketClient2, FIONBIO, (u_long FAR*) & iMode);//非阻塞设置
	


	addrServer.sin_addr.S_un.S_addr = inet_addr(SERVER_IP);
	addrServer.sin_family = AF_INET;
	addrServer.sin_port = 8080;

	addrServer2.sin_addr.S_un.S_addr = inet_addr(SERVER_IP);
	addrServer2.sin_family = AF_INET;
	addrServer2.sin_port = 1234;
	
	ZeroMemory(buffer, sizeof(buffer));

	//为了测试与服务器的连接，可以使用 -time 命令从服务器端获得当前时间
	//使用 -testgbn [X] [Y] 测试 GBN 其中[X]表示数据包丢失概率
	// [Y]表示 ACK 丢包概率
	printTips();

	srand((unsigned)time(NULL));
	while (true) {
		ZeroMemory(cmd, sizeof(cmd));
		gets_s(buffer);
		ret = sscanf(buffer, "%s%f%f", &cmd, &packetLossRatio, &ackLossRatio);
		//开始 GBN 测试，使用 GBN 协议实现 UDP 可靠文件传输
		if (!strcmp(cmd, "-testsr")) {
			printf("%s\n", "Begin to test SR protocol, please don't abort the process");
			printf("The loss ratio of packet is %.2f,the loss ratio of ack is % .2f\n", packetLossRatio, ackLossRatio);
			printf("Sequence is from 1 to 20......\n");

			sendto(socketClient, "-testsr", strlen("-testsr") + 1, 0,
				(SOCKADDR*)&addrServer, sizeof(SOCKADDR));
			//将测试数据读入内存
			std::ifstream icin;
			icin.open(PATH);
			
			ZeroMemory(mydata, sizeof(mydata));
			icin.read(mydata, 1024 * 24);
			icin.close();
			totalPacket = sizeof(mydata) / 1024;
			for (int i = 0; i < SEQ_SIZE; ++i) {
				ack[i] = TRUE;
				havesave[i] = false;
			}
			

			//建立两个线程，分别在子线程握手、一个收一个发
			//=======================创建两个线程=======================//
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
			//=======================进行传输=======================//
			CloseHandle(handle[0]);
			CloseHandle(handle[1]);

		}
		if (buffer[0] != 21)
		{
			//关闭套接字
			sendto(socketClient, buffer, strlen(buffer) + 1, 0,
				(SOCKADDR*)&addrServer, sizeof(SOCKADDR));
		}
		
		ret = recvfrom(socketClient, buffer, BUFFER_LENGTH, 0, (SOCKADDR*)&addrServer, &len);
		printf("%s\n", buffer);
		if (!strcmp(buffer, "Good bye!")) {
			break;
		}
		printTips();
	}
	

	closesocket(socketClient);
	closesocket(socketClient2);
	WSACleanup();
	return 0;
}