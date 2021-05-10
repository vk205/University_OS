/* Работа с разделяемой памятью */

#pragma once
#include "Windows.h"
#include "GridDraw.h"
#include <string>

// Открытие объекта - отображения
void OpenStorage(const std::wstring& name, int size);

// Закрытие объекта - отображения
void CloseStorage();

// Чтение из файла
int* GetStorage();

// Запись в файл изменненного игрового поля
void SetField(int[kNMax][kNMax], int);