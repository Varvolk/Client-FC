#pragma once
#include "stdafx.h"
#include <winsock2.h>
#include <WS2tcpip.h>
#include <opencv2/opencv.hpp>
#include <vector>

#pragma comment(lib,"Ws2_32.lib")
bool azvonu = false, NeKonecrazgovorap = true, youTalk = false, IsError = false;
cv::Mat frame2, sendFraem;
HANDLE hNetThread;
DWORD dwNetThreadId;
int numberOfcalls = 0;
char strNameWindow[100]="windows";

static char ipoponent[15]= "";

DWORD WINAPI sendVideotoSpekers(LPVOID lpParam){
	// Инициализация WinSock
	WORD wVersionRequested = MAKEWORD(2, 2);
	WSADATA wsaData;
	int err = WSAStartup(wVersionRequested, &wsaData);
	if (err != 0) {
		MessageBox(NULL, (LPWSTR)WSAGetLastError(), _T("WSAStartup error:"), MB_OKCANCEL);
		IsError = true;
	}
	// Открытие и закрытие сокета
	SOCKET my_sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (my_sock == INVALID_SOCKET)
	{
		MessageBox(NULL, (LPWSTR)WSAGetLastError(), _T("socket() error:"), MB_OKCANCEL);
		IsError = true;
		WSACleanup();
	}
	// Шаг 3 - обмен сообщений с сервером
	HOSTENT *hostent;
	sockaddr_in dest_addr;
	dest_addr.sin_family = AF_INET;
	dest_addr.sin_port = htons(5150);
	// определение IP-адреса узла
	if (inet_addr(ipoponent))
		dest_addr.sin_addr.s_addr = inet_addr(ipoponent);
	else
		if (hostent = gethostbyname(ipoponent))
			dest_addr.sin_addr.s_addr = ((unsigned long **)
				hostent->h_addr_list)[0][0];
		else
		{
			MessageBox(NULL, (LPWSTR)WSAGetLastError(), _T("Unknown host:"), MB_OKCANCEL);
			IsError = true;
			closesocket(my_sock);
			WSACleanup();
			return -1;
		}
	// открываем камеру
	cv::VideoCapture cap(0);
	cv::Mat frame;
	if(numberOfcalls==1)
	cvNamedWindow(strNameWindow, 1 | CV_GUI_EXPANDED);
	register unsigned char stroka[1920/4];
	cap >> frame2;
	char c;

	//проверка: подключена ли веб камера?
	if((frame2.cols>0)&&(!IsError))
	while (NeKonecrazgovorap)
	{
		cap >> frame; // получаем кадры от камеры	
		resize(frame, sendFraem,cv::Size(frame.cols/4,frame.rows/4));
			for (int y = 0; y < sendFraem.rows; ++y) {
				for (int x = 0; x < sendFraem.cols; ++x) {
					stroka[3 * x] = sendFraem.at<cv::Vec3b>(y, x).val[0];
					stroka[3 * x + 1] = sendFraem.at<cv::Vec3b>(y, x).val[1];
					stroka[3 * x + 2] = sendFraem.at<cv::Vec3b>(y, x).val[2];
				}
				sendto(my_sock, (char*)stroka, 1920/4, 0, (sockaddr *)&dest_addr, sizeof(dest_addr));
			}
		Sleep(70);
		c = cvWaitKey(33);
		if (c == 27) {
			NeKonecrazgovorap = false;
			break;
		}
		
	}
	else {
		MessageBox(NULL, _T("Error:"), _T("WebCam not connected!"), MB_OKCANCEL);
		IsError = true;
	}
	// шаг последний - выход
	shutdown(my_sock, 2);
	closesocket(my_sock);
	WSACleanup();
	azvonu = false;
	return 0;
}

DWORD WINAPI NetThread(LPVOID lpParam)
{
	// Инициализация WinSock
	WORD wVersionRequested = MAKEWORD(2, 2);
	WSADATA wsaData;
	int err = WSAStartup(wVersionRequested, &wsaData);
	if (err != 0)
		MessageBox(NULL, (LPWSTR)WSAGetLastError(), _T("WSAStartup error:"), MB_OKCANCEL);

	// Открытие и закрытие сокета
	SOCKET my_sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (my_sock == INVALID_SOCKET)
	{
		MessageBox(NULL, (LPWSTR)WSAGetLastError(), _T("Socket error:"), MB_OKCANCEL);
		WSACleanup();
	}
	// Связывание сокета 
	sockaddr_in sin;
	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = htonl(INADDR_ANY);
	sin.sin_port = htons(5150);
	int yes = 1;
	if (setsockopt(my_sock, SOL_SOCKET, SO_REUSEADDR, (const char*)&yes, sizeof(int)) == -1) { perror("setsockopt"); }


	if (bind(my_sock, (sockaddr *)&sin, sizeof(sin)) == SOCKET_ERROR)
	{
		MessageBox(NULL, (LPWSTR)WSAGetLastError(), _T("bind error: "), MB_OKCANCEL);
	    closesocket(my_sock);
	}
	// Обработка присланных пактов
	sockaddr_in client_addr;
	int client_addr_size = sizeof(client_addr);

	register unsigned char stroka[1920/4];
	char c;
	cv::Mat frameRecv;
	if(!IsError)
	while (NeKonecrazgovorap)
	{
		for (int y = 0; y < 480/4; y++)
		{
			recvfrom(my_sock, (char*)stroka, 1920/4, 0, (sockaddr *)&client_addr, &client_addr_size);
			for (int x = 0; x < 640/4; ++x) {
				frame2.at<cv::Vec3b>(y, x).val[0] = stroka[3 * x];
				frame2.at<cv::Vec3b>(y, x).val[1] = stroka[3 * x + 1];
				frame2.at<cv::Vec3b>(y, x).val[2] = stroka[3 * x + 2];
			}
		}
		resize(frame2(cv::Rect(0, 0, 640/4, 480/4)), frameRecv, cv::Size(640,480));
		sendFraem.copyTo(frameRecv(cv::Rect(frameRecv.cols- sendFraem.cols, frameRecv.rows - sendFraem.rows, sendFraem.cols, sendFraem.rows)));
		imshow(strNameWindow, frameRecv);
		c = cvWaitKey(33);
		if (c == 27) { 
		NeKonecrazgovorap = false;
		break;
		}
	}
	IsError = false;
	azvonu = false;
	cvDestroyWindow(strNameWindow);
	shutdown(my_sock, 2);
	Sleep(1000);
	NeKonecrazgovorap = true;
	closesocket(my_sock);
	WSACleanup();
	return 0;
}

