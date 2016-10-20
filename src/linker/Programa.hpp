#ifndef _PROGRAMA_H_
#define _PROGRAMA_H_

#include <iostream>
#include <cstdio>
#include <string>
#include <vector>
#include "SymbolTable.hpp"
#include "util.hpp"

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
  /* Construtores */
  Programa ( std::string , bool );
  Programa ( std::string );
  /* Destrutores */
  virtual ~Programa ();

  bool setVerbose ( bool ); // Seta verbose

  std::string getName ();
  bool setName ( std::string );
  int getSize();
  int setSize( int );

  // Recebe matriz de tokens do arquivo MIF
  std::vector< std::vector< std::string > >
    getFileMif();
  // Escreve uma string no arquivo
  void writeFileMif( std::string );
  // Se posiciona no início do arquivo
  void setBeginMif();

  // Lê uma linha no arquivo
  // A substring "NoMove" não move a posição do arquivo
  std::string getLineNoMove();
  std::string getSecondLineNoMove();
  std::string getLine();

  // Funções para as tabelas de símbolos
  bool checkSymbolLocal( std::string );
  bool checkSymbolExtern( std::string );
  // Local
  int getLocalSymbolValue( std::string );
  std::vector<std::string> getLocalNames();
  std::vector<int> getLocalLocations( std::string );
  //Extern
  int getExternSymbolValue( std::string );
  std::vector<std::string> getExternNames();
  std::vector<int> getExternLocations( std::string );
  bool setExternValue( std::string , int );

  // Lê o arquivo de tabela de símbolos ".sbtl"
  bool readTable ();

  // Reloca as posições de endereço dos símbolos
  void translatePositionLocal( int );

  void printAllData (); // DEBUG
};

#endif
