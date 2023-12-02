#include <iostream>
#include <vector>
#include <string>

#include "days.h"

using namespace std;

void day1() {
	vector<string> lines = readLinesFromFile("./data/day1.txt");
	int sum = 0;
	for (int i = 0; i < lines.size(); i++) {
		string line = lines[i];
		const char* begin = line.c_str();
		const char* end = line.c_str() + line.size();
		for (const char* iterator = begin; iterator < end; iterator++) {
			if (*iterator >= '0' && *iterator <= '9') {
				sum += (*iterator - '0') * 10;
				break;
			}
		}
		for (const char* iterator = end - 1; iterator >= begin; iterator--) {
			if (*iterator >= '0' && *iterator <= '9') {
				sum += *iterator - '0';
				break;
			}
		}
	}
	cout << sum << endl;
}