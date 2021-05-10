#include "GridDraw.h"


BOOL DrawLine(HDC hdc, int x1, int y1, int x2, int y2)
{
	MoveToEx(hdc, x1, y1, nullptr);
	return LineTo(hdc, x2, y2);
}

void DrawGrid(HDC hdc, int cellW, int cellH, int dim, COLORREF gridColor)
{
	HPEN hGrid = CreatePen(PS_SOLID, 5, gridColor);
	SelectObject(hdc, hGrid);

	// проприсовка вертикальных линий
	for (int i = 0; i <= cellW * dim; i = i + cellW)
		DrawLine(hdc, i, 0, i, cellH * dim);

	// проприсовка горизонтальных линий
	for (int i = 0; i <= cellH * dim; i = i + cellH)
		DrawLine(hdc, 0, i, cellW*dim, i);

	DeleteObject(hGrid);
}


void DrowCross(HDC hdc, int indexI, int indexJ, int cellW, int cellH, COLORREF crossColor) {

	// координата верхнего левого угла ячейки
	int i = cellW * indexI; 
	int j = cellH * indexJ;

	// редактирование размера фигуры относительно границ ячейки
	int dx = cellW * 0.1;
	int dy = cellH * 0.1;

	HPEN hCross = CreatePen(PS_SOLID, 5, crossColor);
	SelectObject(hdc, hCross);

	DrawLine(hdc, i + cellW - dx, j + cellH - dy, i + dx, j + dy);
	DrawLine(hdc, i + dx, j + cellH - dy, i + cellW - dx, j + dy);

	DeleteObject(hCross);
}

void DrowNull(HDC hdc, int indexI, int indexJ, int cellW, int cellH, COLORREF crossNULL) {

	// координата верхнего левого угла ячейки
	int i = cellW * indexI;
	int j = cellH * indexJ;

	// редактирование размера фигуры относительно границ ячейки
	int dx = cellW * 0.1;
	int dy = cellH * 0.1;

	HBRUSH hbNull = CreateSolidBrush(crossNULL);       
	HPEN hNull = CreatePen(PS_SOLID, 5, crossNULL);      

	SelectObject(hdc, hbNull);
	SelectObject(hdc, hNull);

	Ellipse(hdc, i + cellW - dx, j + cellH - dy, i + dx, j + dy);

	DeleteObject(hbNull);
	DeleteObject(hNull);
}

void DrawFigures(HDC hdc, int field[kNMax][kNMax], int dim, int cellW, int cellH,
	COLORREF crossColor, COLORREF nullColor)
{
	for (int i = 0; i < dim; i++)
		for (int j = 0; j < dim; j++) {
			if (field[i][j] != 0) {
				if (field[i][j] < 0)
					DrowNull(hdc, i, j, cellW, cellH, nullColor);
				else
					DrowCross(hdc, i, j, cellW, cellH, crossColor);
			}
		}
}

void Repaint(HDC hdc, int cellW, int cellH, int dim, int field[kNMax][kNMax],
	COLORREF gridColor, COLORREF crossColor, COLORREF nullColor)
{
	DrawGrid(hdc, cellW, cellH, dim, gridColor);
	DrawFigures(hdc, field, dim, cellW, cellH, crossColor, nullColor);
}
