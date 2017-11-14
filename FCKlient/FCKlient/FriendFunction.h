#pragma once
#include <winsock2.h>
#include <WS2tcpip.h>

//Нужна для определения являеться ли человек из поиска нами, 
//что бы пользователь не мог добавить себя в друзья и звонить 
//самому себе
bool IsMe(char c[20], char a[21])
{
	if ((c[0] != a[0]) && (c[0] != a[0] + 32) && (c[0] != a[0] - 32)) {
		return false;
	}
	for (int i = 1; a[i] != '\0'; ++i)
		if (c[i] != a[i])
			return false;
	return true;
}

//функция проверяет являеться ли пользователь другом,
//что нужно для совершения звонков, а так же 
//для того, что бы пользователь не добавлял одного и того же
//человека себе в друзья несколько раз.
bool FraendOrNot(char c[100][20], char a[20])
{
	for (int j = 0; c[j][0] != '\0';)
		for (int i = 0; a[i] != '\0'; ++i) {
			if (c[j][i] != a[i])
			{
				++j; break;
			}
			else if (a[i + 1] == '\0') {

				return true;
			}
		}
	return false;
}

void DestroyAllWindow(HWND elem, ...) {
	HWND *newElem = &elem;
	while (*newElem)         //--пока есть аргументы
	{
		DestroyWindow(*newElem);
		newElem++;
	}
}

void CleatTCHAR(TCHAR *text, int num) {
	for (int i = 0; i < num; ++i)
		text[i] = _T('\0');
}

void ClearHWND(HWND elem, ...) {
	HWND *newElem = &elem;
	while (*newElem)         //--пока есть аргументы
	{
		SendMessageA(*newElem, WM_SETTEXT, WPARAM(0), LPARAM(""));
		newElem++;
	}
}

char* IndetefitedLocalIp()
{
	char ac[80];
	char Normip[] = "192.168.1.";
	char *ss1;
	if (gethostname(ac, sizeof(ac)) == SOCKET_ERROR) {
		return "";
	}
	struct hostent *phe = gethostbyname(ac);
	if (phe == 0) { return""; }

	for (int i = 0; phe->h_addr_list[i] != 0; ++i) {
		struct in_addr addr;
		memcpy(&addr, phe->h_addr_list[i], sizeof(struct in_addr));
		ss1 = inet_ntoa(addr);
		for (int j = 0; j < 10; ++j)
		{
			if (Normip[j] != ss1[j]) { break; }
			else { if (j == 9)return ss1; }
		}
	}
	return "";
}