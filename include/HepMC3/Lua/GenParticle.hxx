#pragma once

#include "HepMC3/GenParticle.h"
#include "HepMC3/Print.h"

#include "HepMC3/Lua/Attribute.hxx"

#include "sol/sol.hpp"

namespace HepMC3 {

inline sol::usertype<GenParticle> RegisterGenParticle(sol::state &lua) {
  sol::usertype<GenParticle> GenParticle_type = lua.new_usertype<GenParticle>(
      "GenParticle",
      sol::constructors<GenParticle(),
                        GenParticle(FourVector const &, int, int)>());

  //-------------- Property Accessors ---------------------
  GenParticle_type["pid"] =
      sol::property(&GenParticle::pid, &GenParticle::set_pid);
  GenParticle_type["abs_pid"] = sol::readonly(&GenParticle::abs_pid);
  GenParticle_type["status"] =
      sol::property(&GenParticle::status, &GenParticle::set_status);
  GenParticle_type["momentum"] =
      sol::property(&GenParticle::momentum, &GenParticle::set_momentum);
  GenParticle_type["generated_mass"] = sol::property(
      &GenParticle::generated_mass, &GenParticle::set_generated_mass);

  //-------------- Convenience methods---------------------
  GenParticle_type["in_event"] = &GenParticle::in_event;
  GenParticle_type["id"] = &GenParticle::id;
  GenParticle_type["production_vertex"] =
      sol::resolve<ConstGenVertexPtr() const>(&GenParticle::production_vertex);
  GenParticle_type["end_vertex"] =
      sol::resolve<ConstGenVertexPtr() const>(&GenParticle::end_vertex);

  GenParticle_type["parents"] =
      sol::resolve<std::vector<ConstGenParticlePtr>() const>(
          &GenParticle::parents);
  GenParticle_type["children"] =
      sol::resolve<std::vector<ConstGenParticlePtr>() const>(
          &GenParticle::children);

  GenParticle_type["unset_generated_mass"] = &GenParticle::unset_generated_mass;
  GenParticle_type["is_generated_mass_set"] =
      &GenParticle::is_generated_mass_set;

  GenParticle_type["parent_event"] =
      sol::resolve<const GenEvent *() const>(&GenParticle::parent_event);

  //-------------- Attribute helpers---------------------
  DecorateWithAttributeCastHelpers(GenParticle_type);

  //-------------- Extra Lua methods---------------------
  GenParticle_type["to_str"] = [](GenParticle const &self) -> std::string {
    // We need to copy the GenParticle to use this method, not ideal
    std::stringstream ss;
    Print::line(ss, std::make_shared<GenParticle>(self));
    return ss.str();
  };

  return GenParticle_type;
}

} // namespace HepMC3