#include "stdafx.h"
#include <CommCtrl.h>

LOGFONT CreateNewFont(LONG lfHeight, LONG lfWidth, LONG lfEscapement, LONG lfOrientation, LONG lfWeight,
	BYTE lfItalic, BYTE lfUnderline, BYTE lfStrikeOut, BYTE lfCharSet, BYTE lfOutPrecision, BYTE lfClipPrecision,
	BYTE lfQuality, BYTE lfPitchAndFamily, const char* lfFaceName) {
	LOGFONT font;
	font.lfHeight = lfHeight;// Устанавливает высоту шрифта или символа
	font.lfWidth = lfWidth;// Устанавливает среднюю ширину символов в шрифте
	font.lfEscapement = lfEscapement;// Устанавливает угол, между вектором наклона и осью X устройства
	font.lfOrientation = lfOrientation;// Устанавливает угол, между основной линией каждого символа и осью X устройства
	font.lfWeight = lfWeight;// Устанавливает толщину шрифта в диапазоне от 0 до 1000
	font.lfItalic = lfItalic;// Устанавливает курсивный шрифт
	font.lfUnderline = lfUnderline;// Устанавливает подчеркнутый шрифт
	font.lfStrikeOut = lfStrikeOut;// Устанавливает зачеркнутый шрифт
	font.lfCharSet = lfCharSet;// Устанавливает набор символов
	font.lfOutPrecision = lfOutPrecision;// Устанавливает точность вывода
	font.lfClipPrecision = lfClipPrecision;// Устанавливает точность отсечения
	font.lfQuality = lfQuality;// Устанавливает качество вывода
	font.lfPitchAndFamily = lfPitchAndFamily;// Устанавливает ширину символов и семейство шрифта
	strcpy((char*)font.lfFaceName, lfFaceName);//  устанавливает название шрифта
	return font;
}