#include "../headers/Grammar.h"
#include "../headers/headers.h"

// 0: A > BC.DE, $,a,b
// FIRST(0, 3, {$,a,b}) == FIRST(D) U FIRST(E) U {$,a,b} if epsilon in FIRST(D) and FIRST(E)
unordered_set<string> Grammar::FIRST(int rule, int pos, unordered_set<string> look){
    unordered_set<string> vals;

    bool cont = true;
    for(int i = pos; i < this->rules[rule].size() && cont; i++){
        cont = false;
        unordered_set<string> firstSet = this->_FIRST[this->rules[rule][i]];
        for(auto j = firstSet.begin(); j != firstSet.end(); j++){
            if(*j == "%")
                cont = true;
            else
                vals.insert(*j);
        }
    }
    if(cont){
        for(auto j = look.begin(); j != look.end(); j++){
            vals.insert(*j);
        }
    }
    return vals;
}