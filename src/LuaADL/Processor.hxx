#pragma once

#include "HepMC3/LuaInterface.hxx"

#include "HepMC3/GenEvent.h"

namespace LuaADL {
class Processor {

  sol::function filter_function;
  sol::function project_function;
  sol::function weight_function;
  sol::function filter_and_project_function;

  bool has_filter_and_project_function() const;

public:
  enum class ScriptType { file, literal };

  Processor(sol::state &lua, std::string const &script,
            ScriptType t = ScriptType::file);

  bool has_filter() const;

  bool filter(HepMC3::GenEvent const &ev) const;

  bool has_project() const;
  std::vector<double> project(HepMC3::GenEvent const &ev) const;

  bool has_weight() const;
  std::vector<double> weight(HepMC3::GenEvent const &ev) const;

  bool has_filter_and_project() const;
  bool filter_and_project(HepMC3::GenEvent const &ev,
                          std::vector<double> &out_projections) const;
};
} // namespace LuaADL