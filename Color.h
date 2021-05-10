/* Раюота с цветами */

#pragma once
#include <Windows.h>

// Вспомогательная функция получения рандомного числа
UINT Random(UINT);

// Функция получения рандомного цвета
COLORREF GetRandomColor();

// Функция получения следующего за переданным цветом согласно переданному направлению
COLORREF GetNextColor(COLORREF, UINT);


