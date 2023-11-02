#include "particle.h"

#include <cmath>
#include <iostream>

#include "resonancetype.h"

int Particle::fNParticleType = 0;
ParticleType* Particle::fParticleType[Particle::fMaxNumParticleType]{};

int Particle::FindParticle(const char* name) {
  int i{0};
  for (; i < fNParticleType; i++) {
    if (name == fParticleType[i]->GetName()) return i;
  }
  if (i == fNParticleType) i = -1;
  return i;
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
int Particle::GetIndex() const { return fIndex; }
void Particle::ListParticles() {
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
double Particle::Energy() const {
  double m = Particle::GetMass();
  return std::sqrt(m * m + fPx * fPx + fPy * fPy + fPz * fPz);
}
double Particle::InvMass(Particle& p) {
  double sum_E = Energy() + p.Energy();
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