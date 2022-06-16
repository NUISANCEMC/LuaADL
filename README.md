# LuaADL

A lua-based Analysis Descriptor Language (ADL) framework for HepMC3 events. Analysis-specific cuts and projections can be defined in short scripts. The ADL is implemented as additional helper methods provided in the lua environment that user scripts are run in. It can be naturally extended by including additional methods in user code.

## Example

```cpp

#include "LuaADL/Processor.hxx"

#include "HepMC3/ReaderAscii.h"

int main(int argc, char *const argv[]) {

  HepMC3::ReaderAscii rdr(argv[1]);
  LuaADL::Processor prs(argv[2]);

  while(!rdr.failed()){
    HepMC3::GenEvent evt;
    rdr.read_event(evt);

    if(rdr.failed()){
      break;
    }

    if(prs.select(evt)){

      std::vector<double> projections = prs.project(evt);
      std::vector<double> weights = prs.weight(evt);

      //do something with selected events
    }
  }
}

```

```lua
--Check that we have LuaADL
assert(LuaADL)

function isCC0Pi(in_GenEvent)
  if not LuaADL.sel.part.beam(in_GenEvent, 14) then return false end

  local nnother = LuaADL.sel.parts.other_out(in_GenEvent, {13, 2212, 2112}):size()
  if not nnother == 0 then return false end

  if not LuaADL.sel.part.out.highest_momentum(in_GenEvent, 13) then 
    return false 
  end

  return true
end

function isTrueCCQE(in_GenEvent)
  if not in_GenEvent:vertices() or 
    in_GenEvent:vertices():size() == 0 or
    not in_GenEvent:vertices()[1] then
    return false
  end

  local attr = in_GenEvent:vertices()[1]:attribute_as_int("HardScatterMode")

  return attr and (attr:value() == 1)
end

function filter(in_GenEvent)
  return isTrueCCQE(in_GenEvent) and isCC0Pi(in_GenEvent)
end

function project(in_GenEvent, out_projections)

  local ISnu = LuaADL.sel.part.beam(in_GenEvent, 14)
  print("Enu: ", ISnu.momentum.e)
  out_projections:add(ISnu.momentum.e)

  local FSmu = LuaADL.sel.part.highest_momentum.out(in_GenEvent, 13)
  print("Pmu: ", FSmu.momentum:length())
  out_projections:add(FSmu.momentum:length())

  local q0 = LuaADL.proj.parts.q0(ISnu,FSmu)
  print("q0: ", q0)
  out_projections:add(q0)

  local q3 = LuaADL.proj.parts.q3(ISnu,FSmu)
  print("q3: ", q3)
  out_projections:add(q3)

  local Q2 = LuaADL.proj.parts.Q2(ISnu,FSmu)
  print("Q2: ", Q2)
  out_projections:add(Q2)

  local cpt = LuaADL.proj.parts.pt(LuaADL.sel.parts.out(in_GenEvent, {13, 2212}))
  print("charged pt: ", cpt)
  out_projections:add(cpt)

end

function weight(in_GenEvent, out_weights)
  local ISnu = LuaADL.sel.part.beam(in_GenEvent, 14)
  local FSmu = LuaADL.sel.part.highest_momentum.out(in_GenEvent, 13)
  out_weights:add(1.0/LuaADL.proj.parts.Q2(ISnu,FSmu))
end

```

## HepMC3/LuaInterface

The Lua bindings for HepMC3 were written to serve LuaADL. As such, they do not expose the full functionality of HepMC3 and are not intended to be used in their current state to read, construct, modify, or write HepMC3 objects from pure Lua. Notably there are no bindings for the Reader/Writer subclasses, and many object constructors are not bound. That said, these missing features would not be difficult to add, if you are interested, please file an issue or submit a PR.

The currently implemented bindings are detailed below:

