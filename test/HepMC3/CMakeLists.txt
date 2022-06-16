foreach(t Attribute FourVector)
	add_executable(${t} ${t}.cxx)
	target_link_libraries(${t} HepMC3 sol2)
	add_test(NAME ${t}
	         COMMAND ${t})
endforeach()