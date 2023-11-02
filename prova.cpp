#include <iostream>

#include "particle.h"
#include "particletype.h"
#include "resonancetype.h"

int main() {
  Particle::AddParticleType("P+", 1., 1);
  Particle::AddParticleType("S-", 2., -2);
  Particle p{"P+", 3, 4, 5};
  Particle s{"S-", -2, 2, -2};
  std::cout << "Invariant mass 1: " << p.InvMass(s) << '\n';
  std::cout << "Invariant mass 2: " << s.InvMass(p) << '\n';
}