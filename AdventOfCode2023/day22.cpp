#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <set>
#include <queue>

#include "days.h"
#include "Table.h"

using namespace std;

struct Block {
	int x_min;
	int y_min;
	int z_min;
	int x_max;
	int y_max;
	int z_max;
};

vector<Block> readBlocks(const vector<string>& lines) {
	vector<Block> blocks;
	for (auto& l : lines) {
		int splitPoint = l.find('~');
		string lPart1 = l.substr(0, splitPoint);
		string lPart2 = l.substr(splitPoint + 1);
		Block b;
		auto parts = split(lPart1, ',');
		b.x_min = atoi(parts[0].c_str());
		b.y_min = atoi(parts[1].c_str());
		b.z_min = atoi(parts[2].c_str());
		parts = split(lPart2, ',');
		b.x_max = atoi(parts[0].c_str());
		b.y_max = atoi(parts[1].c_str());
		b.z_max = atoi(parts[2].c_str());
		if (b.x_max < b.x_min) swap(b.x_min, b.x_max);
		if (b.y_max < b.y_min) swap(b.y_min, b.y_max);
		if (b.z_max < b.z_min) swap(b.z_min, b.z_max);
		blocks.push_back(b);
	}
	return blocks;
}

struct Field {
	int height;
	int blockId;
	Field() {
		height = 0;
		blockId = -1;
	}
	friend std::ostream& operator<<(std::ostream& os, const Field& f) {
		os << f.height;
		return os;
	}
};

void part12(const vector<Block>& blocks) {
	int width = 0;
	int height = 0;
	int blockSize = blocks.size();
	for (const Block& b : blocks) {
		if (b.x_max > width) width = b.x_max;
		if (b.y_max > height) height = b.y_max;
	}
	vector<set<int>> standsOn;
	Table<Field> fields(width + 1, height + 1, [](const int& x, const int& y) { return Field(); });
	int blockId = 0;
	for (const Block& b : blocks) {
		int y_max = 0;
		set<int> standOn;
		for (int x = b.x_min; x <= b.x_max; x++)
			for (int y = b.y_min; y <= b.y_max; y++) {
				if (fields[x][y].height > y_max) {
					y_max = fields[x][y].height;
					standOn.clear();
					standOn.insert(fields[x][y].blockId);
				}
				else if (fields[x][y].height == y_max && fields[x][y].blockId >= 0) {
					standOn.insert(fields[x][y].blockId);
				}
			}
		standsOn.push_back(standOn);
		y_max += b.z_max - b.z_min + 1;
		for (int x = b.x_min; x <= b.x_max; x++)
			for (int y = b.y_min; y <= b.y_max; y++) {
				fields[x][y].height = y_max;
				fields[x][y].blockId = blockId;
			}
		blockId++;
		// cout << fields << "\n\n---\n\n";
	}
	vector<bool> canBeDisintegrate;
	for (int i = 0; i < blockSize; i++) canBeDisintegrate.push_back(true);
	for (const auto& standOn : standsOn) {
		if (standOn.size() != 1) continue;
		canBeDisintegrate[*standOn.begin()] = false;
	}
	int sum = 0;
	for (const bool& val : canBeDisintegrate) {
		if (val) sum++;
	}
	cout << sum << endl;

	// part 2

	vector<set<int>> isFoundationFor;
	for (int i = 0; i < blockSize; i++) isFoundationFor.push_back({});
	for (int i = 0; i < standsOn.size(); i++) {
		for (const int& foundation : standsOn[i]) {
			isFoundationFor[foundation].insert(i);
		}
	}
	vector<int> standsCount;
	sum = 0;
	for (int i = 0; i < blockSize; i++) {
		if (canBeDisintegrate[i]) continue;
		int fallCount = 0;
		standsCount.clear();
		for (const auto& s : standsOn) standsCount.push_back(s.size());
		priority_queue<int, vector<int>, greater<int>> Q;
		for (const int& f : isFoundationFor[i]) Q.push(f);
		while (!Q.empty()) {
			int index = Q.top();
			Q.pop();
			standsCount[index]--;
			if (standsCount[index] == 0) {
				fallCount++;
				for (const int& f : isFoundationFor[index]) Q.push(f);
			}
		}
		sum += fallCount;
	}

	cout << sum << endl;
}

void day22() {
	vector<string> lines = readLinesFromFile("./data/day22.txt");
	vector<Block> blocks = readBlocks(lines);
	sort(blocks.begin(), blocks.end(), [](const Block& b1, const Block& b2) { return b1.z_min < b2.z_min; });

	part12(blocks);
}