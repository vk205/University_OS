/* ���������� ���� */

#pragma once
#include "Windows.h"

// ������������ ����������� �������� ����
const int kNMax = 10;

// ��������������� ������� ��������� �����
BOOL DrawLine(HDC, int, int, int, int);
// ������� ���������� �����
void DrawGrid(HDC, int cellW, int cellH, int dim, COLORREF gridColor);
// ������� ���������� ������
void DrowCross(HDC, int, int, int, int, COLORREF);
// ������� ���������� �����
void DrowNull(HDC, int, int, int, int, COLORREF);
// ������� ���������� ����� �� ������� ����
void DrawFigures(HDC, int[kNMax][kNMax], int, int, int, COLORREF, COLORREF);
// ������� ����������� �������� ����
void Repaint(HDC, int, int, int, int[kNMax][kNMax], COLORREF, COLORREF, COLORREF);