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

bool clearLine( std::string& ); // Remove comentários, "\t" e espaços na linha da string
std::string string_binary( int );
void print( std::vector <std::string> & ); // Imprime o vetor de strings
std::string int_to_hex( int ); // Convert pra HEX
bool isInteger( const std::string & ); // isInteger

#endif
