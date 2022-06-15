#include "LuaADL/Processor.hxx"

#include "HepMC3/ReaderAscii.h"
#include "HepMC3/WriterAscii.h"

#include "ParticleGenerator.hxx"

#include <sstream>

std::string TestFile() {
  std::stringstream ss("");
  HepMC3::WriterAscii wrtr(ss);

  {
    HepMC3::GenEvent evt;
    evt.set_event_number(1);
    HepMC3::GenVertexPtr vertex = std::make_shared<HepMC3::GenVertex>();
    evt.add_vertex(vertex);

    vertex->add_particle_in(GenerateParticle(kBeamStatus, 14, 1.1, 0));
    vertex->add_particle_in(
        GenerateParticle(kTargetStatus, NuclearPDG(6, 12), 0, 0));

    vertex->add_particle_out(GenerateParticle(kObservableStatus, 13, 0.75, 20));
    vertex->add_particle_out(
        GenerateParticle(kObservableStatus, 2212, 0.2, 20));

    vertex->add_attribute("HardScatterMode",
                          std::make_shared<HepMC3::IntAttribute>(1));

    wrtr.write_event(evt);
  }

  {
    HepMC3::GenEvent evt;
    evt.set_event_number(2);
    HepMC3::GenVertexPtr vertex = std::make_shared<HepMC3::GenVertex>();
    evt.add_vertex(vertex);

    vertex->add_particle_in(GenerateParticle(kBeamStatus, 14, 1.2, 0));
    vertex->add_particle_in(
        GenerateParticle(kTargetStatus, NuclearPDG(6, 12), 0, 0));

    vertex->add_particle_out(GenerateParticle(kObservableStatus, 13, 0.8, 40));
    vertex->add_particle_out(GenerateParticle(kObservableStatus, 2212, 0.2, 0));

    vertex->add_attribute("HardScatterMode",
                          std::make_shared<HepMC3::IntAttribute>(1));

    wrtr.write_event(evt);
  }

  {
    HepMC3::GenEvent evt;
    evt.set_event_number(3);
    HepMC3::GenVertexPtr vertex = std::make_shared<HepMC3::GenVertex>();
    evt.add_vertex(vertex);

    vertex->add_particle_in(GenerateParticle(kBeamStatus, 14, 1.3, 0));
    vertex->add_particle_in(
        GenerateParticle(kTargetStatus, NuclearPDG(6, 12), 0, 0));

    vertex->add_particle_out(GenerateParticle(kObservableStatus, 13, 0.6, 40));
    vertex->add_particle_out(GenerateParticle(kObservableStatus, 2212, 0.2, 0));
    vertex->add_particle_out(GenerateParticle(kObservableStatus, 2212, 0.56, 12));

    vertex->add_attribute("HardScatterMode",
                          std::make_shared<HepMC3::IntAttribute>(1));

    wrtr.write_event(evt);
  }

  {
    HepMC3::GenEvent evt;
    evt.set_event_number(4);
    HepMC3::GenVertexPtr vertex = std::make_shared<HepMC3::GenVertex>();
    evt.add_vertex(vertex);

    vertex->add_particle_in(GenerateParticle(kBeamStatus, 14, 1.4, 0));
    vertex->add_particle_in(
        GenerateParticle(kTargetStatus, NuclearPDG(6, 12), 0, 0));

    vertex->add_particle_out(GenerateParticle(kObservableStatus, 13, 0.35, 40));
    vertex->add_particle_out(GenerateParticle(kObservableStatus, 2212, 0.2, 0));
    vertex->add_particle_out(GenerateParticle(kObservableStatus, 2212, 0.3, 15));
    vertex->add_particle_out(GenerateParticle(kObservableStatus, 211, 0.3, 15));

    vertex->add_attribute("HardScatterMode",
                          std::make_shared<HepMC3::IntAttribute>(3));

    wrtr.write_event(evt);
  }

  wrtr.close();

  return ss.str();
}

int main(int argc, char *const argv[]) {

  std::stringstream ss("");

  ss << TestFile();

  HepMC3::ReaderAscii rdr(ss);

  LuaADL::Processor prcs(argv[1]);

  std::cout << "Has filter: " << prcs.has_filter() << std::endl;
  std::cout << "Has Project: " << prcs.has_project() << std::endl;
  std::cout << "Has Weight: " << prcs.has_weight() << std::endl;
  std::cout << "Has Filter and Project: " << prcs.has_filter_and_project() << std::endl;

  while(!rdr.failed()){
    HepMC3::GenEvent evt;
    rdr.read_event(evt);

    if(rdr.failed()){
      break;
    }
    
    HepMC3::Print::listing(evt);

    std::vector<double> projections;
    bool passed = prcs.filter_and_project(evt, projections);

    std::cout << "Event: " << (passed ? "passed selection":"was cut") << std::endl;
    std::cout << "Projections: " << std::endl;
    for(auto &i : projections){
      std::cout << i << std::endl;
    }
    std::cout << "================" << std::endl;
  }
}