```lua

Attribute:to_string()

IntAttribute:to_string()
IntAttribute:value()

DoubleAttribute:to_string()
DoubleAttribute:value()

FourVector.x
FourVector.y
FourVector.z
FourVector.t
FourVector.px
FourVector.py
FourVector.pz
FourVector.e
FourVector:length2()
FourVector:length()
FourVector:m2()
FourVector:m()
FourVector:phi()
FourVector:theta()
FourVector:is_zero()
FourVector:to_str() -- uses HepMC3::Print::line to convert the FourVector to a string

GenEvent.event_number
GenEvent.event_pos
GenEvent:run_info()
GenEvent:shift_position_by()
GenEvent:shift_position_to()
GenEvent:boost()
GenEvent:rotate()
GenEvent:reflect()
GenEvent:particles()
GenEvent:beams()
GenEvent:vertices()
GenEvent:weights()
GenEvent:weight_names()
GenEvent:to_str() -- uses HepMC3::Print::listing to convert the FourVector to a string

GenParticle.pid
GenParticle.status
GenParticle.momentum
GenParticle.generated_mass
GenParticle:abs_pid()
GenParticle:in_event()
GenParticle:id()
GenParticle:production_vertex()
GenParticle:end_vertex()
GenParticle:parents()
GenParticle:children()
GenParticle:is_generated_mass_set()
GenParticle:parent_event()
GenParticle:attribute_names()
GenParticle:attribute_as_string(attr_name)
GenParticle:attribute_as_int(attr_name)
GenParticle:attribute_as_double(attr_name)
GenParticle:to_str() -- uses HepMC3::Print::line to convert the FourVector to a string

ToolInfo:name()
ToolInfo:version()
ToolInfo:description()

GenRunInfo:tools()
GenRunInfo:attribute_names()
GenRunInfo:attribute_as_string(attr_name)
GenRunInfo:attribute_as_int(attr_name)
GenRunInfo:attribute_as_double(attr_name)
GenRunInfo:to_str()

GenVertex.id
GenVertex.status
GenVertex.position
GenVertex:has_set_position()
GenVertex:particles_in()
GenVertex:particles_out()
GenVertex:attribute_names()
GenVertex:attribute_as_string(attr_name)
GenVertex:attribute_as_int(attr_name)
GenVertex:attribute_as_double(attr_name)
GenVertex:to_str() -- uses HepMC3::Print::line to convert the FourVector to a string

```

Note that Lua's call syntax for 'member functions' differs from C-like languages (and python). `myobj.x` accesses the `x` member of `myobj`, `myobj.func` is a reference to the function, and so `myobj.func()` calls the function, but without binding `myobj` as the `this` or `self` parameter. The desired effect could be achieved with `myobj.func(myobj)`, but the colon operator can be used as a sweet syntactic shortcut, `myobj:func()`, invokes `myobj.func` passing `myobj` as the first parameter.

## LuaADL hooks

Analysis Descriptor lua scripts should implement some subset of the four hook functions: `filter`, `project`, `weight`, and `filter_and_project`. Each act on a single `HepMC3::GenEvent`.

```lua
function filter(in_GenEvent) 
  if some_cut(in_GenEvent) then return false end
  return true
end

function project(in_GenEvent, out_projections) 
  x = calculate_projection(in_GenEvent)
  out_projections:add(x)
end

function weight(in_GenEvent, out_weights) 
  y = calculate_weight(in_GenEvent)
  out_weights:add(y)
end

function filter_and_project(in_GenEvent, out_projections)
  if some_cut(in_GenEvent) then return false end
  x = calculate_projection(in_GenEvent)
  out_projections:add(x)
  return true
end
```

These functions are wrapped in C++ functions with the following signatures:

```cpp
bool LuaADL::Processor::filter(HepMC3::GenEvent const &ev);

std::vector<double> LuaADL::Processor::project(HepMC3::GenEvent const &ev);

std::vector<double> LuaADL::Processor::weight(HepMC3::GenEvent const &ev);

bool LuaADL::Processor::filter_and_project(HepMC3::GenEvent const &ev,
                                            std::vector<double> &out_projections);
```

The final function is available for convenience and efficiency. If it is not defined in a given script, but `LuaADL::Processor::filter_and_project` is called from C++, then the `filter` and `project` lua functions will be called sequentially and their results combined. These functions should be implemented for a given analysis, and can make use of the LuaADL convenience functions, described below.

