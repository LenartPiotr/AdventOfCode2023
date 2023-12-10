#include <iostream>
#include <vector>
#include <array>
#include <string>
#include <sstream>
#include <algorithm>

#include "days.h"

using namespace std;

inline bool inRange(int x, int y, int w, int h) {
	return x >= 0 && x < w && y >= 0 && y < h;
}

// directions: 1 - top, 2 - right, 4 - bottom, 8 - left
int getDirections(char c) {
	switch (c)
	{
	case '|': return 1 | 4;
	case '-': return 2 | 8;
	case 'L': return 1 | 2;
	case 'F': return 2 | 4;
	case '7': return 4 | 8;
	case 'J': return 8 | 1;
	default: return 0;
	}
}

inline int oppositeDir(int dir) {
	return (dir & 5 ? 5 : 10) - dir;
}

int getLoopSize(const vector<string>& table, const int& startX, const int& startY, bool** mainLoop) {
	int width = table[0].length();
	int height = table.size();

	int startDirection = 0;
	if (inRange(startX + 1, startY, width, height) && (getDirections(table[startY][startX + 1]) & 8)) startDirection = 2;
	else if (inRange(startX - 1, startY, width, height) && (getDirections(table[startY][startX - 1]) & 2)) startDirection = 8;
	else startDirection = 1;

	char currentLetter = 'S';
	int x = startX;
	int y = startY;
	int direction = startDirection;
	int steps = 0;
	do {
		mainLoop[y][x] = true;
		x += direction & 2 ? 1 : direction & 8 ? -1 : 0;
		y += direction & 4 ? 1 : direction & 1 ? -1 : 0;
		currentLetter = table[y][x];
		steps++;
		direction = getDirections(currentLetter) & (15 ^ oppositeDir(direction));
	} while (currentLetter != 'S');
	return steps;
}

int countInside(const vector<string>& table, const int& startX, const int& startY, bool** mainLoop) {
	int width = table[0].length();
	int height = table.size();

	// Determine start orientation
	int startDirection = 0;
	if (inRange(startX + 1, startY, width, height) && (getDirections(table[startY][startX + 1]) & 8)) startDirection |= 2;
	if (inRange(startX - 1, startY, width, height) && (getDirections(table[startY][startX - 1]) & 2)) startDirection |= 8;
	if (inRange(startX, startY + 1, width, height) && (getDirections(table[startY + 1][startX]) & 1)) startDirection |= 4;
	if (inRange(startX, startY - 1, width, height) && (getDirections(table[startY - 1][startX]) & 4)) startDirection |= 1;

	int sum = 0;
	bool outside;
	int dir;
	int x = 0;
	int y = 0;
	for (auto& line : table) {
		outside = true;
		x = 0;
		for (auto& c : line) {
			dir = getDirections(c);
			if (mainLoop[y][x]) {
				if (dir & 1) outside = !outside;
				else if (c == 'S' && startDirection & 1) outside = !outside;
			}
			else if (!outside) sum++;
			x++;
		}
		y++;
	}
	return sum;
}

void day10() {
	vector<string> lines = readLinesFromFile("./data/day10.txt");
	int width = lines[0].length();
	int height = lines.size();
	int startX, startY;

	{
		bool foundStart = false;
		for (int y = 0; y < height; y++) {
			for (int x = 0; x < width; x++) {
				if (lines[y][x] == 'S') {
					startX = x;
					startY = y;
					foundStart = true;
					break;
				}
			}
			if (foundStart) break;
		}
		if (!foundStart) {
			cerr << "No start found!" << endl;
			exit(1);
		}
	}

	bool** mainLoop = new bool* [height];
	for (int y = 0; y < height; y++) {
		mainLoop[y] = new bool[width];
		for (int x = 0; x < width; x++) mainLoop[y][x] = false;
	}

	cout << getLoopSize(lines, startX, startY, mainLoop) / 2 << endl;

	cout << countInside(lines, startX, startY, mainLoop) << endl;
}