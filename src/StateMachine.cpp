#include "../headers/StateMachine.h"
#include "../headers/Grammar.h"
#include "../headers/headers.h"

StateMachine::StateMachine(Grammar* grammar) : start(nullptr), grammar(grammar) {
    this->start = makeState({make_tuple(0,1,unordered_set<string>({"$"}))});
}

State* StateMachine::makeState(vector<tuple<int,int,unordered_set<string>>> k){
    State* state = new State(k);

    // if state exists already return it
    for(int i = 0; i < this->states.size(); i++){
        if(*state == *this->states[i]){
            delete state;
            return this->states[i];
        }
    }

    // if state does not exist add it to the list of existing states
    this->states.push_back(state);

    // create state transitions
    auto transitions = state->makeTransitions(this->grammar);

    for(auto i = transitions.begin(); i != transitions.end(); i++){
        state->transitions[i->first] = makeState(i->second);
    }

    return state;
}

void StateMachine::printStates(){
    for(int i = 0; i < this->states.size(); i++){
        cout << "State " << i << " :" << endl;
        // print kernels
        for(int j = 0; j < this->states[i]->kernel.size(); j++){
            int first = get<0>(this->states[i]->kernel[j]);
            int second = get<1>(this->states[i]->kernel[j]);
            unordered_set<string> third = get<2>(this->states[i]->kernel[j]);
            cout << "    " << this->grammar->rules[first][0] << " >";
            for(int x = 1; x < this->grammar->rules[first].size(); x++){
                cout << " ";
                if(x == second)
                    cout << ".";
                cout << this->grammar->rules[first][x];
            }
            if(second == this->grammar->rules[first].size())
                cout << " .";
            cout << " , {" << this->stringSet(third) << "}" << endl;
        }

        // print closures
        cout << "    ----------------" << endl;
        for(auto j = this->states[i]->closed.begin(); j != this->states[i]->closed.end(); j++){
            int first = j->first;
            int second = 1;
            unordered_set<string> third = j->second;

            cout << "    " << this->grammar->rules[j->first][0] << " >";
            for(int x = 1; x < this->grammar->rules[j->first].size(); x++){
                cout << " ";
                if(x == 1)
                    cout << ".";
                cout << this->grammar->rules[j->first][x];
            }
            if(second == this->grammar->rules[first].size())
                cout << " .";
            cout << " , {" << this->stringSet(j->second) << "}" << endl;
        }

        // print transitions
        cout << "    Transitions:" << endl;
        for(auto j = this->states[i]->transitions.begin(); j != this->states[i]->transitions.end(); j++){
            cout << "        " << j->first << " -> ";
            State* state = j->second;
            for(int x = 0; x < this->states.size(); x++){
                if(state == this->states[x]){
                    cout << "State " << x << endl;
                    break;
                }
            }
        }
        cout << endl;
    }
}

string StateMachine::stringSet(unordered_set<string>& set){
    auto i = set.begin();
    string res = *i;
    i++;
    for(; i != set.end(); i++){
        res += ", " + *i;
    }
    return res;
}