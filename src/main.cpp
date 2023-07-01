#include "../headers/headers.h"
#include "../headers/Grammar.h"
#include "../headers/State.h"
#include "../headers/StateMachine.h"

int main(){
    Grammar grammar;

    getGrammar(grammar);
    makeFIRST(grammar);

    StateMachine sm(&grammar);
    sm.printStates();
    return 0;
}

void getGrammar(Grammar& grammar){
    string start = "";
    std::ifstream file;
    string input;
    string lhs = "";
    int ID = 0;

    file.open("../input.txt");

    if(!file.is_open())
        return;

    std::getline(file, input);

    while(file){
        bool n = true;
        string curr = "";
        lhs = "";

        for (int i = 0; i < input.size(); i++){
            if(input[i] != ' ')
                curr += input[i];
            if((input[i] == ' ') || i == input.size()-1 && curr != ""){
                if(lhs == ""){
                    lhs = curr;
                    grammar.nonTerminals.insert(lhs);
                    if(grammar.rules.size() <= ID)
                        grammar.rules.push_back({lhs});
                    else
                        grammar.rules[ID].push_back(curr);

                    if(start == "")
                        start = lhs;
                }
                else{
                    if(grammar.rules[ID].size() == 1 && n){
                        n = false;
                        curr = "";
                        continue;
                    }
                    grammar.rules[ID].push_back(curr);
                }
                curr = "";
            }
        }

        if(grammar.rules[ID].size() < 2){
            cout << "Production Rule must have a lhs and rhs";
            return;
        }

        ID++;

        std::getline(file, input);
    }

    // augment grammar with new start by adding ' 
    string augStart = start;

    while(grammar.nonTerminals.find(augStart) != grammar.nonTerminals.end()){
        augStart += "'";
    }
    grammar.nonTerminals.insert(augStart);
    grammar.rules.insert(grammar.rules.begin(), {augStart, start});

    // find the terminals
    for(int i = 0; i < grammar.rules.size(); i++){
        for(int j = 0; j < grammar.rules[i].size(); j++){
            if(grammar.nonTerminals.find(grammar.rules[i][j]) == grammar.nonTerminals.end())
                grammar.terminals.insert(grammar.rules[i][j]);
        }
    }
}

void makeFIRST(Grammar& grammar){
    for (auto a = grammar.terminals.begin(); a != grammar.terminals.end(); a++){
        if(grammar._FIRST.find(*a) == grammar._FIRST.end()){
            grammar._FIRST[*a] = unordered_set<string>({*a});
        }
    }
    for (auto A = grammar.nonTerminals.begin(); A != grammar.nonTerminals.end(); A++){
        grammar._FIRST[*A] = unordered_set<string>();
    }

    bool firstChanged = true;

    while (firstChanged) {
        firstChanged = false;

        for (int i = 0; i < grammar.rules.size(); i++){
            bool cont = true;
            unordered_set<string> rhs;
            for (int j = 1; j < grammar.rules[i].size() && cont; j++){
                string B = grammar.rules[i][j];
                for (auto n = grammar._FIRST[B].begin(); n != grammar._FIRST[B].end(); n++){
                    if (*n != "%" || j == grammar.rules[i].size()-1)
                        rhs.insert(*n);
                }
                if (grammar._FIRST[B].find("%") == grammar._FIRST[B].end())
                    cont = false;
            }
            for (auto r = rhs.begin(); r != rhs.end(); r++){
                if (grammar._FIRST[grammar.rules[i][0]].find(*r) == grammar._FIRST[grammar.rules[i][0]].end())
                    firstChanged = true;
                grammar._FIRST[grammar.rules[i][0]].insert(*r);
            }
            rhs.clear();
        }
    }
}