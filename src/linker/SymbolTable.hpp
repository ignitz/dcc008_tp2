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
  std::vector<int> location; // Grava todas as posições de ocorrencia
  Symbol( std::string );
};

class SymbolTable {
private:
  std::vector<Symbol*> symbol;
public:
  bool verbose;
  // int get_symbol_value( std::string );
  // int get_symbol_value( int );
  // bool insertSymbol( std::string , int );
  // bool insertSymbol( std::string , int , std::string );
  // bool checkSymbol( std::string );
  // bool set_symbol_extern( std::string );

  SymbolTable();

};

#endif
