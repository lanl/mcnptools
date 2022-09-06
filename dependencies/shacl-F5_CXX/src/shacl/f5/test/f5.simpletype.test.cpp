#include <vector>

#include <catch2/catch.hpp>

#include "shacl/f5.hpp"

TEST_CASE("read/write simple type", "[h5type]") {
  // Create file, group.
  auto file = f5::File("h5simpletype_test.h5", 'w');

  SECTION("read/write vectors of all the supported simple types") {
    // Note, vectors of bools are not contiguous, so while bools are supported
    // in compound types and raw pointers, they are not supported in vectors.

    // Test size-defined integers to verify correct sizing.
    SECTION("int_least8") {
      std::vector<int_least8_t> out_data = {1, 2, 3, 4, 5};
      auto ds = file.writeDataset("int_least8", out_data);

      auto type = H5Dget_type(ds.getID());
      REQUIRE(H5Tequal(type, H5T_STD_I8LE));
      H5Tclose(type);

      std::vector<int_least8_t> in_data(5);
      file.readDataset("int_least8", in_data);

      REQUIRE(out_data == in_data);
    }
    SECTION("uint_least8") {
      std::vector<uint_least8_t> out_data = {1, 2, 3, 4, 5};
      auto ds = file.writeDataset("uint_least8", out_data);

      auto type = H5Dget_type(ds.getID());
      REQUIRE(H5Tequal(type, H5T_STD_U8LE));
      H5Tclose(type);

      std::vector<uint_least8_t> in_data(5);
      file.readDataset("uint_least8", in_data);

      REQUIRE(out_data == in_data);
    }
    SECTION("int_least16") {
      std::vector<int_least16_t> out_data = {1, 2, 3, 4, 5};
      auto ds = file.writeDataset("int_least16", out_data);

      auto type = H5Dget_type(ds.getID());
      REQUIRE(H5Tequal(type, H5T_STD_I16LE));
      H5Tclose(type);

      std::vector<int_least16_t> in_data(5);
      file.readDataset("int_least16", in_data);

      REQUIRE(out_data == in_data);
    }
    SECTION("uint_least16") {
      std::vector<uint_least16_t> out_data = {1, 2, 3, 4, 5};
      auto ds = file.writeDataset("uint_least16", out_data);

      auto type = H5Dget_type(ds.getID());
      REQUIRE(H5Tequal(type, H5T_STD_U16LE));
      H5Tclose(type);

      std::vector<uint_least16_t> in_data(5);
      file.readDataset("uint_least16", in_data);

      REQUIRE(out_data == in_data);
    }
    SECTION("int_least32") {
      std::vector<int_least32_t> out_data = {1, 2, 3, 4, 5};
      auto ds = file.writeDataset("int_least32", out_data);

      auto type = H5Dget_type(ds.getID());
      REQUIRE(H5Tequal(type, H5T_STD_I32LE));
      H5Tclose(type);

      std::vector<int_least32_t> in_data(5);
      file.readDataset("int_least32", in_data);

      REQUIRE(out_data == in_data);
    }
    SECTION("uint_least32") {
      std::vector<uint_least32_t> out_data = {1, 2, 3, 4, 5};
      auto ds = file.writeDataset("uint_least32", out_data);

      auto type = H5Dget_type(ds.getID());
      REQUIRE(H5Tequal(type, H5T_STD_U32LE));
      H5Tclose(type);

      std::vector<uint_least32_t> in_data(5);
      file.readDataset("uint_least32", in_data);

      REQUIRE(out_data == in_data);
    }
    SECTION("int_least64") {
      std::vector<int_least64_t> out_data = {1, 2, 3, 4, 5};
      auto ds = file.writeDataset("int_least64", out_data);

      auto type = H5Dget_type(ds.getID());
      REQUIRE(H5Tequal(type, H5T_STD_I64LE));
      H5Tclose(type);

      std::vector<int_least64_t> in_data(5);
      file.readDataset("int_least64", in_data);

      REQUIRE(out_data == in_data);
    }
    SECTION("uint_least64") {
      std::vector<uint_least64_t> out_data = {1, 2, 3, 4, 5};
      auto ds = file.writeDataset("uint_least64", out_data);

      auto type = H5Dget_type(ds.getID());
      REQUIRE(H5Tequal(type, H5T_STD_U64LE));
      H5Tclose(type);

      std::vector<uint_least64_t> in_data(5);
      file.readDataset("uint_least64", in_data);

      REQUIRE(out_data == in_data);
    }

    SECTION("size_t") {
      std::vector<size_t> out_data = {1, 2, 3, 4, 5};
      auto ds = file.writeDataset("size_t", out_data);

      auto type = H5Dget_type(ds.getID());
      REQUIRE(H5Tequal(type, H5T_STD_U64LE));
      H5Tclose(type);

      std::vector<size_t> in_data(5);
      file.readDataset("size_t", in_data);

      REQUIRE(out_data == in_data);
    }

    // Test char
    SECTION("char") {
      std::vector<char> out_data = {1, 2, 3, 4, 5};
      auto ds = file.writeDataset("char", out_data);

      auto type = H5Dget_type(ds.getID());
      if (std::is_signed<char>::value) {
        REQUIRE(H5Tequal(type, H5T_STD_I8LE));
      } else {
        REQUIRE(H5Tequal(type, H5T_STD_U8LE));
      }
      H5Tclose(type);

      std::vector<char> in_data(5);
      file.readDataset("char", in_data);

      REQUIRE(out_data == in_data);
    }

    // Test basic floats
    SECTION("float") {
      std::vector<float> out_data11 = {1, 2, 3, 4, 5};
      auto ds = file.writeDataset("float", out_data11);

      auto type = H5Dget_type(ds.getID());
      REQUIRE(H5Tequal(type, H5T_IEEE_F32LE));
      H5Tclose(type);

      std::vector<float> in_data11(5);
      file.readDataset("float", in_data11);

      REQUIRE(out_data11 == in_data11);
    }
    SECTION("double") {
      std::vector<double> out_data12 = {1, 2, 3, 4, 5};
      auto ds = file.writeDataset("double", out_data12);

      auto type = H5Dget_type(ds.getID());
      REQUIRE(H5Tequal(type, H5T_IEEE_F64LE));
      H5Tclose(type);

      std::vector<double> in_data12(5);
      file.readDataset("double", in_data12);

      REQUIRE(out_data12 == in_data12);
    }
  }

  // Clean up
  remove("h5simpletype_test.h5");
}
