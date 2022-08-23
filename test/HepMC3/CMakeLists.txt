foreach(t Attribute FourVector)
	add_executable(${t} ${t}.cxx)
	target_link_libraries(${t} HepMC3Lua)
	add_test(NAME ${t}
	         COMMAND ${t})
endforeach()