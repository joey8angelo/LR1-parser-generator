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

using std::vector, std::string, std::pair, std::unordered_set, std::unordered_map, std::cout, std::endl, std::make_tuple, std::tuple, std::get;

class Grammar;

void getGrammar(Grammar&);
void makeFIRST(Grammar&);