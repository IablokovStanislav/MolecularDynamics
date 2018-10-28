//
// Created by root on 28.10.2018.
//

#ifndef MD_SYSTEM_H
#define MD_SYSTEM_H

#include <list>
#include <string>
#include <vector>
#include "Particle.h"

using namespace std;

class System
{
public:
    System();
    void addParticle(Particle _particle, bool _random);
    void setGeometry(double _boxWidth, double _boxDepth, double _boxHeight);
    void setTemperature(double _temperature);
    void setPressure(double _pressure);
    void setBoundaryConditions(bool _bcX, bool _bcY, bool _bcZ);
    void setRB(double _rb);
    void run(int _steps, double _dt);
private:

    // methods
    void __initRandomSequence(unsigned long _randomSequenceLength);
    double __random();
    double __randomBD(double _dt);
    void __saveCoordinates(string _fname);

    // variables
    list<Particle> __particles;
    //vector<Particle> __particles;
    double __boxWidth, __boxDepth, __boxHeight;
    double __temperature, __pressure;
    bool __bcX = true, __bcY = true, __bcZ = true;

    double __RB = 1.0;

    unsigned long __randomSequenceLength, __randomSequenceIndex;
    vector<double> __randomSequence;

};


#endif //MD_SYSTEM_H
