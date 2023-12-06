#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>

#include "days.h"

#define isDigit(X) ((X) >= '0' && (X) <= '9')

using namespace std;

inline int getValue(int maxTime, int time) {
	return time * (maxTime - time);
}

template <typename T>
T getRange(T time, T dist) {
	double delta = time * time - 4 * dist;
	if (delta < 0) {
		cout << "No solutions!\n";
		return 0;
	}
	delta = sqrt(delta);
	double x1 = time / 2. + delta / 2.;
	double x2 = time / 2. - delta / 2.;
	if (x2 < x1) swap(x1, x2);
	T rx1 = ceil(x1);
	T rx2 = floor(x2);
	if (getValue(time, rx1) == dist) rx1++;
	if (getValue(time, rx2) == dist) rx2++;
	return rx2 - rx1 + 1;
}

void day6() {
	vector<string> lines = readLinesFromFile("./data/day6.txt");
	vector<int> times;
	vector<int> distances;
	string word;
	int number;
	{
		istringstream stream(lines[0]);
		stream >> word;
		while (stream >> number) times.push_back(number);
	}
	{
		istringstream stream(lines[1]);
		stream >> word;
		while (stream >> number) distances.push_back(number);
	}

	int product = 1;
	for (int i = 0; i < times.size(); i++) {
		product *= getRange(times[i], distances[i]);
	}
	cout << product << endl;

	long long bigTime;
	{
		istringstream stream(lines[0]);
		string l;
		getline(stream, l, ':');
		getline(stream, l);
		l.erase(remove_if(l.begin(), l.end(), isspace), l.end());
		bigTime = stoll(l);
	}

	long long bigDistance;
	{
		istringstream stream(lines[1]);
		string l;
		getline(stream, l, ':');
		getline(stream, l);
		l.erase(remove_if(l.begin(), l.end(), isspace), l.end());
		bigDistance = stoll(l);
	}

	auto result = getRange(bigTime, bigDistance);
	cout << result << endl;
}