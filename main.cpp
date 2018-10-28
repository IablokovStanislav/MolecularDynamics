#include <iostream>
#include <vector>

#include "inc/core/Particle.h"
#include "inc/core/System.h"

using namespace std;
int main()
{
    double radius = 1.0;

    System system = System();
    system.setGeometry(100.0, 100.0, 100.0);        // in nm^3
    system.setTemperature(298.0);                   // in K
    system.setPressure(1.0);                        // in bar
    system.setBoundaryConditions(true, true, true); // WDH: true for periodic
    system.setRB(1.0);                              // ???

    for(int i = 0; i < 1; i++)
    {
        Particle particle = Particle(radius);
        system.addParticle(particle, true);   // true for random position
    }

    system.run(1000, 1); // steps, dt in ns

    return 0;
}