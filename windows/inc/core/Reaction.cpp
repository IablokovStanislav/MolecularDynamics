//
// Created by root on 05.11.2018.
//

#include "Reaction.h"

Reaction::Reaction(string _name)
{
    this->name = _name;
};

void Reaction::setProbability(double _probability)
{
    this->probability = _probability;
};

void Reaction::addReactant(string _reactantName, unsigned int _stochiometry)
{
    this->reactants[_reactantName] = _stochiometry;
};

void Reaction::addProduct(string _productName, unsigned int _stochiometry)
{
    this->products[_productName] = _stochiometry;
};