#pragma once

#include <cmath>

constexpr long NuclearPDG(int Z, int A) {
  return 1000000000 + (Z * 10000) + (A * 10);
}

constexpr double pdg_mass(int const pdg) {
  switch (pdg) {
  case 2122: {
    return 0.938272;
  }
  case 2112: {
    return 0.939565;
  }
  case 13: {
    return 0.105658;
  }
  case 14: {
    return 0;
  }
  case 211: {
    return 0.13957039;
  }
  case NuclearPDG(6, 12): {
    return 12;
  }
  case NuclearPDG(8, 16): {
    return 16;
  }
  }

  return 0;
}

int const kBeamStatus = 4;
int const kTargetStatus = 11;
int const kPreFSIStatus = 3;
int const kObservableStatus = 1;

HepMC3::GenParticlePtr GenerateParticle(int status, int pdg, double tmom,
                                        double theta_deg) {

  double xcomp = std::sin(theta_deg * (M_PI / 180.0));
  double zcomp = std::cos(theta_deg * (M_PI / 180.0));
  double ecomp = std::sqrt(std::pow(tmom, 2) + std::pow(pdg_mass(pdg), 2));

  HepMC3::GenParticlePtr gp = std::make_shared<HepMC3::GenParticle>(
      HepMC3::FourVector{xcomp, 0, zcomp, ecomp}, pdg, status);

  gp->set_generated_mass(pdg_mass(pdg));
  return gp;
}