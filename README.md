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
require "math"

function select(GenEvent)

  muon = select.final_state(GenEvent, 13, select.highest_momentum)

  -- If there are no final state muons, cut
  if not muon then return false end

  -- Apply true phase space cuts
  return (muon.momentum:length() > 0.25) 
        and (muon.momentum:theta() < math.rad(20))

end

function project(GenEvent, out_projections)
  muon = select.particles.final_state(GenEvent, 13, select.highest_momentum)
  numu = select.particles.initial_state(GenEvent, 14)

  if (not muon) or (not numu) then return end

  -- Add some event projections
  out_projections:add(muon.momentum.e)
  out_projections:add(project.pt(select.particles.final_states(GenEvent, select.particles.charged)));
  out_projections:add(project.W(select.particles.final_states(GenEvent)));
end

function weight(GenEvent, out_weights)
  muon = select.particles.final_state(GenEvent, 13, select.highest_momentum)

  -- Weight based on the Q2 calculated from the muon production vertex
  out_weights:add(1.0/project.Q2(muon:production_vertex()))
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
  -- ADL.proj.part
  ----------------
  ADL.proj.parts.q0({part1, part2, ...}) -- calculate the energy transfer between the first two particles in a sequence
  ADL.proj.parts.q3({part1, part2, ...}) -- calculate the 3momentum transfer between the first two particles in a sequence
  ADL.proj.parts.Q2({part1, part2, ...}) -- calculate the 4momentum transfer between the first two particles in a sequence
  ADL.proj.parts.theta({part1, part2, ...}) -- calculate the opening angle between the first two particles in a sequence
  ADL.proj.parts.costheta({part1, part2, ...}) -- calculate the cosine of the opening angle between the first two particles in a sequence

  ADL.proj.parts.W({part1, part2, ...}) -- calculate the invariant mass of a sequence of particles
  ADL.proj.parts.pt({part1, part2, ...}) -- calculate the transverse momentum from the 3momentum sum of a sequence of particles
  ADL.proj.parts.pmiss({part1, part2, ...}, {part1, part2, ...}) -- calculate the missing 3momentum between two sequences of particles
  ADL.proj.parts.emiss({part1, part2, ...}, {part1, part2, ...}) -- calculate the missing total energy between two sequences of particles

```
