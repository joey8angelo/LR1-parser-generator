#include "../headers/headers.h"

int main(){
    Grammar grammar;
    getGrammar(grammar);
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