#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>

#include "days.h"
#include "hash.cpp"

using namespace std;

int moveTopCoundWeight(vector<string> lines) {
	int width = lines[0].length();
	int height = lines.size();
	int* nextPossible = new int[width];
	int sum = 0;
	for (int x = 0; x < width; x++) {
		nextPossible[x] = 0;
	}
	for (int y = 0; y < height; y++) {
		string line = lines[y];
		for (int x = 0; x < width; x++) {
			char c = line[x];
			switch (c) {
			case '#':
				nextPossible[x] = y + 1;
				break;
			case 'O':
				sum += height - nextPossible[x];
				nextPossible[x]++;
				break;
			}
		}
	}
	delete[] nextPossible;
	return sum;
}

int coundWeight(vector<string> lines) {
	int width = lines[0].length();
	int height = lines.size();
	int sum = 0;
	for (int y = 0; y < height; y++) {
		string line = lines[y];
		for (int x = 0; x < width; x++) {
			if (line[x] == 'O') sum += height - y;
		}
	}
	return sum;
}

vector<string> cycle(const vector<string>& lines) {
	vector<string> output;
	for (const string& str : lines) {
		output.push_back(string(str));
	}
	int width = lines[0].length();
	int height = lines.size();
	int* nextPossible = new int[width];

	// up
	for (int x = 0; x < width; x++) {
		nextPossible[x] = 0;
	}
	for (int y = 0; y < height; y++) {
		string& line = output[y];
		for (int x = 0; x < width; x++) {
			char& c = line[x];
			switch (c) {
			case '#':
				nextPossible[x] = y + 1;
				break;
			case 'O':
				c = '.';
				output[nextPossible[x]][x] = 'O';
				nextPossible[x]++;
				break;
			}
		}
	}

	// left
	for (int y = 0; y < height; y++) {
		string& line = output[y];
		int next = 0;
		for (int x = 0; x < width; x++) {
			char& c = line[x];
			switch (c) {
			case '#':
				next = x + 1;
				break;
			case 'O':
				c = '.';
				line[next] = 'O';
				next++;
				break;
			}
		}
	}

	// down
	for (int x = 0; x < width; x++) {
		nextPossible[x] = height - 1;
	}
	for (int y = height - 1; y >= 0; y--) {
		string& line = output[y];
		for (int x = 0; x < width; x++) {
			char& c = line[x];
			switch (c) {
			case '#':
				nextPossible[x] = y - 1;
				break;
			case 'O':
				c = '.';
				output[nextPossible[x]][x] = 'O';
				nextPossible[x]--;
				break;
			}
		}
	}

	// right
	for (int y = 0; y < height; y++) {
		string& line = output[y];
		int next = width - 1;
		for (int x = width - 1; x >= 0; x--) {
			char& c = line[x];
			switch (c) {
			case '#':
				next = x - 1;
				break;
			case 'O':
				c = '.';
				line[next] = 'O';
				next--;
				break;
			}
		}
	}
	delete[] nextPossible;

	return output;
}

void print(vector<string> lines) {
	for (string& line : lines) {
		cout << line << endl;
	}
	cout << endl;
}

bool same(vector<string> a, vector<string> b) {
	for (auto ia = a.begin(), ib = b.begin(); ia != a.end() && ib != b.end(); ia++, ib++) {
		if (*ia != *ib) return false;
	}
	return true;
}

void day14() {
	vector<string> lines = readLinesFromFile("./data/day14.txt");

	cout << moveTopCoundWeight(lines) << endl;

	vector<vector<string>> history;

	vector<string> current = lines;
	int hi;
	int cyclePeriod = -1; // 58
	int periodStart = 0;
	for (int i = 0; i < 10000; i++) {
		history.push_back(current);
		current = cycle(current);
		hi = 0;
		for (const auto& h : history) {
			if (same(current, h)) {
				cyclePeriod = i + 1 - hi;
				periodStart = hi;
				goto cycleFound;
			}
			hi++;
		}
	}
	cout << "No cycle found!" << endl;
	return;

cycleFound:
	cout << "Found cycle with period " << cyclePeriod << " at start " << periodStart << endl;
	
	int endCycle = 1000000000;
	int beginCycle = endCycle % cyclePeriod;
	while (beginCycle < periodStart) beginCycle += cyclePeriod;

	cout << coundWeight(history[beginCycle]) << endl;
}