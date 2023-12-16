#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <queue>

#include "days.h"
#include "hash.cpp"

using namespace std;

enum fieldType {
	NONE, MIRROR, MIRROR_REV, FLIP_HOR, FLIP_VERT
};

#define UP 1
#define RIGHT 2
#define DOWN 4
#define LEFT 8

struct field {
	fieldType type;
	int visited;
};

struct lightBeam {
	int x;
	int y;
	int dx;
	int dy;
	lightBeam(int sx, int sy, int sdx, int sdy) :x(sx), y(sy), dx(sdx), dy(sdy) {}
};

field** vecToTab(vector<string> lines) {
	int width = lines[0].length();
	int height = lines.size();
	field** fields = new field * [width];
	for (int x = 0; x < width; x++) {
		fields[x] = new field[height];
		for (int y = 0; y < height; y++) {
			field& f = fields[x][y];
			switch (lines[y][x]) {
			case '.': f.type = NONE; break;
			case '/': f.type = MIRROR_REV; break;
			case '\\': f.type = MIRROR; break;
			case '|': f.type = FLIP_HOR; break;
			case '-': f.type = FLIP_VERT; break;
			}
			f.visited = 0;
		}
	}
	return fields;
}

void delField(field** tab, int width) {
	for (int x = 0; x < width; x++) delete[] tab[x];
	delete[] tab;
}

inline int getDir(int x, int y) {
	return y == -1 ? UP : y == 1 ? DOWN : x == 1 ? RIGHT : x == -1 ? LEFT : 0;
}

inline int inRange(int x, int y, int w, int h) {
	return x >= 0 && y >= 0 && x < w && y < h;
}

int countLight(vector<string> lines, lightBeam startBeam) {
	int width = lines[0].length();
	int height = lines.size();
	field** t = vecToTab(lines);
	queue<lightBeam> lights;
	lights.push(startBeam);
	while (!lights.empty()) {
		auto l = lights.front();
		lights.pop();
		if (!inRange(l.x, l.y, width, height)) continue;
		if (t[l.x][l.y].visited & getDir(l.dx, l.dy)) continue;
		bool splitted = false;
		while (inRange(l.x, l.y, width, height) && !splitted) {
			t[l.x][l.y].visited |= getDir(l.dx, l.dy);
			switch (t[l.x][l.y].type) {
			case MIRROR: swap(l.dx, l.dy); break;
			case MIRROR_REV: swap(l.dx, l.dy); l.dx *= -1; l.dy *= -1; break;
			case FLIP_HOR:
				if (l.dy != 0) break;
				lights.push(lightBeam(l.x, l.y, 0, 1));
				lights.push(lightBeam(l.x, l.y, 0, -1));
				splitted = true;
				break;
			case FLIP_VERT:
				if (l.dx != 0) break;
				lights.push(lightBeam(l.x, l.y, 1, 0));
				lights.push(lightBeam(l.x, l.y, -1, 0));
				splitted = true;
				break;
			}
			l.x += l.dx;
			l.y += l.dy;
		}
	}
	int sum = 0;
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			if (t[x][y].visited) sum++;
		}
	}
	delField(t, width);
	return sum;
}

void day16() {
	vector<string> lines = readLinesFromFile("./data/day16.txt");
	cout << countLight(lines, lightBeam(0, 0, 1, 0)) << endl;

	// part 2

	int width = lines[0].length();
	int height = lines.size();
	
	int maxCount = 0;
	for (int x = 0; x < width; x++) {
		maxCount = max(maxCount, countLight(lines, lightBeam(x, 0, 0, 1)));
		maxCount = max(maxCount, countLight(lines, lightBeam(x, height - 1, 0, -1)));
	}
	for (int y = 0; y < width; y++) {
		maxCount = max(maxCount, countLight(lines, lightBeam(0, y, 1, 0)));
		maxCount = max(maxCount, countLight(lines, lightBeam(width - 1, y, -1, 0)));
	}
	cout << maxCount << endl;
}