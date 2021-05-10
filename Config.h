/* ������ � ���������������� ������ */

#pragma once
#include <windows.h>
#include "Properties.h"

// ������� ������ �������� � ���������������� ���� ���������� WinAPI
void WriteConfigUsingWinApi(const Properties* prefs);
// ������� ������ �������� �� ����������������� ����� ���������� WinAPI
Properties* ReadConfigUsingWinApi();
// ������� ������ �������� � ���������
Properties* ReadConfigUsingCommandLine();
// ������� ��������� �������� ���������
Properties* GetProperties();