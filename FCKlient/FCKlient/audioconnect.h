#pragma once
#include "ConnectwithUSer.h"

#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <CommCtrl.h>
#include <Shlobj.h>
#include <conio.h>
#include <mmsystem.h>
#include <fstream>
#include <Shellapi.h>

#pragma comment(lib,"Comctl32.lib")
#pragma comment(lib,"winmm.lib") 


#define MAX_LOADSTRING 100
using namespace std;

HANDLE hNetThread5;
DWORD dwNetThreadId5;

const int NUMPTS = 22050;
int sampleRate = 22050;//частота дискретизации
short int waveIn[NUMPTS];
short int waveOut[22050];
HWAVEOUT hWaveOut;
WAVEHDR WaveOutHdr;
char h[2646];
int chislo;
bool isEndtalk = false;

DWORD WINAPI readAudio(LPVOID lpParam)
{

	while (NeKonecrazgovorap) {
		waveOutWrite(hWaveOut, &WaveOutHdr, sizeof(WAVEHDR));
	}
	PostThreadMessage(dwNetThreadId5, WM_QUIT, 0, 0);//завершение потока
	isEndtalk = true;
	waveOutClose(hWaveOut);
	return 0;
}

DWORD WINAPI talk2(LPVOID lpParam)
{
	WAVEFORMATEX pFormat;// структура определяет формат формы волны аудио данных
	pFormat.wFormatTag = WAVE_FORMAT_PCM;     // simple, uncompressed format
	pFormat.nChannels = 2;                    //  1=mono, 2=stereo
	pFormat.wBitsPerSample = 16;              //  16 for high quality, 8 for telephone-grade
	pFormat.nSamplesPerSec = sampleRate;      //частота дискретизации
	pFormat.nAvgBytesPerSec = sampleRate * pFormat.nChannels * pFormat.wBitsPerSample / 8;//средняя скорость передачи данных в байтах в секунду
	pFormat.nBlockAlign = pFormat.nChannels * pFormat.wBitsPerSample / 8;//Блок выравнивания, в байтах.
	pFormat.cbSize = 0;//Размер, в байтах дополнительной информации о формате прилагается к концу WAVEFORMATEX структуры
	HWAVEIN hWaveIn;
	WAVEHDR waveInHdr;
	waveInOpen(&hWaveIn, WAVE_MAPPER, &pFormat, 0L, 0L, WAVE_FORMAT_DIRECT);
	waveInHdr.lpData = (LPSTR)waveIn;
	waveInHdr.dwBufferLength = NUMPTS * 2;
	waveInHdr.dwBytesRecorded = 0;
	waveInHdr.dwUser = 0L;
	waveInHdr.dwFlags = 0L;
	waveInHdr.dwLoops = 0L;
	waveInPrepareHeader(hWaveIn, &waveInHdr, sizeof(WAVEHDR));//функция подготавливает буфер для сигнала аудио входа
	waveInAddBuffer(hWaveIn, &waveInHdr, sizeof(WAVEHDR));//функция посылает входной буфер для данного устройства ввода сигнала аудио

	waveOutOpen(&hWaveOut, WAVE_MAPPER, &pFormat, 0L, 0L, WAVE_FORMAT_DIRECT);
	WaveOutHdr.lpData = (LPSTR)waveOut;
	WaveOutHdr.dwBufferLength = NUMPTS * 2;
	WaveOutHdr.dwBytesRecorded = 0;
	WaveOutHdr.dwUser = 0L;
	WaveOutHdr.dwFlags = 0L;
	WaveOutHdr.dwLoops = 1L;
	waveOutPrepareHeader(hWaveOut, &WaveOutHdr, sizeof(WAVEHDR));
	waveOutSetVolume(hWaveOut, 0xFFFF*2);
	waveInStart(hWaveIn);//функция начинает вход на данном устройстве входного сигнала аудио
	HANDLE hNetThread;
	DWORD dwNetThreadId;
	hNetThread = CreateThread(NULL, 0, readAudio, 0, 0, &dwNetThreadId);
	// Wait until finished recording
	int ch;
	do {
		waveInAddBuffer(hWaveIn, &waveInHdr, sizeof(WAVEHDR));//функция посылает входной буфер для данного устройства ввода сигнала аудио
	} while (1);
	waveInClose(hWaveIn);//очистка буфера заполнения
	waveOutClose(hWaveOut);
	return 0;
}
DWORD WINAPI NetThreadAud(LPVOID lpParam) // прием audio от другого клиента
{
	SOCKET  sServerListen;
	struct sockaddr_in localaddr, clientaddr;
	int   iSize;
	sServerListen = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (sServerListen == INVALID_SOCKET)
	{
		return 0;
	}
	localaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	localaddr.sin_family = AF_INET;
	localaddr.sin_port = htons(7779);
	if (bind(sServerListen, (struct sockaddr *)&localaddr,
		sizeof(localaddr)) == SOCKET_ERROR)
	{
		return 1;
	}
	while (!isEndtalk)
	{
		int s2 = 0;
		char h2[2646];
		for (int j = 0; j < 50; ++j) {
			recv(sServerListen, h2, 2646, 0);
			for (int i = 0; i < 441; ++i) {
				waveOut[s2] = h2[5 + 6 * i] - 48;
				waveOut[s2] += (h2[4 + 6 * i] - 48) * 10;
				waveOut[s2] += (h2[3 + 6 * i] - 48) * 100;
				waveOut[s2] += (h2[2 + 6 * i] - 48) * 1000;
				waveOut[s2] += (h2[1 + 6 * i] - 48) * 10000;
				if (h2[6 * i] == '-')waveOut[s2] *= -1;
				s2++;
			}
		}
		Sleep(100);
	}
	closesocket(sServerListen);
	return 0;
}
DWORD WINAPI talk(LPVOID lpParam)//передача аудио другому клиенту
{
	WSADATA wsd;
	if (WSAStartup(MAKEWORD(2, 2), &wsd) != 0)
	{
		return 0;
	}
	SOCKET   sSocket;
	struct   sockaddr_in servaddr;
	char    szServerName[1024], szMessage[1024];
	struct hostent *host = NULL;
	strcpy(szServerName, ipoponent);
	sSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (sSocket == INVALID_SOCKET)
	{
		return 0;
	}
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(7779);
	servaddr.sin_addr.s_addr = inet_addr(szServerName);
	if (servaddr.sin_addr.s_addr == INADDR_NONE)
	{
		host = gethostbyname(szServerName);
		if (host == NULL)
		{
			return 1;
		}
		CopyMemory(&servaddr.sin_addr, host->h_addr_list[0],
			host->h_length);
	}
	
	hNetThread5 = CreateThread(NULL, 0, talk2, 0, 0, &dwNetThreadId5);
	int ch;
	Sleep(40);
	while (!isEndtalk) {
		int s = 0;
		for (int j = 0; j < 50; ++j) {
			for (int i = 0; i < 441; ++i) {
				ch = (waveIn[s]);
				if (waveIn[s] < 0)h[6 * i] = '-';
				else h[6 * i] = '0';
				chislo = waveIn[s] / 10000;
				h[1 + 6 * i] = chislo + 48;
				ch -= chislo * 10000; chislo = ch / 1000; h[2 + 6 * i] = chislo + 48;
				ch -= chislo * 1000; chislo = ch / 100; h[3 + 6 * i] = chislo + 48;
				ch -= chislo * 100; chislo = ch / 10; h[4 + 6 * i] = chislo + 48;
				ch -= chislo * 10;  h[5 + 6 * i] = ch + 48;
				++s;
			}
			sendto(sSocket, h, 2646, 0, (struct sockaddr *)&servaddr, sizeof(servaddr));
		}
		Sleep(200);
	}
	return 0;
}
