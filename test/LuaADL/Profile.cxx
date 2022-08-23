#include "LuaADL/Engine.hxx"

#include "LuaADL/Selectors.ixx"

#include "HepMC3/ReaderAscii.h"

#include <chrono>

int main(int argc, char *const argv[]) {

  {
    HepMC3::ReaderAscii rdr("MiniBooNE_CH2_numu.hepmc3");
    size_t nevents = 0;

    auto just_read_before = std::chrono::high_resolution_clock::now();
    while (!rdr.failed()) {
      HepMC3::GenEvent evt;
      rdr.read_event(evt);

      if (rdr.failed()) {
        break;
      }
      nevents++;
    }
    auto just_read_after = std::chrono::high_resolution_clock::now();

    std::cout << "[INFO]: Read " << nevents << " events "
              << " in "
              << std::chrono::duration_cast<std::chrono::milliseconds>(
                     just_read_after - just_read_before)
                         .count() /
                     1E3
              << " s." << std::endl;
  }

  {
    HepMC3::ReaderAscii rdr("MiniBooNE_CH2_numu.hepmc3");
    size_t nevents = 0, nselected = 0;

    auto cpp_selproj_before = std::chrono::high_resolution_clock::now();
    while (!rdr.failed()) {
      HepMC3::GenEvent evt;
      rdr.read_event(evt);

      if (rdr.failed()) {
        break;
      }
      nevents++;

      std::vector<double> projections;

      auto ISnu = LuaADL::particle_1pdg<LuaADL::kBeam, LuaADL::kFirst>(evt, 14);
      if (!ISnu) {
        continue;
      }
      auto FSmus = LuaADL::particles_1pdg<LuaADL::kUndecayedPhysical,
                                          LuaADL::kFromPDGList>(evt, 13);
      if (FSmus.size() != 1) {
        continue;
      }

      auto FSOther =
          LuaADL::particles<LuaADL::kUndecayedPhysical,
                            LuaADL::kNotFromPDGList>(evt, {13, 2212, 2112});
      if (FSOther.size()) {
        continue;
      }

      projections.push_back(FSmus[0]->momentum().length());
      projections.push_back(FSmus[0]->momentum().theta());
      nselected++;
    }
    auto cpp_selproj_after = std::chrono::high_resolution_clock::now();

    std::cout << "[INFO]: C++ Selected " << nselected << "/" << nevents
              << " events "
              << " in "
              << std::chrono::duration_cast<std::chrono::milliseconds>(
                     cpp_selproj_after - cpp_selproj_before)
                         .count() /
                     1E3
              << " s." << std::endl;
  }

  {
    HepMC3::ReaderAscii rdr("MiniBooNE_CH2_numu.hepmc3");

    LuaADL::Engine evt_eng;
    auto processor_id = evt_eng.AddProcessorFromFile("noop.lua");
    auto prcs = evt_eng.GetProcessor(processor_id);

    size_t nevents = 0, nselected = 0;

    auto lua_selproj_before = std::chrono::high_resolution_clock::now();
    while (!rdr.failed()) {
      HepMC3::GenEvent evt;
      rdr.read_event(evt);

      if (rdr.failed()) {
        break;
      }
      nevents++;

      std::vector<double> projections;
      bool passed = prcs.filter_and_project(evt, projections);
      if (passed) {
        nselected++;
      }
    }
    auto lua_selproj_after = std::chrono::high_resolution_clock::now();

    std::cout << "[INFO]: Lua No-Op'd through " << nevents << " events "
              << " in "
              << std::chrono::duration_cast<std::chrono::milliseconds>(
                     lua_selproj_after - lua_selproj_before)
                         .count() /
                     1E3
              << " s." << std::endl;
  }

  {
    HepMC3::ReaderAscii rdr("MiniBooNE_CH2_numu.hepmc3");

    LuaADL::Engine evt_eng;
    auto processor_id =
        evt_eng.AddProcessorFromFile("MiniBooNE_sel_LuaADL.lua");
    auto prcs = evt_eng.GetProcessor(processor_id);

    size_t nevents = 0, nselected = 0;

    auto lua_selproj_before = std::chrono::high_resolution_clock::now();
    while (!rdr.failed()) {
      HepMC3::GenEvent evt;
      rdr.read_event(evt);

      if (rdr.failed()) {
        break;
      }
      nevents++;

      std::vector<double> projections;
      bool passed = prcs.filter_and_project(evt, projections);
      if (passed) {
        nselected++;
      }
    }
    auto lua_selproj_after = std::chrono::high_resolution_clock::now();

    std::cout << "[INFO]: LuaADL Selected " << nselected << "/" << nevents
              << " events "
              << " in "
              << std::chrono::duration_cast<std::chrono::milliseconds>(
                     lua_selproj_after - lua_selproj_before)
                         .count() /
                     1E3
              << " s." << std::endl;
  }

  {
    HepMC3::ReaderAscii rdr("MiniBooNE_CH2_numu.hepmc3");

    LuaADL::Engine evt_eng;
    auto processor_id =
        evt_eng.AddProcessorFromFile("MiniBooNE_sel_imperative.lua");
    auto prcs = evt_eng.GetProcessor(processor_id);

    size_t nevents = 0, nselected = 0;

    auto lua_selproj_before = std::chrono::high_resolution_clock::now();
    while (!rdr.failed()) {
      HepMC3::GenEvent evt;
      rdr.read_event(evt);

      if (rdr.failed()) {
        break;
      }
      nevents++;

      std::vector<double> projections;
      bool passed = prcs.filter_and_project(evt, projections);
      if (passed) {
        nselected++;
      }
    }
    auto lua_selproj_after = std::chrono::high_resolution_clock::now();

    std::cout << "[INFO]: Lua Selected " << nselected << "/" << nevents
              << " events "
              << " in "
              << std::chrono::duration_cast<std::chrono::milliseconds>(
                     lua_selproj_after - lua_selproj_before)
                         .count() /
                     1E3
              << " s." << std::endl;
  }
}