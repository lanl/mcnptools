add_library(mcnptools_catch INTERFACE)
target_link_libraries(mcnptools_catch INTERFACE mcnptools Catch2::Catch2)

add_executable( HDF5PtrackEventConverters HDF5PtrackEventConverters.cpp )
target_link_libraries( HDF5PtrackEventConverters PRIVATE mcnptools_catch )
add_test( NAME HDF5PtrackEventConverters COMMAND HDF5PtrackEventConverters )

add_executable( HDF5PtrackBuffer HDF5PtrackBuffer.cpp )
target_link_libraries( HDF5PtrackBuffer PRIVATE mcnptools_catch )
add_test( NAME HDF5PtrackBuffer COMMAND HDF5PtrackBuffer )

add_executable( HDF5PtrackReadEvents HDF5PtrackReadEvents.cpp )
target_link_libraries( HDF5PtrackReadEvents PRIVATE mcnptools_catch )
add_test( NAME HDF5PtrackReadEvents COMMAND HDF5PtrackReadEvents )

add_executable( HDF5PtrackReadHistories HDF5PtrackReadHistories.cpp )
target_link_libraries( HDF5PtrackReadHistories PRIVATE mcnptools_catch )
add_test( NAME HDF5PtrackReadHistories COMMAND HDF5PtrackReadHistories )

configure_file(../test_files/testing.h5 ./tests/testing.h5 COPYONLY)
configure_file(../test_files/parallel_testing.h5 ./tests/parallel_testing.h5 COPYONLY)
configure_file(../test_files/ptrack_all_events.h5 ./tests/ptrack_all_events.h5 COPYONLY)
