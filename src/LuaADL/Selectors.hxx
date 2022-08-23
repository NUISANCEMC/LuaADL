#pragma once

#include "sol/sol.hpp"

namespace LuaADL {

class Selectors {
public:
  Selectors() {}

  void RegisterLuaFunctions(sol::state &lua);
};
} // namespace LuaADL