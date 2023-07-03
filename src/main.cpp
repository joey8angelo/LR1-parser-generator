#include "../headers/headers.h"
#include "../headers/Grammar.h"
#include "../headers/State.h"
#include "../headers/StateMachine.h"

int main(){
    Grammar grammar;

    if(!getGrammar(grammar))
        return 1;
    makeFIRST(grammar);

    StateMachine sm(&grammar);
    sm.printStates();
    // make parsing table
    vector<vector<pair<char,int>>> table;
    vector<State*> states = sm.getStates();
    if(!makeTable(table, states, grammar))
        return 1;
    // parse inputs
    return 0;
}

bool getGrammar(Grammar& grammar){
    string start = "";
    std::ifstream file;
    string input;
    string lhs = "";
    int ID = 0;
    bool ret = true;

    file.open("../input.txt");

    if(!file.is_open())
        return false;

    std::getline(file, input);

    while(file){
        bool hasEpsilon = false;
        bool n = true;
        string curr = "";
        lhs = "";

        for (int i = 0; i < input.size(); i++){
            if(input[i] != ' ')
                curr += input[i];
            if((input[i] == ' ') || i == input.size()-1 && curr != ""){
                if(lhs == ""){
                    lhs = curr;
                    if(lhs == "%"){
                        cout << "Epsilon cannot be a nonTerminal" << endl;
                        ret = false;
                    }
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
                    if(curr != "%")
                        grammar.rules[ID].push_back(curr);
                    else
                        hasEpsilon = true;
                }
                curr = "";
            }
        }

        if(grammar.rules[ID].size() < 2 && !hasEpsilon){
            cout << "Production Rule must have a lhs and rhs '" << grammar.rules[ID][0] << " > ' is not a valid rule" << endl;
            ret = false;
        }

        ID++;

        std::getline(file, input);
    }

    // augment grammar with new start by adding ' 
    string augStart = start;
    grammar.start = start;

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

    // assign each token to an ID
    ID = 0;
    for(auto i = grammar.nonTerminals.begin(); i != grammar.nonTerminals.end(); i++){
        grammar.IDS[*i] = ID++;
    }
    for(auto i = grammar.terminals.begin(); i != grammar.terminals.end(); i++){
        grammar.IDS[*i] = ID++;
    }
    grammar.IDS["$"] = ID;

    return ret;
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
            if(grammar.rules[i].size() == 1){
                rhs.insert("%");
            }
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

bool makeTable(vector<vector<pair<char,int>>>& table, vector<State*>& states, Grammar& grammar){
    bool ret = true;

    for(int i = 0; i < states.size(); i++){
        table.push_back({});
        for(int j = 0; j < grammar.IDS.size(); j++){
            table[table.size()-1].push_back(make_pair('-',0));
        }
    }
    auto n = states[0]->transitions;
    auto m = n[grammar.start];
    table[states[0]->transitions[grammar.start]][grammar.IDS["$"]] = make_pair('A', 0);

    for(int i = 0; i < states.size(); i++){
        for(auto j = states[i]->transitions.begin(); j != states[i]->transitions.end(); j++){
            if(grammar.nonTerminals.find(j->first) != grammar.nonTerminals.end())
                table[i][grammar.IDS[j->first]] = make_pair('G', j->second);
            else
                table[i][grammar.IDS[j->first]] = make_pair('S', j->second);
        }
        for(int j = 0; j < states[i]->kernel.size(); j++){
            tuple<int,int,unordered_set<string>> k = states[i]->kernel[j];
            if(grammar.rules[get<0>(k)].size() == get<1>(k)){
                for(auto x = get<2>(k).begin(); x != get<2>(k).end(); x++){
                    if(table[i][grammar.IDS[*x]].first == 'S'){
                        cout << "SHIFT/REDUCE CONFLICT in state " << i << " on " << *x << 
                                " while attempting to reduce rule " << get<0>(k) << endl;
                        ret = false;
                    }
                    else if(table[i][grammar.IDS[*x]].first == 'R'){
                        cout << "REDUCE/REDUCE CONFLICT in state " << i << " on " << *x <<
                                " while attempting to reduce rule " << get<0>(k) << endl;
                        ret = false;
                    }
                    table[i][grammar.IDS[*x]] = make_pair('R', get<0>(k));
                }
            }
        }
    }
    return ret;
}