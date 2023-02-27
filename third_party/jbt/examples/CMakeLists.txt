project(JBT_examples)

set(JBT_EXAMPLES
	"example_1.cpp"
	"example_2.cpp"
)


foreach(example IN LISTS JBT_EXAMPLES)
	add_executable("JBT_${example}" ${example})
	target_link_libraries("JBT_${example}" PRIVATE jbt)
	if (CMAKE_VERSION VERSION_GREATER 3.12)
	  set_property(TARGET "JBT_${example}" PROPERTY CXX_STANDARD 20)
	endif()
endforeach()