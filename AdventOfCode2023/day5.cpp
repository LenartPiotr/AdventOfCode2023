#include <iostream>
#include <vector>
#include <array>
#include <string>
#include <sstream>
#include <algorithm>

#include "days.h"

#define isDigit(X) ((X) >= '0' && (X) <= '9')

using namespace std;

int findRightMapIndex(const vector<array<long long, 3>>& map, long long value) {
	for (int i = 0; i < map.size(); i++) {
		if (map[i][1] <= value && value < map[i][1] + map[i][2]) return i;
	}
	return -1;
}

int findRightMapIndexBinarySearch(const vector<array<long long, 3>>& map, long long value) {
	int p = 0;
	int q = map.size() - 1;
	while (p <= q) {
		int m = (p + q) / 2;
		if (map[m][1] <= value && map[m][1] + map[m][2] > value) return m;
		if (map[m][1] < value) p = m + 1;
		else q = m - 1;
	}
	return -1;
}

void applyMapToVector(vector<long long>& numbers, const vector<vector<array<long long, 3>>>& maps) {
	for (auto map : maps) {
		auto minValue = map[0][1];
		auto maxValue = map.back()[1] + map.back()[2];
		for (int i = 0; i < numbers.size(); i++) {
			long long num = numbers[i];
			if (num < minValue || num > maxValue) continue;
			int index = findRightMapIndexBinarySearch(map, num);
			if (index == -1) continue;
			numbers[i] = map[index][0] + num - map[index][1];
		}
	}
}

void day5() {
	vector<string> lines = readLinesFromFile("./data/test.txt");
	vector<long long> seeds;
	vector<vector<array<long long, 3>>> maps;
	
	vector<array<long long, 3>> map;
	long long num;
	string word;
	
	// read seeds
	istringstream streamLine0(lines[0]);
	streamLine0 >> word;
	while (streamLine0 >> num) seeds.push_back(num);
	
	for (int i = 2; i < lines.size(); i++) {
		string line = lines[i];
		if (line == "") {
			sort(map.begin(), map.end(), [](const array<long long, 3>& a, const array<long long, 3>& b) { return a[1] < b[1]; });
			maps.push_back(map);
			vector<array<long long, 3>> newMap;
			map = newMap;
			continue;
		}
		if (!isDigit(line[0])) continue;
		array<long long, 3> range{};
		istringstream stream(line);
		stream >> range[0] >> range[1] >> range[2];
		map.push_back(range);
	}
	sort(map.begin(), map.end(), [](const array<long long, 3>& a, const array<long long, 3>& b) { return a[1] < b[1]; });
	maps.push_back(map);

	// part 1

	vector<long long> seedsCopy(seeds);
	applyMapToVector(seedsCopy, maps);
	long long minValue = *std::min_element(seedsCopy.begin(), seedsCopy.end());
	cout << minValue << endl;
}