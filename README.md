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

      projections.clear();
      weights.clear();

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