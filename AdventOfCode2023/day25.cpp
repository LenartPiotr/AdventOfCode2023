#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <set>
#include <map>

#include "days.h"
#include "Table.h"

using namespace std;

struct InData {
	string signature;
	vector<string> connections;
};

vector<InData> readData(const vector<string>& lines) {
	vector<InData> data;
	for (const string& l : lines) {
		InData d;
		d.signature = l.substr(0, 3);
		d.connections = split(l.substr(5), ' ');
		data.push_back(d);
	}
	return data;
}

struct Data {
	int index;
	string signature;
	vector<int> connections;
};

void day25() {
	vector<string> lines = readLinesFromFile("./data/day25.txt");
	vector<InData> inData = readData(lines);
	set<string> uniquesNames;
	for (auto& d : inData) {
		uniquesNames.insert(d.signature);
		for (auto& c : d.connections) uniquesNames.insert(c);
	}
	map<string, int> nameToIndex;
	vector<Data> data;
	for (auto& u : uniquesNames) {
		Data d;
		d.index = data.size();
		d.signature = u;
		nameToIndex[u] = d.index;
		data.push_back(d);
	}

	vector<vector<int>> conn(data.size(), vector<int>(data.size(), 0));
	for (auto& d : inData) {
		int i = nameToIndex[d.signature];
		for (auto& c : d.connections) {
			int j = nameToIndex[c];
			data[i].connections.push_back(j);
			data[j].connections.push_back(i);
			conn[i][j] = conn[j][i] = 1;
		}
	}

	// From https://en.wikipedia.org/wiki/Stoer-Wagner_algorithm
	auto globalMinCut = [](std::vector<std::vector<int>>& mat) -> std::pair<int, std::vector<int>> {
		std::pair<int, std::vector<int>> best = { std::numeric_limits<int>::max(), {} };
		int n = mat.size();
		std::vector<std::vector<int>> co(n);

		for (int i = 0; i < n; i++)
			co[i] = { i };

		for (int ph = 1; ph < n; ph++) {
			std::vector<int> w = mat[0];
			size_t s = 0, t = 0;
			for (int it = 0; it < n - ph; it++) { // O(V^2) -> O(E log V) with prio. queue
				w[t] = std::numeric_limits<int>::min();
				s = t, t = max_element(w.begin(), w.end()) - w.begin();
				for (int i = 0; i < n; i++) w[i] += mat[t][i];
			}
			best = min(best, { w[t] - mat[t][t], co[t] });
			co[s].insert(co[s].end(), co[t].begin(), co[t].end());
			for (int i = 0; i < n; i++) mat[s][i] += mat[t][i];
			for (int i = 0; i < n; i++) mat[i][s] = mat[s][i];
			mat[0][t] = std::numeric_limits<int>::min();
		}

		return best;
	};

	auto result = globalMinCut(conn);
	cout << result.second.size() * (data.size() - result.second.size()) << endl;

	/*for (auto& d : data) {
		cout << d.signature << " " << d.index << ":";
		for (auto& c : d.connections) {
			cout << " " << c;
		}
		cout << endl;
	}*/
}