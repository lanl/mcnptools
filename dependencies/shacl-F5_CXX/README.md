F5 - A Wrapper Library for HDF5
===============================

This library is designed to provide an interface to the HDF5 library.  There are two motivations for this library.  First, the C API for HDF5 is the easiest to work with; it's always available on target machines, it is feature complete, and there are fewer compiler issues.  Second, HDF5 requires the developer to ensure that all access handles are closed correctly.  This library builds entirely off of the C API, but eliminates the need for the developer to deal with handles by closing them automatically.  If a feature is not supported by this library, raw calls to the HDF5 C api can be made using the raw HDF5 identifiers accessed through the `getID()` member function provided by all types in this library.


Interested developers can add this to their code by including `f5.hpp`.  The basic API is as follows:

# Files

Files can be created as follows:
```cpp
auto file = f5::File("filename.h5", 'a');
```
The first argument is the filename, and the second argument is the file access mode. `'a'` appends to files if they exist, and creates them if they don't. `'w'` will overwrite a file if it exists and create it if it doesn't.  `'r'` opens a file for reads only.

As with all handles, files will self-close when they go out of scope.
# Groups

Groups form the basic organization of HDF5 files. Groups can be opened as follows:
```cpp
auto group = other_group.openGroup("group name");
```
Groups must be opened on top of other groups.  `File` types act as groups with name "/".

As with all handles, groups will self-close when they go out of scope.

# Listing of Objects

Within files or groups, a list of the names of objects within can be obtained with the listNames() function:
```cpp
auto objectlist=group.listNames();
```

# Types

Types contain information on how things are laid out in memory or how we want to lay them out on the disk.  This library has a number of built-in types for support of the Fortran side:

```cpp
// Create a type based on basic C types
// Supports most integers, float, and double at the moment
auto type = f5::getSimpleType<type>();
// Create a fixed-size string that will be read from a char*
int string_length = 5;
auto type = f5::detail::getFortranStringType(string_length);
```

## Custom basic types

Users can create their own types and pass them to the H5Type object for memory management:

```cpp
// Copy of Fortran string example
// Memory type
auto h5_type = H5Tcopy(H5T_C_S1);
H5Tset_strpad(h5_type, H5T_STR_SPACEPAD);
H5Tset_size(h5_type, str_size);
auto mtype = f5::BasicType(h5_type, true);
// Disk type
h5_type = H5Tcopy(H5T_C_S1);
H5Tset_strpad(h5_type, H5T_STR_NULLTERM);
H5Tset_size(h5_type, str_size + 1);
auto dtype = f5::BasicType(h5_type, true);
auto type = Type(mtype, ftype);
```
The second argument determines if `H5Type` needs to close the type when it goes out of scope.  The value of `true` is required for custom types, but not required for HDF5 built-in types.

## String support

