list(APPEND TEST_SRCS
  MeshtalCode.cpp
  MeshtalComment.cpp
  MeshtalNps.cpp
  MeshtalTallyBounds.cpp
  MeshtalTallyError.cpp
  MeshtalTallyID.cpp
  MeshtalTallyStatisticalCombination.cpp
  MeshtalTallyValue.cpp
  MeshtalTallyVolume.cpp
  MeshtalVersion.cpp
  MeshtalWrite.cpp
)

foreach(tsrc IN LISTS TEST_SRCS)
  MakeTest(Mctal ${tsrc} meshtal_tests)
endforeach()
