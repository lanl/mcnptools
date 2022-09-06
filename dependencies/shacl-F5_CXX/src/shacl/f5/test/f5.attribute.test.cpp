#include <vector>
#include <string>

#include "shacl/f5/hdf5_nowarnings.hpp"
#include <catch2/catch.hpp>

#include "shacl/f5.hpp"

TEST_CASE("read/write attribute", "[h5readattribute]") {
  // Create file.
  auto file = f5::File("attribute_test.h5", 'w');

  SECTION("read/write on file") {
    std::vector<double> out_data = {1, 2, 3, 4, 5};
    file.writeAttribute("test", out_data);

    std::vector<double> in_data = {};
    file.readAttribute("test", in_data);

    REQUIRE(out_data == in_data);

    SECTION("check existence") {
      REQUIRE(file.checkAttribute("test") == true);
      REQUIRE(file.checkAttribute("non_existent") == false);
    }

    SECTION("delete") {
      file.deleteAttribute("test");
      REQUIRE(file.checkAttribute("test") == false);
    }

    SECTION("write out a single bool") {
      bool logical = true;
      bool read = false;
      file.writeAttribute("boolean", logical);
      file.readAttribute("boolean", read);
      REQUIRE(read);
    }
  }

  SECTION("read/write on group") {
    auto group = file.openGroup("test");
    std::vector<double> out_data = {1, 2, 3, 4, 5};
    group.writeAttribute("test", out_data);

    std::vector<double> in_data = {};
    group.readAttribute("test", in_data);

    REQUIRE(out_data == in_data);

    SECTION("check existence") {
      REQUIRE(group.checkAttribute("test") == true);
      REQUIRE(group.checkAttribute("non_existent") == false);
    }

    SECTION("delete") {
      group.deleteAttribute("test");
      REQUIRE(group.checkAttribute("test") == false);
    }
  }

  SECTION("read/write on dataset") {
    std::vector<double> out_data = {1, 2, 3, 4, 5};
    auto dataset = file.writeDataset("test", out_data);

    dataset.writeAttribute("test", out_data);

    std::vector<double> in_data = {};
    dataset.readAttribute("test", in_data);

    REQUIRE(out_data == in_data);

    SECTION("check existence") {
      REQUIRE(dataset.checkAttribute("test") == true);
      REQUIRE(dataset.checkAttribute("non_existent") == false);
    }

    SECTION("delete") {
      dataset.deleteAttribute("test");
      REQUIRE(dataset.checkAttribute("test") == false);
    }
  }

  SECTION("read/write with free function interface used by Fortran") {

    std::vector<double> ds_data = {1, 2, 3, 4, 5};
    auto dataset = file.writeDataset("test", ds_data);

    double out_data = 1.23;
    f5::writeH5Attribute(dataset.getID(), "attr", &out_data,
                         f5::getSimpleType<double>(),
                         std::vector<hsize_t>({1}));

    double in_data;
    f5::readH5Attribute(dataset.getID(), "attr", &in_data,
                        f5::getSimpleType<double>());

    REQUIRE(out_data == in_data);
  }

  SECTION("read/write scalar on dataset") {
    std::vector<double> out_data = {1, 2, 3, 4, 5};
    auto dataset = file.writeDataset("test", out_data);

    double out_value = 12345.0;
    dataset.writeAttribute("test", out_value);

    double in_value = 0.0;
    dataset.readAttribute("test", in_value);

    REQUIRE(out_value == in_value);
    REQUIRE(out_value == static_cast<double>(12345.0));
  }

  SECTION("read/write a string attribute") {
    std::vector<double> out_data = {1, 2, 3, 4, 5};
    auto dataset = file.writeDataset("test", out_data);

    std::string att = "units";
    file.writeAttribute("test", att);

    // Can also write as const char * for convenience
    dataset.writeAttribute("test2", "other real long string");
    dataset.writeAttribute("empty_string", "");

    std::string read{};
    file.readAttribute("test", read);
    REQUIRE(read == att);
    dataset.readAttribute("test2", read);
    REQUIRE(read == std::string("other real long string"));
    dataset.readAttribute("empty_string", read);
    REQUIRE(read.empty());
  }

  SECTION("read/write a vector of strings as attribute") {
    std::vector<double> out_data = {1, 2, 3, 4, 5};
    auto dataset = file.writeDataset("test", out_data);

    // Must explicitly use vector as container
    std::vector<std::string> empty{""};
    std::vector<std::string> single{"one_string"};
    std::vector<std::string> many{"data", "", "other"};
    dataset.writeAttribute("single", single);
    dataset.writeAttribute("many", many);
    dataset.writeAttribute("empty", empty);

    std::vector<std::string> single_read{};
    std::vector<std::string> many_read{};
    std::vector<std::string> empty_read{};
    dataset.readAttribute("single", single_read);
    dataset.readAttribute("many", many_read);
    dataset.readAttribute("empty", empty_read);

    REQUIRE(single_read == single);
    REQUIRE(many_read == many);
    REQUIRE(empty_read[0].empty());
  }

  SECTION("read/write a vector of fixed-length strings as attribute") {
    std::vector<double> out_data = {1, 2, 3, 4, 5};
    auto dataset = file.writeDataset("test", out_data);

    // Must explicitly use vector as container. The default is a
    // null terminated string
    std::vector<f5::FixedLengthString<10>> many{
        "data00    ", "", "will be truncated", std::string("stuff")};
    dataset.writeAttribute("strings", many);

    // Read in as a regular string
    std::vector<std::string> data{};
    dataset.readAttribute("strings", data);
    REQUIRE(data[0] == std::string("data00    "));
    REQUIRE(data[1] == std::string(""));
    REQUIRE(data[2] == std::string("will be tr"));
    REQUIRE(data[3] == std::string("stuff"));

    // Read in as a fixed length string
    std::vector<f5::FixedLengthString<10>> data_fixed{};
    dataset.readAttribute("strings", data_fixed);
    REQUIRE(data_fixed[0].to_string() == std::string("data00    "));
    REQUIRE(data_fixed[1].to_string() == std::string(""));
    REQUIRE(data_fixed[2].to_string() == std::string("will be tr"));
    REQUIRE(data_fixed[3].to_string() == std::string("stuff"));
  }

  SECTION("read a fortran-padded string as fixed-length string") {
    std::vector<double> out_data = {1, 2, 3, 4, 5};
    auto dataset = file.writeDataset("test", out_data);

    auto fortran_type = f5::detail::getFortranStringType(20);
    H5Tset_strpad(fortran_type.file_type.getID(), H5T_STR_NULLPAD);
    char x[11] = "cool bbbbb";
    dataset.writeAttribute("fortran", &x, fortran_type,
                           std::vector<hsize_t>{1});

    // Can read as fortran (space-padded) string or c null terminated,
    // however, one of the characters may get truncated as HDF5 reserves
    // a spot for the C null terminator
    std::vector<f5::FixedLengthString<10, H5T_STR_SPACEPAD>> fort_str_in{};
    std::vector<f5::FixedLengthString<10>> c_str_in{};
    dataset.readAttribute("fortran", fort_str_in);
    dataset.readAttribute("fortran", c_str_in);

    REQUIRE(fort_str_in[0].to_string() == std::string("cool bbbbb"));
    REQUIRE(c_str_in[0].to_string() == std::string("cool bbbb"));
  }

  SECTION("read a fortran or null-padded string as a std::string") {
    std::vector<double> out_data = {1, 2, 3, 4, 5};
    auto dataset = file.writeDataset("test", out_data);

    // Fortran strings are space-padded:
    std::vector<f5::FixedLengthString<10, H5T_STR_SPACEPAD>> fort_str{
        "data00    ", "", "a stuff  ", "will be truncated"};
    dataset.writeAttribute("fortran", fort_str);

    // Read back in as vector of std::strings
    std::vector<std::string> read_strings;
    dataset.readAttribute("fortran", read_strings);
    REQUIRE(read_strings[0] == "data00"); // right space padding is stripped off
    REQUIRE(read_strings[1] == "");
    REQUIRE(read_strings[2] == "a stuff");
    REQUIRE(read_strings[3] == "will be tr");
    REQUIRE(read_strings.size() == 4);

    // HDF5 also allows null-padding.  This has the same behavior
    // as null termination as far as F5 is concerned, and null termination
    // should be preferred.  It is only provided in case a file format requires that on disk.
    // This may also have some incompatibility issues with H5py.
    std::vector<f5::FixedLengthString<10, H5T_STR_NULLPAD>> zero_str{
        "data000000", "", "a stuff", "will be truncated"};
    dataset.writeAttribute("nullpad", zero_str);
    dataset.readAttribute("nullpad", read_strings);
    REQUIRE(read_strings[0] == "data000000");
    REQUIRE(read_strings[1] == "");
    REQUIRE(read_strings[2] == "a stuff");
    REQUIRE(read_strings[3] == "will be tr");
    REQUIRE(read_strings.size() == 4);
  }

  // Clean up
  remove("attribute_test.h5");
}
