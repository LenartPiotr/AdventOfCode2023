#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>

#include "days.h"

#define isDigit(X) ((X) >= '0' && (X) <= '9')

using namespace std;

struct card {
	string txt;
	int bid;
	int strength;
	int value;
	int power;
};

int getValue(char c) {
	switch (c) {
	case 'A': return 12;
	case 'K': return 11;
	case 'Q': return 10;
	case 'J': return 9;
	case 'T': return 8;
	}
	return c - '0' - 2;
}

int getValue(string s) {
	int base = 1;
	int sum = 0;
	for (auto c = s.rbegin(); c != s.rend(); c++) {
		sum += getValue(*c) * base;
		base *= 13;
	}
	return sum;
}

int getStrength(string c) {
	string c2(c);
	sort(c2.begin(), c2.end());
	int sum = 0;
	int countInRow = 0;
	char lastChar = '-';
	for (char c : c2) {
		if (lastChar == c) {
			countInRow++;
			continue;
		}
		sum += countInRow * countInRow;
		countInRow = 1;
		lastChar = c;
	}
	sum += countInRow * countInRow;
	return sum;
}

int getValuePart2(char c) {
	switch (c) {
	case 'A': return 12;
	case 'K': return 11;
	case 'Q': return 10;
	case 'J': return 0;
	case 'T': return 9;
	}
	return c - '0' - 1;
}

int getValuePart2(string s) {
	int base = 1;
	int sum = 0;
	for (auto c = s.rbegin(); c != s.rend(); c++) {
		sum += getValuePart2(*c) * base;
		base *= 13;
	}
	return sum;
}

int getStrengthPart2(string c) {
	string c2(c);
	sort(c2.begin(), c2.end());
	vector<int> uniquesCount;
	int sum = 0;
	int countInRow = 0;
	int jockers = 0;
	char lastChar = '-';
	for (char c : c2) {
		if (c == 'J') {
			jockers++;
			continue;
		}
		if (lastChar == c) {
			countInRow++;
			continue;
		}
		uniquesCount.push_back(countInRow);
		countInRow = 1;
		lastChar = c;
	}
	uniquesCount.push_back(countInRow);
	if (jockers == 0) {
		for (int i : uniquesCount) sum += i * i;
		return sum;
	}
	sort(uniquesCount.begin(), uniquesCount.end(), greater<int>());
	uniquesCount[0] += jockers;
	for (int i : uniquesCount) sum += i * i;
	return sum;
}

void day7() {
	vector<string> lines = readLinesFromFile("./data/day7.txt");
	vector<card> cards;
	string str;
	int num;

	// part 1

	for (string line : lines) {
		istringstream stream(line);
		card c;
		stream >> c.txt >> c.bid;
		c.strength = getStrength(c.txt);
		c.value = getValue(c.txt);
		c.power = c.strength * 371293 + c.value;
		cards.push_back(c);
	}
	sort(cards.begin(), cards.end(), [](const card& c1, const card& c2) {return c1.power < c2.power; });
	int sum = 0;
	for (int i = 0; i < cards.size(); i++) {
		sum += (i + 1) * cards[i].bid;
	}
	cout << sum << endl;

	// part 2
	for (auto& c : cards) {
		c.strength = getStrengthPart2(c.txt);
		c.value = getValuePart2(c.txt);
		c.power = c.strength * 371293 + c.value;
	}
	sort(cards.begin(), cards.end(), [](const card& c1, const card& c2) {return c1.power < c2.power; });
	sum = 0;
	for (int i = 0; i < cards.size(); i++) {
		sum += (i + 1) * cards[i].bid;
	}
	cout << sum << endl;
}