#pragma once
#include "headers.h"

class Grammar {
    public:
    unordered_set<string> FIRST(int, int, unordered_set<string>);
    vector<vector<string>> rules;
    unordered_set<string> nonTerminals;
    unordered_set<string> terminals;
    unordered_map<string, unordered_set<string>> _FIRST;
};