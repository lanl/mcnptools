#include <cstdio>
#include <vector>

#include "shacl/f5/hdf5_nowarnings.hpp" //also includes mpi.h
#include <catch2/catch.hpp>

#include "shacl/f5.hpp"

TEST_CASE("MPI tests") {

  SECTION("MPI initialize") {
    // Create empty initialization arguments
    int argc = 0;
    char **argv = nullptr;
    MPI_Init(&argc, &argv);
  }

  SECTION(" run the tests ") {
    MPI_Comm comm = MPI_COMM_WORLD;
    MPI_Info info = MPI_INFO_NULL;

    int rank;
    int size;
    MPI_Comm_rank(comm, &rank);
    MPI_Comm_size(comm, &size);

    SECTION("create HDF5 file with MPI", "[h5file]") {
      // Create file, close handle.
      { auto file = f5::File("test.h5", 'w', comm, info); }

      MPI_Barrier(comm);

      // Test if file exists.
      SECTION("Was the write actually collective") {
        if (rank == 0) {
          REQUIRE(f5::checkFile("test.h5"));

          // Clean up
          remove("test.h5");
        }
      }

      MPI_Barrier(comm);
    }

    SECTION("test independent IO (read/write) through a dataset",
            "[h5dataset]") {

      SECTION("Write out the data", "[h5dataset]") {
        auto file = f5::File("test.h5", 'w', comm, info);
        auto group = file.openGroup("gtest");

        std::vector<double> data{1, 2, 3, 4, 5};
        std::string name = "test";
        for (int i = 0; i < size; ++i) {
          // Create a particular ranks dataset, on EACH rank
          auto ds = f5::DatasetBuilder().inferFrom(data).build(
              group, "test" + std::to_string(i));

          // Only write my ranks dataset
          if (i == rank) {
            ds.write(data);
          }
        }

        MPI_Barrier(comm);
      }

      SECTION(
          "data was correctly written and can be read back in by single rank",
          "[h5dataset]") {
        if (rank == 0) {
          auto file = f5::File("test.h5", 'r');
          auto group = file.openGroup("gtest");

          std::vector<double> data{1, 2, 3, 4, 5};
          std::string name = "test";

          for (int i = 0; i < size; ++i) {
            std::vector<double> data_in(5);
            group.readDataset(name + std::to_string(i), data_in);
            REQUIRE(data == data_in);
          }
        }

        MPI_Barrier(comm);

        // Clean up (separate to close handle)
        if (rank == 0) {
          remove("test.h5");
        }
      }
    }

    SECTION("test collective IO (read/write) through MPI dataset",
            "[h5dataset]") {

      // Make data unique to each rank
      std::vector<double> data{1., 2., 3., 4., 5., 6.};
      for (auto &value : data) {
        value *= rank;
      }

      // Require rank 1 to write a zero-length slab
      SECTION("Require at least two ranks to run this test") {
        REQUIRE(size > 2);
      }

      if (rank == 1) {
        data.clear();
      }

      unsigned int offset = (rank > 1) ? (rank - 1) * 6 : rank * 6;
      unsigned int total_size = (size - 1) * 6;

      auto type = f5::getSimpleType<double>();
      auto dataset_builder = f5::DatasetBuilder()
                                 .setShape({total_size})
                                 .setChunkShape({6})
                                 .setType(type);
      // Open dataset
      auto file = f5::File("test.h5", 'w', comm, info);
      auto ds = dataset_builder.build(file, "test");

      SECTION("test collective IO (write)", "[h5dataset]") {

        // Open and write on all ranks at once (with compression!)
        // Here, we pass in an instance of the collective Property.
        // This  is only so we can check the mpi_io_mode
        auto collective_properties = f5::collectiveProperty();
        ds.write(data, offset, data.size(), collective_properties);

        MPI_Barrier(comm);

        // Check collective write succeeded
        if (rank == 0) {
          H5D_mpio_actual_io_mode_t io_policy;
          H5Pget_mpio_actual_io_mode(collective_properties.getID(), &io_policy);
          REQUIRE_FALSE(io_policy == H5D_MPIO_NO_COLLECTIVE);
        }

        MPI_Barrier(comm);

        // Read in the data and make sure it makses sense.
        std::vector<double> data_in((rank == 1) ? 0 : 6);
        INFO("The offset is: " << offset);
        ds.read(data_in, offset, data_in.size(), f5::collectiveProperty());

        INFO("the rank is: " << rank);
        CHECK(data == data_in);

        MPI_Barrier(comm);

        if (rank == 0) {
          remove("test.h5");
        }
      }

      // Barrier to prevent tests from closing out before checking done
      MPI_Barrier(comm);
    }
  } // END SECTION("run the tests")

  SECTION("MPI finalize") {
    MPI_Finalize();
  }
}
