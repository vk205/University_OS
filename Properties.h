/* Работа с настройками программы */

#pragma once
#include <Windows.h>

struct Properties
{
	// Информация о размере игрового поля
	UINT WindowHeight;
	UINT WindowWidth;

	// Информация о размерности игрового поля
	UINT GridSize;

	// Информация о цвете фона игрового поля
	COLORREF BackgroundColor;

	// Информация о цвете сетки игрового поля
	COLORREF GridColor;
};

// Функция для задания начальных настроек программы
Properties* SetProperties();

// Функция для выделения из строки поля настроек программы
Properties* StringToProperties(const TCHAR*);

// Функция для записи полей настроек программы в строку
TCHAR* PropertiesToString(const Properties*);
