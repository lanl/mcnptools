#include <stdexcept>

extern "C" {
void b (){
  try{
    throw std::runtime_error("ya dun f'd up");
  } catch (std::runtime_error&) {
  }
}
}
