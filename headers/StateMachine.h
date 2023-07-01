#pragma once
#include "State.h"
#include "headers.h"

class StateMachine{
    public:

    StateMachine(Grammar*);
    ~StateMachine(){}

    void printStates();
    string stringSet(unordered_set<string>&);

    private:

    State* makeState(vector<tuple<int,int,unordered_set<string>>>);

    State* start;
    vector<State*> states;
    Grammar* grammar;
};