#ifndef UNICODE
#define UNICODE
#endif 

#define _CRT_SECURE_NO_WARNINGS

// Подключение библиотек
#include <windows.h>
#include <windowsx.h> 
#include "GridDraw.h"
#include "SharedStorage.h"
#include "Config.h"
#include "Color.h"
#include "ThreadDraw.h"
#include "GameRules.h"


const wchar_t CLASS_NAME[] = L"Sample Window Class"; // Имя класса окна
const wchar_t WINDOW_NAME[] = L"Tick-Tack-Toe";      // Заголовок окна

HDC hdc;
WNDCLASS wc = { };

unsigned WM_FIELDUPDATE = RegisterWindowMessage(L"WM_FIELDUPDATE"); // Сообщение о необходимости перерисовки игрового поля

Properties * prefs; // Структура для работы с настройками программы

COLORREF crossColor = 0x00000ff; // Цвет фигуры "крест"
COLORREF nullColor = 0x0000ff;   // Цвет фигуры "круг"

HBRUSH hBrush; // Цвет фона

int N; // Размерность игрового поля

int field[kNMax][kNMax] = { 0 }; // Игровое поле

int playerCount;

int cellH; // Высота ячейки
int cellW; // Ширина ячейки

// Прототипы функций
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void GetIndex(LPARAM, int &, int &);
void RunNotepad(void);
COLORREF SetBackround(HWND);
void IfClicked(HWND);

int Count = 0;

BOOL CALLBACK CountWindowsProc(HWND hwnd, LPARAM)
{
	wchar_t  WinClass[MAX_PATH] = { 0 };
	GetClassName(hwnd, WinClass, MAX_PATH);
	char* sWinClass = (char *)malloc(MAX_PATH);
	sprintf(sWinClass, "%S", WinClass);
	char* sCLASS_NAME = (char *)malloc(MAX_PATH);
	sprintf(sCLASS_NAME, "%S", CLASS_NAME);
	if (!strcmp(sWinClass, sCLASS_NAME)) Count += 1;
	return TRUE;
}

// Функция подсчета открытых экземпляров данного приложения
int CountWindow()
{
	EnumWindows(CountWindowsProc, 0L);
	return Count;
}

// Главная функция
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR pCmdLine, int nCmdShow)
{
	int windCount = CountWindow(); // количество окон экземпляров программы
	if (windCount == 2)
	{
		MessageBox(nullptr, L"Ошибка при добавлении игрока.\n Уже присоединены двое.", L"Attention",
			MB_OK | MB_ICONASTERISK);
		return 0;
	}

	playerCount = windCount;

	SetupGame(windCount);

	MSG msg;                 // Структура для работы с сообщениями
	prefs = GetProperties(); // Получение начальных настроек программы
	N = prefs->GridSize;

	OpenStorage(L"GameGrid", N * sizeof(int));

    // Определение класса окна
	wc.lpfnWndProc = WindowProc;             // Функция окна
	wc.hInstance = hInstance;                // Идентификатор приложения
	wc.lpszClassName = CLASS_NAME;           // Имя класса окна
	wc.style = CS_HREDRAW | CS_VREDRAW;      // Стиль окна 
	wc.lpszMenuName = NULL;                  // Нет меню
	wc.cbClsExtra = 0;                       // Нет дополнительных данных класса
	wc.cbWndExtra = 0;                       // Нет дополнительных данных окна
	wc.hbrBackground = CreateSolidBrush(prefs->BackgroundColor); // Задний фон окна

	// Регистрация класса окна
	if (!RegisterClass(&wc)) return 0;  

	RECT rect;
	// Клиентская область (внутри рамки) по центру экрана и заданного размера
	rect.left = (GetSystemMetrics(SM_CXSCREEN) - prefs->WindowWidth) / 2;
	rect.top = (GetSystemMetrics(SM_CYSCREEN) - prefs->WindowHeight) / 2;
	rect.right = rect.left + prefs->WindowWidth;
	rect.bottom = rect.top + prefs->WindowHeight;
	
	// Стандартное окно -- имеет заголовок, можно изменить размер и т.д.
	DWORD dwStyle = WS_OVERLAPPEDWINDOW;
	// Вычислим область окна с рамкой. Последний параметр -- наличие меню
	AdjustWindowRect(&rect, dwStyle, FALSE);

	// Создание окна
	HWND hwnd = CreateWindowEx(
		0,                              // Optional window styles.
		CLASS_NAME,                     // Имя класса окна
		WINDOW_NAME,                    // Заголовок окна
		dwStyle,                        // Стиль окна
		rect.left,                  // Горизонтальная позиция окна
		rect.top,                  // Вертикальная позиция окна
		rect.right - rect.left,             // Ширина окна
		rect.bottom - rect.top,            // Высота окна
		NULL,                           // Дескриптор родительского окна  
		NULL,                           // Нет меню
		hInstance,                      // Дескриптор приложения
		NULL                            // Дополнительной информации нет
	);

	ShowWindow(hwnd, nCmdShow);         //Показать окно

	Launch(hwnd);

    // Цикл обработки сообщений
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);         // Функция трансляции кодов нажатой клавиши
		DispatchMessage(&msg);          // Посылает сообщение функции WindowProc()
	}

	if (!FindWindow(NULL, WINDOW_NAME))
		WriteConfigUsingWinApi(SetProperties()); // Запись начальных настроек, только при завершении работы последнего окна с игройы
	DestroyWindow(hwnd);
	UnregisterClass(CLASS_NAME, hInstance);

	Stop();

	return 0;
}

