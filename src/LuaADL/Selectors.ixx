#include "HepMC3/GenEvent.h"
#include "HepMC3/GenParticle.h"

namespace LuaADL {

static const int kUndecayedPhysical = 1;
static const int kBeam = 4;

static const bool kFromPDGList = true;
static const bool kNotFromPDGList = false;

static const int kFirst = 0;
static const int kHighestMomentum = 1;

template <int status, bool select_from_pdg_list = true>
std::vector<HepMC3::ConstGenParticlePtr> particles(HepMC3::GenEvent const &evt,
                                                   std::vector<int> pdgs) {

  std::vector<HepMC3::ConstGenParticlePtr> selected_parts = {};

  for (auto const &part : evt.particles()) {
    if (part->status() != status) {
      continue;
    }

    if (!pdgs.size() || ((std::find(pdgs.begin(), pdgs.end(), part->pid()) !=
                          pdgs.end()) == select_from_pdg_list)) {
      selected_parts.push_back(part);
    }
  }
  return selected_parts;
}

template <int status, int selection_strategy>
HepMC3::ConstGenParticlePtr particle(HepMC3::GenEvent const &evt,
                                     std::vector<int> pdgs) {

  static_assert(
      (selection_strategy == kFirst) ||
          (selection_strategy == kHighestMomentum),
      "LuaADL::Selectors::particles selection strategy, must be one of "
      "LuaADL::Selectors::kFirst or LuaADL::Selectors::kHighestMomentum");

  HepMC3::ConstGenParticlePtr hmpart = nullptr;
  double hmmom2 = 0;

  for (auto const &part : evt.particles()) {
    if (part->status() != status) {
      continue;
    }

    if (!pdgs.size() ||
        (std::find(pdgs.begin(), pdgs.end(), part->pid()) != pdgs.end())) {

      if constexpr (selection_strategy == kHighestMomentum) {
        double part_mom2 = part->momentum().length2();
        if (part_mom2 > hmmom2) {
          hmpart = part;
          hmmom2 = part_mom2;
        }
      } else {
        return part;
      }
    }
  }

  return hmpart;
}

// Provides a convenience overload for selecting on a single pdg
template <int status, bool select_from_pdg_list = true>
std::vector<HepMC3::ConstGenParticlePtr>
particles_1pdg(HepMC3::GenEvent const &evt, int pdg) {
  return particles<status, select_from_pdg_list>(evt, {
                                                          pdg,
                                                      });
}

// Provides a convenience overload for selecting on a single pdg
template <int status, int selection_strategy>
HepMC3::ConstGenParticlePtr particle_1pdg(HepMC3::GenEvent const &evt,
                                          int pdg) {
  return particle<status, selection_strategy>(evt, {
                                                       pdg,
                                                   });
}

// non-templated versions
template <int status, bool select_from_pdg_list = true>
std::vector<HepMC3::ConstGenParticlePtr>
particles_status(HepMC3::GenEvent const &evt, int status,
                 std::vector<int> pdgs) {

  std::vector<HepMC3::ConstGenParticlePtr> selected_parts = {};

  for (auto const &part : evt.particles()) {
    if (part->status() != status) {
      continue;
    }

    if (!pdgs.size() || ((std::find(pdgs.begin(), pdgs.end(), part->pid()) !=
                          pdgs.end()) == select_from_pdg_list)) {
      selected_parts.push_back(part);
    }
  }
  return selected_parts;
}

template <int selection_strategy>
HepMC3::ConstGenParticlePtr particle_status(HepMC3::GenEvent const &evt,
                                            int status, std::vector<int> pdgs) {

  static_assert(
      (selection_strategy == kFirst) ||
          (selection_strategy == kHighestMomentum),
      "LuaADL::Selectors::particles selection strategy, must be one of "
      "LuaADL::Selectors::kFirst or LuaADL::Selectors::kHighestMomentum");

  HepMC3::ConstGenParticlePtr hmpart = nullptr;
  double hmmom2 = 0;

  for (auto const &part : evt.particles()) {
    if (part->status() != status) {
      continue;
    }

    if (!pdgs.size() ||
        (std::find(pdgs.begin(), pdgs.end(), part->pid()) != pdgs.end())) {

      if constexpr (selection_strategy == kHighestMomentum) {
        double part_mom2 = part->momentum().length2();
        if (part_mom2 > hmmom2) {
          hmpart = part;
          hmmom2 = part_mom2;
        }
      } else {
        return part;
      }
    }
  }

  return hmpart;
}

// Provides a convenience overload for selecting on a single pdg
template <int status, bool select_from_pdg_list = true>
std::vector<HepMC3::ConstGenParticlePtr>
particles_status_1pdg(HepMC3::GenEvent const &evt, int status, int pdg) {
  return particles_status<select_from_pdg_list>(evt, status,
                                                {
                                                    pdg,
                                                });
}

// Provides a convenience overload for selecting on a single pdg
template <int status, int selection_strategy>
HepMC3::ConstGenParticlePtr particle_status_1pdg(HepMC3::GenEvent const &evt,
                                                 int status, int pdg) {
  return particle_status<selection_strategy>(evt, status,
                                             {
                                                 pdg,
                                             });
}

} // namespace LuaADL