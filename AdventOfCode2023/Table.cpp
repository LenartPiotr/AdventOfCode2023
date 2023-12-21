#include "Table.h"

template<typename T>
Table<T>::Table(int w, int h, T def)
{
	this->tab = new T * [w];
	for (int x = 0; x < w; x++) {
		this->tab[x] = new T[h];
		for (int y = 0; y < h; y++) this->tab[x][y] = def;
	}
	this->width = w;
	this->height = h;
}

template<typename T>
Table<T>::~Table()
{
	for (int x = 0; x < this->width; x++) delete[] this->tab[x];
	delete[] this->tab;
}

/*template<typename T>
std::ostream& operator<<(std::ostream& os, const Table<T>& t)
{
	os << "abc";
	for (int y = 0; y < t.height; y++) {
		for (int x = 0; x < t.width; x++) {
			os << t.tab[x][y];
		}
		os << std::endl;
	}
	return os;
}*/
