/* ������ � ����������� ������� */

#pragma once
#include "Windows.h"
#include "GridDraw.h"
#include <string>

// �������� ������� - �����������
void OpenStorage(const std::wstring& name, int size);

// �������� ������� - �����������
void CloseStorage();

// ������ �� �����
int* GetStorage();

// ������ � ���� ������������ �������� ����
void SetField(int[kNMax][kNMax], int);