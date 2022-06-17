#include "LuaADL/Processor.hxx"

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

      auto ISnu =
          LuaADL::Selectors::particle_1pdg<LuaADL::Selectors::kBeam,
                                           LuaADL::Selectors::kFirst>(evt, 14);
      if (!ISnu) {
        continue;
      }
      auto FSmus = LuaADL::Selectors::particles_1pdg<
          LuaADL::Selectors::kUndecayedPhysical,
          LuaADL::Selectors::kFromPDGList>(evt, 13);
      if (FSmus.size() != 1) {
        continue;
      }

      auto FSOther =
          LuaADL::Selectors::particles<LuaADL::Selectors::kUndecayedPhysical,
                                       LuaADL::Selectors::kNotFromPDGList>(
              evt, {13, 2212, 2112});
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
    LuaADL::Processor prcs("noop.lua");

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
    LuaADL::Processor prcs("MiniBooNE_sel_LuaADL.lua");

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
    LuaADL::Processor prcs("MiniBooNE_sel_imperative.lua");

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