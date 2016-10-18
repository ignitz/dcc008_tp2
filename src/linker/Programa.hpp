#ifndef _PROGRAMA_H_
#define _PROGRAMA_H_

#include <iostream>
#include <cstdio>
#include <string>
#include <vector>
#include "SymbolTable.hpp"


/*
 * boost/algorithm/string.hpp
 * Utilizado para extrair tokens de strings
 * Foi o que mais gostei de utilizar mas tem como pré-requisito
 * O C++
 */
#include <boost/algorithm/string.hpp>

/* Files management */
#include <iomanip>
#include <fstream>

/*
 * class Programa
 * Esta classe trata cada módulo, tanto o arquivo mif de saída como
 * os arquivos que serão efetuados a ligação
 */
class Programa {
private:
  bool bVerbose;
  std::string name;
  std::fstream fileMif; // Arquivo Mif
  std::fstream fileStbl; // Arquivo Symbol Table
  int size; // Tamanho do módulo
  SymbolTable tableLocal; // Para as variáveis locais
  SymbolTable tableExtern; // Para as variáveis globais
public:

  Programa ( std::string , bool );
  Programa ( std::string );
  virtual ~Programa ();

  std::string getName ();
  bool setName ( std::string );
  int getSize();
  int setSize( int );

  std::vector<
      std::vector< std::string > > getFileMif();
  void writeFileMif( std::string );

  bool setVerbose ( bool );
  bool readTable ();

  void translatePositionLocal( int );
  void translatePositionExtern( int );

  void printAllData (); // DEBUG
};

#endif
