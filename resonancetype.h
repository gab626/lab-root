#ifndef RESONANCETYPE_H
#define RESONANCETYPE_H

#include "particletype.h"

class ResonanceType : public ParticleType {
 public:
  ResonanceType(const char* name, double mass, int charge, double width);
  double GetWidth() const override;
  void Print() const override;

 private:
  const double fWidth;
};

#endif