#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>

#include "days.h"
#include "Table.h"

using namespace std;

struct Vector3 {
	long long x, y, z;

	Vector3 crossProduct2(const Vector3& v2) const {
		return { x * v2.y - y * v2.x, y * v2.x - x * v2.y, 0 };
	}
	long long dot2(const Vector3& v2) const {
		return x * v2.x + y * v2.y;
	}
	Vector3 crossProduct3(const Vector3& v2) const {
		return { y * v2.z - z * v2.y, z * v2.x - x * v2.z, x * v2.y - y * v2.x };
	}
	long long dot3(const Vector3& v2) const {
		return x * v2.x + y * v2.y + z * v2.z;
	}

	Vector3 operator+(const Vector3& other) const {
		return { x + other.x, y + other.y, z + other.z };
	}
	Vector3 operator-(const Vector3& other) const {
		return { x - other.x, y - other.y, z - other.z };
	}
	Vector3 operator*(int scalar) const {
		return { x * scalar, y * scalar, z * scalar };
	}
	double operator/(const Vector3& other) const {
		if (other.x != 0 && x != 0) return x / (double)other.x;
		if (other.y != 0 && y != 0) return y / (double)other.y;
		if (other.z != 0 && z != 0) return z / (double)other.z;
		return 0;
	}
};

struct SemiStraight {
	Vector3 p;
	Vector3 v;
};

struct Cross
{
	int index1;
	int index2;
	double crossX;
	double crossY;
	double t;
	double u;
};

vector <SemiStraight> readSemiStraights(const vector<string>& lines) {
	vector<SemiStraight> sstraights;
	char skip;
	for (auto& l : lines) {
		Vector3 point;
		Vector3 direction;
		istringstream stream(l.substr(0, l.find('@')));
		stream >> point.x >> skip >> point.y >> skip >> point.z;
		istringstream stream2(l.substr(l.find('@') + 1));
		stream2 >> direction.x >> skip >> direction.y >> skip >> direction.z;
		sstraights.push_back({ point, direction });
	}
	return sstraights;
}

vector<Cross> part1(vector<SemiStraight> ss, long long areaMin, long long areaMax) {
	int sum = 0;
	vector<Cross> crosses;
	for (int i = 0; i < ss.size(); i++) {
		for (int j = i + 1; j < ss.size(); j++) {
			// cout << i << " x " << j << ": ";
			SemiStraight s1 = ss[i];
			SemiStraight s2 = ss[j];
			Vector3 vProduct = s1.v.crossProduct2(s2.v);
			Vector3 dProduct = (s2.p - s1.p).crossProduct2(s1.v);
			bool cond1 = vProduct.x == 0 && vProduct.y == 0;
			bool cond2 = dProduct.x == 0 && dProduct.y == 0;
			if (cond1 && cond2) {
				double t0 = (s2.p - s1.p).dot2(s1.v) / (double)s1.v.dot2(s1.v);
				double t1 = t0 + s2.v.dot2(s1.v) / (double)s1.v.dot2(s1.v);
				cout << "COLINEARS!" << endl;
			}
			else if (!cond1) {
				double t = (s2.p - s1.p).crossProduct2(s2.v) / vProduct;
				double u = (s2.p - s1.p).crossProduct2(s1.v) / vProduct;
				if (t >= 0 && u >= 0) {
					double crossX = s1.p.x + s1.v.x * t;
					double crossY = s1.p.y + s1.v.y * t;
					crosses.push_back({ i, j, crossX, crossY, t, u });
					if (crossX < areaMin || crossX > areaMax || crossY < areaMin || crossY > areaMax) {
						// cout << "outside" << endl;
						continue;
					}
					sum++;
					// cout << "METTING " << crossX << " " << crossY << endl;
				}
				else {
					// cout << "none 2" << endl;
				}
			}
			else {
				// cout << "none" << endl;
			}
		}
	}
	cout << sum << endl;
	return crosses;
}

bool same(const double& d1, const double& d2) {
	return abs(d1 - d2) < 1e-14;
}

void part2(vector<SemiStraight> ss, vector<Cross> cr) {
	for (auto& c : cr) {
		auto s1 = ss[c.index1];
		auto s2 = ss[c.index2];
		double crossZ1 = s1.p.z + s1.v.z * c.t;
		double crossZ2 = s2.p.z + s2.v.z * c.u;
		if (same(crossZ1, crossZ2)) {
			cout << "same: " << c.crossX << " " << c.crossY << " " << crossZ1 << endl;
		}
	}
}

void day24() {
	vector<string> lines = readLinesFromFile("./data/day24.txt");
	auto semiStraights = readSemiStraights(lines);
	auto cr = part1(semiStraights, 200000000000000, 400000000000000);
	// part2(semiStraights, 200000000000000, 400000000000000);
	part2(semiStraights, cr);
}