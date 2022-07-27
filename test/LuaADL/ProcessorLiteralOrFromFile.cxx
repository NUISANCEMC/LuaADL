#include "LuaADL/Engine.hxx"

#include "HepMC3/ReaderAscii.h"

#include <chrono>

int main(int argc, char *const argv[]) {
  LuaADL::Engine evt_eng;

    size_t nevents_file = 0, nselected_file = 0;


  {
    HepMC3::ReaderAscii rdr("MiniBooNE_CH2_numu.hepmc3");

    auto processor_id =
        evt_eng.AddProcessorFromFile("MiniBooNE_sel_LuaADL.lua");
    auto prcs = evt_eng.Processor(processor_id);

    auto lua_selproj_before = std::chrono::high_resolution_clock::now();
    while (!rdr.failed()) {
      HepMC3::GenEvent evt;
      rdr.read_event(evt);

      if (rdr.failed()) {
        break;
      }
      nevents_file++;

      std::vector<double> projections;
      bool passed = prcs.filter_and_project(evt, projections);
      if (passed) {
        nselected_file++;
      }
      if (nevents_file > 250) {
        break;
      }
    }
    auto lua_selproj_after = std::chrono::high_resolution_clock::now();

    std::cout << "[INFO]: LuaADL Selected " << nselected_file << "/" << nevents_file
              << " events "
              << " in "
              << std::chrono::duration_cast<std::chrono::milliseconds>(
                     lua_selproj_after - lua_selproj_before)
                         .count() /
                     1E3
              << " s." << std::endl;
  }

    size_t nevents_literal = 0, nselected_literal = 0;

  {
    HepMC3::ReaderAscii rdr("MiniBooNE_CH2_numu.hepmc3");

    auto processor_id = evt_eng.AddProcessorLiteral(R"(
          function filter_and_project(in_GenEvent, out_projections)

            local ISnu = LuaADL.sel.part.beam(in_GenEvent, 14)
            if not ISnu then return false end

            local FSmus = LuaADL.sel.parts.out(in_GenEvent, 13)
            if not (FSmus:size() == 1) then return false end

            local nnother = LuaADL.sel.parts.other_out(in_GenEvent, {13, 2212, 2112}):size()
            if nnother > 0 then return false end

            out_projections:add(FSmus[1].momentum:length())
            out_projections:add(FSmus[1].momentum:theta())

            return true
            
          end)");
    auto prcs = evt_eng.Processor(processor_id);


    auto lua_selproj_before = std::chrono::high_resolution_clock::now();
    while (!rdr.failed()) {
      HepMC3::GenEvent evt;
      rdr.read_event(evt);

      if (rdr.failed()) {
        break;
      }
      nevents_literal++;

      std::vector<double> projections;
      bool passed = prcs.filter_and_project(evt, projections);
      if (passed) {
        nselected_literal++;
      }
      if (nevents_literal > 250) {
        break;
      }
    }
    auto lua_selproj_after = std::chrono::high_resolution_clock::now();

    std::cout << "[INFO]: LuaADL Selected " << nselected_literal << "/" << nevents_literal
              << " events "
              << " in "
              << std::chrono::duration_cast<std::chrono::milliseconds>(
                     lua_selproj_after - lua_selproj_before)
                         .count() /
                     1E3
              << " s." << std::endl;
  }

  assert(nselected_literal == nselected_file);
}