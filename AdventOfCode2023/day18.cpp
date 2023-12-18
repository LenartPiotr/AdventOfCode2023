#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <set>

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

struct digStep {
	int direction;
	int length;
	string color;
};

struct boundaries {
	int x_min;
	int x_max;
	int y_min;
	int y_max;
};

vector<digStep> getSteps(vector<string> lines) {
	vector<digStep> steps;
	char c;
	int num;
	string s;
	for (string l : lines) {
		digStep step;
		step.direction = l[0] == 'U' ? UP : l[0] == 'R' ? RIGHT : l[0] == 'D' ? DOWN : l[0] == 'L' ? LEFT : 0;
		istringstream stream(l);
		stream >> c;
		stream >> num;
		step.length = num;
		stream >> s;
		step.color = s.substr(2, 6);
		steps.push_back(step);
	}
	return steps;
}

boundaries getBoundies(vector<digStep> steps) {
	boundaries bound;
	bound.x_min = 0;
	bound.x_max = 0;
	bound.y_min = 0;
	bound.y_max = 0;
	int x = 0, y = 0;
	for (const auto& s : steps) {
		switch (s.direction) {
		case UP:
			y -= s.length;
			bound.y_min = min(y, bound.y_min);
			break;
		case LEFT:
			x -= s.length;
			bound.x_min = min(x, bound.x_min);
			break;
		case DOWN:
			y += s.length;
			bound.y_max = max(y, bound.y_max);
			break;
		case RIGHT:
			x += s.length;
			bound.x_max = max(x, bound.x_max);
			break;
		}
	}
	return bound;
}

struct field {
	bool visited;
	int direction;
};

void part1(vector<digStep> steps, boundaries bound) {
	int width = bound.x_max - bound.x_min + 1;
	int height = bound.y_max - bound.y_min + 1;
	field** t = new field*[width];
	for (int x = 0; x < width; x++) {
		t[x] = new field[height];
		for (int y = 0; y < height; y++) t[x][y] = { false, 0 };
	}

	int currentX = -bound.x_min;
	int currentY = -bound.y_min;
	int target;
	for (const auto& step : steps) {
		switch (step.direction) {
		case UP:
			target = currentY - step.length;
			for (int y = currentY; y >= target; y--) {
				t[currentX][y].visited = true;
				if (y != currentY) t[currentX][y].direction |= DOWN;
				if (y != target) t[currentX][y].direction |= UP;
			}
			currentY = target;
			break;
		case DOWN:
			target = currentY + step.length;
			for (int y = currentY; y <= target; y++) {
				t[currentX][y].visited = true;
				if (y != currentY) t[currentX][y].direction |= UP;
				if (y != target) t[currentX][y].direction |= DOWN;
			}
			currentY = target;
			break;
		case LEFT:
			target = currentX - step.length;
			for (int x = currentX; x >= target; x--) {
				t[x][currentY].visited = true;
				if (x != currentX) t[x][currentY].direction |= RIGHT;
				if (x != target) t[x][currentY].direction |= LEFT;
			}
			currentX = target;
			break;
		case RIGHT:
			target = currentX + step.length;
			for (int x = currentX; x <= target; x++) {
				t[x][currentY].visited = true;
				if (x != currentX) t[x][currentY].direction |= LEFT;
				if (x != target) t[x][currentY].direction |= RIGHT;
			}
			currentX = target;
			break;
		}
	}

	int area = 0;
	for (int y = 0; y < height; y++) {
		bool inside = false;
		for (int x = 0; x < width; x++) {
			if (t[x][y].direction & UP) inside = !inside;
			if (inside || t[x][y].visited) area++;
		}
	}

	cout << area << endl;

	for (int x = 0; x < width; x++) delete[] t[x];
	delete[] t;
}

inline int dirFromHex(char l) {
	return l == '0' ? RIGHT : l == '1' ? DOWN : l == '2' ? LEFT : l == '3' ? UP : 0;
}

