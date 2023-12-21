#include <iostream>
#include <functional>

#pragma once

template <typename T>
class Table
{
public:
	T** tab;
	int width;
	int height;

	Table(int w, int h, T def) {
		this->tab = new T * [w];
		for (int x = 0; x < w; x++) {
			this->tab[x] = new T[h];
			for (int y = 0; y < h; y++) this->tab[x][y] = def;
		}
		this->width = w;
		this->height = h;
	}
	~Table() {
		for (int x = 0; x < this->width; x++) delete[] this->tab[x];
		delete[] this->tab;
	}

	inline bool inRange(int x, int y) {
		return x >= 0 && x < this->width && y >= 0 && y < this->height;
	}

	template <typename Function>
	void forEach(Function f) {
		for (int x = 0; x < this->width; x++)
		{
			for (int y = 0; y < this->height; y++)
			{
				f(this->tab[x][y]);
			}
		}
	}

	friend std::ostream& operator<<(std::ostream& os, const Table<T>& t) {
		for (int y = 0; y < t.height; y++) {
			for (int x = 0; x < t.width; x++) {
				if (x != 0) os << " ";
				os << t.tab[x][y];
			}
			os << std::endl;
		}
		return os;
	}
	T* operator[](int rowIndex) {
		return tab[rowIndex];
	}

	T& operator()(int rowIndex, int colIndex) {
		return tab[rowIndex][colIndex];
	}
};