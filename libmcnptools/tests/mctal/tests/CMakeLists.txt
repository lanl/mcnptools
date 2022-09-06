list(APPEND TEST_SRCS
  MctalCode.cpp
  MctalComment.cpp
  MctalDump.cpp
  MctalExcept.cpp
  MctalKcodeCycles.cpp
  MctalKcodeNdat.cpp
  MctalKcodeValue.cpp
  MctalNps.cpp
  MctalRandoms.cpp
  MctalTallyCBins.cpp
  MctalTallyEBins.cpp
  MctalTallyError.cpp
  MctalTallyFBins.cpp
  MctalTallyId.cpp
  MctalTallyList.cpp
  MctalTallyTBins.cpp
  MctalTallyTFC.cpp
  MctalTallyTFCBins.cpp
  MctalTallyValue.cpp
  MctalTallyValues.cpp
)

foreach(tsrc IN LISTS TEST_SRCS)
  MakeTest(Mctal ${tsrc} mctal_tests)
endforeach()
