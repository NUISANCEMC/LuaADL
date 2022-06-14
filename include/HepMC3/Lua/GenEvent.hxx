#pragma once

#include "HepMC3/GenEvent.h"
#include "HepMC3/Print.h"

#include "HepMC3/Lua/Attribute.hxx"

#include "sol/sol.hpp"

namespace HepMC3 {

inline sol::usertype<GenEvent> RegisterGenEvent(sol::state &lua) {
  sol::usertype<GenEvent> GenEvent_type =
      lua.new_usertype<GenEvent>("GenEvent", sol::constructors<GenEvent()>());

  //-------------- Property Accessors ---------------------
  GenEvent_type["event_number"] =
      sol::property(&GenEvent::event_number, &GenEvent::set_event_number);
  GenEvent_type["event_pos"] =
      sol::property(&GenEvent::event_pos, &GenEvent::shift_position_to);

  //-------------- Convenience methods---------------------
  GenEvent_type["run_info"] = &GenEvent::run_info;

  GenEvent_type["shift_position_by"] = &GenEvent::shift_position_by;
  GenEvent_type["shift_position_to"] = &GenEvent::shift_position_to;
  GenEvent_type["boost"] = &GenEvent::boost;
  GenEvent_type["rotate"] = &GenEvent::rotate;
  GenEvent_type["reflect"] = &GenEvent::reflect;

  GenEvent_type["particles"] =
      sol::resolve<const std::vector<ConstGenParticlePtr> &() const>(
          &GenEvent::particles);

  GenEvent_type["beams"] =
      sol::resolve<std::vector<ConstGenParticlePtr>() const>(&GenEvent::beams);

  GenEvent_type["vertices"] =
      sol::resolve<const std::vector<ConstGenVertexPtr> &() const>(
          &GenEvent::vertices);

  GenEvent_type["weights"] =
      sol::resolve<const std::vector<double> &() const>(&GenEvent::weights);

  GenEvent_type["weight_names"] =
      sol::resolve<const std::vector<std::string> &() const>(
          &GenEvent::weight_names);

  //-------------- Extra Lua methods---------------------
  GenEvent_type["to_str"] = [](GenEvent const &self) -> std::string {
    std::stringstream ss;
    Print::listing(ss, self);
    return ss.str();
  };

  return GenEvent_type;
}

} // namespace HepMC3