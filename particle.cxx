#include "particle.h"

#include <cmath>
#include <cstdlib>
#include <iostream>

#include "resonancetype.h"

int Particle::fNParticleType = 0;
ParticleType* Particle::fParticleType[Particle::fMaxNumParticleType]{};

int Particle::FindParticle(const char* name) {
  for (int i{0}; i < fNParticleType; i++) {
    if (name == fParticleType[i]->GetName()) return i;
  }
  return -1;
}

void Particle::AddParticleType(const char* name, double mass, int charge,
                               double width) {
  if (Particle::FindParticle(name) == -1 &&
      fNParticleType <= fMaxNumParticleType) {
    std::cout << "Adding new particle " << name << '\n';
    if (width != 0) {
      ResonanceType* P = new ResonanceType{name, mass, charge, width};
      fParticleType[fNParticleType] = P;
    } else {
      ParticleType* P = new ParticleType{name, mass, charge};
      fParticleType[fNParticleType] = P;
    }
    fNParticleType++;
  } else
    std::cout << "Particle " << name << " already added!\n";
}

void Particle::SetIndex(int index) {
  if (index >= 0 && index < fNParticleType) fIndex = index;
}

void Particle::SetIndex(const char* name) {
  if (Particle::FindParticle(name) != -1) fIndex = Particle::FindParticle(name);
}

Particle::Particle(const char* name, double x, double y, double z)
    : fPx(x), fPy(y), fPz(z) {
  if (Particle::FindParticle(name) != -1)
    fIndex = Particle::FindParticle(name);
  else
    std::cout << "Particle type " << name << " not found.\n";
}

Particle::Particle() {};

int Particle::GetIndex() const { return fIndex; }
void Particle::PrintArray() {
  std::cout << "LIST OF PARTICLE/RESONANCE TYPES\n";
  for (int i{0}; i < fNParticleType; i++) {
    std::cout << "Particle type in index " << i << '\n';
    Particle::fParticleType[i]->Print();
    std::cout << "---------------------------------------------\n";
  }
}

void Particle::Print() const {
  std::cout << "Index: " << fIndex << '\n';
  std::cout << "Name: " << fParticleType[fIndex]->GetName() << '\n';
  std::cout << "Px: " << fPx << '\n';
  std::cout << "Py: " << fPy << '\n';
  std::cout << "Pz: " << fPz << '\n';
}

double Particle::GetPx() const { return fPx; }
double Particle::GetPy() const { return fPy; }
double Particle::GetPz() const { return fPz; }
double Particle::GetMass() const { return fParticleType[fIndex]->GetMass(); }
double Particle::GetEnergy() const {
  double m = Particle::GetMass();
  return std::sqrt(m * m + fPx * fPx + fPy * fPy + fPz * fPz);
}

double Particle::InvMass(Particle& p) {
  double sum_E = GetEnergy() + p.GetEnergy();
  double sum_Px = fPx + p.fPx;
  double sum_Py = fPy + p.fPy;
  double sum_Pz = fPz + p.fPz;
  return std::sqrt(sum_E * sum_E - sum_Px * sum_Px - sum_Py * sum_Py -
                   sum_Pz * sum_Pz);
}

void Particle::SetP(double px, double py, double pz) {
  fPx = px;
  fPy = py;
  fPz = pz;
}

int Particle::Decay2body(Particle& dau1, Particle& dau2) {
  if (GetMass() == 0.0) {
    printf("Decayment cannot be preformed if mass is zero\n");
    return 1;
  }
  // MODIFICATO fIParticle IN fIndex
  double massMot = GetMass();
  double massDau1 = dau1.GetMass();
  double massDau2 = dau2.GetMass();

  if (fIndex > -1) {  // add width effect

    // gaussian random numbers

    float x1, x2, w, y1;

    double invnum = 1. / RAND_MAX;
    do {
      x1 = 2.0 * rand() * invnum - 1.0;
      x2 = 2.0 * rand() * invnum - 1.0;
      w = x1 * x1 + x2 * x2;
    } while (w >= 1.0);

    w = sqrt((-2.0 * log(w)) / w);
    y1 = x1 * w;

    massMot += fParticleType[fIndex]->GetWidth() * y1;
  }

  if (massMot < massDau1 + massDau2) {
    printf(
        "Decayment cannot be preformed because mass is too low in this "
        "channel\n");
    return 2;
  }

  double pout =
      sqrt(
          (massMot * massMot - (massDau1 + massDau2) * (massDau1 + massDau2)) *
          (massMot * massMot - (massDau1 - massDau2) * (massDau1 - massDau2))) /
      massMot * 0.5;

  double norm = 2 * M_PI / RAND_MAX;

  double phi = rand() * norm;
  double theta = rand() * norm * 0.5 - M_PI / 2.;
  dau1.SetP(pout * sin(theta) * cos(phi), pout * sin(theta) * sin(phi),
            pout * cos(theta));
  dau2.SetP(-pout * sin(theta) * cos(phi), -pout * sin(theta) * sin(phi),
            -pout * cos(theta));

  double energy = sqrt(fPx * fPx + fPy * fPy + fPz * fPz + massMot * massMot);

  double bx = fPx / energy;
  double by = fPy / energy;
  double bz = fPz / energy;

  dau1.Boost(bx, by, bz);
  dau2.Boost(bx, by, bz);

  return 0;
}

void Particle::Boost(double bx, double by, double bz) {
  double energy = GetEnergy();

  // Boost this Lorentz vector
  double b2 = bx * bx + by * by + bz * bz;
  double gamma = 1.0 / sqrt(1.0 - b2);
  double bp = bx * fPx + by * fPy + bz * fPz;
  double gamma2 = b2 > 0 ? (gamma - 1.0) / b2 : 0.0;

  fPx += gamma2 * bp * bx + gamma * bx * energy;
  fPy += gamma2 * bp * by + gamma * by * energy;
  fPz += gamma2 * bp * bz + gamma * bz * energy;
}