#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <queue>

#include "days.h"
#include "Table.h"

using namespace std;

struct Step {
	int x;
	int y;
	int dist;
};

void part1(Table<bool>& t, int sx, int sy, int limit) {
	Table<int> visited(t.width, t.height, INT_MAX);
	
	queue<Step> steps;
	steps.push({ sx, sy, 0 });

	pair<int, int> directions[4] = { {0, 1}, {1, 0}, {0, -1}, {-1, 0} };

	while (!steps.empty()) {
		Step s = steps.front();
		steps.pop();
		if (s.dist > limit) continue;
		if (!t.inRange(s.x, s.y)) continue;
		if (t[s.x][s.y]) continue;
		if (visited[s.x][s.y] <= s.dist) continue;
		visited[s.x][s.y] = s.dist;
		for (int i = 0; i < 4; i++) {
			steps.push({ s.x + directions[i].first, s.y + directions[i].second, s.dist + 1 });
		}
	}

	int even = 0;
	visited.forEach([&even](int value) {
		if (value != INT_MAX && value % 2 == 0) even++;
	});
	cout << even << endl;
}

void day21() {
	vector<string> lines = readLinesFromFile("./data/day21.txt");
	int width = lines[0].length();
	int height = lines.size();
	
	int sx = 0, sy = 0;
	Table<bool> tab(width, height, false);
	for (int y = 0; y < height; y++) {
		string line = lines[y];
		for (int x = 0; x < width; x++) {
			if (line[x] == '#') tab[x][y] = true;
			else if (line[x] == 'S') {
				sx = x;
				sy = y;
			}
		}
	}

	part1(tab, sx, sy, 64);
}