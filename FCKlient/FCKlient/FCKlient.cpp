#include "stdafx.h"
#include <string>
#include <Shlobj.h>
#include <conio.h>
#include <mmsystem.h>
#include <fstream>
#include <Shellapi.h>
#include "FCKlient.h"

#include "FontCreate.h"
#include "FriendFunction.h"
#include "ConnectionToServer.h"


#pragma comment(lib,"Comctl32.lib")
#pragma comment(lib,"winmm.lib") 

#define MAX_LOADSTRING 100
using namespace std;

HINSTANCE hInst;
TCHAR szTitle[MAX_LOADSTRING];
TCHAR szWindowClass[MAX_LOADSTRING];
ATOM  MyRegisterClass(HINSTANCE hInstance);
BOOL  InitInstance(HINSTANCE, int);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK About(HWND, UINT, WPARAM, LPARAM);

int Width = GetSystemMetrics(SM_CXSCREEN);
int Height = GetSystemMetrics(SM_CYSCREEN);

//кнопки
HWND ButRegistration, ButLogin, ButToRegister, ButReturn, ButCall, ButaddFreand, ButgoOut, ButSearch,ButSearchFreand;
//текстовое поле ввода
HWND hEdit, hEdit2, hEdit3, hEdit4;
// Бокс в котором выбирается дата рождения
HWND hDataTime;
// Бокс с выбором вариатнов (комбо боксы)
HWND hCombo, hCombo2;
HANDLE hBitmap, hOldBitmap;
RECT Rect;
BITMAP Bitmap;
HDC  hCompatibleDC;
bool reg = false, entry = false, correctinput = true, dataUser = false;
HWND hWnd2, Texts, Texts2, Texts3, Texts4, Texts5, TextOnlineStatys;
wchar_t wtext[MAX_PATH];
wchar_t Frrname[20];

SOCKET soc_client;

TCHAR Text[20];
static TCHAR errorss[30];
TCHAR pol[4];
char ipserver[15] = "192.168.0.102";


char NIK[20];
char NIKFRND[20];
char nikf[20];
//переменные храняшие пустые строки 
char Empty[MAX_PATH] = "", Empty2[20] = "";
//переменные хранящие отправные данные
char theSendData[MAX_PATH] = "", theSendDat2[MAX_PATH] = "";
//переменные хранящие получаемые данные
char recevData[MAX_PATH] = "",recevData2[MAX_PATH] = "";
char EmptyRecevData[MAX_PATH] = "";
//строки которые содержат ключевые значения, которые использует клиент для общения с сервером
char sendMes[3], answer[3];
//переменная используемая для временного хранения значений при обработке текста
char hh[20];

char MyFreand[100][20];
char People[100][35];
char HeisMyFreand = true;
bool itisMe = true;
char FrendIP[15] = "\0";
//int seconds = time(NULL);


HANDLE hNetThread2;
DWORD dwNetThreadId2;
TCHAR Text2[35];
///////////////////////////////////////////////////////////******************************************************

SOCKET Listen;
WSAData data2;
WORD version = MAKEWORD(1, 0);
int res = WSAStartup(version, &data2);
struct addrinfo hints;
struct addrinfo *result;

