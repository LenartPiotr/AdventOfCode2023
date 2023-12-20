#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <array>
#include <map>
#include <stack>

#include "days.h"
#include "hash.cpp"

using namespace std;

struct Rule {
	int index;
	bool less;
	int value;
	string targetName;
	int targetIndex;
};

struct Workflow {
	string name;
	int index;
	vector<Rule> rules;
	string targetName;
	int targetIndex;
};

int applyPartToRules(const array<int, 4>& part, const vector<Rule>& rules, int els) {
	for (auto& r : rules) {
		if (part[r.index] == r.value) continue;
		bool greather = (part[r.index] - r.value) > 0;
		if (greather ^ r.less) return r.targetIndex;
	}
	return els;
}

void part1(const vector<Workflow>& workflow, const vector<array<int, 4>>& parts, int startIndex) {
	int sum = 0;
	for (auto& p : parts) {
		int index = startIndex;
		while (index >= 0) {
			auto& w = workflow[index];
			index = applyPartToRules(p, w.rules, w.targetIndex);
		}
		if (index == -1) sum += p[0] + p[1] + p[2] + p[3];
	}
	cout << sum << endl;
}

struct Range {
	array<pair<int, int>, 4> data;
	int index;
};

Range cloneRange(Range r) {
	return { {
			make_pair(r.data[0].first, r.data[0].second),
			make_pair(r.data[1].first, r.data[1].second),
			make_pair(r.data[2].first, r.data[2].second),
			make_pair(r.data[3].first, r.data[3].second)
		}, r.index };
}

void part2(const vector<Workflow>& workflow, int startIndex) {
	long long sum = 0ll;
	
	stack<Range> ranges;
	ranges.push({ { make_pair(1, 4000), make_pair(1, 4000), make_pair(1, 4000), make_pair(1, 4000) }, startIndex });

	while (!ranges.empty()) {
		Range r = ranges.top();
		ranges.pop();

		bool skipThisOne = false;
		for (int i = 0; i < 4; i++) {
			if (r.data[i].first > r.data[i].second) {
				skipThisOne = true;
				break;
			}
		}
		if (skipThisOne) continue;

		if (r.index == -2) continue;
		if (r.index == -1) {
			long long multi = 1ll;
			for (int i = 0; i < 4; i++) {
				multi *= static_cast<long long>(r.data[i].second) - r.data[i].first + 1;
			}
			sum += multi;
			continue;
		}
		const Workflow& w = workflow[r.index];
		for (auto& rule : w.rules) {
			int splitPoint = rule.value;
			Range copy = cloneRange(r);
			if (rule.less) {
				copy.data[rule.index].second = splitPoint - 1;
				r.data[rule.index].first = splitPoint;
			}
			else {
				copy.data[rule.index].first = splitPoint + 1;
				r.data[rule.index].second = splitPoint;
			}
			copy.index = rule.targetIndex;
			ranges.push(copy);
		}
		r.index = w.targetIndex;
		ranges.push(r);
	}

	cout << sum << endl;
}

void day19() {
	vector<string> lines = readLinesFromFile("./data/day19.txt");

	vector<Workflow> workflow;
	vector<array<int, 4>> parts;

	auto emptyLine = find_if(lines.begin(), lines.end(), [](string s) { return s.length() == 0; });
	for (auto it = lines.begin(); it != emptyLine; it++) {
		Workflow w = { "", 0, vector<Rule>(), "", 0 };
		int si = (*it).find('{');
		w.name = (*it).substr(0, si);
		int so = (*it).find('}');
		vector<string> parts = split((*it).substr(si + 1, so - si - 1), ',');
		for (int i = 0; i < parts.size() - 1; i++) {
			string p = parts[i];
			Rule r = { 0, false, 0, "", 0 };
			r.index = p[0] == 'x' ? 0 : p[0] == 'm' ? 1 : p[0] == 'a' ? 2 : 3;
			int semi = p.find(':');
			r.less = p[1] == '<';
			r.value = atoi(p.substr(2, semi - 2).c_str());
			r.targetName = p.substr(semi + 1);
			w.rules.push_back(r);
		}
		w.targetName = parts[parts.size() - 1];
		workflow.push_back(w);
	}

	map<string, int> namesToIndexes = { {"A", -1}, {"R", -2} };
	for (int i = 0; i < workflow.size(); i++) {
		namesToIndexes[workflow[i].name] = i;
		workflow[i].index = i;
	}

	for (Workflow& w : workflow) {
		if (namesToIndexes.count(w.targetName) == 0) {
			cout << "Not found key: " << w.targetName << endl;
		}
		w.targetIndex = namesToIndexes[w.targetName];
		for (Rule& r : w.rules) {
			if (namesToIndexes.count(r.targetName) == 0) {
				cout << "Not found key: " << r.targetName << endl;
			}
			r.targetIndex = namesToIndexes[r.targetName];
		}
	}

	for (auto it = emptyLine + 1; it != lines.end(); it++) {
		string values = (*it).substr(1, (*it).size() - 2);
		vector<string> steps = split(values, ',');
		array<int, 4> part;
		for (int i = 0; i < 4; i++) {
			part[i] = atoi(steps[i].substr(steps[i].find('=') + 1).c_str());
		}
		parts.push_back(part);
	}

	/*for (auto& w : workflow) {
		cout << w.index << ": ";
		for (auto& r : w.rules) {
			cout << r.index << (r.less ? '<' : '>') << r.value << ":" << r.targetIndex << ",";
		}
		cout << w.targetIndex << endl;
	}

	for (auto& p : parts) {
		cout << p[0] << "," << p[1] << "," << p[2] << "," << p[3] << endl;
	}*/

	part1(workflow, parts, namesToIndexes["in"]);

	part2(workflow, namesToIndexes["in"]);
}