## LuaADL Reference

### Selectors

Functions that select particles from `GenEvent`s

```lua
  -- LuaADL.sel.part
  -- Return a reference to a particle, a nil return value denotes that none were found
  -------------------------------------------------------------------------------
  LuaADL.sel.part.beam(event, 14) -- selects a single incoming particle with PDG 14
  LuaADL.sel.part.beam(event, {12,14,16}) -- selects a single incoming particle with PDG 12, 14, or 16

  LuaADL.sel.part.first.out(event, 13) -- selects the first undecayed physical outgoing particle with PDG 13
  LuaADL.sel.part.first.out(event, {11,13,15}) -- selects the first undecayed physical outgoing particle with PDG 11, 13, or 15

  LuaADL.sel.part.highest_momentum.out(event, 14) -- selects the highest momentum undecayed physical outgoing particle with PDG 13
  LuaADL.sel.part.highest_momentum.out(event, {12,14,16}) -- selects the first highest momentum physical outgoing particle with PDG 11, 13, or 15
  -------------------------------------------------------------------------------

  -- LuaADL.sel.parts
  -- Return a sequence of particles, empty sequence denotes that none were found
  -------------------------------------------------------------------------------
  LuaADL.sel.parts.beam(event, 14) -- selects all incoming particles with PDG 14
  LuaADL.sel.parts.beam(event, {12,14,16}) -- selects all incoming particles with PDG 12, 14, or 16

  LuaADL.sel.parts.out(event, 14) -- selects all undecayed physical outgoing particles with PDG 14
  LuaADL.sel.parts.out(event, {12,14,16}) -- selects all undecayed physical outgoing particles with PDG 12, 14, or 16

  LuaADL.sel.parts.other_out(event, 14)  -- selects all undecayed physical outgoing particles with any PDG other than 14
  LuaADL.sel.parts.other_out(event, {12,14,16})  -- selects all undecayed physical outgoing particles with any PDG other than 12, 14, or 16
  -------------------------------------------------------------------------------

```
### Projectors

Functions that project out kinematic quantities from particles or sequences of particles

```lua
  -- Most functions return doubles, a return value of 0xdeadbeef indicates an error
  -- This usually occurs because the function was passed an empty shared_ptr

  -- Two particle functions
  ----------------
  LuaADL.proj.parts.q0(part1, part2) -- calculate the energy transfer between two particles
  LuaADL.proj.parts.q0({part1, part2, ...}) -- calculate the energy transfer between the first two particles in a sequence

  LuaADL.proj.parts.q3(part1, part2) -- calculate the 3momentum transfer between two particles
  LuaADL.proj.parts.q3({part1, part2, ...}) -- calculate the 3momentum transfer between the first two particles in a sequence

  LuaADL.proj.parts.Q2(part1, part2) -- calculate the 4momentum transfer between two particles
  LuaADL.proj.parts.Q2({part1, part2, ...}) -- calculate the 4momentum transfer between the first two particles in a sequence

  LuaADL.proj.parts.theta(part1, part2) -- calculate the opening angle between two particles
  LuaADL.proj.parts.theta({part1, part2, ...}) -- calculate the opening angle between the first two particles in a sequence

  LuaADL.proj.parts.costheta(part1, part2) -- calculate the cosine of the opening angle between two particles
  LuaADL.proj.parts.costheta({part1, part2, ...}) -- calculate the cosine of the opening angle between the first two particles in a sequence

  -- N particle functions
  ----------------
  LuaADL.proj.parts.W({part1, part2, ...}) -- calculate the invariant mass of a sequence of particles
  LuaADL.proj.parts.pt({part1, part2, ...}) -- calculate the magnitude of the transverse momentum from the 3momentum sum of a sequence of particles

  -- Returns a HepMC3::FourVector, pmiss.x == 0xdeadbeef for failure 
  LuaADL.proj.parts.pmiss({part1, part2, ...}, {part1, part2, ...}) -- calculate the missing 4momentum between two sequences of particles

```
