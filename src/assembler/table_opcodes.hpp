#ifndef _TAB_OPCODE_
#define _TAB_OPCODE_

#include <iostream>
#include <cstdio>
#include <string>
#include <vector>
#include "symbol_table.hpp"
#include "util.hpp"

/*
 * Elemento opcode com mneumonico e valor representado
 */
class Opcode {
public:
  std::string name;
  int value;
  Opcode(std::string, int);
};

/*
 * Tabela de Opcodes da classe acima
 */

class TableOpcode : public SymbolTable {
private:
  std::vector<Opcode*> opcode;
public:
  /* Verifica se um vetor de strings contém um opcode no primeiro iostream
   * ou no segundo, caso o primeiro seja label
   */
  std::string extract_opcode( std::vector<std::string> );

  // Verifica se uma string é um opcode
  bool isOpcode( std::string );

  // Faz o trabalho do .data
  std::string get_data( std::vector<std::string> );

  int get_opcode_value( std::string );
  // Faz o trabalho todo
  std::string get_line_mif( std::vector<std::string> );
  int get_instruction( std::vector<std::string> );
  int get_register( std::string );

  TableOpcode();
};

#endif
