target_sources(mcnptools PRIVATE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Lnk3Dnt.cpp;
                    ${CMAKE_CURRENT_SOURCE_DIR}/Mctal.cpp;
                    ${CMAKE_CURRENT_SOURCE_DIR}/MctalKcode.cpp;
                    ${CMAKE_CURRENT_SOURCE_DIR}/MctalTally.cpp;
                    ${CMAKE_CURRENT_SOURCE_DIR}/Meshtal.cpp;
                    ${CMAKE_CURRENT_SOURCE_DIR}/MeshtalTally.cpp;
                    ${CMAKE_CURRENT_SOURCE_DIR}/ptrack/legacy/PtracEvent.cpp;
                    ${CMAKE_CURRENT_SOURCE_DIR}/ptrack/legacy/PtracHistory.cpp;
                    ${CMAKE_CURRENT_SOURCE_DIR}/ptrack/legacy/PtracNps.cpp;
                    ${CMAKE_CURRENT_SOURCE_DIR}/ptrack/Ptrac.cpp;
                    ${CMAKE_CURRENT_SOURCE_DIR}/XSDir.cpp;
                    ${CMAKE_CURRENT_SOURCE_DIR}/mergeMctals.cpp;
                    ${CMAKE_CURRENT_SOURCE_DIR}/mergeMeshtals.cpp>
)
