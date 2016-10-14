
#ifndef _LINKER_H_
#define _LINKER_H_

#include <iostream>
#include <cstdio>
#include <string>
#include <vector>

#include "Programa.hpp"

class Linker {
private:
  int iNumArgs;
  Programa output;
  std::vector<Programa> modulos;
public:
  /* Construtor */
  Linker ();
  /* Desconstrutor */
  virtual ~Linker ();

};

#endif
