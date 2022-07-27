#pragma once

#include "HepMC3/Lua/Attribute.hxx"
#include "HepMC3/Lua/FourVector.hxx"
#include "HepMC3/Lua/GenEvent.hxx"
#include "HepMC3/Lua/GenParticle.hxx"
#include "HepMC3/Lua/GenRunInfo.hxx"
#include "HepMC3/Lua/GenVertex.hxx"

#include "sol/sol.hpp"

namespace HepMC3 {
struct LuaInterface {

  AttributeTypes Attribute_types;
  sol::usertype<FourVector> FourVector_type;
  sol::usertype<GenParticle> GenParticle_type;
  GenRunInfoTypes GenRuninfo_types;
  sol::usertype<GenEvent> GenEvent_type;
  sol::usertype<GenVertex> GenVertex_type;

  LuaInterface() {}
  void RegisterLuaFunctions(sol::state &lua) {
    Attribute_types = RegisterAttributes(lua);
    GenRuninfo_types = RegisterGenRunInfo(lua);

    FourVector_type = RegisterFourVector(lua);
    GenParticle_type = RegisterGenParticle(lua);
    GenVertex_type = RegisterGenVertex(lua);
    GenEvent_type = RegisterGenEvent(lua);
  }
};

} // namespace HepMC3