#include "LuaADL/Processor.hxx"

namespace LuaADL {

bool Processor::has_filter_and_project_function() const {
  return filter_and_project_function.valid();
}

Processor::Processor(sol::state &lua, std::string const &script,
                     ScriptType t)
    : filter_function(sol::lua_nil), project_function(sol::lua_nil),
      weight_function(sol::lua_nil), filter_and_project_function(sol::lua_nil) {

  switch (t) {
  case ScriptType::file: {
    lua.safe_script_file(script);
    break;
  }
  case ScriptType::literal: {
    lua.safe_script(script);
    break;
  }
  }

  filter_function = lua["filter"];
  project_function = lua["project"];
  weight_function = lua["weight"];
  filter_and_project_function = lua["filter_and_project"];
}

bool Processor::has_filter() const { return filter_function.valid(); }

bool Processor::filter(HepMC3::GenEvent const &ev) const {
  return filter_function(ev);
}

bool Processor::has_project() const { return project_function.valid(); }
std::vector<double> Processor::project(HepMC3::GenEvent const &ev) const {
  std::vector<double> projections;
  project_function(ev, projections);
  return projections;
}

bool Processor::has_weight() const { return weight_function.valid(); }
std::vector<double> Processor::weight(HepMC3::GenEvent const &ev) const {
  std::vector<double> weights;
  weight_function(ev, weights);
  return weights;
}

bool Processor::has_filter_and_project() const {
  return has_filter_and_project_function() || (has_filter() && has_project());
}

bool Processor::filter_and_project(HepMC3::GenEvent const &ev,
                                   std::vector<double> &out_projections) const {
  if (!has_filter_and_project_function() && (has_filter() && has_project())) {
    if (!filter(ev)) {
      return false;
    } else {
      out_projections = project(ev);
      return true;
    }
  }

  return filter_and_project_function(ev, out_projections);
}
} // namespace LuaADL