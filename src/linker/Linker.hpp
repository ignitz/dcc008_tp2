
#ifndef _LINKER_H_
#define _LINKER_H_

#include <iostream>
#include <cstdio>
#include <string>
#include <vector>
#include <iomanip>
#include <fstream>

#include "Programa.hpp"

#include "util.hpp"

class Linker {
private:
  bool bVerbose;
  int iNumArgs;
public:
  Programa* output;
  Programa* mainProg;
  std::vector<Programa*> modulos;

  void appendFiles (); // Apenda arquivos MIF
  void updateAddress (); // Desloca o posicionamento absoluto nas tabelas

  void // Insere o dado em binario em tal lugar xD
  setBinMif( Programa* , int , int );

  void printAllData ();

  /* Construtor */
  Linker ( std::vector<std::string> );
  Linker ( std::vector<std::string> , bool );
  /* Desconstrutor */
  virtual ~Linker ();

};

#endif
