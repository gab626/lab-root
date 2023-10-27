#include "particletype.h"

#include <iostream>

ParticleType::ParticleType(const char* name, double mass, int charge)
    : fName(name), fMass(mass), fCharge(charge) {}
const char* ParticleType::GetName() const { return fName; }
    ParticleType::~ParticleType() {}
double ParticleType::GetMass() const { return fMass; }
int ParticleType::GetCharge() const { return fCharge; }
void ParticleType::Print() const {
  std::cout << "Name: " << fName << '\n'
            << "Mass: " << fMass << '\n'
            << "Charge: " << fCharge << '\n';
}