#ifndef _SYMBOLTABLE_H_
#define _SYMBOLTABLE_H_

#include <iostream>
#include <cstdio>
#include <string>
// #include <deque>
#include <vector>
#include <sstream>      // std::ostringstream

enum TypeSymbol {label, variable, externCall};

/*
 * Elemento contendo o Símbolo e posicao
 */
class Symbol {
public:
  bool bVerbose;
  std::string name;
  bool valueInit; // Indica se o valor foi iniciado
  int value; // Valor representado na tabela
  std::vector<int> location; // Grava todas as posições de ocorrencia

  Symbol( std::string );
};

class SymbolTable {
private:
  bool bVerbose;
  std::vector<Symbol*> symbol;
public:
  SymbolTable();

  void translateSymbol( int );
  bool insertSymbol( std::string );
  bool insertValue( int );
  bool checkSymbol( std::string );
  bool setVerbose( bool );
  void printSymbols( );

};

#endif
