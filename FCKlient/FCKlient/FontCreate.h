#include "stdafx.h"
#include <CommCtrl.h>

LOGFONT CreateNewFont(LONG lfHeight, LONG lfWidth, LONG lfEscapement, LONG lfOrientation, LONG lfWeight,
	BYTE lfItalic, BYTE lfUnderline, BYTE lfStrikeOut, BYTE lfCharSet, BYTE lfOutPrecision, BYTE lfClipPrecision,
	BYTE lfQuality, BYTE lfPitchAndFamily, const char* lfFaceName) {
	LOGFONT font;
	font.lfHeight = lfHeight;// ������������� ������ ������ ��� �������
	font.lfWidth = lfWidth;// ������������� ������� ������ �������� � ������
	font.lfEscapement = lfEscapement;// ������������� ����, ����� �������� ������� � ���� X ����������
	font.lfOrientation = lfOrientation;// ������������� ����, ����� �������� ������ ������� ������� � ���� X ����������
	font.lfWeight = lfWeight;// ������������� ������� ������ � ��������� �� 0 �� 1000
	font.lfItalic = lfItalic;// ������������� ��������� �����
	font.lfUnderline = lfUnderline;// ������������� ������������ �����
	font.lfStrikeOut = lfStrikeOut;// ������������� ����������� �����
	font.lfCharSet = lfCharSet;// ������������� ����� ��������
	font.lfOutPrecision = lfOutPrecision;// ������������� �������� ������
	font.lfClipPrecision = lfClipPrecision;// ������������� �������� ���������
	font.lfQuality = lfQuality;// ������������� �������� ������
	font.lfPitchAndFamily = lfPitchAndFamily;// ������������� ������ �������� � ��������� ������
	strcpy((char*)font.lfFaceName, lfFaceName);//  ������������� �������� ������
	return font;
}