//
// Created by root on 05.11.2018.
//

#ifndef CODE_REACTION_H
#define CODE_REACTION_H

#include <string>
#include <map>

using namespace std;

class Reaction
{
public:
    Reaction(string _name);
    Reaction() {}; // for iterations in containers
    void addReactant(string _reactantName, unsigned int _stochiometry);
    void addProduct(string _productName, unsigned int _stochiometry);
    void setProbability(double _probability);

    string name;
    double probability = 0.0;
    map<string, unsigned int> reactants = {};
    map<string, unsigned int> products = {};
};


#endif //CODE_REACTION_H
