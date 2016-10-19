/*
 * Apenas epifanias para manipular strings e debugar.
 */

#ifndef UTIL_H
#define UTIL_H
#include <iostream>
#include <cstdio>
#include <string>
#include <vector>
#include <iomanip>
#include <sstream>

/*
 * boost/algorithm/string.hpp
 * Utilizado para extrair tokens de strings
 * Foi o que mais gostei de utilizar mas tem como pré-requisito
 * O C++ 11
 */
#include <boost/algorithm/string.hpp>

bool clearLine( std::string& ); // Remove comentários, "\t" e espaços na linha da string
std::string int_to_binary( int );
void print( std::vector <std::string> & ); // Imprime o vetor de strings
std::string int_to_hex( int ); // Convert pra HEX
bool isInteger( const std::string & ); // isInteger

std::vector<std::string> // Retorna tokens em uma string
tokenizer( std::string , std::string );

#endif
