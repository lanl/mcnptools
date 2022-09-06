#include <vector>
#include <string>

#include <catch2/catch.hpp>

#include "shacl/f5.hpp"

TEST_CASE("read/write string dataset", "[h5dataset]") {
  std::vector<std::string> stringdata_out = {"longer string 1", "a", ""};

  {
    auto file = f5::File("h5stringdataset_test.h5", 'w');
    file.writeDataset("string_dataset", stringdata_out);
  }

  {
    auto file = f5::File("h5stringdataset_test.h5", 'r');
    std::vector<std::string> stringdata_in;
    file.readDataset("string_dataset", stringdata_in);
    REQUIRE(stringdata_in[0] == "longer string 1");
    REQUIRE(stringdata_in[1] == "a"); // short string optimization
    REQUIRE(stringdata_in[2] == "");
  }

  SECTION("With an empty vector of strings") {
    {
      std::vector<std::string> empty_strings{};
      auto file = f5::File("h5stringdataset_empty.h5", 'w');
      file.writeDataset("string_dataset", empty_strings);
    }

    {
      auto file = f5::File("h5stringdataset_empty.h5", 'r');
      std::vector<std::string> stringdata_in;
      file.readDataset("string_dataset", stringdata_in);
      REQUIRE(stringdata_in.empty());
    }
    remove("h5stringdataset_empty.h5");
  }

  // Clean up
  remove("h5stringdataset_test.h5");
}

TEST_CASE("read/write fixed length string dataset") {
  std::vector<f5::FixedLengthString<5>> stringdata_out = {"longer string", "a",
                                                          ""};

  {
    auto file = f5::File("h5fixedstringdataset_test.h5", 'w');
    file.writeDataset("string_dataset", stringdata_out);
  }

  {
    //Read back in as fixed-length strings of the same padtype and size
    auto file = f5::File("h5fixedstringdataset_test.h5", 'r');
    std::vector<f5::FixedLengthString<5>> stringdata_in;
    file.readDataset("string_dataset", stringdata_in);
    REQUIRE(stringdata_in[0].to_string() == "longe");
    REQUIRE(stringdata_in[1].to_string() == "a");
    REQUIRE(stringdata_in[2].to_string() == "");
  }

  {
    //Read back in as a std::string, where fixed-length size does not need to be known at compile time
    auto file = f5::File("h5fixedstringdataset_test.h5", 'r');
    std::vector<std::string> stringdata_in;
    file.readDataset("string_dataset", stringdata_in);
    REQUIRE(stringdata_in[0] == "longe");
    REQUIRE(stringdata_in[1] == "a"); // short string optimization
    REQUIRE(stringdata_in[2] == "");
  }

  remove("h5fixedstringdataset_test.h5");
}

