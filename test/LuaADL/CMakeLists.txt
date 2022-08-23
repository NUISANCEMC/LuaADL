file(COPY 
		MiniBooNE_sel_LuaADL.lua 
		MiniBooNE_sel_imperative.lua 
		noop.lua 
		MiniBooNE_CH2_numu.hepmc3 
	DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

foreach(t ProcessorScoping ProcessorFunctionExistanceChecks ProcessorLiteralOrFromFile Profile)
	add_executable(${t} ${t}.cxx)
	target_link_libraries(${t} LuaADL)
	add_test(NAME ${t}
	         COMMAND ${t})

	find_program(valgrindapp valgrind)
	if(NOT valgrindapp STREQUAL "valgrindapp-NOTFOUND")
		add_test(NAME memcheck_${t}
		         COMMAND valgrind --trace-children=yes 
		         --leak-check=full --error-exitcode=1 
		         --default-suppressions=yes --gen-suppressions=all 
		         ${CMAKE_CURRENT_BINARY_DIR}/${t})
	endif()

endforeach()