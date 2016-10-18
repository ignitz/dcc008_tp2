#ifndef _SYMBOL_TABLE_
#define _SYMBOL_TABLE_

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
  std::string name;
  bool valueInit; // Indica se o valor foi iniciado
  int value; // Valor representado na tabela
  std::vector<int> location; // Grava todas as posições de ocorrencia

  Symbol( std::string );
};

class SymbolTable {
private:
  std::vector<Symbol*> symbol;
public:
  bool verbose;

  SymbolTable();

  // int get_symbol_value( std::string );
  // int get_symbol_value( int );
  bool insertSymbol( std::string );
  bool insertValue( int );
  // bool insertSymbol( std::string , int , std::string );
  bool checkSymbol( std::string );
  void printSymbols( );


};

#endif
