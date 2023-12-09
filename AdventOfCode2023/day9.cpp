#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>

#include "days.h"

#define isDigit(X) ((X) >= '0' && (X) <= '9')

using namespace std;

int predict(vector<int> v) {
	vector<int> numbers(v);
	vector<int> distances;
	vector<int> lastNums;
	bool onlyZeros = false;
	while (!onlyZeros) {
		onlyZeros = true;
		for (int i = 1; i < numbers.size(); i++) {
			int diff = numbers[i] - numbers[i - 1];
			distances.push_back(diff);
			if (diff != 0) onlyZeros = false;
		}
		lastNums.push_back(numbers.back());
		numbers.swap(distances);
		distances.clear();
	}
	int value = 0;
	for (int i = lastNums.size() - 1; i >= 0; i--) {
		value = lastNums[i] + value;
	}
	return value;
}

int predictBackwards(vector<int> v) {
	vector<int> numbers(v);
	vector<int> distances;
	vector<int> firstNums;
	bool onlyZeros = false;
	while (!onlyZeros) {
		onlyZeros = true;
		for (int i = 1; i < numbers.size(); i++) {
			int diff = numbers[i] - numbers[i - 1];
			distances.push_back(diff);
			if (diff != 0) onlyZeros = false;
		}
		firstNums.push_back(numbers[0]);
		numbers.swap(distances);
		distances.clear();
	}
	int value = 0;
	for (int i = firstNums.size() - 1; i >= 0; i--) {
		value = firstNums[i] - value;
	}
	return value;
}

void day9() {
	vector<string> lines = readLinesFromFile("./data/day9.txt");
	vector<vector<int>> values;
	int num;
	for (string line : lines) {
		istringstream stream(line);
		vector<int> v;
		while (stream >> num) v.push_back(num);
		values.push_back(v);
	}
	int sum = 0;
	for (const auto& v : values) {
		sum += predict(v);
	}
	cout << sum << endl;
	sum = 0;
	for (const auto& v : values) {
		sum += predictBackwards(v);
	}
	cout << sum << endl;
}