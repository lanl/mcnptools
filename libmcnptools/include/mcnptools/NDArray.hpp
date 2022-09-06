/*
File: NDArray.hpp
Author: Clell J. (CJ) Solomon Jr. [clell.solomon@gmail.com]
Description: Provides a light weight multidimensional array class
*/
#ifndef __CJSOFT_NDARRAY_HPP__
#define __CJSOFT_NDARRAY_HPP__

#include <vector>
#include <exception>
#include <string>
#include <cassert>
#include <memory>

#ifdef __MCNPTOOLS_BOOST_SERIALIZATION__
#include <boost/serialization/access.hpp>
#include <boost/serialization/nvp.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/shared_ptr.hpp>
#endif

namespace cjsoft {

// Forward declaration of NDArray
template<class T, size_t N>
class NDArray;

// NDArrayException
// Provides an excpetion class
class NDArrayException : public std::exception {
  public:
    NDArrayException(std::string msg) : m_msg(msg) {};
    virtual ~NDArrayException() throw() {};

    virtual const char* what() const throw() {
      return m_msg.c_str();
    };

  private:
    const std::string m_msg;
};

// NDArrayShaper
// Provides a class for shaping an NDArray
template<class T, class U, size_t N, size_t M>
class NDArrayShaper {
  friend class NDArrayShaper<T,U,N,M+1>;
  public:
    NDArrayShaper<T,U,N,M-1> operator[](const U& size) {
      std::vector<U> shape = m_shape;
      shape.push_back(size);
      return NDArrayShaper<T,U,N,M-1>(shape);
    };

  private:
    NDArrayShaper(const std::vector<U>& shape) : m_shape(shape) {};
    std::vector<U> m_shape;
};

template<class T, class U, size_t N>
class NDArrayShaper<T,U,N,N> {
  public:
    NDArrayShaper() {};

    NDArrayShaper<T,U,N,N-1> operator[](const U& size) {
      std::vector<U> shape = m_shape;
      shape.push_back(size);
      return NDArrayShaper<T,U,N,N-1>(shape);
    };

  private:
    std::vector<U> m_shape;
};


template<class T, class U, size_t N>
class NDArrayShaper<T,U,N,0> {
  friend class NDArrayShaper<T,U,N,1>;
  public:
    U operator()(const size_t& i) const {
      return m_shape.at(i);
    };

  private:
    NDArrayShaper(const std::vector<U>& shape) : m_shape(shape) {};
    std::vector<U> m_shape;
};

// NDArrayIndexer
// Provides a class for indexing an NDArray
template<class T, size_t N, size_t M>
class NDArrayIndexer {
  public:
    NDArrayIndexer(T* const offset, const std::vector<size_t>& mults) :  m_mults(mults), m_offset(offset) {};

    NDArrayIndexer<T,N,M-1> operator[](size_t i) {
      return NDArrayIndexer<T,N,M-1>(m_offset + i * m_mults[N-M], m_mults) ;
    };

  private:
    const std::vector<size_t>& m_mults;
    T* const m_offset;
};

template<class T, size_t N>
class NDArrayIndexer<T,N,1> {
  public:
    NDArrayIndexer(T* const offset, const std::vector<size_t>& mults) :  m_mults(mults), m_offset(offset) {};

    T& operator[](size_t i) {
      return *(m_offset + i * m_mults[N-1]);
    };

  private:
    const std::vector<size_t>& m_mults;
    T* const m_offset;
};

// NDArray
// Provides the NDArray class
template<class T, size_t N>
class NDArray {
  public:
    enum ordering {
      ROWMAJOR,
      COLMAJOR
    };
    
    typedef NDArrayShaper<T,size_t,N,N> shaper_type;

    NDArray() : m_order(ROWMAJOR) {};
 
    NDArray(const NDArrayShaper<T,size_t,N,0>& shape, ordering order=ROWMAJOR) : m_order(order) {
      for( size_t i=0; i<N; i++) {
        m_shape.push_back(shape(i));
      }
      Allocate();
    };

    void DeepCopy(const NDArray<T,N>& array) {
      m_order = array.m_order;
      m_shape = array.m_shape;
      Allocate();
      *m_data = *(array.m_data);
    };

    void Reshape(const NDArrayShaper<T,size_t,N,0>& shape, ordering order=ROWMAJOR) {
      m_order = order;
      m_shape.empty();
      for( size_t i=0; i<N; i++) {
        m_shape.push_back(shape(i));
      }
      Allocate();
    };

    std::vector<size_t> Shape() const {return m_shape;};

    NDArrayIndexer<T,N,N-1> operator[](size_t i) const {
      if( m_data == NULL ) {
        throw NDArrayException("Attempt to access unallocated NDArray data");
      }

      return  NDArrayIndexer<T,N,N-1>(GetData() + i*m_mults[0], m_mults);
    };

    T& GetAtIndices(const std::vector<size_t>& indices) const {
      size_t offset = 0;

      for(size_t i=0; i<N; i++) {
        offset += indices[i] * m_mults[i];
      }

      return (*m_data)[offset];
    };

    T* GetData() const { return &((*m_data)[0]); };
    size_t GetDataSize() const {
      size_t size = 1;
      for(size_t i=0; i<N; i++) {
        size *= m_shape[i];
      }
      return size * sizeof(T);
    };

    size_t GetMult(size_t i) const {
      return m_mults[i];
    };

  private:

    void Allocate() {
      size_t size = 1;
      for(size_t i=0; i<N; i++) {
        size *= m_shape[i];
      }

      m_mults.clear();
      for(size_t i=0; i<N; i++) {
        size_t mult=1;

        size_t start = i+1;
        size_t end = N;
        if( m_order == COLMAJOR ) {
          start = 0;
          end = i;
        }
        for(size_t j=start; j<end; j++) {
          mult *= m_shape[j];
        }

        m_mults.push_back(mult);
      }

      if( m_data ) {
        m_data->resize(size);
      }
      else {
        m_data.reset( new std::vector<T>(size) );
      }
    };

    ordering m_order;
    std::vector<size_t> m_shape;
    std::vector<size_t> m_mults;
    std::shared_ptr<std::vector<T> > m_data;

#ifdef __MCNPTOOLS_BOOST_SERIALIZATION__ 
    friend class boost::serialization::access;

    template<typename Archive>
    void serialize(Archive& ar, const unsigned int version) { 
      using namespace boost::serialization;

      ar & make_nvp("order", m_order);
      ar & make_nvp("shape", m_shape);
      ar & make_nvp("mults", m_mults);
      ar & make_nvp("data", m_data);
    };
#endif
};

} // namespace
#endif
