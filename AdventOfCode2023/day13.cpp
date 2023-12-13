#include <iostream>
#include <vector>
#include <functional>
#include <string>
#include <sstream>
#include <algorithm>

#include "days.h"
#include "hash.cpp"

using namespace std;

inline bool checkBit(int bits) {
	return bits && !(bits & (bits - 1));
}

inline bool missingBit(int a, int b) {
	return checkBit(a ^ b);
}

void decomposition(vector<string> image, int* collumns, int* rows) {
	int value;
	for (int x = 0; x < image[0].length(); x++) {
		collumns[x] = 0;
	}
	for (int y = 0; y < image.size(); y++) {
		rows[y] = 0;
		for (int x = 0; x < image[y].length(); x++) {
			value = image[y][x] == '#' ? 1 : 0;
			rows[y] *= 2;
			rows[y] += value;
			collumns[x] *= 2;
			collumns[x] += value;
		}
	}
}

bool checkReflection(const int* values, const int& size, int position, bool mistakes = false) {
	const int* start = values - 1;
	const int* end = values + size;
	const int* itBack = values + position;
	const int* itFor = values + position + 1;
	int miss = 0;
	while (itBack != start && itFor != end) {
		if (*itFor != *itBack) {
			if (missingBit(*itFor, *itBack))
				miss++;
			else
				return false;
		}
		itFor++;
		itBack--;
	}
	if (mistakes) {
		return (miss == 1);
	}
	return (miss == 0);
}

int detectReflection(vector<string> image, bool mistakes = false) {
	int height = image.size();
	int width = image[0].size();
	int* collumns = new int[width];
	int* rows = new int[height];
	decomposition(image, collumns, rows);

	/*
	for (int x = 0; x < width; x++) {
		cout << collumns[x] << " ";
	}
	cout << endl;
	for (int y = 0; y < height; y++) {
		cout << rows[y] << " ";
	}
	cout << endl;
	/**/
	
	int value = collumns[0];
	for (int x = 1; x < width; x++) {
		if (value == collumns[x] || (mistakes && missingBit(value, collumns[x])))
			if (checkReflection(collumns, width, x / 2, mistakes))
				return (x / 2) + 1;
	}
	value = collumns[width - 1];
	for (int x = 0; x < width - 1; x++) {
		if (value == collumns[x] || (mistakes && missingBit(value, collumns[x])))
			if (checkReflection(collumns, width, (x + width - 1) / 2, mistakes))
				return (x + width - 1) / 2 + 1;
	}
	value = rows[0];
	for (int y = 1; y < height; y++) {
		if (value == rows[y] || (mistakes && missingBit(value, rows[y])))
			if (checkReflection(rows, height, y / 2, mistakes))
				return ((y / 2) + 1) * 100;
	}
	value = rows[height - 1];
	for (int y = 0; y < height - 1; y++) {
		if (value == rows[y] || (mistakes && missingBit(value, rows[y])))
			if (checkReflection(rows, height, (y + height - 1) / 2, mistakes))
				return (((y + height - 1) / 2) + 1) * 100;
	}
	return -100000;
}

void day13() {
	vector<string> lines = readLinesFromFile("./data/day13.txt");
	
	vector<string> image;
	int sum = 0, sum2 = 0;
	for (int i = 0; i < lines.size(); i++) {
		if (lines[i].length() == 0) {
			sum += detectReflection(image);
			sum2 += detectReflection(image, true);
			vector<string> newImage;
			image = newImage;
		}
		else {
			image.push_back(lines[i]);
		}
	}
	sum += detectReflection(image);
	sum2 += detectReflection(image, true);
	cout << sum << endl;
	cout << sum2 << endl;
}