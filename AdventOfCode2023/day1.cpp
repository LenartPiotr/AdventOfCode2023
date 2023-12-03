#include <iostream>
#include <vector>
#include <string>

#include "days.h"

using namespace std;

void day1() {
	vector<string> lines = readLinesFromFile("./data/day1.txt");

	// part 1
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

	// part 2
	sum = 0;
	int firstDigit;
	int firstDigitPos;
	int lastDigit;
	int lastDigitPos;
	int pos;
	bool found;
	const char* begin;
	const char* end;
	string spelledDigits[10] = { "zero", "one", "two", "three", "four", "five", "six", "seven", "eight", "nine" };
	for (int i = 0; i < lines.size(); i++) {
		string line = lines[i];
		firstDigit = 0;
		firstDigitPos = line.size();
		lastDigit = 0;
		lastDigitPos = 0;
		for (int j = 0; j < 10; j++) {
			pos = line.find(spelledDigits[j]);
			if (pos < firstDigitPos && pos != -1) {
				firstDigitPos = pos;
				firstDigit = j;
			}
			pos = line.rfind(spelledDigits[j]);
			if (pos > lastDigitPos && pos != -1) {
				lastDigitPos = pos;
				lastDigit = j;
			}
		}
		begin = line.c_str();
		end = line.c_str() + firstDigitPos;
		found = false;
		for (const char* iterator = begin; iterator < end; iterator++) {
			if (*iterator >= '0' && *iterator <= '9') {
				sum += (*iterator - '0') * 10;
				found = true;
				break;
			}
		}
		if (!found) {
			sum += firstDigit * 10;
		}
		begin = line.c_str() + lastDigitPos;
		end = line.c_str() + line.size();
		found = false;
		for (const char* iterator = end - 1; iterator >= begin; iterator--) {
			if (*iterator >= '0' && *iterator <= '9') {
				sum += *iterator - '0';
				found = true;
				break;
			}
		}
		if (!found) {
			sum += lastDigit;
		}
	}
	cout << sum << endl;
}