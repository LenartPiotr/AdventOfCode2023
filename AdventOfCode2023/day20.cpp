#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <map>
#include <queue>

#include <fstream>

#include "days.h"
#include "hash.cpp"

using namespace std;

enum ModuleType {
	FLIP_FLOP, CONJUNCTION, BROADCAST, OUTPUT
};

struct Connect {
	string name;
	int id;
	int inputId;
};

struct Module {
	ModuleType type;
	string name;
	int index;
	vector<Connect> connections;
	int memoryIndex;
};

struct Impulse {
	int id;
	bool high;
	int inputId;
};

struct ConjunctionMemory {
	vector<bool> signals;
	int highCount;
};

ostream& operator<<(ostream& os, const ConjunctionMemory& conMem) {
	for (bool signal : conMem.signals) {
		os << signal;
	}
	return os;
}

void part1(vector<Module> modules, vector<bool> memFlip, vector<ConjunctionMemory> memCon, int idBroadcaster) {
	queue<Impulse> Q;
	bool state, allHighs;
	int flipFlopOnCount = 0;
	bool memConjAllOff;
	int totalLows = 0;
	int totalHighs = 0;

	vector<int> highsMem;
	vector<int> lowsMem;

	int cycle = 0;

	do {
		cycle++;
		int lowsImpulsesCount = 0;
		int highImpulsesCount = 0;
		Q.push({ idBroadcaster, false, 0 });
		while (!Q.empty()) {
			Impulse imp = Q.front();
			Q.pop();
			// cout << imp.id << " " << imp.high << " " << imp.inputId << endl;
			if (imp.high) highImpulsesCount++;
			else lowsImpulsesCount++;
			Module m = modules[imp.id];
			switch (m.type)
			{
			case BROADCAST:
				for (Connect c : m.connections) {
					Q.push({ c.id, imp.high, c.inputId });
				}
				break;
			case FLIP_FLOP:
				if (imp.high) break;
				state = memFlip[m.memoryIndex];
				state = !state;
				if (state) flipFlopOnCount++;
				else flipFlopOnCount--;
				memFlip[m.memoryIndex] = state;
				for (Connect c : m.connections) {
					Q.push({ c.id, state, c.inputId });
				}
				break;
			case CONJUNCTION:
				state = memCon[m.memoryIndex].signals[imp.inputId];
				if (state != imp.high) {
					if (imp.high) memCon[m.memoryIndex].highCount++;
					else memCon[m.memoryIndex].highCount--;
					memCon[m.memoryIndex].signals[imp.inputId] = imp.high;
				}
				allHighs = memCon[m.memoryIndex].highCount == memCon[m.memoryIndex].signals.size();
				for (Connect c : m.connections) {
					Q.push({ c.id, !allHighs, c.inputId });
				}
				break;
			}
		}
		totalLows += lowsImpulsesCount;
		totalHighs += highImpulsesCount;
		lowsMem.push_back(lowsImpulsesCount);
		highsMem.push_back(highImpulsesCount);
		// cout << lowsImpulsesCount << " " << highImpulsesCount << endl;
		memConjAllOff = find_if(memCon.begin(), memCon.end(), [](const ConjunctionMemory& mem) { return mem.highCount != 0; }) == memCon.end();
	} while ((flipFlopOnCount != 0 || !memConjAllOff) && cycle < 1000);

	cout << "Cycle: " << cycle << endl;
	cout << "Total lows: " << totalLows << endl;
	cout << "Total highs: " << totalHighs << endl;
	int times = 1000;
	int result;
	int r = (times % cycle);
	if (r == 0) {
		result = (times / cycle) * totalLows * (times / cycle) * totalHighs;
	}
	else {
		cout << "diff res" << endl;
		result = (times / cycle) * totalLows * (times / cycle) * totalHighs + (lowsMem[r - 1] * highsMem[r - 1]);
	}
	cout << result << endl;
}

void printConjunctionMemory(ConjunctionMemory m) {
	for (int i = 0; i < m.signals.size(); i++) {
		cout << m.signals[i];
	}
}

