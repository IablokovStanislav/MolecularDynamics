//
// Created by root on 28.10.2018.
//
#include <iostream>
#include <ctime>
#include <vector>
#include <cmath>
#include <string>
#include <fstream>
#include <tuple>
#include <set>

#include "System.h"
#include "Particle.h"

#define PI 3.1415926

System* System::__instance = nullptr;

System::System()
{
    srand(unsigned(time(0)));
}

System* System::getInstance()
{
    if (__instance == nullptr)
    {
        __instance = new System();
    }

    return __instance;
}

void System::addParticle(Particle _particle, bool _random)
{
    if(_random)
    {
        double rnd;
        rnd = (double)(rand() % 10000) / 10000.0;
        _particle.x = this->__boxWidth * rnd;

        rnd = (double)(rand() % 10000) / 10000.0;
        _particle.y = this->__boxDepth * rnd;

        rnd = (double)(rand() % 10000) / 10000.0;
        _particle.z = this->__boxHeight * rnd;

    }

    _particle.particleId = this->__particleIdCounter++;
    this->__particles.push_back(_particle);
};

void System::addReaction(Reaction _reaction)
{
    this->reactions[_reaction.name] = _reaction;
};


void System::setGeometry(double _boxWidth, double _boxDepth, double _boxHeight)
{
    this->__boxWidth = _boxWidth;
    this->__boxDepth = _boxDepth;
    this->__boxHeight = _boxHeight;
};

void System::setTemperature(double _temperature)
{
    this->__temperature = _temperature;
};

void System::setPressure(double _pressure)
{
    this->__pressure = _pressure;
};

void System::setBoundaryConditions(bool _bcX, bool _bcY, bool _bcZ)
{
    this->__bcX = _bcX;
    this->__bcY = _bcY;
    this->__bcZ = _bcZ;
};

void System::setRB(double _rb)
{
    this->__RB = _rb;
}

///////////////////////////////////////////////////////////////

void System::__initRandomSequence(unsigned long _randomSequenceLength)
{
    this->__randomSequenceLength = _randomSequenceLength;
    this->__randomSequence.resize(_randomSequenceLength);

    for(unsigned long i = 0; i < _randomSequenceLength; i++)
        this->__randomSequence[i] = (double)rand() / RAND_MAX;

    this->__randomSequenceIndex = 0;
};

double System::__random()
{
    if(this->__randomSequenceIndex >= this->__randomSequence.size())
        this->__randomSequenceIndex = 0;

    return this->__randomSequence[this->__randomSequenceIndex++];
}

double System::__randomBD(double _dt)
{
    return pow(abs(-2.0 * 2.0 * _dt * this->__RB * log(this->__random() + 0.000000001)), 0.5) * cos(2.0 * PI * this->__random());
}

void System::__saveCoordinates(string _fname)
{
    ofstream fOut(_fname, ofstream::out);
    for (auto &particle : this->__particles)
    {
        fOut << particle.particleId << " " << particle.type << " " << particle.x << " " << particle.y << " " << particle.z << endl;
    }

    fOut.close();
}

double System::__interactionEnergy(Particle &_particle1, Particle &_particle2)
{
    double r = sqrt(pow(_particle1.x - _particle2.x, 2) + pow(_particle1.y - _particle2.y, 2)) + pow(_particle1.z - _particle2.z, 2);
    return 1 / (r+0.00001);
}

void System::run(int _steps, double _dt)
{
    this->__initRandomSequence(10000000);

    // timestep cycle
    for(int step = 0; step < _steps; step++)
    {
        // update particle positions
        for (auto &particle : this->__particles)
        {
            // make brownian motion update
            particle.x += this->__randomBD(_dt);
            particle.y += this->__randomBD(_dt);
            particle.z += this->__randomBD(_dt);

            // fix for boundaries
            if(this->__bcX)
                particle.x -= rint(particle.x / this->__boxWidth - 0.5) * this->__boxWidth;
            if(this->__bcY)
                particle.y -= rint(particle.y / this->__boxDepth - 0.5) * this->__boxDepth;
            if(this->__bcZ)
                particle.z -= rint(particle.z / this->__boxHeight - 0.5) * this->__boxHeight;

        }

        // reaction cycle
        list<pair<Reaction *, map<string, list<Particle *>>>> reactionList;
        for (auto &particle1 : this->__particles)
        {
            // initialize counts for reaction's initial particles (reactants)
            // using pointers for Reaction objects (!)
            map<Reaction *, map<string, list<Particle *>>> reactionData = {};

            for (auto &reaction : particle1.reactions)
            {
                map<string, list<Particle *>> validParticles = {};
                for (auto & [particleType, stochiometry] : reaction.reactants)
                    validParticles[particleType] = list<Particle *>();

                reactionData[&reaction] = validParticles;
            }

            // find particles that can react with particle1
            for (auto &particle2 : this->__particles)
            {
                for (auto &reaction : particle1.reactions)
                {
                    if(reaction.reactants.find(particle2.type) != reaction.reactants.end()) // found!
                    {
                        if (this->__interactionEnergy(particle1, particle2) > 0.3)
                            reactionData[&reaction][particle2.type].push_back(&particle2);
                    }
                }
            }

            // check minimum stochiometry count for each reaction
            for (auto &reaction : particle1.reactions)
            {
                bool canProceed = true;
                for (auto & [particleType, stochiometry] : reaction.reactants)
                {
                    if (reactionData[&reaction][particleType].size() < stochiometry)
                    {
                        canProceed = false;
                        break;
                    }
                }

                if (canProceed)
                {
                    // minimum stochiometry satisfied, reaction can proceed
                    reactionList.emplace_back(make_pair(&reaction, reactionData[&reaction]));
                }
            }
        }

        set<Particle*> particlesToRemove;  // using set for unique particles

        for (auto & [reaction, particleData] : reactionList)
        {
            /*
             * for debugging

            cout << "r: " << reaction->name << endl;
            for (auto & [particleType, particles] : particleData)
            {
                cout << "   " << particleType << " ";
                for (auto &particle : particles)
                    cout << &particle << "(" << particle->x << ", " << particle->y << ", " << particle->z << ") ";
                cout << endl;
            }
            /**/

            if(reaction->probability > __random())
            {
                // proceed with reaction
                for (auto &[particleType, particles] : particleData)
                {
                    for (auto &particle : particles)
                    {
                        particle->reacted = true;
                        particlesToRemove.insert(particle);
                    }
                }

                for (auto & [particleType, stochiometry]: reaction->products)
                {
                    Particle particle = Particle(particleType);
                    this->addParticle(particle, true);   // true for random position
                }
                cout << "Reaction: " << reaction->name << endl;
            }
        }

        // remove reacted particles
        for (auto &particle : particlesToRemove)
        {
            cout << "Removed: " << particle->type << " " << particle->particleId << endl;
            this->__particles.remove(*particle);
        }

        // logging
        if((step + 1) % 100 == 0)
        {
            cout << "Step: " << (step + 1) << " / " << _steps << endl;
            this->__saveCoordinates("./log/" + to_string(step + 1) + ".txt");
        }

    }

}