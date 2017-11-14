#pragma once
#include "ConnectwithUSer.h"
#include "audioconnect.h"
void ConnectionToS(char ip[15], char clientdata[260], char datalogin[260], char commandToServer[3], char answeratServer[3])
{

	for (int i = 0; i < 260; ++i)
		clientdata[i] = '\0';
	u_short port = 6050;
	WSAData wsa;
	WORD Version = MAKEWORD(2, 1);
	WSAStartup(Version, &wsa);
	SOCKET Connect = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	SOCKADDR_IN Client;
	Client.sin_family = AF_INET;
	Client.sin_port = htons(port);
	Client.sin_addr.s_addr = inet_addr(ip);
	while (1)
	{
		if (connect(Connect, (SOCKADDR*)&Client, sizeof(Client)))
		{
			send(Connect, commandToServer, 3, 0);
			send(Connect, datalogin, MAX_PATH, 0);
			recv(Connect, answeratServer, 3, 0);
			if ((answeratServer[0] == 'f') && (answeratServer[1] == 'n') && (answeratServer[2] == 'd'))
				recv(Connect, clientdata, MAX_PATH, 0);
			break;
		}
	}
	for (int i = 0; i < 260; ++i)
		datalogin[i] = '\0';
	closesocket(Connect);
	WSACleanup();
}

void ConnectionToUser(char ip[15], char datalogin[260], char commandToServer[3], char answeratServer[3])
{

	u_short port = 7772;
	WSAData wsa;
	WORD Version = MAKEWORD(2, 1);
	WSAStartup(Version, &wsa);
	SOCKET Connect = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	SOCKADDR_IN Client;
	Client.sin_family = AF_INET;
	Client.sin_port = htons(port);
	Client.sin_addr.s_addr = inet_addr(ip);
	while (1)
	{
		if (connect(Connect, (SOCKADDR*)&Client, sizeof(Client)))
		{
			send(Connect, commandToServer, 3, 0);
			send(Connect, datalogin, MAX_PATH, 0);
			recv(Connect, answeratServer, 3, 0);
			if (answeratServer[0] == 't') {

				strNameWindow[numberOfcalls+7]='.';
				strNameWindow[numberOfcalls + 8] = '\0';
				++numberOfcalls;
				youTalk = true;
				HANDLE hNetThread3;
				DWORD dwNetThreadId3;
				azvonu = true;
				strcpy(ipoponent, "192.168.0.102");
				isEndtalk = false;
					hNetThread3 = CreateThread(NULL, 0, sendVideotoSpekers, 0, 0, &dwNetThreadId3);
					hNetThread = CreateThread(NULL, 0, NetThread, 0, 0, &dwNetThreadId);
					HANDLE hNetThreadaud;
					DWORD dwNetThreadIdaud;
					hNetThreadaud = CreateThread(NULL, 0, talk, 0, 0, &dwNetThreadIdaud);
					HANDLE hNetThread4;
					DWORD dwNetThreadId4;
					hNetThread4 = CreateThread(NULL, 0, NetThreadAud, 0, 0, &dwNetThreadId4);


			}
			break;
		}
	}
	closesocket(Connect);
	WSACleanup();
}