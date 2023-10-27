#include "particletype.h"
#include "resonancetype.h"
#include "particle.h"

#include <iostream>

int main() {
  Particle::AddParticleType("P+", 1., 1);
  Particle::AddParticleType("S-", 2., -2);
  Particle::AddParticleType("P+", 5., 5);
  Particle p{"P+"};
  Particle c{"T0"};
  Particle::ListParticles();
}