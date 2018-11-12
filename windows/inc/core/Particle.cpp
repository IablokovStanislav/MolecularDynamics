//
// Created by root on 28.10.2018.
//

#include <iostream>
#include <string>
#include <map>
#include "Particle.h"
#include "System.h"

using namespace std;

Particle::Particle(string _type)
{
    this->type = _type;

    // every time new particle is being created we add all reaction types it participates in
    this->__addReactions();

};

void Particle::__addReactions()
{
    System* system = System::getInstance();
    //for(pair<string, Reaction> reactionItem: system->reactions)
    for(auto const & [reactionName, reaction]: system->reactions)
    {
        //Reaction reaction = reactionItem.second;
        if(reaction.reactants.find(this->type) != reaction.reactants.end()) // found!
        {
            this->reactions.push_back(reaction);
            //cout << "added reaction " << reaction.name << " for " << this->type << endl;
        }
    }
};