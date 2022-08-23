set(LuaADL_Impl_Files
  Engine.cxx
  Processor.cxx
  Projectors.cxx
  Selectors.cxx
)

set(LuaADL_Hdr_Files
  Engine.hxx
  Processor.hxx
  Projectors.hxx
  Selectors.hxx
)

add_library(LuaADL SHARED ${LuaADL_Impl_Files})
target_link_libraries(LuaADL HepMC3Lua)
set_target_properties(LuaADL PROPERTIES 
  PUBLIC_HEADER "${LuaADL_Hdr_Files}"
  EXPORT_NAME LuaADL)

install(TARGETS LuaADL
  LIBRARY DESTINATION lib
  PUBLIC_HEADER DESTINATION include/LuaADL)