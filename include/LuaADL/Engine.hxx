#pragma once

#include "sol/sol.hpp"

#include "LuaADL/Processor.hxx"
#include "LuaADL/Projectors.hxx"
#include "LuaADL/Selectors.hxx"

#include "HepMC3/LuaInterface.hxx"

#include <string>
#include <vector>

namespace LuaADL {

class Engine {

  sol::state lua;
  Projectors Pj;
  Selectors Sl;
  HepMC3::LuaInterface LI;

  std::vector<std::string> processor_files;
  std::vector<Processor> processors;

public:
  using ProcessorId_t = size_t;

  Engine() {
    lua.open_libraries();

    LI.RegisterLuaFunctions(lua);

    Pj.RegisterLuaFunctions(lua);
    Sl.RegisterLuaFunctions(lua);
  }

  ProcessorId_t AddProcessorFromFile(std::string const &filename) {
    processor_files.emplace_back(filename);
    processors.emplace_back(lua, filename);

    // clean up global handles
    lua["filter"] = sol::lua_nil;
    lua["project"] = sol::lua_nil;
    lua["weight"] = sol::lua_nil;
    lua["filter_and_project"] = sol::lua_nil;

    return processors.size() - 1;
  }

  ProcessorId_t AddProcessorLiteral(std::string const &script) {
    processor_files.emplace_back("script_literal");
    processors.emplace_back(lua, script, Processor::ScriptType::literal);

    // clean up global handles
    lua["filter"] = sol::lua_nil;
    lua["project"] = sol::lua_nil;
    lua["weight"] = sol::lua_nil;
    lua["filter_and_project"] = sol::lua_nil;
    
    return processors.size() - 1;
  }

  Processor const &Processor(ProcessorId_t pid) {
    if (processors.size() > pid) {
      return processors[pid];
    }

    throw;
  }
};

} // namespace LuaADL