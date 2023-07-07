#pragma once
#include "State.h"
#include "headers.h"

class StateMachine{
    public:

    StateMachine(Grammar*);
    ~StateMachine(){
        for(int i = 0; i < this->states.size(); i++){
            delete this->states[i];
        }
    }

    void printStates();
    string stringSet(unordered_set<string>&);
    vector<State*> getStates();

    private:

    int makeState(vector<tuple<int,int,unordered_set<string>>>);

    vector<State*> states;
    Grammar* grammar;
};