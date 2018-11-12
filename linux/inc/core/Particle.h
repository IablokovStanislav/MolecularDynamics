//
// Created by root on 28.10.2018.
//

#ifndef MD_PARTICLE_H
#define MD_PARTICLE_H

#include <string>
#include <list>
#include "Reaction.h"

using namespace std;

class Particle
{
public:
    Particle(string _type);
    double x, y, z;
    double r;
    string type;
    bool reacted = false;

    unsigned int particleId;

    list<Reaction> reactions; // list of reactions this particle participates in

    bool operator == (const Particle &_particle) const { return this->particleId == _particle.particleId;}

private:
    void __addReactions();

};


#endif //MD_PARTICLE_H
