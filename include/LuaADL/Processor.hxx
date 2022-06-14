#pragma once

#include "HepMC3/LuaInterface.hxx"

#include "HepMC3/GenEvent.h"

namespace LuaADL {
class Processor {

  HepMC3::LuaInterface LI;
  Projectors Pj;
  Selectors Sl;

  sol::function select_function;
  sol::function project_function;
  sol::function weight_function;
  sol::function projelection_function;

public:
  Processor(std::string const &filename) {
    LI.lua.safe_script_file(filename);

    select_function = LI.lua["select"];
    project_function = LI.lua["project"];
    weight_function = LI.lua["weight"];
    projection_selection_function = LI.lua["projelect"];
  }

  bool select(HepMC3::GenEvent const &ev) {
    return select_function(ev, out_projections);
  }

  std::vector<double> project(HepMC3::GenEvent const &ev) {
    std::vector<double> projections;
    project_function(ev, projections);
    return projections;
  }

  std::vector<double> weight(HepMC3::GenEvent const &ev) {
    std::vector<double> projections;
    weight_function(ev, projections);
    return projections;
  }

  bool projelect(HepMC3::GenEvent const &ev,
                 std::vector<double> &out_projections) {
    return projection_selection_function(ev, out_projections);
  }
};
} // namespace LuaADL