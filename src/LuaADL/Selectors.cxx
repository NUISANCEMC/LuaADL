#include "LuaADL/Selectors.hxx"

#include "LuaADL/Selectors.ixx"

namespace LuaADL {

void Selectors::RegisterLuaFunctions(sol::state &lua) {
  if (!lua["LuaADL"].valid()) {
    lua["LuaADL"] = lua.create_table();
  }
  lua["LuaADL"]["sel"] = lua.create_table();
  lua["LuaADL"]["sel"]["part"] = lua.create_table();
  lua["LuaADL"]["sel"]["parts"] = lua.create_table();

  lua["LuaADL"]["sel"]["part"]["beam"] =
      sol::overload(&particle_1pdg<kBeam, kFirst>, &particle<kBeam, kFirst>);

  lua["LuaADL"]["sel"]["parts"]["beam"] = sol::overload(
      &particles_1pdg<kBeam, kFromPDGList>, &particles<kBeam, kFromPDGList>);

  lua["LuaADL"]["sel"]["part"]["highest_momentum"] = lua.create_table();
  lua["LuaADL"]["sel"]["part"]["first"] = lua.create_table();
  lua["LuaADL"]["sel"]["part"]["highest_momentum"]["out"] =
      sol::overload(&particle_1pdg<kUndecayedPhysical, kHighestMomentum>,
                    &particle<kUndecayedPhysical, kHighestMomentum>);
  lua["LuaADL"]["sel"]["part"]["first"]["out"] =
      sol::overload(&particle_1pdg<kUndecayedPhysical, kFirst>,
                    &particle<kUndecayedPhysical, kFirst>);

  lua["LuaADL"]["sel"]["parts"]["out"] =
      sol::overload(&particles_1pdg<kUndecayedPhysical, kFromPDGList>,
                    &particles<kUndecayedPhysical, kFromPDGList>);
  lua["LuaADL"]["sel"]["parts"]["other_out"] =
      sol::overload(&particles_1pdg<kUndecayedPhysical, kNotFromPDGList>,
                    &particles<kUndecayedPhysical, kNotFromPDGList>);
}
} // namespace LuaADL