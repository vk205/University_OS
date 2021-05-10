#include "GameRules.h"

int ourSign;
int opSign;
bool isOurTurn;

// Функция инициализации фигуры игрока
void SetSign(int n)
{
	if (n == 0)
	{
		ourSign = 1;
		opSign = -1;
	}
	else
	{
		ourSign = -1;
		opSign = 1;
	}
}


// Функция, отвечающая за начало игры
void SetupGame(int n)
{
	SetSign(n);

	if (ourSign == 1)
		isOurTurn = true;
	else 
		isOurTurn = false;
}

// Функция, обрабатывающая ситуацию победы текущего игрока
void GameWon()
{
	if(ourSign==1)
		MessageBox(nullptr, Win1, L"Победа!", MB_OK );
	else
		MessageBox(nullptr, Win2, L"Победа!", MB_OK);
	SendNotifyMessage(HWND_BROADCAST, WM_FINISH, 0, 0);
	PostQuitMessage(0);
}

// Функция, обрабатывающая ситуацию проигрыша текущего игрока
void GameLost()
{
	if (ourSign == 1)
		MessageBox(nullptr, Win2, L"Победа!", MB_OK | MB_ICONASTERISK);
	else
		MessageBox(nullptr, Win1, L"Победа!", MB_OK | MB_ICONASTERISK);
	SendNotifyMessage(HWND_BROADCAST, WM_FINISH, 0, 0);
	PostQuitMessage(0);
}

// Функция, обрабатывающая ситуацию завершения игры ничьей
void Draw()
{
	MessageBox(nullptr, Draw0, L"Ничья!", MB_OK);
	SendNotifyMessage(HWND_BROADCAST, WM_FINISH, 0, 0);
	PostQuitMessage(0);
}

// Функция, анализирующая статус игры
status statsResult(int ours, int ops, int dim)
{
	// Если строка/ столбец/ поле полностью из наших знаков
	if (ours == dim)
	{
		// победа
		return win;
	}
	// Если строка/ столбец/ поле полностью из знаков противника
	if (ops == dim)
	{
		// поражение
		return lose;
	}
	// иначе игра еще в процессе
	return inprocess;
}

// Функция, определяющая статус игры
status GetStatus(int dim, int field[kNMax][kNMax])
{
	int our = 0, op = 0;
	status result;

	{
		// Проверка заполнения всех ячеек
		int count = 0;

		for (int row = 0; row < dim; ++row)
		{
			for (int col = 0; col < dim; ++col)
			{
				if (field[row][col] != 0)
				{
					++count;
				}
			}

		}
		if (count == dim * dim)
		{
			return draw;
		}
		return inprocess;
	}
	
		
		// Проверка строк 
		for (int row = 0; row < dim; ++row)
		{
			int ours = 0;
			int ops = 0;

			for (int col = 0; col < dim; ++col)
			{
				if (field[row][col] == ourSign)
				{
					++ours;
				}
				else if (field[row][col] == opSign)
				{
					++ops;
				}
			}

			result = statsResult(ours, ops, dim);

			if (result != inprocess)
			{
				return result;
			}
		}

	// Проверка колонок
	for (int col = 0; col < dim; ++col)
	{
		int ours = 0;
		int ops = 0;

		for (int row = 0; row < dim; ++row)
		{
			if (field[row][col] == ourSign)
			{
				++ours;
			}
			else if (field[row][col] == opSign)
			{
				++ops;
			}
		}

		result = statsResult(ours, ops, dim);

		if (result != inprocess)
		{
			return result;
		}
	}

	{
		// Проверка главной диагонали
		int ours = 0;
		int ops = 0;
		for (int row = 0; row < dim; ++row)
		{
			const int col = row;
			if (field[row][col] == ourSign)
			{
				++ours;
			}
			else if (field[row][col] == opSign)
			{
				++ops;
			}
		}

		result = statsResult(ours, ops, dim);
		if (result != inprocess)
		{
			return result;
		}
	}

	{
		// Проверка побочной диагонали
		int ours = 0;
		int ops = 0;
		for (int row = 0; row < dim; ++row)
		{
			const int col = dim - row - 1;
			if (field[row][col] == ourSign)
			{
				++ours;
			}
			else if (field[row][col] == opSign)
			{
				++ops;
			}
		}

		result = statsResult(ours, ops, dim);

		if (result != inprocess)
		{
			return result;
		}
	}
}

// Функция проверки условий завершения игры
void CheckStatus(int dim, int field[kNMax][kNMax])
{
	status st = GetStatus(dim, field);
	switch (st)
	{
	case win:
		GameWon();
	case lose:
		GameLost();
	case draw:
		Draw();
	}
}

// Функция, проверки возможности начала хода текущего игрока
bool StartTurn()
{
	if (!isOurTurn)
	{
		MessageBox(nullptr, L"Ход принадлежит другому игроку", L"Внимание",
			MB_OK | MB_ICONASTERISK);
		return false;
	}	
	return true;
}

// Функция, отвечающая за завершение хода текущего игрока
void FinishTurn()
{
	isOurTurn = false;
	// Сообщение о смене очереднности хода
	PostMessage(HWND_BROADCAST, WM_TURNNOTIFY, GetCurrentProcessId(), 0);
}

// Функция, отвечающая за смену очередности
void SetTurn(const WPARAM wParam)
{
	if (wParam != GetCurrentProcessId())
	{
		isOurTurn = true;
	}
}

// Функция, устанавливающая фигуру текущего игрока в ячейку поля
bool SetValue(int iIndex, int jIndex, int field[kNMax][kNMax])
{
	if (field[iIndex][jIndex] != 0)
	{
		MessageBox(nullptr, L"Ячейка уже занята.", L"Ошибка", MB_OK | MB_ICONASTERISK);
		return false;
	}
	field[iIndex][jIndex] = ourSign;
	return true;
}