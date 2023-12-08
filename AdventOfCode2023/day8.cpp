#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <sstream>
#include <algorithm>

#include "days.h"

#define isDigit(X) ((X) >= '0' && (X) <= '9')

using namespace std;

struct vertex {
	string signature;
	string left;
	string right;
	int leftIndex;
	int rightIndex;
	bool endWithZ;
};

int part1(string stepsGuide, map<string, int>& namesOfVertices, const vector<vertex>& vertices) {
	string currentName = "AAA";
	int currentIndex = namesOfVertices[currentName];
	int steps = 0;
	int stepIndex = 0;
	while (currentName != "ZZZ") {
		if (stepsGuide[stepIndex] == 'L')
			currentIndex = vertices[currentIndex].leftIndex;
		else
			currentIndex = vertices[currentIndex].rightIndex;
		currentName = vertices[currentIndex].signature;
		steps++;
		stepIndex++;
		if (stepIndex >= stepsGuide.size()) stepIndex = 0;
	}
	return steps;
}

int part2(const string stepsGuide, map<string, int>& namesOfVertices, const vector<vertex>& vertices) {
	int vCount = vertices.size();
	vector<int>* stepsWithEnd = new vector<int>[vCount];
	int* indexAtEnd = new int[vCount];

	for (int startIndex = 0; startIndex < vCount; startIndex++) {
		int currentIndex = startIndex;
		int step = 0;
		vector<int> endIndexes;
		for (const char& c : stepsGuide) {
			if (vertices[currentIndex].endWithZ) endIndexes.push_back(step);
			currentIndex = c == 'L' ? vertices[currentIndex].leftIndex : vertices[currentIndex].rightIndex;
			step++;
		}
		indexAtEnd[startIndex] = currentIndex;
		stepsWithEnd[startIndex] = endIndexes;
	}

	vector<int> positions;
	for (int i = 0; i < vCount; i++) {
		if (vertices[i].signature[2] == 'A') positions.push_back(i);
	}

	for (int i = 0; i < vCount; i++) {
		cout << i << " " << vertices[i].signature << " -";
		for (int j = 0; j < stepsWithEnd[i].size(); j++) {
			cout << " " << stepsWithEnd[i][j];
		}
		cout << " - " << indexAtEnd[i] << endl;
	}

	int steps = 0;
	while (true) {
		int smallestVectorCount = stepsWithEnd[positions[0]].size();
		int smallestVectorIndex = positions[0];
		for (int i = 1; i < positions.size(); i++) {
			int value = stepsWithEnd[positions[i]].size();
			if (value < smallestVectorCount) {
				smallestVectorCount = value;
				smallestVectorIndex = positions[i];
			}
		}
		int element = 0;
		bool sameElement = false;
		vector<int>& v = stepsWithEnd[smallestVectorIndex];
		for (int i = 0; i < v.size(); i++) {
			element = v[i];
			sameElement = true;
			for (const int& p : positions) {
				if (!binary_search(stepsWithEnd[p].begin(), stepsWithEnd[p].end(), element)) {
					sameElement = false;
					break;
				}
			}
			if (sameElement) break;
		}
		if (sameElement) {
			steps += element;
			break;
		}
		steps += stepsGuide.length();
		for (int& p : positions) {
			p = indexAtEnd[p];
		}
	}

	delete[] stepsWithEnd;
	delete[] indexAtEnd;

	return steps;
}

long long NWD(long long a, long long b) {
	long long c;
	while (b != 0) {
		c = b;
		b = a % b;
		a = c;
	}
	return a;
}

long long NWW(long long a, long long b) {
	return a * b / NWD(a, b);
}

long long part2_sol2(const string stepsGuide, map<string, int>& namesOfVertices, const vector<vertex>& vertices) {
	int vCount = vertices.size();
	int* indexAtEnd = new int[vCount];

	for (int startIndex = 0; startIndex < vCount; startIndex++) {
		int currentIndex = startIndex;
		int step = 0;
		for (const char& c : stepsGuide) {
			currentIndex = c == 'L' ? vertices[currentIndex].leftIndex : vertices[currentIndex].rightIndex;
			step++;
		}
		indexAtEnd[startIndex] = currentIndex;
	}

	vector<int> positions;
	for (int i = 0; i < vCount; i++) {
		if (vertices[i].signature[2] == 'A') positions.push_back(i);
	}

	long long nww = 1;

	for (int i = 0; i < positions.size(); i++) {
		int pos = positions[i];
		for (int j = 0; j < 1000; j++) {
			if (vertices[pos].endWithZ) {
				nww *= j;
				break;
			}
			pos = indexAtEnd[pos];
		}
	}

	delete[] indexAtEnd;

	return nww * stepsGuide.size();
}

void day8() {
	vector<string> lines = readLinesFromFile("./data/day8.txt");
	vector<vertex> vertices;
	map<string, int> namesOfVertices;

	for (auto it = lines.begin() + 2; it != lines.end(); it++) {
		vertex v;
		v.signature = (*it).substr(0, 3);
		v.left = (*it).substr(7, 3);
		v.right = (*it).substr(12, 3);
		v.endWithZ = v.signature[2] == 'Z';
		vertices.push_back(v);
	}

	for (int i = 0; i < vertices.size(); i++) {
		namesOfVertices[vertices[i].signature] = i;
	}

	for (int i = 0; i < vertices.size(); i++) {
		vertices[i].leftIndex = namesOfVertices[vertices[i].left];
		vertices[i].rightIndex = namesOfVertices[vertices[i].right];
	}

	long long steps;

	steps = part1(lines[0], namesOfVertices, vertices);
	cout << steps << endl;

	steps = part2_sol2(lines[0], namesOfVertices, vertices);
	cout << steps << endl;
}