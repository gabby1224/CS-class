//#include "stdafx.h" //创建 VS 项目包含的预编译头文件
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

#define SERVER_PORT 12340       //端口号
#define SERVER_PORT2 8080      //端口号
#define SERVER_IP "0.0.0.0"     //IP 地址

const int BUFFER_LENGTH = 1026; //缓冲区大小，（以太网中 UDP 的数据帧中包长度应小于 1480 字节）
const int SEND_WIND_SIZE = 10;//发送窗口大小为 10， GBN 中应满足 W + 1 <= N（W 为发送窗口大小， N 为序列号个数）
						
//本例取序列号 0...19 共 20 个
//如果将窗口大小设为 1，则为停-等协议
const int SEQ_SIZE = 20; //序列号的个数，从 0~19 共计 20 个
								//由于发送数据第一个字节如果值为 0，则数据会发送失败
								//因此接收端序列号为 1~20，与发送端一一对应
BOOL ack[SEQ_SIZE];//收到 ack 情况，对应 0~19 的 ack
BOOL havesave[SEQ_SIZE];
int send_base;
int curSeq;						//当前数据包的 seq
int curAck;						//当前等待确认的 ack
int totalSeq;					//收到的包的总数
int totalPacket;				//需要发送的包总数
int timer[SEQ_SIZE];
int u_code;
int recvSeq;
int waitSeq;
int seq;
int b;

