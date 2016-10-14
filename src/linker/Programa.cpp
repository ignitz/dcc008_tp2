#include "Programa.hpp"

Programa::Programa ( std::string name, std::fstream file ) {
  std::cout << name << std::endl;
}

Programa::Programa () {
  
}

Programa::~Programa () {

}

std::string Programa::getName(){
  return this->name;
}

bool Programa::setName( std::string name ){
  this->name = name;
  return true; // Quando pode dar false?
}
