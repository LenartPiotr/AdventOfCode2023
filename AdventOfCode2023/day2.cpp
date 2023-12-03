#include <iostream>
#include <vector>
#include <string>
#include <sstream>

#include "days.h"

using namespace std;

void day2() {
	vector<string> lines = readLinesFromFile("./data/day2.txt");
	
	int maxRedCubes = 12, maxGreenCubes = 13, maxBlueCubes = 14;
	int sum = 0;

	string word;
	int number, gameId;
	char c;
	
	for (int i = 0; i < lines.size(); i++) {
		istringstream stream(lines[i]);
		stream >> word;
		stream >> gameId;
		stream >> c;

		bool possible = true;
		while (stream >> number) {
			stream >> word;
			c = word.back();
			if (c == ',' || c == ';') word.pop_back();
			if (word == "red" && number > maxRedCubes) {
				possible = false;
				break;
			}
			if (word == "green" && number > maxGreenCubes) {
				possible = false;
				break;
			}
			if (word == "blue" && number > maxBlueCubes) {
				possible = false;
				break;
			}
		}
		if (possible) {
			sum += gameId;
		}
	}
	cout << sum << endl;

	// part 2
	int redCount, greenCount, blueCount;
	sum = 0;

	for (int i = 0; i < lines.size(); i++) {
		istringstream stream(lines[i]);
		stream >> word;
		stream >> gameId;
		stream >> c;

		redCount = 0;
		greenCount = 0;
		blueCount = 0;

		while (stream >> number) {
			stream >> word;
			c = word.back();
			if (c == ',' || c == ';') word.pop_back();
			if (word == "red" && number > redCount) {
				redCount = number;
			}
			if (word == "green" && number > greenCount) {
				greenCount = number;
			}
			if (word == "blue" && number > blueCount) {
				blueCount = number;
			}
		}

		sum += redCount * greenCount * blueCount;
	}

	cout << sum;
}