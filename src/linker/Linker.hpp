
#ifndef _LINKER_H_
#define _LINKER_H_

#include <iostream>
#include <cstdio>
#include <string>
#include <vector>

#include "Programa.hpp"

class Linker {
private:
  bool bVerbose;
  int iNumArgs;
public:
  Programa* output;
  Programa* mainProg;
  std::vector<Programa*> modulos;
  void printAllData();

  /* Construtor */
  Linker ( std::vector<std::string> );
  Linker ( std::vector<std::string> , bool );
  /* Desconstrutor */
  virtual ~Linker ();

};

#endif
