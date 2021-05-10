/* Прорисовка поля */

#pragma once
#include "Windows.h"

// Максимальная размерность игрового поля
const int kNMax = 10;

// Вспомогательная функция рисования линии
BOOL DrawLine(HDC, int, int, int, int);
// Функция прорисовки сетки
void DrawGrid(HDC, int cellW, int cellH, int dim, COLORREF gridColor);
// Функция прорисовки креста
void DrowCross(HDC, int, int, int, int, COLORREF);
// Функция прорисовки круга
void DrowNull(HDC, int, int, int, int, COLORREF);
// Функция прорисовки фигур на игровом поле
void DrawFigures(HDC, int[kNMax][kNMax], int, int, int, COLORREF, COLORREF);
// Функция перерисовки игрового поля
void Repaint(HDC, int, int, int, int[kNMax][kNMax], COLORREF, COLORREF, COLORREF);