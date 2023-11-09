#include <cmath>
#include <vector>

#include "particle.h"
#include "particletype.h"
#include "resonancetype.h"

int main() {
  Particle::AddParticleType("Pione+", .13957, 1);
  Particle::AddParticleType("Pione-", .13957, -1);
  Particle::AddParticleType("Kaone+", .49367, 1);
  Particle::AddParticleType("Kaone-", .49367, -1);
  Particle::AddParticleType("Protone+", .93827, 1);
  Particle::AddParticleType("Protone-", .93827, -1);
  Particle::AddParticleType("K*", .89166, 0, .050);
  gRandom->SetSeed();

  std::vector<Particle> EventParticles(
      120);  // vedi con array nativi
             // questa dichiarazione non va bene, rivedere costruttore?

  for (int i = 0; i < 1E5; i++) {    // simulazione 10^5 eventi
    for (int j = 0; j < 100; j++) {  // setting type x 100 particles x evento
      double x = gRandom->Rndm();
      if (x < .4)
        EventParticles[j].SetIndex(0);  // chiama SetIndex con name
      else if (x < .8)
        EventParticles[j].SetIndex(1);
      else if (x < .85)
        EventParticles[j].SetIndex(2);
      else if (x < .9)
        EventParticles[j].SetIndex(3);
      else if (x < .945)
        EventParticles[j].SetIndex(4);
      else if (x < .99)
        EventParticles[j].SetIndex(5);
      else
        EventParticles[j].SetIndex(6);

      for (int k = 0; k < 100; k++) {  // decadimento delle K* particles
        if (EventParticles[k].GetIndex() == 6) {
          double x = gRandom->Rndm();
          if (x < .5) {
            Particle pp{"Pione+"};
            Particle km{"Kaone-"};
            EventParticles[k].Decay2body(pp, km); // return di decay2body è verificato?
            EventParticles.push_back(pp); // decay2body dovrebbe restituire un int
            EventParticles.push_back(km);
          } else {
            Particle pm{"Pione-"};
            Particle kp{"Kaone+"};
            EventParticles[k].Decay2body(pm, kp);
            EventParticles.push_back(pm);
            EventParticles.push_back(kp);
          }
        }
      }

      for (int n = 0; n < 100; n++) {
        double phi = gRandom->Uniform(0, 2 * M_PI);
        double theta = gRandom->Uniform(0, M_PI);
        double rho = gRandom->Exp(1);
        EventParticles[n].SetP(rho * sin(theta) * cos(phi),
                               rho * sin(theta) * sin(phi), rho * cos(theta));
      }
    }
  }
}
