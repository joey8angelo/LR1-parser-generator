#include "../headers/State.h"
#include "../headers/Grammar.h"
#include "../headers/headers.h"

bool State::operator==(const State& s){
    if(this->kernel.size() != s.kernel.size())
        return false;
    
    for(int i = 0; i < s.kernel.size(); i++){
        auto a = this->kernel[i];
        auto b = s.kernel[i];
        if(get<0>(a) != get<0>(b) || get<1>(a) != get<1>(b) || get<2>(a) != get<2>(b))
            return false;
    }
    return true;
}

// build closures and create temporary transitions from them
unordered_map<string, vector<tuple<int,int,unordered_set<string>>>> State::makeTransitions(Grammar* grammar){
    // token mapped to kernels
    unordered_map<string, vector<tuple<int,int,unordered_set<string>>>> transitions;
    // init closures
    for(int i = 0; i < this->kernel.size(); i++){
        int first = get<0>(this->kernel[i]);
        int second = get<1>(this->kernel[i]);
        unordered_set<string> third = get<2>(this->kernel[i]);

        if(second < grammar->rules[first].size() && grammar->nonTerminals.find(grammar->rules[first][second]) != grammar->nonTerminals.end()){
            for(int j = 0; j < grammar->rules.size(); j++){
                if(grammar->rules[j][0] == grammar->rules[first][second]){
                    this->closed[j] = grammar->FIRST(first, second+1, third);
                }
            }
        }
        if(second < grammar->rules[first].size()){
            if(transitions.find(grammar->rules[first][second]) == transitions.end())
                transitions[grammar->rules[first][second]] = {make_tuple(first, second+1, third)};
            else
                transitions[grammar->rules[first][second]].push_back(make_tuple(first, second, third));
        }
    }

    // compute closures
    bool closedChanged = true;
    while(closedChanged){
        closedChanged = false;

        // each item in closed
        for(auto i = this->closed.begin(); i != this->closed.end(); i++){
            if(grammar->rules[i->first].size() == 1)
                continue;
            string first = grammar->rules[i->first][1];

            // if the dotted position in the rule is nonTerm
            if(grammar->nonTerminals.find(first) != grammar->nonTerminals.end()){
                // find rules with same nonTerm as lhs
                for(int r = 0; r < grammar->rules.size(); r++){
                    if(grammar->rules[r][0] == first){
                        // if rule not in closed add empty set for lookaheads
                        if(this->closed.find(r) == this->closed.end()){
                            this->closed[r] = unordered_set<string>();
                            closedChanged = true;
                        }
                        // fill lookaheads with FIRST()
                        unordered_set<string> looks = grammar->FIRST(i->first, 2, i->second);
                        for(auto j = looks.begin(); j != looks.end(); j++){
                            if(this->closed[r].find(*j) == this->closed[r].end())
                                closedChanged = true;
                            closed[r].insert(*j);
                        }
                    }
                }
            }
        }
    }

    for(auto i = closed.begin(); i != closed.end(); i++){
        if(1 < grammar->rules[i->first].size()){
            if(transitions.find(grammar->rules[i->first][1]) == transitions.end())
                transitions[grammar->rules[i->first][1]] = {make_tuple(i->first, 2, i->second)};
            else
                transitions[grammar->rules[i->first][1]].push_back(make_tuple(i->first, 2, i->second));
        }
    }

    // make sure kernels are sorted for ease of comparison
    for(auto i = transitions.begin(); i != transitions.end(); i++){
        std::sort(i->second.begin(), i->second.end(), 
            [](const tuple<int,int,unordered_set<string>>& a,
            const tuple<int,int,unordered_set<string>>& b) -> bool{
                if(get<0>(a) != get<0>(b))
                    return get<0>(a) < get<0>(b);
                else
                    return get<1>(a) < get<1>(b);
            });
    }

    return transitions;
}