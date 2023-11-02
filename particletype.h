#ifndef PARTICLETYPE_H
#define PARTICLETYPE_H

class ParticleType {
 public:
  ParticleType(const char* name, double mass, int charge);
  const char* GetName() const;
  virtual ~ParticleType();
  double GetMass() const;
  int GetCharge() const;
  virtual void Print() const;
  virtual double GetWidth() const; // va bene o abstract pure?

 private:
  const char* fName;
  const double fMass;
  const int fCharge;
};

#endif