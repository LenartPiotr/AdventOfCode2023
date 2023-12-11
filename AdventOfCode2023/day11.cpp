#include <iostream>
#include <vector>
#include <array>
#include <string>
#include <sstream>
#include <algorithm>

#include "days.h"

using namespace std;

template <typename T>
T expandAndMesureDistances(const vector<string>& lines, T expandValue) {
	int width = lines[0].length();
	int height = lines.size();

	expandValue--;

	vector<pair<T, T>> galaxies;

	bool* hasCollumnGalaxies = new bool[width];
	bool* hasRowGalaxies = new bool[height];
	T* additionalHorizontalMovement = new T[width];
	T* additionalVerticalMovement = new T[height];

	for (int x = 0; x < width; x++) {
		hasCollumnGalaxies[x] = false;
		additionalHorizontalMovement[x] = false;
	}
	for (int y = 0; y < width; y++) {
		hasRowGalaxies[y] = false;
		additionalVerticalMovement[y] = false;
	}

	{
		int y = 0, x = 0;
		for (auto& line : lines) {
			x = 0;
			for (auto& c : line) {
				if (c == '#') {
					galaxies.push_back({ x,y });
					hasCollumnGalaxies[x] = true;
					hasRowGalaxies[y] = true;
				}
				x++;
			}
			y++;
		}
	}

	T addon = 0;
	for (int x = 0; x < width; x++) {
		if (!hasCollumnGalaxies[x]) addon += expandValue;
		additionalHorizontalMovement[x] = addon;
	}
	addon = 0;
	for (int y = 0; y < height; y++) {
		if (!hasRowGalaxies[y]) addon += expandValue;
		additionalVerticalMovement[y] = addon;
	}

	for (auto& point : galaxies) {
		point.first += additionalHorizontalMovement[point.first];
		point.second += additionalVerticalMovement[point.second];
	}

	T sum = 0;
	for (auto& g1 : galaxies) {
		for (auto& g2 : galaxies) {
			if (g1 != g2) sum += abs(g2.first - g1.first) + abs(g2.second - g1.second);
		}
	}

	delete[] hasCollumnGalaxies;
	delete[] hasRowGalaxies;
	delete[] additionalHorizontalMovement;
	delete[] additionalVerticalMovement;

	return sum / 2;
}

void day11() {
	vector<string> lines = readLinesFromFile("./data/day11.txt");
	cout << expandAndMesureDistances<int>(lines, 2) << endl;
	cout << expandAndMesureDistances<long long>(lines, 10) << endl;
	cout << expandAndMesureDistances<long long>(lines, 100) << endl;
	cout << expandAndMesureDistances<long long>(lines, 1000000) << endl;
}