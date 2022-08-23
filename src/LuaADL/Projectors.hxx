#pragma once

#include "sol/sol.hpp"

namespace LuaADL {

class Projectors {

public:
  Projectors() {}

  void RegisterLuaFunctions(sol::state &lua);
};
} // namespace LuaADL