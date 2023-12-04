#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>

#include "days.h"

using namespace std;

int sameNumber(vector<int> tab1, vector<int> tab2) {
	int same = 0;
	for (int i = 0, j = 0, max1 = tab1.size(), max2 = tab2.size(); i < max1 && j < max2; tab1[i] < tab2[j] ? i++ : j++) {
		if (tab1[i] == tab2[j]) same++;
	}
	return same;
}

void day4() {
	vector<string> lines = readLinesFromFile("./data/day4.txt");
	vector<vector<int>> winning;
	vector<vector<int>> chosen;
	vector<int> cards;
	int size = lines.size();

	string word;
	int num;
	int sum = 0;
	char c;

	for (string line : lines) {
		istringstream stream(line);
		stream >> word >> word; // Card, number, colon & space
		vector<int> win;
		vector<int> choose;
		cards.push_back(1);
		while (stream >> num) win.push_back(num);
		if (stream.fail()) {
			stream.ignore();
			stream.clear();
		}
		stream >> word;
		while (stream >> num) choose.push_back(num);
		sort(win.begin(), win.end());
		sort(choose.begin(), choose.end());
		winning.push_back(win);
		chosen.push_back(choose);
	}

	int same;
	for (int i = 0; i < size; i++) {
		same = sameNumber(winning[i], chosen[i]);
		if (same == 0) continue;
		num = 1;
		for (int j = 1; j < same; j++) num *= 2;
		sum += num;
	}
	cout << sum << endl;

	// part 2
	sum = 0;
	int count;
	for (int i = 0; i < size; i++) {
		same = sameNumber(winning[i], chosen[i]);
		count = cards[i];
		sum += count;
		for (int j = 0; j < same && i + j + 1 < size; j++) {
			cards[i + j + 1] += count;
		}
	}
	cout << sum << endl;
}