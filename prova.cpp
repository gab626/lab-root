#include "particletype.h"
#include "resonancetype.h"
#include "particle.h"

#include <iostream>

int main() {
  Particle::AddParticleType("P+", 1., 1);
  Particle::AddParticleType("S-", 2., -2);
  Particle::AddParticleType("T0", 3., 0, 3.);
  Particle::AddParticleType("P+", 5., 5);
  Particle p{"P+"};
  p.Print();
  Particle s{"S-", 3, 4, 5};
  Particle t{"T0", -2, 1, 0};
  std::cout << "s energy: " << s.Energy() << '\n';
  std::cout << "t energy: " << t.Energy() << '\n';
  std::cout << "Invariant mass between s and t: " << s.InvMass(t) << '\n';
  Particle::ListParticles();
}