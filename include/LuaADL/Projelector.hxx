#pragma once

#include "HepMC3/LuaInterface.hxx"

#include "HepMC3/GenEvent.h"

namespace LuaADL {
class Projelector {

  HepMC3::LuaInterface LI;

  sol::function projection_selection_function;

public:
  Projelector(std::string const &filename) {
    LI.lua.safe_script_file(filename);

    projection_selection_function = LI.lua["projelect"];
  }

  bool projelect(HepMC3::GenEvent const &ev,
                 std::vector<double> &out_projections) {
    return projection_selection_function(ev, out_projections);
  }
};
} // namespace LuaADL