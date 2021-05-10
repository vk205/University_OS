#include <Windows.h>
#include <sstream> // для потокового ввода и вывода
#include <iomanip>
#include "Properties.h"

Properties* SetProperties()
{
	Properties* prefs = new Properties;
	prefs->WindowWidth = 320;
	prefs->WindowHeight = 280;
	prefs->GridSize = 3;
	prefs->BackgroundColor = 0xff0000;
	prefs->GridColor = 0x0000ff;
	return prefs;
}

Properties* StringToProperties(const TCHAR* content)
{
	Properties* prefs = new Properties;
	std::wistringstream is(content); // создание "строкового входного потока данных" 
	const std::ios_base::fmtflags f(is.flags()); // определение внешнего вида выходных данных

	is >> prefs->WindowWidth
		>> prefs->WindowHeight
		>> prefs->GridSize
		>> std::hex >> prefs->BackgroundColor
		>> std::hex >> prefs->GridColor;
	is.flags(f);

	while (is.peek() == '\r' || is.peek() == '\n')
	{
		is.ignore();
	}

	return prefs;
}

TCHAR* PropertiesToString(const Properties* prefs)
{
	std::wostringstream os; // создание "строкового выходного потока данных" 
	const std::ios_base::fmtflags f(os.flags()); // определение внешнего вида выходных данных

	COLORREF BgColor = prefs->BackgroundColor;
	COLORREF GrColor = prefs->GridColor;

	os << prefs->WindowWidth << ' '
		<< prefs->WindowHeight << '\n'
		<< prefs->GridSize << '\n'
		<< std::hex << std::setfill(L'0') << std::setw(6)
		<< BgColor << '\n'
		<< GrColor << '\n';

	os.flags(f);

	os << '\0';
	const UINT kLen = os.tellp();
	TCHAR* buffer = new TCHAR[kLen];
	wcscpy_s(buffer, kLen, os.str().c_str()); // извлечение исходной строки из потока
	return buffer;
}

