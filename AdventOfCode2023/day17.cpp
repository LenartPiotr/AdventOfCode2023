#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <queue>

#include "days.h"
#include "hash.cpp"

using namespace std;

#define UP 1
#define RIGHT 2
#define DOWN 4
#define LEFT 8

inline int getDir(int x, int y) {
	return y == -1 ? UP : y == 1 ? DOWN : x == 1 ? RIGHT : x == -1 ? LEFT : 0;
}

inline int inRange(int x, int y, int w, int h) {
	return x >= 0 && y >= 0 && x < w && y < h;
}

inline int oppositeDir(int dir) {
	return (dir & 5 ? 5 : 10) - dir;
}

inline int dirToIndex(int dir) {
	return dir == UP ? 0 : dir == RIGHT ? 1 : dir == DOWN ? 2 : 3;
}

int** createTab(int w, int h, int d) {
	int** t = new int* [w];
	for (int x = 0; x < w; x++) {
		t[x] = new int[h];
		for (int y = 0; y < h; y++)
			t[x][y] = d;
	}
	return t;
}

void deleteTab(int** t, int w) {
	for (int x = 0; x < w; x++) delete[] t[x];
	delete[] t;
}

struct point {
	int x;
	int y;
	int lastDir;
	int lenInDir;
	int dist;

	bool operator<(const point& other) const {
		return dist > other.dist;
	}
};

void part1(int** t, int w, int h) {
	int*** visited = new int** [w];
	for (int x = 0; x < w; x++) {
		visited[x] = new int* [h];
		for (int y = 0; y < h; y++) {
			visited[x][y] = new int[4];
			for (int i = 0; i < 4; i++) visited[x][y][i] = 10;
		}
	}
	priority_queue<point> q;
	q.push({ 0, 0, 0, 0, 0 });
	pair<int, int> directions[4] = { {1, 0}, {-1, 0}, {0, 1}, {0, -1} };
	int minValue = 0;
	while (!q.empty()) {
		point p = q.top();
		q.pop();
		if (p.x == w - 1 && p.y == h - 1) {
			minValue = p.dist;
			break;
		}
		if (visited[p.x][p.y][dirToIndex(p.lastDir)] <= p.lenInDir) continue;
		visited[p.x][p.y][dirToIndex(p.lastDir)] = p.lenInDir;
		for (int i = 0; i < 4; i++) {
			int dx = directions[i].first;
			int dy = directions[i].second;
			if (!inRange(p.x + dx, p.y + dy, w, h)) continue;
			int dir = getDir(dx, dy);
			if (oppositeDir(dir) & p.lastDir) continue;
			if (p.lenInDir == 3 && p.lastDir & dir) continue;
			int lenDir = dir == p.lastDir ? p.lenInDir + 1 : 1;
			point p2 = { p.x + dx, p.y + dy, dir, lenDir, p.dist + t[p.x + dx][p.y + dy] };
			q.push(p2);
		}
	}

	for (int x = 0; x < w; x++) {
		for (int y = 0; y < h; y++) {
			delete[] visited[x][y];
		}
		delete[] visited[x];
	}
	delete[] visited;

	cout << minValue << endl;
}

void part2(int** t, int w, int h) {
	int*** visited = new int** [w];
	for (int x = 0; x < w; x++) {
		visited[x] = new int* [h];
		for (int y = 0; y < h; y++) {
			visited[x][y] = new int[4];
			for (int i = 0; i < 4; i++) visited[x][y][i] = 10;
		}
	}
	priority_queue<point> q;
	q.push({ 0, 0, 0, 0, 0 });
	pair<int, int> directions[4] = { {1, 0}, {-1, 0}, {0, 1}, {0, -1} };
	int minValue = 0;
	while (!q.empty()) {
		point p = q.top();
		q.pop();
		if (p.x == w - 1 && p.y == h - 1) {
			minValue = p.dist;
			break;
		}
		if (visited[p.x][p.y][dirToIndex(p.lastDir)] <= p.lenInDir) continue;
		visited[p.x][p.y][dirToIndex(p.lastDir)] = p.lenInDir;
		for (int i = 0; i < 4; i++) {
			int dx = directions[i].first;
			int dy = directions[i].second;
			if (!inRange(p.x + dx, p.y + dy, w, h)) continue;
			int dir = getDir(dx, dy);
			if (oppositeDir(dir) & p.lastDir) continue;
			if (p.lenInDir < 4 && dir != p.lastDir && p.lastDir) continue;
			if (p.lenInDir == 10 && p.lastDir & dir) continue;
			int lenDir = dir == p.lastDir ? p.lenInDir + 1 : 1;
			point p2 = { p.x + dx, p.y + dy, dir, lenDir, p.dist + t[p.x + dx][p.y + dy] };
			q.push(p2);
		}
	}

	for (int x = 0; x < w; x++) {
		for (int y = 0; y < h; y++) {
			delete[] visited[x][y];
		}
		delete[] visited[x];
	}
	delete[] visited;

	cout << minValue << endl;
}

void day17() {
	vector<string> lines = readLinesFromFile("./data/test.txt");
	int width = lines[0].length();
	int height = lines.size();
	int** tab = new int* [width];
	for (int x = 0; x < width; x++) {
		tab[x] = new int[height];
		for (int y = 0; y < height; y++) {
			tab[x][y] = lines[y][x] - '0';
		}
	}

	part2(tab, width, height);

	for (int x = 0; x < width; x++) delete[] tab[x];
	delete[] tab;
}