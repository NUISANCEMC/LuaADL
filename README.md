# LuaADL

A lua-based Analysis Descriptor Language (ADL) framework for HepMC3 events.

Cuts and projections can be defined in short, dedicated lua scripts that 
describe signal definitions and observables from published HEP data.

The ADL is implemented as additional helper methods provided in the lua 
environment that user scripts are run in. The ADL can be naturally extended by 
including additional ADL methods in user code.

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

  local FSmu = LuaADL.sel.part.out.highest_momentum(in_GenEvent, 13)
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
  local FSmu = LuaADL.sel.part.out.highest_momentum(in_GenEvent, 13)
  out_weights:add(1.0/LuaADL.proj.parts.Q2(ISnu,FSmu))
end

```

## ADL Reference

### Selectors

Functions that select particles and vertices from `GenEvent`s

```lua
  -- ADL.sel.part
  -- Return a reference to a particle, a nil return value denotes that none were found
  -------------------------------------------------------------------------------
  ADL.sel.part.beam(event, 14) -- selects a single incoming particle with PDG 14
  ADL.sel.part.beam(event, {12,14,16}) -- selects a single incoming particle with PDG 12, 14, or 16

  ADL.sel.part.out.first(event, 13) -- selects the first undecayed physical outgoing particle with PDG 13
  ADL.sel.part.out.first(event, {11,13,15}) -- selects the first undecayed physical outgoing particle with PDG 11, 13, or 15

  ADL.sel.part.out.highest_momentum(event, 14) -- selects the highest momentum undecayed physical outgoing particle with PDG 13
  ADL.sel.part.out.highest_momentum(event, {12,14,16}) -- selects the first highest momentum physical outgoing particle with PDG 11, 13, or 15
  -------------------------------------------------------------------------------

  -- ADL.sel.parts
  -- Return a sequence of particles, empty sequence denotes that none were found
  -------------------------------------------------------------------------------
  ADL.sel.parts.beam(event, 14) -- selects all incoming particles with PDG 14
  ADL.sel.parts.beam(event, {12,14,16}) -- selects all incoming particles with PDG 12, 14, or 16

  ADL.sel.parts.out(event, 14) -- selects all undecayed physical outgoing particles with PDG 14
  ADL.sel.parts.out(event, {12,14,16}) -- selects all undecayed physical outgoing particles with PDG 12, 14, or 16

  ADL.sel.parts.other_out(event, 14)  -- selects all undecayed physical outgoing particles with any PDG other than 14
  ADL.sel.parts.other_out(event, {12,14,16})  -- selects all undecayed physical outgoing particles with any PDG other than 12, 14, or 16
  -------------------------------------------------------------------------------

```
### Projectors

Functions that project out kinematic quantities from particles or sequences of particles

```lua
  -- Most functions return doubles, a return value of 0xdeadbeef indicates an error
  -- This usually occurs because the function was passed an empty shared_ptr

  -- Two particle functions
  ----------------
  ADL.proj.parts.q0(part1, part2) -- calculate the energy transfer between two particles
  ADL.proj.parts.q0({part1, part2, ...}) -- calculate the energy transfer between the first two particles in a sequence

  ADL.proj.parts.q3(part1, part2) -- calculate the 3momentum transfer between two particles
  ADL.proj.parts.q3({part1, part2, ...}) -- calculate the 3momentum transfer between the first two particles in a sequence

  ADL.proj.parts.Q2(part1, part2) -- calculate the 4momentum transfer between two particles
  ADL.proj.parts.Q2({part1, part2, ...}) -- calculate the 4momentum transfer between the first two particles in a sequence

  ADL.proj.parts.theta(part1, part2) -- calculate the opening angle between two particles
  ADL.proj.parts.theta({part1, part2, ...}) -- calculate the opening angle between the first two particles in a sequence

  ADL.proj.parts.costheta(part1, part2) -- calculate the cosine of the opening angle between two particles
  ADL.proj.parts.costheta({part1, part2, ...}) -- calculate the cosine of the opening angle between the first two particles in a sequence

  -- N particle functions
  ----------------
  ADL.proj.parts.W({part1, part2, ...}) -- calculate the invariant mass of a sequence of particles
  ADL.proj.parts.pt({part1, part2, ...}) -- calculate the magnitude of the transverse momentum from the 3momentum sum of a sequence of particles

  -- Returns a HepMC3::FourVector, pmiss.x == 0xdeadbeef for failure 
  ADL.proj.parts.pmiss({part1, part2, ...}, {part1, part2, ...}) -- calculate the missing 4momentum between two sequences of particles

```