TEST_CASE("read/write dataset", "[h5dataset]") {
  // Create file, group.
  auto file = f5::File("h5dataset_test.h5", 'w');
  auto group = file.openGroup("test");

  SECTION("read/write on file") {
    std::vector<double> out_data = {1, 2, 3, 4, 5};
    file.writeDataset("test1", out_data);

    std::vector<double> in_data = {0, 0, 0, 0, 0};
    file.readDataset("test1", in_data);

    REQUIRE(out_data == in_data);

    SECTION("check existence") {
      REQUIRE(file.checkLink("test1") == true);
      REQUIRE(file.checkLink("non_existent") == false);
    }

    SECTION("check if name is UTF-8") {
      H5L_info_t buffer;
      H5Lget_info(file.getID(), "test1", &buffer, H5P_DEFAULT);

      REQUIRE(buffer.cset == H5T_CSET_UTF8);
    }

    SECTION("delete") {
      file.deleteLink("test1");
      REQUIRE(file.checkLink("test1") == false);
    }
  }

  SECTION("read/write on group") {
    std::vector<double> out_data = {1, 2, 3, 4, 5};
    group.writeDataset("test1", out_data);

    std::vector<double> in_data = {0, 0, 0, 0, 0};
    group.readDataset("test1", in_data);

    REQUIRE(out_data == in_data);

    SECTION("check existence") {
      REQUIRE(group.checkLink("test1") == true);
      REQUIRE(group.checkLink("non_existent") == false);
    }

    SECTION("delete") {
      group.deleteLink("test1");
      REQUIRE(group.checkLink("test1") == false);
    }
  }

  SECTION("read/write on dataset") {
    std::vector<double> out_data = {1, 2, 3, 4, 5};
    auto dataset_out =
        f5::DatasetBuilder().inferFrom(out_data).build(file, "test3");
    dataset_out.write(out_data);

    std::vector<double> in_data = {0, 0, 0, 0, 0};
    auto dataset_in = file.openDataset("test3");
    dataset_in.read(in_data);

    REQUIRE(out_data == in_data);
  }

  SECTION("read/write illogical datasets") {
    SECTION("Write too large") {
      // Create Dataset
      std::vector<hsize_t> file_shape = {3};
      auto type = f5::getSimpleType<double>();
      auto dataset_out = f5::DatasetBuilder()
                             .setShape(file_shape)
                             .setType(type)
                             .build(group, "test");

      std::vector<double> out_data = {0, 0, 0, 0, 0};
      REQUIRE_THROWS_WITH(dataset_out.write(out_data),
                          "Writing beyond the edge of the dataset.");
    }

    SECTION("Write correct size, but with bad offset") {
      // Create Dataset
      std::vector<hsize_t> file_shape = {5};
      auto type = f5::getSimpleType<double>();
      auto dataset_out = f5::DatasetBuilder()
                             .setShape(file_shape)
                             .setType(type)
                             .build(group, "test");

      std::vector<double> out_data(5);
      REQUIRE_THROWS_WITH(dataset_out.write(out_data, 1, out_data.size()),
                          "Writing beyond the edge of the dataset.");
    }

    SECTION("Read too much, specifying size") {
      // Create Dataset
      std::vector<double> out_data = {1, 2, 3};
      group.writeDataset("test", out_data);

      std::vector<double> in_data(3);
      auto dataset_in = group.openDataset("test");
      REQUIRE_THROWS_WITH(dataset_in.read(in_data, 1, in_data.size()),
                          "Reading beyond the edge of the dataset.");
    }
  }

  SECTION("read/write compressed on dataset") {
    SECTION("manually specified chunking") {
      // Write array with chunks of shape {3} and compression enabled
      std::vector<double> out_data = {1, 2, 3, 4, 5};
      std::vector<hsize_t> chunks = {3};
      auto dataset_out = f5::DatasetBuilder()
                             .inferFrom(out_data)
                             .setChunkShape(chunks)
                             .enableCompression()
                             .build(group, "test4");
      dataset_out.write(out_data);

      // Read array in
      std::vector<double> in_data = {0, 0, 0, 0, 0};
      group.readDataset("test4", in_data);

      REQUIRE(out_data == in_data);
    }
    SECTION("automatic chunking 1D dataset") {
      // Write array with automatically calculated chunks and compression
      // enabled
      std::vector<double> out_data = {1, 2, 3, 4, 5};
      auto dataset_out = f5::DatasetBuilder()
                             .inferFrom(out_data)
                             .setAutoChunk()
                             .enableCompression()
                             .build(group, "test9");
      dataset_out.write(out_data);

      // Read array in
      std::vector<double> in_data = {0, 0, 0, 0, 0};
      group.readDataset("test9", in_data);

      REQUIRE(out_data == in_data);
    }
    SECTION("automatic chunking 3D dataset") {
      // Write array with automatically calculated chunks and compression
      // enabled for 4-D dataset
      auto builder = f5::DatasetBuilder();
      std::vector<hsize_t> extent = {20, 30, 40, 50};
      std::vector<double> out_data(
          extent[0] * extent[1] * extent[2] * extent[3], 1.0);
      auto dataset_out = f5::DatasetBuilder()
                             .inferFrom(out_data)
                             .setShape(extent)
                             .setAutoChunk()
                             .enableCompression()
                             .build(group, "test10");
      dataset_out.write(out_data);

      // Read array in
      std::vector<double> in_data(extent[0] * extent[1] * extent[2] * extent[3],
                                  0.0);
      group.readDataset("test10", in_data);

      REQUIRE(out_data == in_data);
    }
    SECTION("calculate final chunk size") {
      // Confirm that the chunk-size factorization is behaving as expected.
      const std::vector<hsize_t> dim_sizes = {50, 50, 50, 50, 50, 500, 61};
      const std::vector<hsize_t> chunk_sizes = {50, 40, 25, 24, 3, 9, 3};
      const std::vector<hsize_t> correct_sizes = {50, 25, 25, 10, 2, 9, 3};
      std::vector<hsize_t> answer_sizes = {0, 0, 0, 0, 0, 0, 0};

      for (size_t i = 0; i < dim_sizes.size(); ++i) {
        answer_sizes[i] = f5::DatasetBuilder().calculateLastChunk(
            dim_sizes[i], chunk_sizes[i]);
      }

      REQUIRE(answer_sizes == correct_sizes);
    }
    SECTION("automatic chunking zero-length 1D dataset") {
      // Write zero-length array with automatically calculated chunks and
      // compression enabled.
      std::vector<double> out_data = {};
      auto dataset_out = f5::DatasetBuilder()
                             .inferFrom(out_data)
                             .setAutoChunk()
                             .enableCompression()
                             .build(group, "test11");
      dataset_out.write(out_data);

      // Read array in
      std::vector<double> in_data = {};
      group.readDataset("test11", in_data);

      REQUIRE(in_data.size() == 0);
    }
  }

  SECTION("write on hyperslab") {
    // Write length-3 array to shape {5} target offset by one
    std::vector<double> out_data = {1, 2, 3};
    std::vector<hsize_t> file_shape = {5};
    std::vector<hsize_t> chunks = {3};
    auto type = f5::getSimpleType<double>();
    auto dataset_out = f5::DatasetBuilder()
                           .setShape(file_shape)
                           .setChunkShape(chunks)
                           .setType(type)
                           .build(group, "test5");
    dataset_out.write(out_data, 1, out_data.size());

    // Read entire length-5 array
    std::vector<double> in_data = {0, 0, 0, 0, 0};
    group.readDataset("test5", in_data);

    std::vector<double> reference1 = {0, 1, 2, 3, 0};
    REQUIRE(reference1 == in_data);
  }
  SECTION("multiple write on hyperslab") {
    // Write length-2 array twice
    std::vector<double> out_data = {1, 2};
    std::vector<hsize_t> file_shape = {5};
    auto type = f5::getSimpleType<double>();
    auto dataset_out = f5::DatasetBuilder()
                           .setShape(file_shape)
                           .setType(type)
                           .build(group, "test6");
    dataset_out.write(out_data, 0, out_data.size());
    dataset_out.write(out_data, 3, out_data.size());

    // Read entire length-5 array
    std::vector<double> in_data = {0, 0, 0, 0, 0};
    group.readDataset("test6", in_data);

    std::vector<double> reference1 = {1, 2, 0, 1, 2};
    REQUIRE(reference1 == in_data);
  }

  SECTION("read on hyperslab") {
    // Write length-5 array
    std::vector<double> out_data = {1, 2, 3, 4, 5};
    group.writeDataset("test7", out_data);

    // Read length-3 array offset by 1
    std::vector<double> in_data = {0, 0, 0};
    auto dataset_in = group.openDataset("test7");
    dataset_in.read(in_data, 1, in_data.size());

    std::vector<double> reference1 = {2, 3, 4};
    REQUIRE(reference1 == in_data);
  }

  SECTION("write extendable dataset") {
    // Write length-5 array, followed by another length 5
    std::vector<double> out_data1 = {1, 2, 3, 4, 5};
    std::vector<double> out_data2 = {6, 7, 8, 9, 10};

    auto type = f5::getSimpleType<double>();
    auto dataset_out = f5::DatasetBuilder()
                           .setShape({5})
                           .setMaxShape({H5S_UNLIMITED}) // Or just 10
                           .setChunkShape({5})
                           .setType(type)
                           .build(group, "test8");

    // Write first chunk
    dataset_out.write(out_data1);

    // Check first chunk
    std::vector<double> in_data1(5);
    group.readDataset("test8", in_data1);

    REQUIRE(out_data1 == in_data1);

    // Write second chunk at offset of 5
    auto shape = dataset_out.shape();
    shape[0] += 5;
    dataset_out.reshape(shape);
    dataset_out.write(out_data2, 5, out_data2.size());

    // Check second chunk
    out_data1.insert(out_data1.end(), out_data2.begin(), out_data2.end());

    std::vector<double> in_data2(10);
    group.readDataset("test8", in_data2);
    REQUIRE(out_data1 == in_data2);
  }

  // Clean up
  remove("h5dataset_test.h5");
}
