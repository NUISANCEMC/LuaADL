#pragma once

#include "LuaADL/Selectors.hxx"
#include "LuaADL/Projectors.hxx"

#include "HepMC3/LuaInterface.hxx"

#include "HepMC3/GenEvent.h"

namespace LuaADL {
class Processor {

  HepMC3::LuaInterface LI;
  Projectors Pj;
  Selectors Sl;

  sol::function filter_function;
  sol::function project_function;
  sol::function weight_function;
  sol::function filter_and_project_function;

public:
  Processor(std::string const &filename) : Pj(LI.lua), Sl(LI.lua) {
    LI.lua.safe_script_file(filename);

    filter_function = LI.lua["filter"];
    project_function = LI.lua["project"];
    weight_function = LI.lua["weight"];
    filter_and_project_function = LI.lua["filter_and_project"];
  }

  bool has_filter() { return bool(filter_function); }

  bool filter(HepMC3::GenEvent const &ev) { return filter_function(ev); }

  bool has_project() { return bool(project_function); }
  std::vector<double> project(HepMC3::GenEvent const &ev) {
    std::vector<double> projections;
    project_function(ev, projections);
    return projections;
  }

  bool has_weight() { return bool(weight_function); }
  std::vector<double> weight(HepMC3::GenEvent const &ev) {
    std::vector<double> weights;
    weight_function(ev, weights);
    return weights;
  }

  bool has_filter_and_project() { return bool(filter_and_project_function); }
  bool filter_and_project(HepMC3::GenEvent const &ev,
                          std::vector<double> &out_projections) {
    if (!has_filter_and_project() && (has_filter() && has_project())) {
      if (!filter(ev)) {
        return false;
      } else {
        out_projections = project(ev);
        return true;
      }
    }

    return filter_and_project_function(ev, out_projections);
  }
};
} // namespace LuaADL