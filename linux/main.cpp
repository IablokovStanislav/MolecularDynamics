#include <iostream>
#include <vector>
#include <fstream>

#include "inc/core/Particle.h"
#include "inc/core/System.h"

#include "inc/lib/jsoncpp/json.h"

using namespace std;
int main()
{
    System* system = System::getInstance();
    system->setGeometry(100.0, 100.0, 100.0);        // in nm^3
    system->setTemperature(298.0);                   // in K
    system->setPressure(1.0);                        // in bar
    system->setBoundaryConditions(true, true, true); // WDH: true for periodic
    system->setRB(1.0);                              // ???


    Json::Value config;
    ifstream fConfig("config.json", ifstream::binary);
    fConfig >> config;
    fConfig.close();

    // loading reactions
    for(unsigned int iRxn = 0; iRxn < config["reactions"].size(); iRxn++)
    {
        Reaction reaction(config["reactions"][iRxn]["name"].asString());
        reaction.setProbability(config["reactions"][iRxn]["probability"].asDouble());

        Json::Value reactants = config["reactions"][iRxn]["reactants"];
        for(unsigned int iRct = 0; iRct < reactants.size(); iRct++)
            reaction.addReactant(reactants[iRct]["type"].asString(), reactants[iRct]["stochiometry"].asUInt());

        Json::Value products = config["reactions"][iRxn]["products"];
        for(unsigned int iPrd = 0; iPrd < products.size(); iPrd++)
            reaction.addProduct(products[iPrd]["type"].asString(), products[iPrd]["stochiometry"].asUInt());

        system->addReaction(reaction);

        cout << "Reaction: " << config["reactions"][iRxn]["name"].asString() << endl;
    }

    // loading particles
    for(unsigned int iPt = 0; iPt < config["particles"].size(); iPt++)
    {
        cout << "Particle: " << config["particles"][iPt]["type"].asString() << endl;
        for(unsigned int ip = 0; ip < config["particles"][iPt]["amount"].asUInt(); ip++)
        {
            Particle particle = Particle(config["particles"][iPt]["type"].asString());
            system->addParticle(particle, true);   // true for random position
        }
        cout << " added: " << config["particles"][iPt]["amount"].asUInt() << " particles" << endl;
    }


    system->run(1000, 1); // steps, dt in ns

    return 0;
}