#include <vector>

#include "shacl/f5/hdf5_nowarnings.hpp"
#include <catch2/catch.hpp>

#include "shacl/f5.hpp"

TEST_CASE("create HDF5 file", "[h5file]") {
  // Create file, close handle.
  { auto file = f5::File("h5file_create_test.h5", 'w'); }
  // Test if file exists.
  REQUIRE(f5::checkFile("h5file_create_test.h5"));

  // Clean up
  remove("h5file_create_test.h5");
}

TEST_CASE("create HDF5 group", "[h5group]") {
  // Create file
  auto file = f5::File("h5group_test.h5", 'w');
  // Create and close group
  { auto group = file.openGroup("test"); }
  // Check if group exists
  REQUIRE(file.checkLink("test") == true);

  // Check if name is written UTF-8
  H5L_info_t buffer;
  H5Lget_info(file.getID(), "test", &buffer, H5P_DEFAULT);

  REQUIRE(buffer.cset == H5T_CSET_UTF8);

  file.deleteLink("test");
  REQUIRE(file.checkLink("test") == false);

  // Clean up
  remove("h5group_test.h5");
}

TEST_CASE("ensure dataset builder is consistent", "[datasetbuilder]") {
  // Create file
  auto file = f5::File("datasetbuilder_test.h5", 'w');
  auto type = f5::getSimpleType<double>();
  std::vector<hsize_t> shape = {5};
  std::vector<hsize_t> max_shape = {5};
  std::vector<hsize_t> bad_max_shape = {4};
  std::vector<hsize_t> unl_max_shape = {H5S_UNLIMITED};
  std::vector<hsize_t> bad_shape = {5, 3};

  SECTION("attempt build without a shape") {
    REQUIRE_THROWS_WITH(f5::DatasetBuilder().setType(type).build(file, "test"),
                        "Cannot create dataset without a file shape.");
  }

  SECTION("attempt build without a type") {
    REQUIRE_THROWS_WITH(
        f5::DatasetBuilder().setShape(shape).build(file, "test"),
        "Cannot create dataset without a type.");
  }

  SECTION("attempt build with inconsistent chunk shape") {
    REQUIRE_THROWS_WITH(
        f5::DatasetBuilder()
            .setType(type)
            .setShape(shape)
            .setChunkShape(bad_shape)
            .build(file, "test"),
        "The chunk shape dimensions are inconsistent with the file shape.");
  }

  SECTION("attempt build with inconsistent max shape") {
    REQUIRE_THROWS_WITH(f5::DatasetBuilder()
                            .setType(type)
                            .setShape(shape)
                            .setChunkShape(shape)
                            .setMaxShape(bad_shape)
                            .build(file, "test"),
                        "Max shape dimension is inconsistent with file shape.");
  }

  SECTION("attempt build with too small max shape") {
    REQUIRE_THROWS_WITH(f5::DatasetBuilder()
                            .setType(type)
                            .setShape(shape)
                            .setChunkShape(shape)
                            .setMaxShape(bad_max_shape)
                            .build(file, "test"),
                        "File shape larger than maximum shape on dimension 0.");
  }

  SECTION("attempt build with unlimited shape and no chunking") {
    REQUIRE_THROWS_WITH(f5::DatasetBuilder()
                            .setType(type)
                            .setShape(shape)
                            .setMaxShape(unl_max_shape)
                            .build(file, "test"),
                        "Cannot make a resizable array without chunking.");
  }

  // Clean up
  remove("datasetbuilder_test.h5");
}

TEST_CASE("Read HDF5 file", "[h5file]") {
  std::vector<double> out_data = {1, 2, 3, 4, 5};
  std::vector<double> in_data1 = {0, 0, 0, 0, 0};
  std::vector<double> in_data2 = {0, 0, 0, 0, 0};
  // Create file, write to it, close handle.
  {
    auto file = f5::File("h5file_test.h5", 'w');

    file.writeDataset("test1", out_data);
  }
  // Append file, write to it, close handle.
  {
    auto file = f5::File("h5file_test.h5", 'a');

    file.writeDataset("test2", out_data);
  }
  // Open file, read, close handle.
  {
    auto file = f5::File("h5file_test.h5", 'r');

    file.readDataset("test1", in_data1);
    file.readDataset("test2", in_data2);
  }

  REQUIRE(out_data == in_data1);
  REQUIRE(out_data == in_data2);

  // Clean up
  remove("h5file_test.h5");
}

TEST_CASE("Query HDF5 file", "[h5file]") {
  std::vector<double> out_data = {1, 2};
  std::vector<double> out_data2 = {4};
  std::vector<double> in_data1;
  {
    auto file = f5::File("h5file_query_test.h5", 'w');
    file.writeDataset("test1", out_data);
    file.writeDataset("test2data", out_data2);
  }
  {
    auto file = f5::File("h5file_query_test.h5", 'r');
    auto mylist = file.listNames();
    REQUIRE(mylist.size() == 2);
    REQUIRE(mylist[0] == "test1");
    REQUIRE(mylist[1] == "test2data");
    file.readDataset(mylist[0], in_data1);
    REQUIRE(out_data == in_data1);
  }
  remove("h5file_query_test.h5");
}

TEST_CASE("attempt to delete nonexistent objects") {
  // Create file.
  auto file = f5::File("delete_test.h5", 'w');

  REQUIRE_THROWS_WITH(
      file.deleteLink("test"),
      "There was a problem attempting to delete link to \"test\".");
  REQUIRE_THROWS_WITH(
      file.deleteAttribute("test"),
      "There was a problem attempting to delete attribute \"test\".");

  // Clean up
  remove("delete_test.h5");
}

TEST_CASE("create a link") {
  // Create file.
  auto file = f5::File("link_test.h5", 'w');

  SECTION("Link Datasets") {
    std::vector<double> out_data = {1, 2};
    file.writeDataset("test1", out_data);
    file.createLink("test2", file, "test1");

    auto group = file.openGroup("test3");
    group.createLink("test4", file, "test1");

    std::vector<double> in_data1;
    std::vector<double> in_data2;

    file.readDataset("test2", in_data1);
    group.readDataset("test4", in_data2);

    REQUIRE(out_data == in_data1);
    REQUIRE(out_data == in_data2);
  }

  SECTION("Link Groups") {
    auto group = file.openGroup("test1");
    std::vector<double> out_data = {1, 2};
    group.writeDataset("test2", out_data);

    file.createLink("test3", file, "test1");

    auto linked_group = file.openGroup("test3");

    std::vector<double> in_data;
    linked_group.readDataset("test2", in_data);

    REQUIRE(out_data == in_data);
  }

  // Clean up
  remove("link_test.h5");
}

TEST_CASE("getting file name, group name, and dataset name") {
  SECTION("get filename") {
    auto file = f5::File("h5path_test.h5", 'w');
    REQUIRE(file.filename() == std::string("h5path_test.h5"));
    SECTION("get group name") {
      auto group = file.openGroup("test");
      REQUIRE(group.path() == std::string("/test"));
      SECTION("get dataset name") {
        std::vector<int> data{1, 2, 3};
        auto dataset = group.writeDataset("firstDataset", data);
        REQUIRE(dataset.path() == std::string("/test/firstDataset"));
        REQUIRE(dataset.filename() == file.filename());

        auto anotherGroup = group.openGroup("anotherGroup");
        auto anotherDataset = anotherGroup.writeDataset("secondDataset", data);
        REQUIRE(anotherDataset.path() ==
                std::string("/test/anotherGroup/secondDataset"));
      }
    }
  }

  remove("h5path_test.h5");
}
