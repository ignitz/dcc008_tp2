#ifndef _SYMBOL_TABLE_
#define _SYMBOL_TABLE_

#include <iostream>
#include <cstdio>
#include <string>
#include <deque>
#include <sstream>      // std::ostringstream

enum TypeSymbol {label, variable};

/*
 * Elemento contendo o Símbolo com seu ID, símbolo e posicao
 */
class Symbol {
public:
  static int id;
  std::string name;
  int value;
  int num_bytes;
  TypeSymbol type;
  Symbol( std::string , int );
  Symbol( std::string , int , int );
  Symbol( std::string , int , std::string );
  Symbol( std::string , int , TypeSymbol );
};

class SymbolTable {
private:
  std::deque<Symbol*> symbol;
public:
  bool verbose;
  int location_counter;
  SymbolTable();
  int get_symbol_value( std::string );
  int get_symbol_value( int );
  bool insertSymbol( std::string , int );
  bool insertSymbol( std::string , int , std::string );
  bool checkSymbol( std::string );

  /*
   * Adiciona memórias com o tamanho em bytes
   */
  bool redefine( int );

  /*
   * Retorna String contendo a tabela de símbolos
   */
  std::string getTableSymbols();

  void printSymbols();
};

#endif