void part2(vector<Module> modules, vector<bool> memFlip, vector<ConjunctionMemory> memCon, map<string, int> names) {
	queue<Impulse> Q;
	bool state, allHighs;

	int cycle = 0;
	int highRxCount = 0;
	int lowRxCount = 0;
	
	int idBroadcaster = names["broadcaster"];

	int i_ds = names["ds"];
	int i_dt = names["dt"];
	int i_bd = names["bd"];
	int i_cs = names["cs"];

	int id[4] = { i_ds, i_dt, i_bd, i_cs };
	vector<vector<int>> values;
	int signalsLeftToDetermine = 0;
	for (int i = 0; i < 4; i++) {
		vector<int> v;
		int s = memCon[modules[id[i]].memoryIndex].signals.size();
		for (int j = 0; j < s; j++) v.push_back(-1);
		values.push_back(v);
		signalsLeftToDetermine += s;
	}

	ofstream file("./temp-data/d.txt");
	if (!file.is_open()) {
		cout << "No file open" << endl;
		return;
	}

	do {
		// if (cycle == 4096)
		{
			file << cycle << ": " << memCon[modules[i_ds].memoryIndex] << " " << memCon[modules[i_dt].memoryIndex]
				<< " " << memCon[modules[i_bd].memoryIndex] << " " << memCon[modules[i_cs].memoryIndex] << "\n";
		}
		/**/

		cycle++;
		highRxCount = 0;
		lowRxCount = 0;
		Q.push({ idBroadcaster, false, 0 });
		while (!Q.empty()) {
			Impulse imp = Q.front();
			Q.pop();
			Module m = modules[imp.id];
			switch (m.type)
			{
			case BROADCAST:
				for (Connect c : m.connections) {
					Q.push({ c.id, imp.high, c.inputId });
				}
				break;
			case FLIP_FLOP:
				if (imp.high) break;
				state = memFlip[m.memoryIndex];
				state = !state;
				memFlip[m.memoryIndex] = state;
				for (Connect c : m.connections) {
					Q.push({ c.id, state, c.inputId });
				}
				break;
			case CONJUNCTION:
				state = memCon[m.memoryIndex].signals[imp.inputId];
				if (state != imp.high) {
					if (imp.high) memCon[m.memoryIndex].highCount++;
					else memCon[m.memoryIndex].highCount--;
					memCon[m.memoryIndex].signals[imp.inputId] = imp.high;
				}
				allHighs = memCon[m.memoryIndex].highCount == memCon[m.memoryIndex].signals.size();
				for (Connect c : m.connections) {
					Q.push({ c.id, !allHighs, c.inputId });
				}
				break;
			case OUTPUT:
				if (imp.high) highRxCount++;
				else lowRxCount++;
				break;
			}
		}

		for (int i = 0; i < 4; i++) {
			Module m = modules[id[i]];
			ConjunctionMemory mem = memCon[m.memoryIndex];
			int s = mem.signals.size();
			for (int j = 0; j < s; j++) {
				if (values[i][j] == -1 && mem.signals[j]) {
					values[i][j] = cycle;
					signalsLeftToDetermine--;
					cout << "Signal " << i << " / " << j << " repeating at " << cycle << " | " << signalsLeftToDetermine << " left" << endl;
				}
			}
		}

	} while (signalsLeftToDetermine > 0 || cycle < 5000);

	for (int i = 0; i < 4; i++) {
		Module m = modules[id[i]];
		ConjunctionMemory mem = memCon[m.memoryIndex];
		int s = mem.signals.size();
		for (int j = 0; j < s; j++) {
			cout << values[i][j] << " ";
		}
		cout << "| ";
	}
	cout << endl;

	file.close();
}

void day20() {
	vector<string> lines = readLinesFromFile("./data/day20.txt");

	vector<bool> memoryFlipFlop;
	vector<ConjunctionMemory> memoryHighConjunction;
	
	vector<Module> modules;
	map<string, int> namesToIndexes;

	for (int i = 0; i < lines.size(); i++) {
		const string& l = lines[i];
		Module m;
		m.index = i;
		int firstSpace = l.find(' ');
		string nameWithPrefix = l.substr(0, firstSpace);
		char firstLetter = nameWithPrefix[0];
		if (firstLetter == '%' || firstLetter == '&') m.name = nameWithPrefix.substr(1);
		else m.name = nameWithPrefix;
		m.type = firstLetter == '%' ? FLIP_FLOP : firstLetter == '&' ? CONJUNCTION : firstLetter == 'b' ? BROADCAST : OUTPUT;
		if (m.type == FLIP_FLOP) {
			m.memoryIndex = memoryFlipFlop.size();
			memoryFlipFlop.push_back(false);
		}
		else if (m.type == CONJUNCTION) {
			m.memoryIndex = memoryHighConjunction.size();
			ConjunctionMemory mem = { {}, 0 };
			memoryHighConjunction.push_back(mem);
		}
		else m.memoryIndex = 0;
		const auto& splits = split(l.substr(firstSpace + 3), ',');
		for (auto& s : splits) {
			Connect conn;
			conn.name = s.substr(1);
			m.connections.push_back(conn);
		}
		modules.push_back(m);
		namesToIndexes[m.name] = m.index;
	}
	Module outModule = { OUTPUT, "output", modules.size(), {}, 0 };
	namesToIndexes["output"] = modules.size();
	modules.push_back(outModule);

	for (Module& m : modules) {
		for (Connect& c : m.connections) {
			c.inputId = 0;
			if (namesToIndexes.count(c.name) == 0) {
				c.id = outModule.index;
				continue;
			}
			c.id = namesToIndexes[c.name];
			if (modules[c.id].type == CONJUNCTION) {
				c.inputId = memoryHighConjunction[modules[c.id].memoryIndex].signals.size();
				memoryHighConjunction[modules[c.id].memoryIndex].signals.push_back(false);
			}
		}
	}

	/* PRINT MODULES
	for (int i = 0; i < modules.size(); i++) {
		const Module& m = modules[i];
		cout << (m.type == CONJUNCTION ? "& " : "") << m.name << " ->";
		for (const Connection& c : m.connections) {
			cout << " " << (modules[c.id].type == CONJUNCTION ? "& " : "") << c.name;
		}
		cout << endl;
	}
	/**/

	// part1(modules, memoryFlipFlop, memoryHighConjunction, namesToIndexes["broadcaster"]);
	part2(modules, memoryFlipFlop, memoryHighConjunction, namesToIndexes);
}