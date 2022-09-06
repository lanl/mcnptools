#ifndef BOOST_SERIALIZATION_MULTI_ARRAY_HPP
#define BOOST_SERIALIZATION_MULTI_ARRAY_HPP 1

#include <iostream>
#include <boost/multi_array.hpp>

namespace boost{
namespace serialization{

template<class Archive, class T, size_t t>
void save(Archive& ar, const boost::multi_array<T,t>& m, const unsigned int version) {
  typedef boost::multi_array<T,t> multi_array_;
  typedef typename multi_array_::size_type size_;

  for(size_t i=0; i<t; i++) {
    std::stringstream bss;
    bss << "base" << i;
    int b = (m.index_bases()[i]);

    ar << make_nvp(bss.str().c_str(), b);

    std::stringstream sss;
    sss << "shape" << i;
    size_ s = (m.shape()[i]);

    ar << make_nvp(sss.str().c_str(), s);
  }

  ar << make_nvp("data", make_array(m.data(), m.num_elements()));
}
  
template<class Archive, class T, size_t t>
void load(Archive& ar, boost::multi_array<T,t>& m, const unsigned int version) {
  typedef boost::multi_array<T,t> multi_array_;
  typedef typename multi_array_::size_type size_;

  std::vector<int> bases;
  std::vector<size_> shapes;

  for(size_t i=0; i<t; i++) {
    std::stringstream bss;
    bss << "base" << i;
    int b;

    ar >> make_nvp(bss.str().c_str(), b);
    bases.push_back(b);

    std::stringstream sss;
    sss << "shape" << i;
    size_ s;

    ar >> make_nvp(sss.str().c_str(), s);
    shapes.push_back(s);
  }

  m.resize(shapes);
  m.reindex(bases);
  ar >> make_nvp("data", make_array(m.data(), m.num_elements()));
}

template<class Archive, class T, std::size_t t>
void serialize(Archive& ar, boost::multi_array<T,t>& m, const unsigned int version) {
  split_free(ar,m,version);
}

} // end namespace serialization
} // end namespace boost

#endif
