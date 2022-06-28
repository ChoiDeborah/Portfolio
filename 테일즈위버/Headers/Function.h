#pragma once

#ifndef __FUNCTION_H__

template <typename T>
void SafeDelete(T& ptr)
{
	if (ptr)
	{
		delete ptr;
		ptr = nullptr;
	}
}

template <typename T>
void SafeDelete_Array(T& ptr)
{
	if (ptr)
	{
		delete[] ptr;
		ptr = nullptr;
	}
}

//bool isDigit(wstring str) {
//	return _wtoi(str.c_str()) != 0 || str.compare(L"0") == 0;
//}

#define __FUNCTION_H__
#endif