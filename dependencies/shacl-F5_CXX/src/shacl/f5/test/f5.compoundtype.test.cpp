#include <vector>

#include <catch2/catch.hpp>

#include "shacl/f5.hpp"

enum class Color { Maize, Blue };

TEST_CASE("read/write custom type", "[h5type]") {
  // Create file, group.
  auto file = f5::File("h5type_test.h5", 'w');

  SECTION("read/write complicated type") {
    struct Test {
      int32_t a;
      int64_t b;
      double c;
      bool d;
      explicit Test(int ind, bool d) : a(ind), b(ind), c(ind), d(d) {}
      explicit Test() : Test(0, false) {}

      bool operator==(const Test &that) const {
        return (this->a == that.a) && (this->b == that.b) &&
               (this->c == that.c) && (this->d == that.d);
      }

      bool operator!=(const Test &that) const {
        return !(*this == that);
      }
    };

    // Create type (has default constructor, so no arguments passed)
    auto type_builder = f5::makeCompoundTypeBuilder(Test());
    type_builder.addItem("a", &Test::a);
    type_builder.addItem("b", &Test::b);
    type_builder.addItem("c", &Test::c);
    type_builder.addItem("d", &Test::d);
    auto type = type_builder.build();

    // Write to file
    std::vector<Test> out_data = {Test(1, true), Test(2, false), Test(3, true)};
    file.writeDataset("test1", out_data, type);

    // Read from file
    std::vector<Test> in_data(3);
    file.readDataset("test1", in_data, type);

    REQUIRE(out_data == in_data);
  }

  SECTION("read/write single inheritance standard layout type") {
    struct TestA {
      int32_t a;
      int64_t b;
      bool c;
      explicit TestA(int ind, bool c) : a(ind), b(ind), c(c) {}
    };

    struct TestB : public TestA {
      explicit TestB(int ind, bool c) : TestA(ind, c) {}
      explicit TestB() : TestB(0, false) {}

      bool operator==(const TestB &that) const {
        return (this->a == that.a) && (this->b == that.b) &&
               (this->c == that.c);
      }

      bool operator!=(const TestB &that) const {
        return !(*this == that);
      }
    };

    // Create type (arguments passed to default constructor)
    auto type_builder = f5::makeCompoundTypeBuilder(TestB(0, false));
    auto type = type_builder.addItem("a", &TestB::a)
                    .addItem("b", &TestB::b)
                    .addItem("c", &TestB::c)
                    .build();

    // Write to file
    std::vector<TestB> out_data = {TestB(1, true), TestB(2, false),
                                   TestB(3, true)};
    file.writeDataset("test1", out_data, type);

    // Read from file
    std::vector<TestB> in_data(3);
    file.readDataset("test1", in_data, type);

    REQUIRE(out_data == in_data);
  }

  SECTION("read/write compound type containing enum") {
    struct EnumTest {
      int a = 0;
      Color color = Color::Maize;

      bool operator==(const EnumTest &that) const {
        return (this->a == that.a) && (this->color == that.color);
      }
    };
    auto enum_type = H5Tenum_create(H5T_NATIVE_INT);
    auto enum_value = Color::Maize;
    H5Tenum_insert(enum_type, "Maize", &enum_value);
    enum_value = Color::Blue;
    H5Tenum_insert(enum_type, "Blue", &enum_value);
    f5::BasicType mf_type(enum_type, true);
    auto f5_enum_type = f5::Type(mf_type, mf_type);
    auto type_builder = f5::makeCompoundTypeBuilder(EnumTest{1, Color::Blue});
    auto compound_type = type_builder.addItem("a", &EnumTest::a)
                             .addItem("color", &EnumTest::color, f5_enum_type)
                             .build();
    std::vector<EnumTest> out_data = {EnumTest{1, Color::Maize},
                                      EnumTest{2, Color::Blue}};
    file.writeDataset("test1", out_data, compound_type);

    // Read from file
    std::vector<EnumTest> in_data(2);
    file.readDataset("test1", in_data, compound_type);
    REQUIRE(out_data == in_data);
  }
  // Clean up
  remove("h5type_test.h5");
}
