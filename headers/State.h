#pragma once
#include "headers.h"

class State{
    public:

    State(vector<tuple<int,int,unordered_set<string>>> k) : kernel(k) {}
    ~State(){}

    bool operator==(const State&);
    unordered_map<string, vector<tuple<int,int,unordered_set<string>>>> makeTransitions(Grammar*);

    unordered_map<string, int> transitions;
    vector<tuple<int,int,unordered_set<string>>> kernel;
    unordered_map<int, unordered_set<string>> closed;
};