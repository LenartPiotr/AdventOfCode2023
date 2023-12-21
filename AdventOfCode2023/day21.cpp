#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>

#include "days.h"
#include "Table.h"

using namespace std;

void day21() {
	vector<string> lines = readLinesFromFile("./data/test.txt");
	int width = lines[0].length();
	int height = lines.size();
	
	int sx, sy;
	Table<bool> tab(width, height, false);
	for (int y = 0; y < height; y++) {
		string line = lines[y];
		for (int x = 0; x < width; x++) {
			if (line[x] == '#') tab.tab[x][y] = true;
			else if (line[x] == 'S') {
				sx = x;
				sy = y;
			}
		}
	}

	// cout << tab << endl;
}