//加载套接字库（必须）
WORD wVersionRequested;
WSADATA wsaData;
//套接字加载时错误提示
int err;
SOCKET sockServer;
SOCKET sockServer2;
int iMode;
SOCKADDR_IN addrServer; //服务器地址
SOCKADDR_IN addrServer2; //服务器地址
SOCKADDR_IN addrClient; //客户端地址
int length = sizeof(SOCKADDR);
char buffer[BUFFER_LENGTH]; //数据发送接收缓冲区
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
// Qualifier: 获取当前系统时间，结果存入 ptime 中
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
// Qualifier: 当前序列号 curSeq 是否可用
//************************************
bool seqIsAvailable() {
	int step;
	step = curSeq - curAck;
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
// Method: ackHandler
// FullName: ackHandler
// Access: public
// Returns: void
// Qualifier: 收到 ack，累积确认，取数据帧的第一个字节
//由于发送数据时，第一个字节（序列号）为 0（ASCII）时发送失败，因此加一了，此处需要减一还原
// Parameter: char c
//************************************
void ackHandler(char c) {
	unsigned char index = (unsigned char)c - 1; //序列号减一
	printf("Recv a ack of %d\n", index);
	//累计确认的机制，收到的ACK之前的都默认都到了，ack[]置位1
	if (curAck <= index) {
		for (int i = curAck; i <= index; ++i) {
			ack[i] = TRUE;
		}
		curAck = (index + 1) % SEQ_SIZE;
	}
	else {
		//ack 超过了最大值，回到了 curAck 的左边
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

//////////////////////////////////////
void __stdcall Sender()
{
	//进入 sr 测试阶段
				//首先 server（server 处于 0 状态）向 client 发送 205 状态码（server进入 1 状态）
				//server 等待 client 回复 200 状态码，如果收到（server 进入 2 状态），则开始传输文件，否则延时等待直至超时\
				//在文件传输阶段， server 发送窗口大小设为
	ZeroMemory(buffer, sizeof(buffer));
	int waitCount = 0;
	runFlag = true;
	stage = 0;
	printf("Begain to test SR protocol,please don't abort the process\n");
	printf("Sequence is from 0 to 19......\n");
	//加入了一个握手阶段
		//首先服务器向客户端发送一个 205 大小的状态码（我自己定义的）表示服务器准备好了，可以发送数据
		//客户端收到 205 之后回复一个 200 大小的状态码，表示客户端准备好了，可以接收数据了
		//服务器收到 200 状态码之后，就开始使用 GBN 发送数据了
	printf("Shake hands stage\n");

	while (runFlag) {
		switch (stage) {
		case 0://发送 205 阶段
			buffer[0] = 205;
			sendto(sockServer, buffer, strlen(buffer) + 1, 0, (SOCKADDR*)&addrServer, sizeof(SOCKADDR));
			Sleep(100);
			stage = 1;
			break;
		case 1://等待接收 200 阶段，没有收到则计数器+1，超时则放弃此次“连接”，等待从第一步开始
			recvSize =
				recvfrom(sockServer, buffer, BUFFER_LENGTH, 0, ((SOCKADDR*)&addrServer), &length);
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
		case 2://数据传输阶段
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
					//发送给客户端的序列号从 1 开始
					buffer[0] = curSeq + 1;
					ack[curSeq] = FALSE;
					//数据发送的过程中应该判断是否传输完成
					//为简化过程此处并未实现
					memcpy(&buffer[1], mydata + 1024 * totalSeq, 1024);
					printf("send a packet with a seq of %d\n", curSeq);
					sendto(sockServer, buffer, BUFFER_LENGTH, 0,
						(SOCKADDR*)&addrServer, sizeof(SOCKADDR));
					++curSeq;
					curSeq %= SEQ_SIZE;
					++totalSeq;
					Sleep(500);
				}
				//等待 Ack，若没有收到，则返回值为-1，计数器+1

				//设一个SEQ_SIZE大小的timer
				//如果收到了一个buffer[0],所有正在计时（还没收到ACK的）的计时器+1；
				//超时重传：每次都扫描一遍等待窗口内的计时器，依次结束计时，并重传；

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
							//发送超时的分组，因为ack乱序到达，超时的可能是send_base后面的,必须每个设一个timer
							//只发送第一个超时分组
							printf("time out error,resend a packet with a seq of %d\n", j);
							buffer[0] = j + 1;
							ack[j] = FALSE;
							int over = 0;
							over = ((curSeq - j) >= 0) ? curSeq - j : curSeq - j + SEQ_SIZE;
							memcpy(&buffer[1], mydata + 1024 * (totalSeq - over), 1024);
							sendto(sockServer, buffer, BUFFER_LENGTH, 0, (SOCKADDR*)&addrServer, sizeof(SOCKADDR));
							//重启计时器
							timer[j] = 0;
							break;
						}
					}
				}
				else {
					//收到 ack
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

	float packetLossRatio = 0.2; //默认包丢失率 0.0
	float ackLossRatio = 0.2; //默认 ACK 丢失率 0.0
	

	//用时间作为随机种子，放在循环的最外面
	srand((unsigned)time(NULL));
	//sendto(sockServer, "OK", 3, 0,
	//	(SOCKADDR*)&addrClient, sizeof(SOCKADDR));
	bool entry2 = true;
	stage2 = 0;
	while (entry2)
	{
		ZeroMemory(buffer2, sizeof(buffer2));
		//等待 server 回复设置 UDP 为阻塞模式
		recvSize2 = recvfrom(sockServer2, buffer2, BUFFER_LENGTH, 0, (SOCKADDR*)&addrServer2, &length);
		if (recvSize2 < 0)
		{
			continue;
		}
		switch (stage2) {
		case 0://等待握手阶段
			u_code = (unsigned char)buffer2[0];
			if ((unsigned char)buffer2[0] == 205)
			{
				printf("Ready for file transmission\n");
				//发送200，就绪，准备接收client的数据
				buffer2[0] = 200;
				buffer2[1] = '\0';
				sendto(sockServer2, buffer2, 2, 0,
					(SOCKADDR*)&addrServer2, sizeof(SOCKADDR));
				stage2 = 1;
				recvSeq = 0;
				waitSeq = 1;
			}
			break;
		case 1://等待接收数据阶段
			seq = (unsigned short)buffer2[0];

			if ((unsigned char)buffer2[0] >= 21)
			{
				entry2 = false;
				printf("\n\n%s\n\n", &buffer2[1]);
				ZeroMemory(buffer2, sizeof(buffer2));
				memcpy(buffer2, "Upload over!!", sizeof("Upload over!!"));
				break;
			}
			//随机法模拟包是否丢失
			b = lossInLossRatio(packetLossRatio);
			if (b) {
				printf("The packet with a seq of %d loss\n", seq);
				continue;
			}
			printf("recv a packet with a seq of %d\n", seq);

			//如果是在[recv_base,recv_base + N - 1]，发送ACK，没缓存过就缓存；
			//如果recv_base == 缓存ack，滑动窗口到连续的分组号之后
			if (waitSeq <= seq < waitSeq + SEND_WIND_SIZE)
			{
				//发送ACK
				////////todo-----done!!!!
				buffer2[0] = seq;

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
					//输出数据
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



////////////主函数////////////////////
int main(int argc, char* argv[])
{

	//版本 2.2
	wVersionRequested = MAKEWORD(2, 2);
	//加载 dll 文件 Scoket 库
	err = WSAStartup(wVersionRequested, &wsaData);
	if (err != 0) {
		//找不到 winsock.dll
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
	
	//设置套接字为非阻塞模式
	iMode = 1; //1：非阻塞， 0：阻塞
	ioctlsocket(sockServer, FIONBIO, (u_long FAR*) & iMode);//非阻塞设置
	
	//addrServer.sin_addr.S_un.S_addr = inet_addr(SERVER_IP);
	addrServer.sin_addr.S_un.S_addr = inet_addr(SERVER_IP);//两者均可
	addrServer.sin_family = AF_INET;
	addrServer.sin_port = 8080;
	err = bind(sockServer, (SOCKADDR*)&addrServer, sizeof(SOCKADDR));

	
	if (err) {
		err = GetLastError();
		printf("Could not bind the port %d for socket.Error code is % d\n",SERVER_PORT,err);
			WSACleanup();
		return -1;
	}
	//绑定另一个用来接收数据的socket
	sockServer2 = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	//设置套接字为阻塞模式
	iMode = 0; //1：非阻塞， 0：阻塞
	ioctlsocket(sockServer2, FIONBIO, (u_long FAR*) & iMode);//阻塞设置
	addrServer2.sin_addr.S_un.S_addr = inet_addr(SERVER_IP);//两者均可
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
	//将测试数据读入内存
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
		//非阻塞接收，若没有收到数据，返回值为-1,数据都读到buffer里
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
			
			//这里开两个线程，一个发一个收，每一个线程完成一个pkg的处理
			//=======================创建两个线程=======================//
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
			//=======================进行传输=======================//
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
	
	//关闭套接字，卸载库
	closesocket(sockServer);
	WSACleanup();
	return 0;
}