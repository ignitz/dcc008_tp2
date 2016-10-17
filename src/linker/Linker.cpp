#include "Linker.hpp"

Linker::Linker( std::vector<std::string> sArgs ) {
  this->bVerbose = false;
  this->iNumArgs = sArgs.size();

  // Checar se o arquivo já existe
  this->output = new Programa(sArgs[0], true);
  sArgs.erase(sArgs.begin());

  this->mainProg = new Programa(sArgs[0]);
  sArgs.erase(sArgs.begin());

  for(auto const& value: sArgs) {
    this->modulos.push_back(new Programa(value));
  }
}

Linker::Linker( std::vector<std::string> sArgs, bool bVerbose ) : Linker(sArgs) {
  this->bVerbose = bVerbose;
  if (bVerbose) {
    for(auto const& modulo: this->modulos) {
      modulo->setVerbose(bVerbose);
    }
  }
}

Linker::~Linker() {
  for(auto const& modulo: this->modulos) {
    delete modulo;
  }
}
