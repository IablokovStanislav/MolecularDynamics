//
// Created by root on 28.10.2018.
//

#ifndef MD_SYSTEM_H
#define MD_SYSTEM_H

#include <list>
#include <string>
#include <vector>
#include <map>
#include "Particle.h"
#include "Reaction.h"

using namespace std;

class System
{
public:
    // singleton definitions
    static System* getInstance();

    void addParticle(Particle _particle, bool _random);
    void addReaction(Reaction _reaction);
    void setGeometry(double _boxWidth, double _boxDepth, double _boxHeight);
    void setTemperature(double _temperature);
    void setPressure(double _pressure);
    void setBoundaryConditions(bool _bcX, bool _bcY, bool _bcZ);
    void setRB(double _rb);
    void run(int _steps, double _dt);

    map<string, Reaction> reactions = {};
private:

    // singleton definitions
    static System* __instance;
    System();

    // methods
    void __initRandomSequence(unsigned long _randomSequenceLength);
    double __random();
    double __randomBD(double _dt);
    void __saveCoordinates(string _fname);

    double __interactionEnergy(Particle &_particle1, Particle &_particle2);

    // variables
    list<Particle> __particles;
    unsigned int __particleIdCounter = 0;

    double __boxWidth, __boxDepth, __boxHeight;
    double __temperature, __pressure;
    bool __bcX = true, __bcY = true, __bcZ = true;

    double __RB = 1.0;

    unsigned long __randomSequenceLength, __randomSequenceIndex;
    vector<double> __randomSequence;

};


#endif //MD_SYSTEM_H
