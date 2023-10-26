#include "particletype.cxx"
#include "resonancetype.cxx"
#include "particle.cxx"

int main() {
  Particle::AddParticleType("P+", 1., 1);
  Particle::AddParticleType("S-", 2., -2);
  Particle::AddParticleType("T0", 3., 0, 3.);
  Particle::ListParticles();
  /* Particle prima("P+");
  Particle seconda("S-");
  Particle terza("T0"); */
}