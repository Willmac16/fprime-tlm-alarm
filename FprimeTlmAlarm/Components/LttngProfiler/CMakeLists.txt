####
# F Prime CMakeLists.txt:
#
# SOURCES: list of source files (to be compiled)
# AUTOCODER_INPUTS: list of files to be passed to the autocoders
# DEPENDS: list of libraries that this module depends on
#
# More information in the F´ CMake API documentation:
# https://fprime.jpl.nasa.gov/latest/docs/reference/api/cmake/API/
#
####

# Module names are derived from the path from the nearest project/library/framework
# root when not specifically overridden by the developer, i.e. the module defined by
# `MyProj/Some/Path/CMakeLists.txt` will be named `MyProj_Some_Path`.

register_fprime_library(
    AUTOCODER_INPUTS
        "${CMAKE_CURRENT_LIST_DIR}/LttngProfiler.fpp"
    SOURCES
        "${CMAKE_CURRENT_LIST_DIR}/LttngProfiler.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/fprime_profiler_tp.c"
    LINK_LIBS
        "-llttng-ust"
)

### Unit Tests ###
register_fprime_ut(
    AUTOCODER_INPUTS
        "${CMAKE_CURRENT_LIST_DIR}/LttngProfiler.fpp"
    SOURCES
        "${CMAKE_CURRENT_LIST_DIR}/test/ut/LttngProfilerTestMain.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/test/ut/LttngProfilerTester.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/fprime_profiler_tp.c"
    LINK_LIBS
        "-llttng-ust"
    DEPENDS
        STest # For rules-based testing
)

### Integration Tests ###
register_fprime_ut(
    NAME "LttngProfilerIntegration"
    AUTOCODER_INPUTS
        "${CMAKE_CURRENT_LIST_DIR}/LttngProfiler.fpp"
    SOURCES
        "${CMAKE_CURRENT_LIST_DIR}/test/integration/LttngIntegrationTest.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/fprime_profiler_tp.c"
    LINK_LIBS
        "-llttng-ust"
)
