#include "LuaADL/Engine.hxx"

#include "ParticleGenerator.hxx"

int main(int argc, char *const argv[]) {

  LuaADL::Engine evt_eng;

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

  {
    auto prcs1 = evt_eng.Processor(evt_eng.AddProcessorLiteral(R"(
        function filter_and_project(in_GenEvent, out_projections)
  
          out_projections:add(1)
          
          return true
        end
        )"));
    assert(prcs1.has_filter_and_project());
    assert(!prcs1.has_filter());
    assert(!prcs1.has_project());
    assert(!prcs1.has_weight());

    std::vector<double> projs;
    bool out = prcs1.filter_and_project(evt, projs);
    assert(out);
    assert(projs.size() == 1);
    assert(projs[0] == 1);
  }

  {
    auto prcs2 = evt_eng.Processor(evt_eng.AddProcessorLiteral(R"(
        function filter(in_GenEvent)
          return true
        end
        )"));

    assert(!prcs2.has_filter_and_project());
    assert(prcs2.has_filter());
    assert(!prcs2.has_project());
    assert(!prcs2.has_weight());

    bool out = prcs2.filter(evt);
    assert(out);
  }

  {
    auto prcs3 = evt_eng.Processor(evt_eng.AddProcessorLiteral(R"(
        function project(in_GenEvent, out_projections)
          out_projections:add(2)
        end
        )"));

    assert(!prcs3.has_filter_and_project());
    assert(!prcs3.has_filter());
    assert(prcs3.has_project());
    assert(!prcs3.has_weight());

    std::vector<double> projs = prcs3.project(evt);
    assert(projs.size() == 1);
    assert(projs[0] == 2);
  }

  {
    auto prcs4 = evt_eng.Processor(evt_eng.AddProcessorLiteral(R"(
        function filter(in_GenEvent)
          return true
        end

        function project(in_GenEvent, out_projections)
          out_projections:add(3)
        end
        )"));
    assert(prcs4.has_filter_and_project());
    assert(prcs4.has_filter());
    assert(prcs4.has_project());
    assert(!prcs4.has_weight());

    std::vector<double> projs;
    bool out = prcs4.filter_and_project(evt, projs);
    assert(out);
    assert(projs.size() == 1);
    assert(projs[0] == 3);
  }

  {
    auto prcs5 = evt_eng.Processor(evt_eng.AddProcessorLiteral(R"(
        function weight(in_GenEvent, weights)
          weights:add(4)
        end
        )"));
    assert(!prcs5.has_filter_and_project());
    assert(!prcs5.has_filter());
    assert(!prcs5.has_project());
    assert(prcs5.has_weight());

    std::vector<double> wghts = prcs5.weight(evt);
    assert(wghts.size() == 1);
    assert(wghts[0] == 4);
  }
}