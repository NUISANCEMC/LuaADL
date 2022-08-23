#include "LuaADL/Engine.hxx"

#include "ParticleGenerator.hxx"

int main(int argc, char *const argv[]) {

  HepMC3::GenEvent evt;
  evt.set_event_number(1);
  HepMC3::GenVertexPtr vertex = std::make_shared<HepMC3::GenVertex>();
  evt.add_vertex(vertex);

  vertex->add_particle_in(GenerateParticle(kBeamStatus, 14, 1.1, 0));
  vertex->add_particle_in(
      GenerateParticle(kTargetStatus, NuclearPDG(6, 12), 0, 0));

  vertex->add_particle_out(GenerateParticle(kObservableStatus, 13, 0.75, 20));
  vertex->add_particle_out(GenerateParticle(kObservableStatus, 2212, 0.2, 20));

  vertex->add_attribute("HardScatterMode",
                        std::make_shared<HepMC3::IntAttribute>(1));

  LuaADL::Engine evt_eng;
  auto prcs1 = evt_eng.GetProcessor(evt_eng.AddProcessorLiteral(R"(
      function filter_and_project(in_GenEvent, out_projections)

        out_projections:add(1)
        
        return true
      end
      )"));

  auto prcs2 = evt_eng.GetProcessor(evt_eng.AddProcessorLiteral(R"(
      function filter_and_project(in_GenEvent, out_projections)

        out_projections:add(2)
        
        return true
      end
      )"));

  auto prcs3 = evt_eng.GetProcessor(evt_eng.AddProcessorLiteral(R"(
      function filter_and_project(in_GenEvent, out_projections)

        out_projections:add(3)
        
        return true
      end
      )"));

  std::vector<double> projs;
  prcs1.filter_and_project(evt, projs);
  prcs2.filter_and_project(evt, projs);
  prcs3.filter_and_project(evt, projs);

  assert(projs.size() == 3);
  assert(projs[0] == 1);
  assert(projs[1] == 2);
  assert(projs[2] == 3);
}