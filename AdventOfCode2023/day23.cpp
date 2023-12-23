#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <set>
#include <queue>
#include <functional>

#include "days.h"
#include "Table.h"

using namespace std;

struct Connection {
	int target;
	int distance;
	bool slope;
};

struct Intersection {
	int x;
	int y;
	vector<Connection> connections;

	Intersection(int _x, int _y) : x(_x), y(_y) { }
};

struct Point {
	int x;
	int y;
	int fromDir;
};

void part1(vector<string> lines) {
	int width = lines[0].length();
	int height = lines.size();

	// detect intersection points
	Table<int> intersectionIndexes(width, height, -1);
	vector<Intersection> intersections;
	for (int y = 0; y < height; y++) {
		string l = lines[y];
		for (int x = 0; x < width; x++) {
			if (l[x] != '.') continue;
			int paths = 0;
			if (intersectionIndexes.inRange(x - 1, y) && lines[y][x - 1] != '#') paths++;
			if (intersectionIndexes.inRange(x + 1, y) && lines[y][x + 1] != '#') paths++;
			if (intersectionIndexes.inRange(x, y - 1) && lines[y - 1][x] != '#') paths++;
			if (intersectionIndexes.inRange(x, y + 1) && lines[y + 1][x] != '#') paths++;
			if (paths == 2) continue;
			intersectionIndexes[x][y] = intersections.size();
			intersections.push_back(Intersection(x, y));
		}
	}

	// count distances between intersections
	pair<int, int> directions[4] = { make_pair(0, -1), make_pair(1, 0), make_pair(-1, 0), make_pair(0, 1) };
	for (int i = 0; i < intersections.size(); i++) {
		Intersection& inter = intersections[i];
		for (int d = 0; d < 4; d++) {
			int x = inter.x + directions[d].first;
			int y = inter.y + directions[d].second;
			if (!intersectionIndexes.inRange(x, y)) continue;
			if (lines[y][x] == '#') continue;
			Point p = { x, y, 3 - d };
			bool forward = true;
			bool backward = true;
			int distance = 1;
			while (intersectionIndexes[p.x][p.y] == -1) {
				distance++;
				int d2;
				// No in data < ^ signs
				const char& s = lines[p.y][p.x];
				if (s == '>' && p.fromDir == 2) backward = false;
				if (s == '>' && p.fromDir == 1) forward = false;
				if (s == 'v' && p.fromDir == 0) backward = false;
				if (s == 'v' && p.fromDir == 3) forward = false;
				for (d2 = 0; d2 < 4; d2++) {
					if (d2 == p.fromDir) continue;
					int x2 = p.x + directions[d2].first;
					int y2 = p.y + directions[d2].second;
					if (!intersectionIndexes.inRange(x2, y2)) continue;
					const char& sign = lines[y2][x2];
					if (sign == '#') continue;
					p.x = x2;
					p.y = y2;
					p.fromDir = 3 - d2;
					break;
				}
			}
			if (p.y < inter.y) continue;
			if (p.y == inter.y && p.x < inter.x) continue;
			int targetId = intersectionIndexes[p.x][p.y];
			intersections[i].connections.push_back({ targetId, distance, !forward });
			intersections[targetId].connections.push_back({ i, distance, !backward });
		}
	}

	// found all paths to end
	int maxDistance = 0;
	vector<bool> visited;
	for (int i = 0; i < intersections.size(); i++) visited.push_back(false);
	int startIndex = 0;
	int endIndex = intersections.size() - 1;

	function<void(int, int)> goFromPoint;

	bool canThroughSlope = false;

	goFromPoint = [&goFromPoint, &maxDistance, &endIndex, &intersections, &visited, &canThroughSlope](int index, int dist) {
		if (index == endIndex) {
			maxDistance = max(maxDistance, dist);
			return;
		}
		for (const Connection& c : intersections[index].connections) {
			if (c.slope && !canThroughSlope) continue;
			if (visited[c.target]) continue;
			visited[c.target] = true;
			goFromPoint(c.target, dist + c.distance);
			visited[c.target] = false;
		}
	};

	goFromPoint(startIndex, 0);
	cout << "max (no slopes): " << maxDistance << endl;

	canThroughSlope = true;
	goFromPoint(startIndex, 0);
	cout << "max (with slopes): " << maxDistance << endl;

	/* PRINT /
	for (int i = 0; i < intersections.size(); i++) {
		auto& inter = intersections[i];
		cout << i << ": " << inter.y << " / " << inter.x << ":";
		for (auto& c : inter.connections) {
			cout << " (" << intersections[c.target].y << ", " << intersections[c.target].x << " - " << c.distance << ")";
		}
		cout << endl;
	}
	/* */
}

void day23() {
	vector<string> lines = readLinesFromFile("./data/day23.txt");
	part1(lines);
}