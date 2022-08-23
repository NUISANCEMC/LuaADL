#pragma once

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

  Engine();

  ProcessorId_t AddProcessorFromFile(std::string const &filename);
  ProcessorId_t AddProcessorLiteral(std::string const &script);

  Processor const &GetProcessor(ProcessorId_t pid);
};

} // namespace LuaADL