#pragma once
#include <iostream>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <string>
#include <fstream>
#include <math.h>

using std::vector, std::string, std::pair, std::unordered_set, std::unordered_map, std::cout, std::endl;

struct Grammar {
    vector<vector<string>> rules;
    unordered_set<string> nonTerminals;
    unordered_set<string> terminals;
    unordered_map<string, unordered_set<string>> FIRST;
};

void getGrammar(Grammar&);