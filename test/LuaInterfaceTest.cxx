#include "HepMC3/LuaInterface.hxx"

#include <cassert>
#include <iostream>

HepMC3::GenEvent DummyEventHelper() {
  HepMC3::GenEvent evt;

  int const kISStatus = 4;
  int const kFSStatus = 1;
  HepMC3::GenParticlePtr ISNeutron = std::make_shared<HepMC3::GenParticle>(
      HepMC3::FourVector{1.5255172492130473e+02, 8.9392830847276528e+01,
                         6.4870597568257821e+01, 9.5825554558124941e+02},
      2112, kISStatus);
  HepMC3::GenParticlePtr ISnumu = std::make_shared<HepMC3::GenParticle>(
      HepMC3::FourVector{0.0000000000000000e+00, 0.0000000000000000e+00,
                         1.5000000000000000e+03, 1.5000000000000000e+03},
      14, kISStatus);
  HepMC3::GenParticlePtr FSmuon = std::make_shared<HepMC3::GenParticle>(
      HepMC3::FourVector{-6.8928697531845643e+01, 4.8219068401438176e+02,
                         1.2406574501351240e+03, 1.3370316161682497e+03},
      13, kFSStatus);
  HepMC3::GenParticlePtr FSProton = std::make_shared<HepMC3::GenParticle>(
      HepMC3::FourVector{2.2148042245314980e+02, -3.9279785316710411e+02,
                         3.2421314743313258e+02, 1.0903266675337304e+03},
      2212, kFSStatus);

  // Set masses
  ISNeutron->set_generated_mass(9.3956499999999994e+02);
  ISnumu->set_generated_mass(0.0000000000000000e+00);
  FSmuon->set_generated_mass(1.0565800000000023e+02);
  FSProton->set_generated_mass(9.3827200000000005e+02);

  // Make vertex
  HepMC3::GenVertexPtr vertex = std::make_shared<HepMC3::GenVertex>(
      HepMC3::FourVector{1E1, 2E2, 3E3, 4E4});

  evt.add_vertex(vertex);

  vertex->add_particle_in(ISNeutron);
  vertex->add_particle_in(ISnumu);
  vertex->add_particle_out(FSmuon);
  vertex->add_particle_out(FSProton);

  vertex->add_attribute("HardScatterMode",
                        std::make_shared<HepMC3::IntAttribute>(1));

  evt.weights() = std::vector<double>{1.23456789, 9.87654321};
  evt.set_event_number(1337);
  evt.set_units(HepMC3::Units::MEV, HepMC3::Units::CM);

  std::shared_ptr<HepMC3::GenRunInfo> gri =
      std::make_shared<HepMC3::GenRunInfo>();
  evt.set_run_info(gri);

  gri->tools().emplace_back(HepMC3::GenRunInfo::ToolInfo{
      "NuDum", "0.99.0", "A dummy neutrino event generator"});

  return evt;
}

int main(int argc, char const *argv[]) {

  HepMC3::LuaInterface LI;

  HepMC3::GenEvent evt = DummyEventHelper();
  LI.lua["event"] = evt;
  LI.lua["part"] = evt.particles().front();
  LI.lua["vert"] = evt.vertices().front();
  LI.lua["fourvector"] = evt.particles().front()->momentum();

  for (int i = 1; i < argc; ++i) {
    std::cout << "[Info]: Running test script: \"" << argv[i] << "\""
              << std::endl;
    int rtn = LI.lua.safe_script_file(argv[i]);
    std::cout << "return value: " << rtn << std::endl;
  }

  std::vector<double> momenta;
  LI.lua["momenta"] = std::ref(momenta);
  LI.lua.safe_script_file("particle_momenta.lua");
  for(auto &i : momenta){
    std::cout << "mom: " << i << std::endl;
  }
}