// Оконная функция вызывается операционной системой
// и получает сообщения из очереди для данного приложения
LRESULT CALLBACK WindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	
	static UINT sx, sy;
	int iIndex, jIndex;
	PAINTSTRUCT ps;

	const auto firstKey = 0x31; // клавиша "1"
	const auto lastKey = 0x35; // клавиша "5"
	
	// Обработчик сообщений
	switch (message)
	{
	case WM_RBUTTONDOWN:
	{
		// Если наша очередь
			if (StartTurn())
			{
				GetIndex(lParam, iIndex, jIndex);
				// и выбранная ячейка пуста
				if (SetValue(iIndex, jIndex, field))
				{
					// совершаем ход
					IfClicked(hwnd);
					// и проверяем условия окончания нашей игры
					CheckStatus(N, field);
					// заканчиваем ход и передаем очередь следующему игроку
					FinishTurn();
				}
			}
	}
	break;

	case WM_RBUTTONUP:
	case WM_LBUTTONUP:
		InvalidateRect(hwnd, NULL, TRUE);
		break;

	case WM_MOUSEWHEEL:
	{
		int dir = 1;
		if (GET_WHEEL_DELTA_WPARAM(wParam) / WHEEL_DELTA < 0) {
			dir = -1;
		}
		prefs->GridColor = GetNextColor(prefs->GridColor, dir);
		WriteConfigUsingWinApi(prefs);
		InvalidateRect(hwnd, NULL, TRUE);
	}
	break;

	case WM_KEYDOWN:
	{
		if (GetAsyncKeyState(VK_CONTROL) && GetAsyncKeyState(0x51)) // Ctrl+Q
		{
			DestroyWindow(hwnd);
			break;
		}
		if (wParam == 27) // ESC
		{
			DestroyWindow(hwnd);
			break;
		}
		if (GetAsyncKeyState(VK_SHIFT) && GetAsyncKeyState(67)) // Shift+C
			RunNotepad();
		if (GetAsyncKeyState(VK_RETURN)) // ENTER
		{
			prefs->BackgroundColor = SetBackround(hwnd);
			WriteConfigUsingWinApi(prefs);
			break;
		}
		if (wParam == VK_SPACE) // SPACE
		{
			СhangeState();
			break;
		}
		else if (wParam >= firstKey && wParam <= lastKey) // 1..7
		{
			SetPriority(wParam - firstKey);
		}
		break;
	}
	break;

	case WM_KEYUP:
		break;

	case WM_SIZE:
	{
		sx = LOWORD(lParam);
		sy = HIWORD(lParam);
		prefs->WindowWidth = sx;
		prefs->WindowHeight = sy;
		WriteConfigUsingWinApi(prefs);
	}
	break;

	case WM_PAINT:
	{
		hdc = BeginPaint(hwnd, &ps);

		cellW = sx / N; // получаем ширину ячейки с учетом высоты и длины
		cellH = sy / N; // получаем высоту ячейки с учетом высоты и длины

		Repaint(hdc, cellW, cellH, N, field, prefs->GridColor, crossColor, nullColor);
		EndPaint(hwnd, &ps);
	}
	break;

	case WM_DESTROY:
	{
		CloseStorage();
		PostQuitMessage(0);
	}
	break;

	default:
		// Изменение информации об игровом поле с использованием чтения из файла-отображения
		if (message == WM_FIELDUPDATE)
		{
			int*p = GetStorage();
			for (int i = 0; i < N; i++)
				for (int j = 0; j < N; j++)
					field[i][j] = *(p++);

			InvalidateRect(hwnd, 0, true);
		}
		else 
			// Смена игроков согласно очередности
			if (message == WM_TURNNOTIFY)
			{
				SetTurn(wParam);
			}
		    // Завершение игры
			else if(message==WM_FINISH)
			{
				DestroyWindow(hwnd);
			}
		break;
	}
	return DefWindowProc(hwnd, message, wParam, lParam);
}

// Функция получения индексов ячейки игрового поля по координатам точки окна
void GetIndex(LPARAM lParam, int &ix, int &jy)
{
	int x = LOWORD(lParam);
	int y = HIWORD(lParam);
	ix = x / cellW;
	jy = y / cellH;
}

// Функция запуска блокнота
void RunNotepad(void)
{
	STARTUPINFO sInfo;
	PROCESS_INFORMATION pInfo;
	ZeroMemory(&sInfo, sizeof(STARTUPINFO));
	CreateProcess(L"C:\\Windows\\notepad.exe", NULL, NULL, NULL, FALSE, 0, NULL, NULL, &sInfo, &pInfo);
}

// Функция изменения цвета заднего фона окна
COLORREF SetBackround(HWND hwnd)
{
	COLORREF colorTemp = GetRandomColor();
	hBrush = CreateSolidBrush(colorTemp);
	SetClassLongPtr(hwnd, GCL_HBRBACKGROUND, (LONG)hBrush);
	InvalidateRect(hwnd, NULL, TRUE);
	return colorTemp;
}

// Функция обработки клика по клиентской области
void IfClicked(const HWND hwnd)
{
	SetField(field, N);
	SendNotifyMessage(HWND_BROADCAST, WM_FIELDUPDATE, 0, 0);
}