/*поток в котором остлеживаються входящие звонки от других пользователей,
если пользователь принимает звонок, то активируеться видео конфиренция*/
DWORD WINAPI wiretappingIncomingCalls(LPVOID lpParam)
{
	SOCKET* Connections;
	SOCKET Connect;

	SOCKET  sServerListen;
	sServerListen = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (sServerListen == INVALID_SOCKET)
	{
		return 0;
	}

	Connections = (SOCKET*)calloc(64, sizeof(SOCKET)); //инициализация масива
	ZeroMemory(&hints, sizeof(hints)); //очистка структуры нулями

	hints.ai_family = AF_INET; //тип сокета 
	hints.ai_flags = AI_PASSIVE;
	hints.ai_socktype = SOCK_STREAM; // тип самого сокета
	hints.ai_protocol = IPPROTO_TCP;

	getaddrinfo(NULL, "7772", &hints, &result);// Передаем инфу о нас /// 7772 - порт, 
	Listen = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	bind(Listen, result->ai_addr, result->ai_addrlen);
	listen(Listen, 64);
	//старт работы сервера
	freeaddrinfo(result);
	/////////////////////////////////////////////////////////////////////

	char save[MAX_PATH];
	char save2[MAX_PATH];
	char name[23];
	char name2[20];
	int connectshion;
	char mes[3];
	while (entry) {
		if (Connect = accept(Listen, NULL, NULL)) //accept возрвщвет тру, когда кто то подключится 
		{
			recv(Connect, mes, 3, 0);
			if ((mes[0] == '1'))
			{
				recv(Connect, name2, 20, 0);
				mbstowcs(wtext, name2, strlen(name2) + 1);
				LPWSTR ptr = wtext;
				connectshion = MessageBox(NULL, (LPCWSTR)ptr, _T("Вам звонит"), MB_OKCANCEL);
				if (connectshion == 1)
				{
					mes[0] = 't'; mes[1] = 'r'; mes[2] = 'u';
					send(Connect, mes, 3, 0);
					//NeKonecrazgovorap = true;
					HANDLE hNetThread3;
					DWORD dwNetThreadId3;
					//	hNetThread3 = CreateThread(NULL, 0, talk, 0, 0, &dwNetThreadId3);
					//	conn3();
				}
				else {
					mes[0] = 'f'; mes[1] = 'a'; mes[2] = 'l';
					send(Connect, mes, 3, 0);
				}
			}
		}
	}
	return 0;
}


