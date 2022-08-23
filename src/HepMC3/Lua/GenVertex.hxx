#pragma once

#include "HepMC3/GenVertex.h"
#include "HepMC3/Print.h"

#include "HepMC3/Lua/Attribute.hxx"

#include "sol/sol.hpp"

namespace HepMC3 {

inline sol::usertype<GenVertex> RegisterGenVertex(sol::state &lua) {
  sol::usertype<GenVertex> GenVertex_type = lua.new_usertype<GenVertex>(
      "GenVertex", sol::constructors<GenVertex()>());

  //-------------- Property Accessors ---------------------
  GenVertex_type["id"] = sol::property(&GenVertex::id, &GenVertex::set_id);
  GenVertex_type["status"] =
      sol::property(&GenVertex::status, &GenVertex::set_status);
  GenVertex_type["position"] =
      sol::property(&GenVertex::position, &GenVertex::set_position);

  //-------------- Convenience methods---------------------
  GenVertex_type["has_set_position"] = &GenVertex::has_set_position;

  GenVertex_type["particles_in"] =
      sol::resolve<const std::vector<ConstGenParticlePtr> &() const>(
          &GenVertex::particles_in);

  GenVertex_type["particles_out"] =
      sol::resolve<const std::vector<ConstGenParticlePtr> &() const>(
          &GenVertex::particles_out);

  //-------------- Attribute helpers---------------------
  DecorateWithAttributeCastHelpers(GenVertex_type);

  //-------------- Extra Lua methods---------------------
  GenVertex_type["to_str"] = [](GenVertex const &self) -> std::string {
    std::stringstream ss;
    Print::line(ss, std::make_shared<GenVertex>(self));
    return ss.str();
  };

  return GenVertex_type;
}

} // namespace HepMC3