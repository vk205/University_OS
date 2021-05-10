#include "SharedStorage.h"
#include <iostream>

bool isOpened = false;

LPVOID mappingAddress = nullptr; // ������������ ��������� ����� ��� �����������
HANDLE mappingHandle = nullptr; // ������������� ����������� ����� �� ������

void OpenStorage(const std::wstring& name, const int size)
{
	// �������� ���� �� ������� ����������� �����
	if (isOpened)
	{
		return;
	}

	if (size <= 0)
	{
		throw std::exception("Can't open shared storage with non-positive size.");
	}

	// ������� ������-�����������
	const auto fileMapSize = size*size;

	mappingHandle = CreateFileMapping(INVALID_HANDLE_VALUE, // ���������� �����,  ��������� �������� CreateFile()
		nullptr, //  ��������� SECURITYATTRIBUTES
		PAGE_READWRITE, // ��������� ������  � ������ 
		0, // ������������ ������ ������������� ����� � ������� ������� DWORD
		fileMapSize, // ����. ������ ������������� ����� � ������ ������� DWORD 
		name.c_str()// ��� ������������� �����, ���� NULL, �� ��� �����
	);


	// ���� ������� �� �������, ������� ��� ������
	if (!mappingHandle)
	{
		throw std::exception("Unable to create file mapping.");
	}

	// ��������� ����������� ����� �� ������
	mappingAddress = MapViewOfFile(mappingHandle,
		FILE_MAP_ALL_ACCESS,
		0,
		0,
		fileMapSize);

	// ���� ��������� ����������� �� �������, ������� ��� ������
	if (!mappingAddress)
	{
		throw std::exception("Unable to retrieve mapping address.");
	}

	isOpened = true;
}

void CloseStorage()
{
	// �������� ���� �� ������� ����������� �����
	if (!isOpened)
	{
		return;
	}

	// �������� ����������� �����
	UnmapViewOfFile(mappingAddress);
	// ����������� ������������� ���������� �������-�����������
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