#include <Windows.h>

#include "Utils.h"
#include <string>

void DebugOut(const char* szFormat, ...)
{
    char szBuff[1024];
    va_list arg;
    va_start(arg, szFormat);
    _vsnprintf_s(szBuff, sizeof(szBuff), szFormat, arg);
    va_end(arg);

    OutputDebugString(szBuff);
}
//
vector<string> split(string line, string delimeter)
{
	vector<string> tokens;
	size_t last = 0; size_t next = 0;
	while ((next = line.find(delimeter, last)) != string::npos)
	{
		tokens.push_back(line.substr(last, next - last));
		last = next + 1;
	}
	tokens.push_back(line.substr(last));

	return tokens;
}
//
//
//char * string to wchar_t* string.
//*/
//wstring ToWSTR(string st)
//{
//	const char *str = st.c_str();
//
//	size_t newsize = strlen(str) + 1;
//	wchar_t * wcstring = new wchar_t[newsize];
//	size_t convertedChars = 0;
//	mbstowcs_s(&convertedChars, wcstring, newsize, str, _TRUNCATE);
//
//	wstring wstr(wcstring);
//
//	// delete wcstring   // << can I ? 
//	return wstr;
//}
//
//
//	Convert char* string to wchar_t* string.
//*/
//LPCWSTR ToLPCWSTR(string st)
//{
//	const char *str = st.c_str();
//
//	size_t newsize = strlen(str) + 1;
//	wchar_t * wcstring = new wchar_t[newsize];
//	size_t convertedChars = 0;
//	mbstowcs_s(&convertedChars, wcstring, newsize, str, _TRUNCATE);
//
//	wstring *w = new wstring(wcstring);
//
//	// delete wcstring   // << can I ? 
//	return w->c_str();
//}

bool CheckIfBound(Rect b1, Rect b2)
{
	if (b2.left > b1.right || b2.top > b1.bottom
		|| b2.bottom < b1.top || b2.right < b1.left)
		return false;
	return true;
}