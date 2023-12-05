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

vector<pair<long long, long long>> vectorToPair(const vector<long long>& vect) {
	vector<pair<long long, long long>> pairs;
	for (int i = 0; i < vect.size(); i += 2) {
		pair<long long, long long> p(vect[i], vect[i + 1]);
		pairs.push_back(p);
	}
	return pairs;
}

vector<pair<long long, long long>> optimizeCountVector(const vector<pair<long long, long long>>& numbers) {
	vector<pair<long long, long long>> copy(numbers);
	sort(copy.begin(), copy.end(), [](const pair<long long, long long>& p1, const pair<long long, long long>& p2) { return p1.first < p2.first; });
	for (int i = 0; i < copy.size() - 1; i++) {
		if (copy[i].first + copy[i].second >= copy[i + 1].first) {
			copy[i].second = copy[i + 1].first + copy[i + 1].second - copy[i].first;
			copy.erase(copy.begin() + i + 1);
			i--;
		}
	}
	return copy;
}

vector<pair<long long, long long>> applyVectorToMapWithCounts(const vector<pair<long long, long long>>& in, const vector<array<long long, 3>>& map) {
	vector<pair<long long, long long>> results;
	auto pairs = optimizeCountVector(in);
	int lastMapIndex = 0;
	auto lastRange = map[0];
	auto firstRangeValue = lastRange[1];
	auto lastRangeValue = lastRange[1] + lastRange[2] - 1ll;
	bool sameToEnd = false;
	for (int pIndex = 0; pIndex < pairs.size(); pIndex++) {
		auto& pair = pairs[pIndex];
		auto firstValue = pair.first;
		auto lastValue = pair.first + pair.second - 1ll;
		while (firstValue <= lastValue) {
			if (sameToEnd) {
				results.push_back({ firstValue, lastValue - firstValue + 1ll });
				break;
			}
			// values are belog map range
			if (firstValue < firstRangeValue) {
				if (lastValue < firstRangeValue) {
					results.push_back({ firstValue, lastValue - firstValue + 1ll });
					break;
				}
				results.push_back({ firstValue, firstRangeValue - firstValue });
				firstValue = firstRangeValue;
			}
			// values are above map range
			if (firstValue > lastRangeValue) {
				lastMapIndex++;
				if (lastMapIndex >= map.size()) {
					sameToEnd = true;
					continue;
				}
				lastRange = map[lastMapIndex];
				firstRangeValue = lastRange[1];
				lastRangeValue = lastRange[1] + lastRange[2] - 1ll;
				continue;
			}
			// first value is in range
			auto firstMappedValue = lastRange[0] + firstValue - lastRange[1];
			if (lastValue <= lastRangeValue) {
				results.push_back({ firstMappedValue, lastValue - firstValue + 1ll });
				break;
			}
			results.push_back({ firstMappedValue, lastRangeValue - firstValue + 1ll });
			firstValue = lastRangeValue + 1ll;
		}
	}
	return results;
}

void day5() {
	vector<string> lines = readLinesFromFile("./data/day5.txt");
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

	// part 2
	auto pairs = vectorToPair(seeds);
	for (auto& map : maps) {
		pairs = applyVectorToMapWithCounts(pairs, map);
	}
	pairs = optimizeCountVector(pairs);
	cout << pairs[0].first << endl;
}