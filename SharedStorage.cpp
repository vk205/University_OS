#include "SharedStorage.h"
#include <iostream>

bool isOpened = false;

LPVOID mappingAddress = nullptr; // предлагаемый начальный адрес для отображения
HANDLE mappingHandle = nullptr; // идентификатор отображения файла на память

void OpenStorage(const std::wstring& name, const int size)
{
	// Проверка было ли создано отображения ранее
	if (isOpened)
	{
		return;
	}

	if (size <= 0)
	{
		throw std::exception("Can't open shared storage with non-positive size.");
	}

	// Создаем объект-отображение
	const auto fileMapSize = size*size;

	mappingHandle = CreateFileMapping(INVALID_HANDLE_VALUE, // дескриптор файла,  открытого функцией CreateFile()
		nullptr, //  структура SECURITYATTRIBUTES
		PAGE_READWRITE, // параметры работы  с файлом 
		0, // максимальный размер проецируемого файла в верхнем разряде DWORD
		fileMapSize, // макс. размер проецируемого файла в нижнем разряде DWORD 
		name.c_str()// имя проецируемого файла, если NULL, то без имени
	);


	// Если создать не удалось, выводим код ошибки
	if (!mappingHandle)
	{
		throw std::exception("Unable to create file mapping.");
	}

	// Выполняем отображение файла на память
	mappingAddress = MapViewOfFile(mappingHandle,
		FILE_MAP_ALL_ACCESS,
		0,
		0,
		fileMapSize);

	// Если выполнить отображение не удалось, выводим код ошибки
	if (!mappingAddress)
	{
		throw std::exception("Unable to retrieve mapping address.");
	}

	isOpened = true;
}

void CloseStorage()
{
	// Проверка было ли закрыто отображение ранее
	if (!isOpened)
	{
		return;
	}

	// Отменяем отображение файла
	UnmapViewOfFile(mappingAddress);
	// Освобождаем идентификатор созданного объекта-отображения
	CloseHandle(mappingHandle);

	isOpened = false;
}

int* GetStorage()
{
	return (int*)mappingAddress;
}

void SetField(int field[kNMax][kNMax], int dim)
{
	int k=0;
	for (int i = 0; i < dim; i++)
		for (int j = 0; j < dim; j++)
			*((LPDWORD)mappingAddress + (k++)) = field[i][j];
}