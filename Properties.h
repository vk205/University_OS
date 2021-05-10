/* ������ � ����������� ��������� */

#pragma once
#include <Windows.h>

struct Properties
{
	// ���������� � ������� �������� ����
	UINT WindowHeight;
	UINT WindowWidth;

	// ���������� � ����������� �������� ����
	UINT GridSize;

	// ���������� � ����� ���� �������� ����
	COLORREF BackgroundColor;

	// ���������� � ����� ����� �������� ����
	COLORREF GridColor;
};

// ������� ��� ������� ��������� �������� ���������
Properties* SetProperties();

// ������� ��� ��������� �� ������ ���� �������� ���������
Properties* StringToProperties(const TCHAR*);

// ������� ��� ������ ����� �������� ��������� � ������
TCHAR* PropertiesToString(const Properties*);
