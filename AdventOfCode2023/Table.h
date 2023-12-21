#include <iostream>

#pragma once

template <typename T>
class Table
{
public:
	T** tab;
	int width;
	int height;

	Table(int w, int h, T def);
	~Table();

	//template<typename T2>
	//friend std::ostream& operator<<(std::ostream& os, const Table<T2>& t);
};