#pragma once
#include <iostream>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <string>
#include <fstream>
#include <math.h>
#include <tuple>
#include <bits/stdc++.h>

using std::vector, std::string, std::pair, std::unordered_set, std::unordered_map, std::cout, std::endl, std::make_tuple, std::make_pair, std::tuple, std::get;

class Grammar;
class State;

bool getGrammar(Grammar&);
void makeFIRST(Grammar&);
bool makeTable(vector<vector<pair<char,int>>>&, vector<State*>&, Grammar&);
bool parse(vector<vector<pair<char,int>>>&, Grammar&, vector<State*>&);