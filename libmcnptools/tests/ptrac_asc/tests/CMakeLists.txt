list(APPEND TEST_SRCS
  AscPtracEventEnergy.cpp
  AscPtracEventPositions.cpp
  AscPtracEventTime.cpp
  AscPtracEventType.cpp
  AscPtracHistoryNps.cpp
  AscPtracHistoryNumEvents.cpp
  AscPtracReadHistories.cpp
  AscPtracReadHistoriesNoHeader.cpp
)

foreach(tsrc IN LISTS TEST_SRCS)
  MakeTest(Ptrac ${tsrc} ptrac_asc_tests)
endforeach()
