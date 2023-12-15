#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>

#include "days.h"
#include "hash.cpp"

using namespace std;

int hashWord(string w) {
	int h = 0;
	for (const char& c : w) {
		h += c;
		h *= 17;
		h %= 256;
	}
	return h;
}

int hashAll(vector<string> lines) {
	int sum = 0;
	int h = 0;
	for (const string& l : lines) for (const char& c : l) {
		if (c == ',') {
			sum += h;
			h = 0;
		}
		else {
			h += c;
			h *= 17;
			h %= 256;
		}
	}
	return sum + h;
}

int processTheHolidayASCIIStringHelperManualArrangementProcedure(vector<string> lines) {
	// HASHMAP for short
	vector<vector<pair<string, int>>> boxes;
	for (int i = 0; i < 256; i++) boxes.push_back({});
	string code;
	int num = 0;
	bool enterNumber = false;

	for (const string& l : lines) for (const char& c : l) {
		if (c == '-') {
			int h = hashWord(code);
			auto& box = boxes[h];
			box.erase(remove_if(box.begin(), box.end(), [&code](const pair<string, int>& lens) { return (lens.first == code); }), box.end());
			continue;
		}
		if (c == '=') {
			num = 0;
			enterNumber = true;
			continue;
		}
		if (c == ',') {
			if (!enterNumber) {
				code = "";
				continue;
			}
			int h = hashWord(code);
			auto& box = boxes[h];
			auto it = find_if(box.begin(), box.end(), [&code](const pair<string, int>& lens) { return lens.first == code; });
			if (it == box.end()) box.push_back({ code, num });
			else it->second = num;
			enterNumber = false;
			code = "";
			continue;
		}
		if (enterNumber) {
			num *= 10;
			num += c - '0';
			continue;
		}
		code.append({ c });
	}
	if (enterNumber) {
		int h = hashWord(code);
		auto& box = boxes[h];
		auto it = find_if(box.begin(), box.end(), [&code](const pair<string, int>& lens) { return lens.first == code; });
		if (it == box.end()) box.push_back({ code, num });
		else it->second = num;
	}

	int sum = 0;
	for (int i = 0; i < 256; i++) {
		auto& box = boxes[i];
		for (int j = 0; j < box.size(); j++) {
			sum += (i + 1) * (j + 1) * box[j].second;
		}
	}
	return sum;
}

void day15() {
	vector<string> lines = readLinesFromFile("./data/day15.txt");
	cout << hashAll(lines) << endl;
	cout << processTheHolidayASCIIStringHelperManualArrangementProcedure(lines) << endl;
}