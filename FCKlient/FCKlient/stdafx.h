// stdafx.h: включаемый файл для стандартных системных включаемых файлов
// или включаемых файлов для конкретного проекта, которые часто используются, но
// не часто изменяются
//
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Исключите редко используемые компоненты из заголовков Windows
#define ISOLATION_AWARE_ENABLED 1
// Файлы заголовков Windows:
#include <windows.h>

// Файлы заголовков C RunTime
//#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

#pragma comment(linker,"\"/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
// TODO: Установите здесь ссылки на дополнительные заголовки, требующиеся для программы
