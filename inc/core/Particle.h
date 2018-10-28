//
// Created by root on 28.10.2018.
//

#ifndef MD_PARTICLE_H
#define MD_PARTICLE_H


class Particle
{
public:
    Particle(double _r);
    double x, y, z;
    double r;
private:
    double __x = 0.0, __y = 0.0, __z = 0.0;
};


#endif //MD_PARTICLE_H
