#ifndef PARTICLE_H
#define PARTICLE_H

#include "particletype.h"

class Particle {
 public:
  Particle(const char* name, double x = 0, double y = 0, double z = 0);
  int GetIndex() const;
  static void AddParticleType(const char* name, double mass, int charge,
                              double width = 0);  // NON FARLA VOID?
  void SetIndex(int index);  // vanno usati anche in costruttore?
  void SetIndex(const char* name);
  static void ListParticles();  // non posso farlo const ovviamente
  void Print() const;
  double GetPx() const;
  double GetPy() const;
  double GetPz() const;
  double GetMass() const;
  double Energy() const;
  double InvMass(Particle& p);
  void SetP(double px, double py, double pz); // MANCA IMPLEMENTAZIONE

 private:
  static const int fMaxNumParticleType = 10;
  static ParticleType* fParticleType[fMaxNumParticleType];
  static int fNParticleType;
  int fIndex;
  double fPx;
  double fPy;
  double fPz;
  static int FindParticle(const char* name);
};

#endif