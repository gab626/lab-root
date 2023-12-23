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

  Particle EventParticles[120];

  TH1F* h[6];
  h[0] = new TH1F("h1", "Particle types", 7, 0, 7);
  h[1] = new TH1F("h2", "Azimuthal angle", 10, 0, 2 * M_PI);
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

  for (int i = 0; i < 1E3; i++) {
    int k_index;
    int new_particles = 0;

    for (int j = 0; j < 100; j++) {
      double x = gRandom->Rndm();
      if (x < .4)
        EventParticles[j].SetIndex("Pione+");
      else if (x < .8)
        EventParticles[j].SetIndex("Pione-");
      else if (x < .85)
        EventParticles[j].SetIndex("Kaone+");
      else if (x < .9)
        EventParticles[j].SetIndex("Kaone-");
      else if (x < .945)
        EventParticles[j].SetIndex("Protone+");
      else if (x < .99)
        EventParticles[j].SetIndex("Protone-");
      else {
        EventParticles[j].SetIndex("K*");
        k_index = EventParticles[j].GetIndex();
      }
      h[0]->Fill(EventParticles[j].GetIndex());

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

      if (EventParticles[j].GetIndex() == k_index) {
        double x = gRandom->Rndm();
        if (x < .5) {
          Particle pp{"Pione+"};
          Particle km{"Kaone-"};
          EventParticles[j].Decay2body(pp, km);
          EventParticles[100 + new_particles] = pp;
          EventParticles[101 + new_particles] = km;
          new_particles += 2;
        } else {
          Particle pm{"Pione-"};
          Particle kp{"Kaone+"};
          EventParticles[j].Decay2body(pm, kp);
          EventParticles[100 + new_particles] = pm;
          EventParticles[101 + new_particles] = kp;
          new_particles += 2;
        }
      }
    }

    for (int n = 0; n < 100; n++) {
      for (int p = 0; p < 100; p++) {
        double inv_mass = EventParticles[n].InvMass(EventParticles[p]);
        m[0]->Fill(inv_mass);
        /* if (EventParticles[n].GetCharge() == EventParticles[l].GetCharge())
          m[1]->Fill(inv_mass);
        else
          m[2]->Fill(inv_mass); */
      }
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