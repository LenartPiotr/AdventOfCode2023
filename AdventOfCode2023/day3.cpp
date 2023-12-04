#include <iostream>
#include <vector>
#include <string>
#include <sstream>

#include "days.h"

#define isNumber(X) ((X) >= '0' && (X) <= '9')

using namespace std;

bool findNextNumber(string line, int startPos, int& pos, int& size, int& number) {
	auto it = line.begin() + startPos;
	auto end = line.end();
	while (it < end && (*it < '0' || *it > '9')) it++;
	if (it >= end) return false;
	number = 0;
	size = 0;
	pos = it - line.begin();
	while (it < end && *it >= '0' && *it <= '9') {
		number *= 10;
		number += *it - '0';
		size++;
		it++;
	}
	return true;
}

int getNumber(string line, int pos) {
	int startPos = pos;
	int endPos = pos;
	while (startPos >= 0 && isNumber(line[startPos])) startPos--;
	while (endPos < line.size() && isNumber(line[endPos])) endPos++;
	startPos++;
	endPos--;
	int number = 0;
	for (int i = startPos; i <= endPos; i++) {
		number *= 10;
		number += line[i] - '0';
	}
	return number;
}

void day3() {
	vector<string> lines = readLinesFromFile("./data/day3.txt");
	int width = lines[0].size();
	int height = lines.size();
	
	int pos, size, number;
	bool hasSign, top, left, right, bottom;
	int sum = 0;

	for (int l = 0; l < height; l++) {
		string line = lines[l];
		pos = 0;
		size = 0;
		while (findNextNumber(line, pos + size, pos, size, number)) {
			hasSign = false;
			top = l != 0;
			bottom = l != height - 1;
			left = pos != 0;
			right = pos + size != width;
			if (top) {
				auto it = lines[l - 1].begin() + pos;
				auto end = it + size;
				if (left) it--;
				if (right) end++;
				for (; it < end; it++) {
					if (!isNumber(*it) && *it != '.') {
						hasSign = true;
						break;
					}
				}
			}
			if (bottom) {
				auto it = lines[l + 1].begin() + pos;
				auto end = it + size;
				if (left) it--;
				if (right) end++;
				for (; it < end; it++) {
					if (!isNumber(*it) && *it != '.') {
						hasSign = true;
						break;
					}
				}
			}
			if (!hasSign && left && !isNumber(line[pos - 1]) && line[pos - 1] != '.') {
				hasSign = true;
			}
			if (!hasSign && right && !isNumber(line[pos + size]) && line[pos + size] != '.') {
				hasSign = true;
			}
			if (hasSign) {
				sum += number;
			}
		}
	}
	cout << sum << endl;

	// part 2
	sum = 0;
	for (int y = 0; y < height; y++) {
		string line = lines[y];
		bool top = y != 0;
		bool bottom = y != height - 1;
		for (int x = 0; x < width; x++) {
			if (line[x] != '*') continue;
			bool left = x != 0;
			bool right = x != width - 1;
			int numbersCount = 0;
			int numbersProduct = 1;
			if (left && isNumber(line[x - 1])) {
				numbersCount++;
				numbersProduct *= getNumber(line, x - 1);
			}
			if (right && isNumber(line[x + 1])) {
				numbersCount++;
				numbersProduct *= getNumber(line, x + 1);
			}
			if (top) {
				if (isNumber(lines[y - 1][x])) {
					numbersCount++;
					numbersProduct *= getNumber(lines[y - 1], x);
				}
				else {
					if (left && isNumber(lines[y - 1][x - 1])) {
						numbersCount++;
						numbersProduct *= getNumber(lines[y - 1], x - 1);
					}
					if (right && isNumber(lines[y - 1][x + 1])) {
						numbersCount++;
						numbersProduct *= getNumber(lines[y - 1], x + 1);
					}
				}
			}
			if (bottom) {
				if (isNumber(lines[y + 1][x])) {
					numbersCount++;
					numbersProduct *= getNumber(lines[y + 1], x);
				}
				else {
					if (left && isNumber(lines[y + 1][x - 1])) {
						numbersCount++;
						numbersProduct *= getNumber(lines[y + 1], x - 1);
					}
					if (right && isNumber(lines[y + 1][x + 1])) {
						numbersCount++;
						numbersProduct *= getNumber(lines[y + 1], x + 1);
					}
				}
			}
			if (numbersCount == 2) {
				sum += numbersProduct;
			}
		}
	}

	cout << sum << endl;
}