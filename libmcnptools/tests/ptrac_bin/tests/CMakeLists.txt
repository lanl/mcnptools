list(APPEND TEST_SRCS
  BinPtracEventEnergy.cpp
  BinPtracEventPositions.cpp
  BinPtracEventTime.cpp
  BinPtracEventType.cpp
  BinPtracHistoryNps.cpp
  BinPtracHistoryNumEvents.cpp
  BinPtracReadHistories.cpp
)

foreach(tsrc IN LISTS TEST_SRCS)
  MakeTest(Ptrac ${tsrc} ptrac_bin_tests)
endforeach()
