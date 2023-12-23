#include <cmath>
#include <vector>

#include "particle.h"
#include "particletype.h"
#include "resonancetype.h"

int main() {
  gBenchmark->Start("Simulation time");
  Particle::AddParticleType("Pione+", .13957, 1);
  Particle::AddParticleType("Pione-", .13957, -1);
  Particle::AddParticleType("Kaone+", .49367, 1);
  Particle::AddParticleType("Kaone-", .49367, -1);
  Particle::AddParticleType("Protone+", .93827, 1);
  Particle::AddParticleType("Protone-", .93827, -1);
  Particle::AddParticleType("K*", .89166, 0, .050);
  gRandom->SetSeed();

  std::vector<Particle> EventParticles(100);  // vedi con array nativi

  TH1F* h[6];
  h[0] = new TH1F("h1", "Particle types", 7, 0, 7);
  h[1] = new TH1F("h2", "Azimuthal angle", 10, 0,
                  2 * M_PI);  // rivedere tutti i valori Nbins
  h[2] = new TH1F("h3", "Polar angle", 10, 0, M_PI);
  h[3] = new TH1F("h4", "Impulse", 100, 0, 7);
  h[4] = new TH1F("h5", "Transverse impulse", 100, 0, 7);
  h[5] = new TH1F("h6", "Particles energy", 100, 0, 7);

  h[0]->SetFillColor(4);
  h[0]->SetLineColor(1);
  for (int i = 1; i < 3; i++) {
    h[i]->SetFillColor(2);
    h[i]->SetLineColor(1);
  }
  for (int i = 3; i < 6; i++) {
    h[i]->SetFillColor(3);
    h[i]->SetLineColor(1);
  }

  TH1F* m[6];
  m[0] = new TH1F("m1", "Inv mass of all particles", 100, 0, 10);
  m[1] = new TH1F("m2", "All particles concordant sign", 100, 0, 10);
  m[2] = new TH1F("m3", "All particles discordant sign", 100, 0, 10);
  // m[3] = new TH1F("m4", "Impulse", 100, 0, 7);
  // m[4] = new TH1F("m5", "Transverse impulse", 100, 0, 7);
  // m[5] = new TH1F("m6", "Particles energy", 100, 0, 7);
  for (int i = 0; i < 3; i++) m[i]->Sumw2();

  for (int i = 0; i < 1E5;
       i++) {  // simulazione 10^5 eventi DA RISISTEMARE IL NUMERO DI EVENTI

    for (int j = 0; j < 100; j++) {  // setting type x 100 particles x evento
                                     // (chiamare SetIndex con name?)
      double x = gRandom->Rndm();
      int index = 0;
      if (x < .4)
        index = 0;
      else if (x < .8)
        index = 1;
      else if (x < .85)
        index = 2;
      else if (x < .9)
        index = 3;
      else if (x < .945)
        index = 4;
      else if (x < .99)
        index = 5;
      else
        index = 6;
      EventParticles[j].SetIndex(index);
      h[0]->Fill(index);

      double phi = gRandom->Uniform(0, 2 * M_PI);
      double theta = gRandom->Uniform(0, M_PI);
      double rho = gRandom->Exp(1);
      EventParticles[j].SetP(rho * sin(theta) * cos(phi),
                             rho * sin(theta) * sin(phi), rho * cos(theta));
      h[1]->Fill(phi);
      h[2]->Fill(theta);
      double px = EventParticles[j].GetPx();
      double py = EventParticles[j].GetPy();
      double pz = EventParticles[j].GetPz();
      h[3]->Fill(std::sqrt(px * px + py * py + pz * pz));
      h[4]->Fill(std::sqrt(px * px + py * py));
      h[5]->Fill(EventParticles[j].GetEnergy());
    }

    int new_particles = 0;
    Particle pp{"Pione+"};
    Particle km{"Kaone-"};
    Particle pm{"Pione-"};
    Particle kp{"Kaone+"};
    for (int k = 0; k < 100; k++) {  // decadimento delle K* particles
      if (EventParticles[k].GetIndex() == 6) {
        double x = gRandom->Rndm();
        if (x < .5) {
          EventParticles[k].Decay2body(
              pp, km);  // return di decay2body è verificato?
          EventParticles.push_back(
              pp);  // decay2body dovrebbe restituire un int
          EventParticles.push_back(km);
          new_particles += 2;
        } else {
          EventParticles[k].Decay2body(pm, kp);
          EventParticles.push_back(pm);
          EventParticles.push_back(kp);
          new_particles += 2;
        }
      }
    }

    for (int n = 0; n < 100 + new_particles; n += 2) {
      double inv_mass = EventParticles[n].InvMass(EventParticles[n+1]);
      m[0]->Fill(inv_mass);
      /* int sign_1 = Particle::fParticleType[EventParticles[n].GetIndex()].GetCharge();
      int sign_2 = Particle::fParticleType[EventParticles[n+1].GetIndex()].GetCharge();
      if ( sign_1 * sign_2 == 1) m[1]->Fill(inv_mass);
      else m[2]->Fill(inv_mass); */
    }
  }

  TCanvas* c1 = new TCanvas("c1", "Histograms", 200, 10, 1000, 1000);
  c1->Divide(3, 2);
  for (int i = 0; i < 6; i++) {
    c1->cd(i + 1);
    h[i]->Draw();
  }

  TCanvas* c2 = new TCanvas("c2", "Invariant mass", 200, 10, 1000, 1000);
  c2->Divide(3, 2);
  for (int i = 0; i < 3; i++) {
    c2->cd(i + 1);
    m[i]->Draw();
  }

  gBenchmark->Show("Simulation time");
}
