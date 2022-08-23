add_library(HepMC3Lua INTERFACE)

target_include_directories(HepMC3Lua INTERFACE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
  $<INSTALL_INTERFACE:include>)

target_link_libraries(HepMC3Lua INTERFACE HepMC3 sol2)

add_subdirectory(LuaADL)

install(DIRECTORY HepMC3 DESTINATION include)