vector<digStep> decode(vector<digStep> steps) {
	vector<digStep> decoded;
	for (const auto& oryginal : steps) {
		digStep step;
		step.color = oryginal.color;
		step.length = stoi(oryginal.color.substr(0, 5), 0, 16);
		step.direction = dirFromHex(oryginal.color[5]);
		decoded.push_back(step);
	}
	return decoded;
}

struct line {
	int from;
	int to;
	int at;
};

void part2(vector<digStep> steps, boundaries bound) {
	int width = bound.x_max - bound.x_min + 1;
	int height = bound.y_max - bound.y_min + 1;
	int currentX = -bound.x_min;
	int currentY = -bound.y_min;
	int target;

	set<int> criticalYPoints;
	vector<line> horizontals;
	vector<line> verticals;

	for (const auto& s : steps) {
		switch (s.direction) {
		case UP:
			target = currentY - s.length;
			criticalYPoints.insert(currentY);
			criticalYPoints.insert(target);
			verticals.push_back({ target, currentY, currentX });
			currentY = target;
			break;
		case DOWN:
			target = currentY + s.length;
			criticalYPoints.insert(currentY);
			criticalYPoints.insert(target);
			verticals.push_back({ currentY, target, currentX });
			currentY = target;
			break;
		case LEFT:
			target = currentX - s.length;
			horizontals.push_back({ target, currentX, currentY });
			currentX = target;
			break;
		case RIGHT:
			target = currentX + s.length;
			horizontals.push_back({ currentX, target, currentY });
			currentX = target;
			break;
		}
	}

	std::sort(horizontals.begin(), horizontals.end(), [](const line& l1, const line& l2) { return l1.at < l2.at; });
	std::sort(verticals.begin(), verticals.end(), [](const line& l1, const line& l2) { return l1.at < l2.at; });

	vector<int> yPoints(criticalYPoints.begin(), criticalYPoints.end());

	long long area = 0;

	for (int iy = 1; iy < yPoints.size(); iy++) {
		int yBegin = yPoints[iy - 1] + 1;
		int yEnd = yPoints[iy] - 1;
		int yDiff = yEnd - yBegin + 1;
		if (yDiff < 1) continue;
		bool outside = true;
		int lastLeft = 0;
		for (const auto& vert : verticals) {
			if (vert.from > yBegin || vert.to < yEnd) continue;
			if (outside) {
				lastLeft = vert.at;
			}
			else {
				area += (vert.at - static_cast<long long>(lastLeft) + 1ll) * ((long long)yDiff);
			}
			outside = !outside;
		}
	}

	for (const int& y : yPoints) {
		// auto horFirst = find_if(horizontals.begin(), horizontals.end(), [y](const line& l) {l.at == y; });
		// auto horLast = find_if(horizontals.rbegin(), horizontals.rend(), [y](const line& l) {l.at == y; });
		// bool foundHorizontals = horFirst != horizontals.end() && horLast != horizontals.rend();
		bool outside = true;
		bool insideUp = false;
		bool insideDown = false;
		int lastLeft = 0;
		for (const auto& vert : verticals) {
			if (vert.from > y || vert.to < y) continue;
			if (outside) {
				lastLeft = vert.at;
				insideUp = vert.from != y;
				insideDown = vert.to != y;
				outside = false;
			}
			else {
				if (vert.from != y) insideUp = !insideUp;
				if (vert.to != y) insideDown = !insideDown;
				if (!insideUp && !insideDown) {
					area += static_cast<long long>(vert.at) - static_cast<long long>(lastLeft) + 1ll;
					outside = true;
				}
			}
		}
	}

	cout << area << endl;
}

void day18() {
	vector<string> lines = readLinesFromFile("./data/day18.txt");
	vector<digStep> steps = getSteps(lines);
	boundaries bound = getBoundies(steps);
	cout << "Boundaries: " << bound.y_min << " " << bound.y_max << " / " << bound.x_min << " " << bound.x_max << endl;

	part1(steps, bound);

	vector<digStep> decoded = decode(steps);
	bound = getBoundies(decoded);
	cout << "Boundaries: " << bound.y_min << " " << bound.y_max << " / " << bound.x_min << " " << bound.x_max << endl;

	part2(decoded, bound);
}