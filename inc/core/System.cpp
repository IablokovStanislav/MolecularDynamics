//
// Created by root on 28.10.2018.
//
#include <iostream>
#include <ctime>
#include <vector>
#include <cmath>
#include <string>
#include <fstream>

#include "System.h"
#include "Particle.h"

#define PI 3.1415926

System::System()
{
    srand(unsigned(time(0)));
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

    this->__particles.push_back(_particle);
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
        fOut << particle.x << " " << particle.y << " " << particle.z << endl;
    }

    fOut.close();
}

void System::run(int _steps, double _dt)
{
    this->__initRandomSequence(10000000);

    for(int step = 0; step < _steps; step++)
    {
        for (auto &particle : this->__particles)
        {
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

        // logging
        if((step + 1) % 10 == 0)
        {
            cout << "Step: " << (step + 1) << " / " << _steps << endl;
            this->__saveCoordinates("./log/" + to_string(step + 1) + ".txt");
        }

    }

}