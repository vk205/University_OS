/* Работа с конфигурационным файлом */

#pragma once
#include <windows.h>
#include "Properties.h"

// Функция записи настроек в конфигурационный файл средствами WinAPI
void WriteConfigUsingWinApi(const Properties* prefs);
// Функция чтения настроек из конфигурационного файла средствами WinAPI
Properties* ReadConfigUsingWinApi();
// Функция чтения настроек с терминала
Properties* ReadConfigUsingCommandLine();
// Функция получение настроек программы
Properties* GetProperties();