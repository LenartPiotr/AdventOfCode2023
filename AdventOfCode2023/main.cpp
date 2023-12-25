#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>

#include "days.h"

using namespace std;

vector<string> readLinesFromFile(const string& fileName) {
    vector<string> lines;
    ifstream file(fileName);

    if (file.is_open()) {
        string line;
        while (getline(file, line)) {
            lines.push_back(line);
        }
        file.close();
    }
    else {
        cerr << "Unable to open file: " << fileName << endl;
    }

    return lines;
}

vector<string> split(const string& s, char delimiter) {
    vector<string> tokens;
    istringstream stream(s);
    string token;
    while (getline(stream, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

int main() {
    day25();
}