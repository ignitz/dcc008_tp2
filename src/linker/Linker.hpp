
#ifndef _LINKER_H_
#define _LINKER_H_

#include <iostream>
#include <cstdio>
#include <string>
#include <vector>
#include <iomanip>
#include <fstream>

#include "Programa.hpp"
#include "SymbolTable.hpp"

#include "util.hpp"

class Linker {
private:
  bool bVerbose;        // Bool para ativar o modo verbose

public:
  Programa* output;     // Programa ligado e relocado
  Programa* mainProg;   // Programa que vai ser inserido no endreço 0
  std::vector<Programa*> modulos; // Progama contendo módulos adicionais

  /* Construtor */
  Linker ( std::vector<std::string> );
  Linker ( std::vector<std::string> , bool );
  /* Destrutor */
  virtual ~Linker ();

  // Concatena arquivos MIF (mainProg + modulos)
  void appendFiles ();

  // Desloca o posicionamento absoluto nas tabelas
  // e atualiza no programa output
  void updateAddress ();

  // Atualiza todos os endereços utilizados pelo .extern
  std::vector<Symbol*>
  updateAllExterns ( Programa* );

  // Insere o dado em string binario no arquivo MIF
  // Programa a ser gravado, endereço, e valor a inserir
  void setBinMif ( Programa* , int , int );

  // Imprime todo o conteúdo da classe.
  // Utilizado pra DEBUG
  void printAllData ();
};

#endif
