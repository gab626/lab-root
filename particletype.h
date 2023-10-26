#ifndef PARTICLETYPE_H
#define PARTICLETYPE_H

class ParticleType {
 public:
  ParticleType(const char* name, double mass, int charge);
  const char* GetName() const;
  double GetMass() const;
  int GetCharge() const;
  virtual void Print() const;

 private:
  const char* fName;
  const double fMass;
  const int fCharge;
};

#endif