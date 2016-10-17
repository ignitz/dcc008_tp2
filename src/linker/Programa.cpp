#include "Programa.hpp"

Programa::Programa ( std::string name, bool bWrite ) {
  std::string modMif, modSbtl;
  modMif  = name + ".mif";
  modSbtl = name + ".stbl";

  this->setName(name);

  if (bWrite) {
    this->fileMif.open( modMif , std::ios::out | std::ios::in | std::ios::trunc );
    this->fileStbl.open( modSbtl , std::ios::out | std::ios::in | std::ios::trunc );
  }
  else {
    this->fileMif.open( modMif , std::ios::in );
    this->fileStbl.open( modSbtl , std::ios::in );
  }

  if (!this->fileMif.is_open() || !this->fileStbl.is_open()) {
    std::cerr << "Erro ao abrir " << this->getName() << std::endl;
    exit(EXIT_FAILURE);
  }
  if (this->bVerbose) std::cout << this->getName() << std::endl;
}

Programa::Programa ( std::string name ) : Programa ( name, false ) {

}

Programa::~Programa () {
  this->fileMif.close();
  this->fileStbl.close();
}

std::string Programa::getName () {
  return this->name;
}

bool Programa::setName ( std::string name ) {
  this->name = name;
  return true; // TODO Quando pode dar false?
}

bool Programa::setVerbose ( bool bVerbose ) {
  this->bVerbose = bVerbose;
  return true; // TODO Quando pode dar false?
}
