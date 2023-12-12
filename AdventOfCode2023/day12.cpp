#include <iostream>
#include <vector>
#include <functional>
#include <string>
#include <sstream>
#include <algorithm>

#include "days.h"
#include "hash.cpp"

using namespace std;

int countArrangements(const string& springs, const vector<int>& nums) {
	int size = springs.length();
	int* tab = new int[size];
	for (int i = 0; i < size; i++) {
		tab[i] = springs[i] == '?' ? 2 : springs[i] == '#' ? 1 : 0;
	}
	vector<int> forwardIndexes;
	vector<int> backwardsIndexes;

	int i = 0, mi, fi;
	for (auto& num : nums) {
		while ((i - 1 >= 0 && tab[i - 1] == 1) || (i + num < size && tab[i + num] == 1)) i++;
		mi = i + num - 1;
		fi = i;	
		while (i <= mi && i < size) {
			if (tab[i] == 0) {
				do i++; while ((i - 1 >= 0 && tab[i - 1] == 1) || (i + num < size && tab[i + num] == 1));
				mi = i + num - 1;
				fi = i;
				continue;
			}
			i++;
		}
		i++;
		forwardIndexes.push_back(fi);
	}

	i = size - 1;
	for (auto inum = nums.rbegin(); inum != nums.rend(); inum++) {
		int num = *inum;
		while ((i + 1 < size && tab[i + 1] == 1) || (i - num >= 0 && tab[i - num] == 1)) i--;
		mi = i - num + 1;
		fi = i;
		while (i >= mi && i >= 0) {
			if (tab[i] == 0) {
				do i--; while ((i + 1 < size && tab[i + 1] == 1) || (i - num >= 0 && tab[i - num] == 1));
				mi = i - num + 1;
				fi = i;
				continue;
			}
			i--;
		}
		i--;
		backwardsIndexes.push_back(fi);
	}

	reverse(backwardsIndexes.begin(), backwardsIndexes.end());
	function<int(int, int)> rec;

	rec = [&tab, &nums, &size, &forwardIndexes, &backwardsIndexes, &rec](int istart, int ni) -> int {
		if (ni >= nums.size()) {
			for (int i = istart; i < size; i++)
				if (tab[i] == 1)
					return 0;
			return 1;
		}
		if (istart >= size) return 0;
		int num = nums[ni];
		int imax = backwardsIndexes[ni] - num + 1;
		int sum = 0;
		for (int i = istart; i <= imax; i++) {
			if (tab[i] == 1) {
				imax = i;
				break;
			}
		}
		istart = max(istart, forwardIndexes[ni]);
		for (int i = istart; i <= imax; i++) {
			if (i - 1 >= 0 && tab[i - 1] == 1) continue;
			if (i + num < size && tab[i + num] == 1) continue;
			bool good = true;
			for (int k = i; k <= i + num - 1; k++) {
				if (tab[k] == 0) {
					good = false;
					i = k;
					break;
				}
			}
			if (!good) continue;
			int ret = rec(i + num + 1, ni + 1);
			sum += ret;
		}
		return sum;
	};

	int sum = rec(0, 0);
	delete[] tab;
	return sum;
}

void day12() {
	vector<string> lines = readLinesFromFile("./data/day12.txt");

	vector<string> springs;
	vector<vector<int>> numbers;
	int num;

	for (auto& line : lines) {
		int spaceIndex = line.find(' ');
		springs.push_back(line.substr(0, spaceIndex));
		string rest = line.substr(spaceIndex + 1);
		replace(rest.begin(), rest.end(), ',', ' ');
		istringstream stream(rest);
		vector<int> nums;
		while (stream >> num) nums.push_back(num);
		numbers.push_back(nums);
	}
	
	int sum = 0, val;
	for (int i = 0; i < numbers.size(); i++) {
		val = countArrangements(springs[i], numbers[i]);
		sum += val;
	}

	cout << sum << endl;

	sum = 0;
	for (int i = 0; i < numbers.size(); i++) {
		string s;
		vector<int> v;
		for (int k = 0; k < 5; k++) {
			s.append(springs[i]);
			for (auto& num : numbers[i]) {
				v.push_back(num);
			}
		}
		val = countArrangements(s, v);
		sum += val;
	}

	cout << sum << endl;
}