int APIENTRY _tWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPTSTR lpCmdLine, _In_ int  nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
	// TODO: разместите код здесь.
	MSG msg;
	HACCEL hAccelTable;
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_FCKLIENT, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);
	// Выполнить инициализацию приложения:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}
	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_FCKLIENT));
	// Цикл основного сообщения:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	return (int)msg.wParam;
}
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_FCKLIENT));
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 9);
	wcex.lpszMenuName = MAKEINTRESOURCE(IDC_FCKLIENT);
	wcex.lpszClassName = TEXT("Forgotten castle");
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));
	return RegisterClassEx(&wcex);
}
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	HWND hWnd;
	hInst = hInstance; // Сохранить дескриптор экземпляра в глобальной переменной
	hWnd = CreateWindow(TEXT("Forgotten castle"), szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);
	if (!hWnd)
	{
		return FALSE;
	}
	ShowWindow(hWnd, SW_MAXIMIZE);
	UpdateWindow(hWnd);
	return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent, m = 0;
	PAINTSTRUCT ps;
	HDC hdc;
	LOGFONT font;
	HFONT hfont;
	switch (message)
	{
	case WM_CREATE:
		if ((!reg) && (!entry)) {
			DestroyAllWindow(ButReturn);
			closesocket(Listen);
			ButRegistration = CreateWindow(_T("button"), _T("Регистрация"), WS_CHILD | BS_PUSHBUTTON | WS_VISIBLE, Width / 2, Height / 2, 120, 20, hWnd, (HMENU)300, NULL, NULL);
			ButLogin = CreateWindow(_T("button"), _T("Вход"), WS_CHILD | BS_PUSHBUTTON | WS_VISIBLE, Width / 2 - 170, Height / 2, 120, 20, hWnd, (HMENU)301, NULL, NULL);
			hEdit = CreateWindowEx(WS_EX_CLIENTEDGE, _T("Edit"), NULL, WS_CHILD | ES_LEFT | WS_VISIBLE, Width / 2 - 170, Height / 2 - 40, 290, 25, hWnd, (HMENU)302, hInst, NULL);
			hEdit2 = CreateWindowEx(WS_EX_CLIENTEDGE, _T("Edit"), NULL, WS_CHILD | ES_LEFT | WS_VISIBLE, Width / 2 - 170, Height / 2 - 85, 290, 25, hWnd, (HMENU)303, hInst, NULL);
			TerminateThread(hNetThread2, NULL);
		}
		if (reg) {
			DestroyAllWindow(ButLogin, ButRegistration);
			SendMessageA(hEdit, WM_SETTEXT, WPARAM(0), LPARAM(""));
			SendMessageA(hEdit2, WM_SETTEXT, WPARAM(0), LPARAM(""));
			hEdit3 = CreateWindowEx(WS_EX_CLIENTEDGE, _T("Edit"), NULL, WS_CHILD | ES_LEFT | WS_VISIBLE, Width / 2 - 170, Height / 2 + 5, 290, 25, hWnd, (HMENU)304, hInst, NULL);
			hEdit4 = CreateWindowEx(WS_EX_CLIENTEDGE, _T("Edit"), NULL, WS_CHILD | ES_LEFT | WS_VISIBLE, Width / 2 - 170, Height / 2 + 50, 290, 25, hWnd, (HMENU)305, hInst, NULL);
			ButToRegister = CreateWindow(_T("button"), _T("Зарегестрироваться"), WS_CHILD | BS_PUSHBUTTON | WS_VISIBLE, Width / 2, Height / 2 + 185, 140, 20, hWnd, (HMENU)306, NULL, NULL);
			hDataTime = CreateWindowEx(0, DATETIMEPICK_CLASS, TEXT("DateTime"), WS_BORDER | WS_CHILD | WS_VISIBLE | DTS_SHOWNONE, Width / 2 - 170, Height / 2 + 95, 220, 20, hWnd, (HMENU)307, hInst, NULL);
			hCombo = CreateWindow(_T("COMBOBOX"), _T("combobox"), WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST, Width / 2 - 170, Height / 2 + 140, 250, 160, hWnd, (HMENU)308, hInst, NULL);
			SendMessage(hCombo, CB_ADDSTRING, 0, (LPARAM)(_T("M")));
			SendMessage(hCombo, CB_ADDSTRING, 0, (LPARAM)(_T("W")));
			ButReturn = CreateWindow(_T("button"), _T("Вернуться"), WS_CHILD | BS_PUSHBUTTON | WS_VISIBLE, Width / 2 - 170, Height / 2 + 185, 140, 20, hWnd, (HMENU)299, hInst, NULL);
		}
		if (entry) {
			for (int i = 0; NIK[i] != '+'; ++i)
				theSendDat2[i] = NIK[i];
			strcat(theSendDat2, "+");

			//передает информацию серверу о том, что мы онлайн
			ConnectionToS(ipserver, EmptyRecevData, theSendDat2, "zdd", answer);
			for (int i = 0; NIK[i] != '+'; ++i)
				theSendData[i] = NIK[i];
			//получаем список своих друзей
			ConnectionToS(ipserver, recevData2, theSendData, "ent", answer);

			hCombo2 = CreateWindow(_T("COMBOBOX"), _T("combobox"), WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST, Width / 2 - 200, Height / 2 - 320, 250, 160, hWnd, (HMENU)310, hInst, NULL);
			ButgoOut = CreateWindow(_T("button"), _T("Выход с аккаунта"), WS_CHILD | BS_PUSHBUTTON | WS_VISIBLE, Width / 2 + 350, Height / 2 + 200, 200, 25, hWnd, (HMENU)314, hInst, NULL);
			if (recevData2[0] != '\0')
			{
				for (int i = 0; i < 20; ++i) {
					nikf[i] = '\0';
					wtext[i] = '\0';
				}
				int i = 0;
				for (; (recevData2[i] != '\0');) {
					int z = 0;
					for (; (recevData2[i] != '+'); ++i)
					{
						nikf[z] = recevData2[i];
						MyFreand[m][z] = recevData2[i];
						++z;
					}
					++m;
					++i;
					mbstowcs(wtext, nikf, strlen(nikf) + 1);
					LPWSTR ptr2 = wtext;
					SendMessage(hCombo2, CB_ADDSTRING, 0, (LPARAM)(ptr2));
				}
			}
			m = 0;

			ButSearchFreand = CreateWindow(_T("button"), _T("Выбрать друга"), WS_CHILD | BS_PUSHBUTTON | WS_VISIBLE, Width / 2 + 80, Height / 2 - 320, 200, 25, hWnd, (HMENU)317, hInst, NULL);
			hEdit = CreateWindowEx(WS_EX_CLIENTEDGE, _T("Edit"), NULL, WS_CHILD | ES_LEFT | WS_VISIBLE, Width / 2 - 200, Height / 2 - 280, 250, 25, hWnd, (HMENU)311, hInst, NULL);
			ButSearch = CreateWindow(_T("button"), _T("Найти пользователя"), WS_CHILD | BS_PUSHBUTTON | WS_VISIBLE, Width / 2 + 80, Height / 2 - 280, 200, 25, hWnd, (HMENU)312, hInst, NULL);
			
			///////////////////////////////////////вывода информации о пользователе/////////////////////////////////////////
			if (dataUser) {
				int z = 0;
				for (int i = 0; i < 20; ++i)hh[i] = '\0';
				int j = 0;
				for (j = 0; recevData[j] != '+'; ++j) {
					hh[z] = recevData[j];
					NIKFRND[z] = hh[z];
					++z;
				}
				//проверка друзья ли мы с этим юзером или нет.
				HeisMyFreand = FraendOrNot(MyFreand, hh);
				//проверяем чи не являеться данный юзер нами
				itisMe = IsMe(NIK, hh);
				mbstowcs(wtext, hh, strlen(hh) + 1);
				LPWSTR ptr = wtext;

				Texts = CreateWindow(_T("STATIC"), ptr, WS_CHILD | WS_VISIBLE, Width / 2 - 200, Height / 2 - 150, 250, 30, hWnd, NULL, NULL, NULL);
				++j; z = 0;
				for (int i = 0; i < 20; ++i)hh[i] = '\0';
				for (; recevData[j] != '+'; ++j) {}
				++j;
				for (; recevData[j] != '+'; ++j) {
					hh[z] = recevData[j];
					++z;
				}
				mbstowcs(wtext, hh, strlen(hh) + 1);
				ptr = wtext;
				Texts2 = CreateWindow(_T("STATIC"), ptr, WS_CHILD | WS_VISIBLE, Width / 2 - 200, Height / 2 - 120, 250, 30, hWnd, NULL, NULL, NULL);
				++j; z = 0;
				for (int i = 0; i < 20; ++i)hh[i] = '\0';
				for (; recevData[j] != '+'; ++j) {
					hh[z] = recevData[j];
					++z;
				}
				mbstowcs(wtext, hh, strlen(hh) + 1);
				ptr = wtext;
				Texts3 = CreateWindow(_T("STATIC"), ptr, WS_CHILD | WS_VISIBLE, Width / 2 - 200, Height / 2 - 90, 250, 30, hWnd, NULL, NULL, NULL);
				++j; z = 0;
				for (int i = 0; i < 20; ++i)hh[i] = '\0';
				for (; recevData[j] != '+'; ++j) {
					hh[z] = recevData[j];
					++z;
				}
				mbstowcs(wtext, hh, strlen(hh) + 1);
				ptr = wtext;
				Texts4 = CreateWindow(_T("STATIC"), ptr, WS_CHILD | WS_VISIBLE, Width / 2 - 200, Height / 2 - 60, 250, 30, hWnd, NULL, NULL, NULL);
				++j; z = 0;
				for (int i = 0; i < 20; ++i)hh[i] = '\0';
				for (; recevData[j] != '+'; ++j) {
					hh[z] = recevData[j];
					++z;
				}
				mbstowcs(wtext, hh, strlen(hh) + 1);
				ptr = wtext;
				Texts5 = CreateWindow(_T("STATIC"), ptr, WS_CHILD | WS_VISIBLE, Width / 2 - 200, Height / 2 - 30, 250, 30, hWnd, NULL, NULL, NULL);
				if ((!HeisMyFreand) && (!itisMe))
					ButaddFreand = CreateWindow(_T("button"), _T("Добавить в друзья"), WS_CHILD | BS_PUSHBUTTON | WS_VISIBLE, Width / 2 - 200, Height / 2 + 20, 250, 25, hWnd, (HMENU)313, hInst, NULL);
				++j; z = 0;
				for (int i = 0; i < 20; ++i)hh[i] = '\0';
				for (; recevData[j] != '\0' && recevData[j] != '+'; ++j) {
					hh[z] = recevData[j];
					++z;
				}
				mbstowcs(wtext, hh, strlen(hh) + 1);
				ptr = wtext;
				TextOnlineStatys = CreateWindow(_T("STATIC"), ptr, WS_CHILD | WS_VISIBLE, Width / 2 -10, Height / 2 -150, 60, 30, hWnd, NULL, NULL, NULL);
				if ((recevData[j] == '+') && (!itisMe))
					ButCall = CreateWindow(_T("button"), _T("Звонить"), WS_CHILD | BS_PUSHBUTTON | WS_VISIBLE, Width / 2 + 80, Height / 2 - 240, 200, 25, hWnd, (HMENU)309, hInst, NULL);
				++j; z = 0;
				for (int i = 0; i < 20; ++i)hh[i] = '\0';
				for (; recevData[j] != '\0'; ++j) {
					ipoponent[z] = recevData[j];
					++z;
				}
			}
			///////////////////////////////////////конец блока вывода информации о пользователе///////////////////
		}
		return 0;
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	case WM_COMMAND:
		wmId = LOWORD(wParam);
		wmEvent = HIWORD(wParam);

		switch (wmId)
		{
		case IDM_ABOUT:
			break;
			/////////////////////////////отмена регистрации///////////////////////////////////////
		case 299:
			correctinput = true;
			DestroyAllWindow(ButToRegister, hEdit, hEdit2, hEdit3, hEdit4, hDataTime, hCombo);
			reg = false;
			SendMessage(hWnd, WM_CREATE, 0, 0);
			InvalidateRect(hWnd, 0, true);
			break;
			/////////////////////////////check in///////////////////////////////////////
		case 300:
			reg = true;
			SendMessage(hWnd, WM_CREATE, 0, 0);
			InvalidateRect(hWnd, 0, true);
			break;
			/////////////////////////////вход в аккаунт///////////////////////////////////////
		case 301:
			lstrcpy(errorss, L"                                  ");
			GetWindowText(hEdit2, Text, 20);
			wcstombs(theSendData, Text, MAX_PATH);
			wcstombs(NIK, Text, 20);
			strcat(NIK, "+");
			if ((strlen(theSendData)<6) || (strlen(theSendData)>18))
			{
				CleatTCHAR(Text, 20);
				correctinput = false;
				lstrcpy(errorss, L"Ник содержит от 6 до 18 символов");
				InvalidateRect(hWnd, 0, true);
				strcpy(theSendData, Empty);
				break;
			}
			GetWindowText(hEdit, Text, 20);
			wcstombs(hh, Text, 20);
			if ((strlen(hh)<8) || (strlen(hh)>16))
			{
				CleatTCHAR(Text, 20);
				correctinput = false;
				lstrcpy(errorss, L"Пароль содержит от 8 до 16 символов");
				InvalidateRect(hWnd, 0, true);
				strcpy(hh, Empty2);
				break;
			}
			strcat(theSendData, "+"); strcat(theSendData, hh);
			/*передает на сервер введенный пользователем пароли и логин,
			в ответ пользователь либо получает данные и вход в свою
			учетную запись либо получает уведомление, что данные
			введены не верно*/
			ConnectionToS(ipserver, recevData, theSendData, "avt", answer);
			if (answer[0] == 'f') {
				correctinput = false;
				lstrcpy(errorss, L"Данные не верны!");
				InvalidateRect(hWnd, 0, true);
				break;
			}
			if (answer[0] == 't') {
				entry = true;
				correctinput = true;
				DestroyAllWindow(ButLogin, ButRegistration, hEdit, hEdit2, ButaddFreand, hCombo2, Texts, Texts2, Texts3, Texts4, Texts5, TextOnlineStatys, ButCall, ButgoOut);
				hNetThread2 = CreateThread(NULL, 0, wiretappingIncomingCalls, 0, 0, &dwNetThreadId2);
				SendMessage(hWnd, WM_CREATE, 0, 0);
				InvalidateRect(hWnd, 0, true);

			}
			break;
			/////////////////////////////зарегаться///////////////////////////////////////
		case 306:
			lstrcpy(errorss, L"                                  ");
			GetWindowText(hEdit2, Text, 20);
			wcstombs(theSendData, Text, MAX_PATH);
			if ((strlen(theSendData)<6) || (strlen(theSendData)>18))
			{
				CleatTCHAR(Text, 20);
				correctinput = false;
				lstrcpy(errorss, L"Ник содержит от 6 до 18 символов");
				InvalidateRect(hWnd, 0, true);
				strcpy(theSendData, Empty);
				break;
			}
			wcstombs(NIK, Text, 20);
			strcat(NIK, "+");
			GetWindowText(hEdit, Text, 20);
			wcstombs(hh, Text, 20);
			if ((strlen(hh)<8) || (strlen(hh)>16))
			{
				CleatTCHAR(Text, 20);
				correctinput = false;
				lstrcpy(errorss, L"Пароль содержит от 8 до 16 символов");
				InvalidateRect(hWnd, 0, true);
				strcpy(hh, Empty2);
				break;
			}
			strcat(theSendData, "+"); strcat(theSendData, hh);
			GetWindowText(hEdit3, Text, 20);
			wcstombs(hh, Text, 20);
			if ((strlen(hh) == 0) || (strlen(hh)>20))
			{
				CleatTCHAR(Text, 20);
				correctinput = false;
				lstrcpy(errorss, L"Введите Фамилию (до 20 сим.)");
				strcpy(hh, Empty2);
				InvalidateRect(hWnd, 0, true);
				break;
			}
			strcat(theSendData, "+"); strcat(theSendData, hh);
			GetWindowText(hEdit4, Text, 20);
			wcstombs(hh, Text, 20);
			if ((strlen(hh) == 0) || (strlen(hh)>20))
			{
				CleatTCHAR(Text, 20);
				correctinput = false;
				lstrcpy(errorss, L"Введите Имя (до 20 сим.)");
				strcpy(hh, Empty2);
				InvalidateRect(hWnd, 0, true);
				break;
			}
			strcat(theSendData, "+"); strcat(theSendData, hh);
			wcstombs(hh, pol, 10);
			if (hh[0] == '\0')
			{
				correctinput = false;
				lstrcpy(errorss, L"Выберите пол");
				InvalidateRect(hWnd, 0, true);
				break;
			}
			strcat(theSendData, "+"); strcat(theSendData, hh);
			GetWindowText(hDataTime, Text, 20);
			wcstombs(hh, Text, 20);
			strcat(theSendData, "+"); strcat(theSendData, hh);
			correctinput = true;
			/*Передает на сервер данные для создания нового акаунта
			а при удачной регестриции тут же авторизируется*/
			ConnectionToS(ipserver, recevData, theSendData, "reg", answer);
			if (answer[0] == 'f') {
				correctinput = false;
				lstrcpy(errorss, L"Данное имя занято!");
				InvalidateRect(hWnd, 0, true);
				break;
			}
			if (answer[0] == 't') {
				entry = true;
				hNetThread2 = CreateThread(NULL, 0, wiretappingIncomingCalls, 0, 0, &dwNetThreadId2);
				DestroyAllWindow(ButToRegister, hEdit, hEdit2, hEdit3, hEdit4, hDataTime, hCombo, ButReturn);
				reg = false;
				SendMessage(hWnd, WM_CREATE, 0, 0);
				InvalidateRect(hWnd, 0, true);
			}
			break;
			//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		case 308:
			GetWindowText(hCombo, pol, 10);
			break;
			////////////////////// call ///////////////////////////////////////////////////////////////////////////
		case 309:
			if (!azvonu) {
				correctinput = true;
				ConnectionToUser(ipoponent, NIK, "123", answer);
			}
			break;
			///////////////////////
		case 317:
			DestroyAllWindow(Texts, Texts2, Texts3, Texts4, Texts5, TextOnlineStatys);
			GetWindowText(hCombo2, Text, 20);
			wcstombs(theSendData, Text, 20);
			// Передает на сервер имя пользователя, а в ответ получает данные о пользователе
			ConnectionToS(ipserver, recevData, theSendData, "src", answer);
			if ((answer[0] == 'f') && (answer[1] == 'n') && (answer[2] == 'd'))
			{
				dataUser = true;
				correctinput = true;
				DestroyAllWindow(hCombo2,ButgoOut, hEdit, ButSearch, ButSearchFreand, ButCall);
				SendMessage(hWnd, WM_CREATE, 0, 0);
				InvalidateRect(hWnd, 0, true);
			}
			if ((answer[0] == 'n') && (answer[1] == 'f') && (answer[2] == 'n'))
			{
				dataUser = false;
				correctinput = false;
				lstrcpy(errorss, L"Пользователь не найден!");
				InvalidateRect(hWnd, 0, true);
			}
			break;
		///////////////////////////поиск пользователя в системе FC ///////////////////////////////////////////
		case 312:
			DestroyAllWindow(Texts, Texts2, Texts3, Texts4, Texts5, TextOnlineStatys);
			GetWindowText(hEdit, Text, 20);
			wcstombs(theSendData, Text, 20);
			// Передает на сервер имя пользователя, а в ответ получает данные о пользователе
			ConnectionToS(ipserver, recevData, theSendData, "src", answer);
			if ((answer[0] == 'f') && (answer[1] == 'n') && (answer[2] == 'd'))
			{
				dataUser = true;
				correctinput = true;
				DestroyAllWindow(hCombo2,  ButgoOut, hEdit, ButSearch, ButSearchFreand, ButCall);
				SendMessage(hWnd, WM_CREATE, 0, 0);
				InvalidateRect(hWnd, 0, true);
			}
			if ((answer[0] == 'n') && (answer[1] == 'f') && (answer[2] == 'n'))
			{
				dataUser = false;
				correctinput = false;
				lstrcpy(errorss, L"Пользователь не найден!");
				InvalidateRect(hWnd, 0, true);
			}
			break;
			///////////////////////////add freand /////////////////////////////////////////////////////////////
		case 313:
			for (int i = 0; i < 260; ++i)
				theSendData[i] = '\0';
			int i;
			for (i = 0; NIK[i] != '\0'; ++i)
				theSendData[i] = NIK[i];
			strcat(NIKFRND, "+");
			int q; q = 0;
			for (; NIKFRND[q] != '\0'; ++i) {
				theSendData[i] = NIKFRND[q]; ++q;
			}
			/*Добавляет пользователя в друзья на сервере и получает обновленные список своих друзей */
			ConnectionToS(ipserver, recevData, theSendData, "add", answer);
			DestroyAllWindow(Texts, Texts2, Texts3, Texts4, Texts5, TextOnlineStatys, ButaddFreand);
			DestroyAllWindow(ButLogin, ButSearch, ButSearchFreand, hCombo2, hEdit, hEdit2);
			dataUser = false;
			SendMessage(hWnd, WM_CREATE, 0, 0);
			break;
			//////////////////////////выход с аккаунта /////////////////////////////////////////////////////////////
		case 314:

			correctinput = true;
			DestroyAllWindow(Texts, Texts2, Texts3, Texts4, Texts5, TextOnlineStatys);
			DestroyAllWindow(ButLogin, ButSearch, ButSearchFreand,  hCombo2, hEdit, hEdit2, ButaddFreand);
			entry = false;
			dataUser = false;
			PostThreadMessage(dwNetThreadId2, WM_QUIT, 0, 0);//завершение потока
			SendMessage(hWnd, WM_CREATE, 0, 0);
			InvalidateRect(hWnd, 0, true);
			DestroyAllWindow(ButgoOut, ButSearch, ButSearchFreand, ButCall);
			break;
		case IDM_EXIT:
			system("taskkill /IM AudioServoklient.exe");
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		if (!entry) {
			SetBkMode(hdc, TRANSPARENT);
			font = CreateNewFont(120, 0, 0, 0, 100, 1, 0, 0, RUSSIAN_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, "Copperplate Gothic Bold");
			SetTextColor(hdc, RGB(255, 255, 255));
			hfont = ::CreateFontIndirect(&font);
			SelectObject(ps.hdc, hfont);
			TextOut(hdc, Width / 2 - 340, Height / 2 - 280, _T("Forgotten Castle"), 16);
		}
		if (reg) {
			//задаем параметры шрифта (функция из FontCreate.h)
			font = CreateNewFont(24, 0, 0, 0, 100, 1, 0, 0, RUSSIAN_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, "Times New Roman");
			SetTextColor(hdc, RGB(255, 255, 255));
			//применяем наш шрифт к отображаемому тексту
			hfont = ::CreateFontIndirect(&font);
			SelectObject(ps.hdc, hfont);
			TextOut(hdc, Width / 2 + 130, Height / 2 - 85, _T("-Ник"), 4);
			TextOut(hdc, Width / 2 + 130, Height / 2 - 40, _T("-Пароль"), 7);
			TextOut(hdc, Width / 2 + 130, Height / 2 + 5, _T("-Фамилия"), 8);
			TextOut(hdc, Width / 2 + 130, Height / 2 + 50, _T("-Имя"), 4);
			TextOut(hdc, Width / 2 + 130, Height / 2 + 95, _T("-Дата"), 5);
			TextOut(hdc, Width / 2 + 130, Height / 2 + 140, _T("-Пол"), 4);
		}
		if ((!reg) && (!entry)) {
			font = CreateNewFont(24, 0, 0, 0, 100, 1, 0, 0, RUSSIAN_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, "Times New Roman");
			SetTextColor(hdc, RGB(255, 255, 255));
			hfont = ::CreateFontIndirect(&font);
			SelectObject(ps.hdc, hfont);
			TextOut(hdc, Width / 2 + 130, Height / 2 - 85, _T("-Ник"), 4);
			TextOut(hdc, Width / 2 + 130, Height / 2 - 40, _T("-Пароль"), 7);
		}
		if (!correctinput) {
			TextOut(hdc, Width / 2 - 120, Height / 2 + 240, errorss, 50);
		}
		if (entry) {
			hBitmap = LoadImage(NULL, _T("paint.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
			GetObject(hBitmap, sizeof(BITMAP), &Bitmap);
			GetObject(hBitmap, sizeof(BITMAP), &Bitmap);
			// создать совместимый с контекстом окна контекст в памяти 
			hCompatibleDC = CreateCompatibleDC(hdc);
			// делаем загруженный битмап текущим в совместимом контексте 
			hOldBitmap = SelectObject(hCompatibleDC, hBitmap);
			// определить размер рабочей области окна 
			GetClientRect(hWnd, &Rect);
			BitBlt(hdc, 0, 0, Rect.right, Rect.bottom, hCompatibleDC, 0, 0, SRCCOPY);
			SetBkMode(hdc, TRANSPARENT);
		}
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		system("taskkill /IM AudioServoklient.exe");
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}