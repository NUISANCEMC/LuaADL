#include "LuaADL/Engine.hxx"

namespace LuaADL {

Engine::Engine() {
  lua.open_libraries();

  LI.RegisterLuaFunctions(lua);

  Pj.RegisterLuaFunctions(lua);
  Sl.RegisterLuaFunctions(lua);
}

Engine::ProcessorId_t
Engine::AddProcessorFromFile(std::string const &filename) {
  processor_files.emplace_back(filename);
  processors.emplace_back(lua, filename);

  // clean up global handles
  lua["filter"] = sol::lua_nil;
  lua["project"] = sol::lua_nil;
  lua["weight"] = sol::lua_nil;
  lua["filter_and_project"] = sol::lua_nil;

  return processors.size() - 1;
}

Engine::ProcessorId_t Engine::AddProcessorLiteral(std::string const &script) {
  processor_files.emplace_back("script_literal");
  processors.emplace_back(lua, script, Processor::ScriptType::literal);

  // clean up global handles
  lua["filter"] = sol::lua_nil;
  lua["project"] = sol::lua_nil;
  lua["weight"] = sol::lua_nil;
  lua["filter_and_project"] = sol::lua_nil;

  return processors.size() - 1;
}

Processor const &Engine::GetProcessor(Engine::ProcessorId_t pid) {
  if (processors.size() > pid) {
    return processors[pid];
  }

  throw;
}

} // namespace LuaADL