> *NOTE* F5 currently assumes all strings are ASCII, and does not natively support the UTF-8 character set. By default, [h5py](https://docs.h5py.org/en/stable/strings.html) will write UTF-8 encoded strings.

F5 provides support for HDF5 variable-length and fixed-length string datasets and attributes. For best compatibility with other HDF5 libraries, prefer to read and write `std::vector<std::string>` as the data type;
only use fixed-length strings when a specific size and padding is needed for a different language (e.g., Fortran) or a specific file format. Note, interoperability between Fortran and C++ strings can have issues because of differences in the null terminator character.

### Reading and writing `std::string` data

The simplest and preferred way to process strings with F5 is through a `std::string`. A `std::vector<std::string>` can be written or read from a file as a single-dimensional dataset.  The data is stored in the file as a variable-length string datatype.  This data type is also compatible with **h5py** strings.  A `std::vector<std::string>`, raw string literals, or `std::string` can all be written as attributes.  Both variable-length and fixed-length string datasets can be *read* into a `std::vector<std::string>`. The following example demonstrates how to write and read string attributes and datasets:

```cpp
auto group = file.openGroup("data");
std::string about {"description of data"};
group.writeAttribute("units", "seconds");
group.writeAttribute("about", about);
std::vector<std::string> string_data { "one", "two", "three" };
group.writeDataset("string_data", string_data};
...
std::vector< std::string > read_multiple {};
std::string read_single {};
group.readAttribute("units", read_single);
group.readAttribute("about", read_multiple);
group.readDataset("string_data", read_multiple);
```
Slicing and raw arrays or pointers to `std::string` are not supported.  

### Fixed-length strings

F5 provides support for fixed-length string datasets when size and padding are specified at compile-time.  This is done through the library provided C++ type `f5::FixedLengthString`, as in the following tests:

```cpp
//FixedLengthString is constructible with c-style or STL strings:
std::vector< f5::FixedLengthString<10> > many{"data01    ", "", "will be truncated", std::string("stuff")};
dataset.writeAttribute("strings", many);

//Read in as a fixed length string dataset
std::vector<f5::FixedLengthString<10>/*, H5_STR_NULLTERM*/> data_fixed {};
dataset.readAttribute("strings", data_fixed);

//To access stored value, convert to a std::string:
REQUIRE( data_fixed[0].to_string() == std::string("data01    ") );
REQUIRE( data_fixed[1].to_string() == std::string("") );
REQUIRE( data_fixed[2].to_string() == std::string("will be tr") );
REQUIRE( data_fixed[3].to_string() == std::string("stuff") );

//Can always read fixed-lengths strings as std::string's
std::vector<std::string> data {};
dataset.readAttribute("strings", data);
REQUIRE( data[0] == std::string("data01    ") );
REQUIRE( data[1] == std::string("") );
REQUIRE( data[2] == std::string("will be tr") );
REQUIRE( data[3] == std::string("stuff") );
```
The full signature of the type is `f5::FixedLengthString<size_t StrSize, h5_str_t PadType = H5_STR_NULLTERM>`, where `StrSize` is the size of the strings and PadType specifies how strings with size less than `StrSize` should be padded. The possible values for the optional `PadType` argument are `H5_STR_NULLTERM` (default), `H5_STR_SPACEPAD`, and `H5_STR_NULLPAD` (padded with extra null characters -- prefer `H5_STR_NULLTERM` when possible).  With `H5_STR_NULLTERM` strings are null-terminated as in standard C++.  With `H5_STR_SPACEPAD`, shorter strings are padded on the right with spaces; space-padded strings can be written and read in Fortran natively.  Any padding characters on the right will be stripped when converting back to a `std::string`, as demonstrated in the following code:

```cpp
f5::FixedLengthString<15, H5_STR_SPACEPAD> fortran_str { "rt pad      " };
REQUIRE( fortran_str.to_string() == std::string("rt pad") );
```
An important note is that the padding choice should match the padding type in the file, or the call to `.to_string()` may produce unexpected results.  In particular, HDF5 can cause a character to be removed when converting space-padded strings to null-terminated strings.

## Custom compound types

F5 supports writing arrays of structures, so long as the structures are standard layout.  As an example, assume one has a struct named `Test` with 3 objects in it: a, b, and c.  The type can be generated as follows:
```cpp
Test t = Test();
auto type_builder = f5::makeCompoundTypeBuilder(t);
type_builder.addItem("a", &Test::a);
type_builder.addItem("b", &Test::b);
type_builder.addItem("c", &Test::c);
auto type = type_builder.build();
```
The resulting `type` can be used wherever a `f5::Type` would be.

Compound datasets can also contain user-defined HDF5 types, like enums.
```cpp
  enum class Foo{ a, b };
  struct Bar{
    Foo foo = Foo::a;
  };
  // create enum_type using calls to HDF5
  f5::BasicType mem_and_file_type(enum_type, true);
  auto f5_enum_type = f5::Type(mem_and_file_type, mem_and_file_type);
  auto type_builder = f5::makeCompoundTypeBuilder( Bar{Foo::a} );
  type_builder.addItem("Foo", &Bar::Foo, f5_enum_type);
```


# Datasets

Datasets are where information is actually stored.  There are a few ways to interact with datasets, depending on how much flexibility is required.

## Operating on Groups
By operating on a group, the syntax is shortest, but it is the least flexible.  There are three write signatures, two for `std::vector` containers, and one for pointers to arrays:

```cpp
// STL version, infers HDF5 data type from the C++ type `double`
std::vector<double> data = {1,2,3,4,5};
group.writeDataset("name", data);
// User-defined type
group.writeDataset("name", data, type);

// Pointer version (need to specify type, shape)
double *data;
f5::H5Type = getSimpleHDF5Type<double>();
std::vector<unsigned long long> shape = {5};
group.writeDataset("name", data, type, shape);
```

There are also three read signatures:
```cpp
// STL version
std::vector<double> data(5);
group.readDataset("name", data);
// User-defined type
group.readDataset("name", data, type);

// Pointer version (need to specify type too)
double *data;
f5::H5Type = getSimpleHDF5Type<double>();
group.readDataset("name", data, type);
```

Both `readDataset` and `writeDataset` return a `Dataset` object, for later use.

## Using the Dataset Builder
In order to use the more advanced features of HDF5, one needs to work with the `DatasetBuilder` class.  The `DatasetBuilder` has a number of associated functions:
```cpp
auto builder = f5::DatasetBuilder();
// Example extent and chunk shape
std::vector<hsize_t> extent = {1,2,3};
std::vector<hsize_t> chunk = {1,1,3};

builder.setShape(extent);                    // Set the shape of the data on disk
builder.setMaxShape(extent);                 // Set the maximum shape of the data on disk
builder.setType(type);                       // Set the type to be used for IO
builder.inferFrom(data);                     // Given an STL vector, infer extent, type
builder.setChunkShape(chunk);                // Set the chunk shape
builder.enableCompression(5);                // Set compression level to 5 (defaults to 9 if not specified)
auto dataset = builder.build(group, "test"); // Constructs a dataset object on group with name "test"
```
All variables allow for overwriting if multiple similar datasets are to be made. A fluent syntax is also available (`f5::DatasetBuilder().[...].build(group, "name")`).

`setShape` will set the shape of the dataset on the disk.  This does not need to be the same shape as the data to be written, which is useful for hyperslabs, discussed later.  The disk extent must be greater than or equal to the shape of the data you intend to write.

`setMaxShape` will set the maximum possible shape of the dataset on the disk for resizable arrays.  This should be greater than or equal to the value used in `setShape`.  If not provided, it defaults to the shape.

`setType` allows setting the type that HDF5 will use for data conversion.

`inferFrom` is a substitute for `setShape` and `setType` in which the one-dimensional extent and type are inferred from an STL vector.  Either the first should be used or the last two should be used.

`setChunkShape` enables dataset chunking, in which the data is split into chunks of the given shape and stored in that way.  Random array IO performance can improve with suitable chunk selection.  Alternatively, one can call the `setAutoChunk()` function with no argument specified.  This will compute automatic chunk sizes such that individual chunks fit into the default HDF5 cache size of 1MB.

`enableCompression` compresses the dataset using GZIP.  This can substantially decrease file size at the cost of increased CPU utilization.  As compression is applied in a per-chunk basis, good chunk shape selection can substantially improve performance.  A good discussion on the topic can be found [here](https://www.hdfgroup.org/2017/05/hdf5-data-compression-demystified-2-performance-tuning/).  In summary: use chunks that correspond to access patterns, and/or try to keep chunks smaller than 1MB.

## Opening a Dataset on Disk

Opening a dataset is as simple as doing the following operation on a group:
```cpp
auto dataset = group.openDataset("name");
```
An optional second argument is the type you'd like to use. The library will examine the object on disk, extract its shape and (if needed) the type, and be available for reading or writing.

## Using Datasets to Read/Write
Reading with a dataset:
```cpp
// STL signature
std::vector<double> data(5);
dataset.read(data, offset={}, shape={});

// Pointer signature
double *data;
dataset.read(data, offset={}, shape={});
```
Writing with a dataset:
```cpp
// STL signature
std::vector<double> data(5);
dataset.write(data, offset={}, shape={});

// Pointer signature
double *data;
dataset.write(data, offset={}, shape={});
```
If not writing a hyperslab, `shape` and `offset` are not needed.  The shape will be inferred from the dataset, and the array will fill the entire dataset.

If writing a hyperslab, `shape` indicates the shape of the array you're writing.  `offset` indicates where the upper corner of the array should begin on disk.

One can extend the array via `reshape(std::vector<hsize_t>)`.  HDF5 will truncate data if reshaped to smaller than what it currently is.

## Hyperslabs
Hyperslabs allow a program to write part of an array at a time.  This is useful to minimize memory usage, and necessary for parallelism (not yet implemented).

The shape of the array on disk is input during the `openDataset` operation, and the shape of the object to be read/written is input during either the `read`/`write` function calls.  For example, writing a length-3 array to a length-5 target, then reading indices 1-4:

```cpp
// Create dataset for array of shape {5} (can't be inferred from our data)
std::vector<unsigned long long> disk_shape = {5};
auto type = f5::getSimpleHDF5Type<double>();
auto ds = group.getBuilder("test")
              .setType(type)
              .setShape(disk_shape);

// Write a length 3 array offset by 1
std::vector<double> data = {1,2,3};
ds.write(data, 1, 3);

// Read indices 1-4
std::vector<double> read_data;
ds.read(read_data, 1, 3);
```
The result is an array of `{1,2,3}`, with `{0,1,2,3,0}` written to disk.  The read operation will resize the vector as needed.

# Attributes
Attributes are small sets of data that can be attached to files, groups, or datasets.  They are written directly to the metadata.  As a result, performance is higher, particularly if the data is small.

The syntax is identical to reading/writing datasets on groups above, except the functions are `writeAttribute` and `readAttribute`, and scalar values can be written directly.  These functions are available on `File`, `Group`, and `Dataset`.  See the [tests](./src/shacl/f5/test/f5.attribute.test.cpp) for more examples.

# Parallel HDF5

## Threading

There is no guarantee of threaded support for F5 from this library.  Although raw HDF5 can be compiled in a thread-safe mode (which sets a lock every time the library is accessed), it requires disabling MPI parallelism (as of HDF5v1.12).    However, as long as only a single thread is performing an HDF5 operataion at a given time, F5 has been demonstrated used successfully with threads writing thread-private data to a single *shared* file and group.  Whenever writing to a dataset, opening a file or group, etc., only a *single thread* can be performing the F5 call.  We have not tested having each thread open their own file.

## MPI

HDF5 natively supports MPI parallelism.  When built agains an HDF5 library compiled with openmpi parallelism, the parallel features of F5 will be automatically built.  With a successful parallel build, the file (and groups) must be opened with an MPI communicator and (optionally) info object.  All ranks perform the opening operations, e.g.,

```cpp
//If using MPI_COMM_WORLD, on all ranks
#include "f5.hpp"
auto file = f5::File("filename.h5", 'w', MPI_COMM_WORLD, MPI_INFO /*optional*/);
auto group = file.openGroup("group_name");
```

Once opened, independent IO operations can be performed through the same calls as for serial.  While writing and reading to datasets in different groups can be performed out of order by ranks, all ranks must perform the same operations when modifying metadata. For example, if each rank wants to have a unique group to write to, every rank must collectively perform the call to open each of those groups.  All ranks must also perform the same data-set resizing calls.  For explicit details on what operations must be performed collectively, see the parallel HDF5 [documentation](https://portal.hdfgroup.org/display/HDF5/Introduction+to+Parallel+HDF5).

To generate higher parallel performance, collective reads and writes of datasets should be used, where each rank writes into a different hyper-slab of a dataset.   For a collective call to be successful, every rank in the communicator must participate in the reading or writing function call, even if they write zero data to the dataset.  The following is an example of each rank collectively writing a fixed-size amount of unique data into a portion of a single, shared dataset:

```cpp
//Make data unique to each rank (identified by rank = MPI_Rank)
std::vector<double> data{1., 2., 3., 4., 5., 6.};
for(auto & value : data) value *= rank;

//Write data into my section of the hyperslab
unsigned int offset = rank * 6;
unsigned int total_size = size * 6;

auto type = f5::getSimpleType<double>();
auto dataset_builder = f5::DatasetBuilder()
                            .setShape({total_size})
                            .setChunkShape({6})
                            .setType(type);

//Open data set as usual
auto ds = dataset_builder.build(file, "test");

// Open and write on all ranks at once
ds.write(data, {offset}, f5::collectiveProperty());
```
Note, it is strongly recommended that users only use the parallel F5 operations on a parallel file system (e.g., lustre), or the cost of the file-system blocking on writes will outweigh any performance gains.  Also, there is currently no way for the dataset to ensure that the file was opened in parallel.  If the file was not opened in parallel, then parallel writes will fail with a generic error message.  For more detailed examples, see the [integration tests](src/shacl/ test/).  For more details on the art of performance tuning, see the parallel HDF5 [documentation](https://portal.hdfgroup.org/display/HDF5/Introduction+to+Parallel+HDF5).

# Paths and Filenames

Every HDF5 object (file, group, dataset, etc.) has an associated filename and path to that object inside of the file.
One can obtain the filename corresponding to any object via the Object::filename method, e.g.
```cpp
F5::file file("test.h5");
auto filename = file.filename(); // filename = "test.h5"
auto group = file.openGroup("testGroup");
filename = group.filename(); // fileanme = "test.h5"
std::vector<double> data{0.0, 1.0};
auto dataset = group.writeDataset{"data", data};
filename = dataset.filename(); // filename = "test.h5"
```

Additionally, one can obtain the path to the object inside of the file
```cpp
auto path = file.path(); // path = "/"
path = group.path(); // path = "/testGroup"
path = dataset.path(); // path = "/testGroup/data"
```

# Links

In HDF5, links are how big objects (groups and datasets) are connected together.
F5 provides some capability to check the existence of, create, and delete these links.

## Check Links

```cpp
parent.checkLink("link_name");
```

This will check to see if `parent` has a child object (group or dataset) by the name of `link_name`.

## Delete Links

```cpp
parent.deleteLink("link_name");
```

This will remove the link from `parent` to `link_name`.  If this was the last link to the object referred to by `link_name`, it will be garbage collected (and effectively deleted).

## Create Links

```cpp
new_parent.createLink("new_name", old_parent, "old_name");
```

This creates a link of the form `new_parent/new_name => old_parent/old_name`. This is useful if you want to refer to a group or dataset in more than one place, but you don't want to store several copies.
