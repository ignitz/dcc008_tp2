// C++
#include <iostream>
#include <cstdio>
#include <string>
#include <iomanip>
#include <fstream>
#include <vector>
#include "symbol_table.hpp"

#include "passOne.hpp"
#include "passTwo.hpp"

#define MSG_NOME_PROGRAMA		"Montador Wombat2\n"
#define MSG_COPYRIGHT 			"(C) 2016 Yuri Niitsuma <ignitzhjfk@gmail.com>\n         Lucas Machado <lucaspedro.machado@gmail.com>\n         Nathalia Campos <nathalia.campos.braga@gmail.com>\n"
#define MSG_USO							"    Uso: montador NOME_ARQUIVO [-o NOME_ARQUIVO_SAIDA] [-v]\n"

int
main(int argc, char *argv[])
{
	if (argc < 2){
		std::cout << MSG_NOME_PROGRAMA << MSG_COPYRIGHT << MSG_USO;
		return -1;
	}

	using namespace std;
	bool bSaida = true;
	bool bVerbose = false;
	std::string sEntrada = argv[1]; // Definicao do arquivo de entrada
	std::string sSaida;


	for(int i = 2; i < argc; i++){
		if(string("-o") == string(argv[i])) { // Verifica se nome de saida foi passado
			bSaida = false;
			sSaida = argv[i+1];
		}
		if(string("-v") == string(argv[i])) { // modo Verbose
			bVerbose = true;
		}
	}

	if(bSaida)
		sSaida = "tst/exec.mif"; // Nome padrão para geracao de executavel

	ifstream fEntrada(sEntrada, ios::in);
	fstream fObjeto(sSaida, ios::out | ios::in | ios::trunc);

	// Checa se os arquivos estão abertos
	if (!fEntrada.is_open() || !fObjeto.is_open()) {
		cout << "Erro ao tentar abrir o arquivo " <<
					(fEntrada.is_open()? sSaida : sEntrada) << endl;
		exit(EXIT_FAILURE);
	}

	TableOpcode table;  // inicialização geral
	table.verbose = bVerbose;
	pass_one(fEntrada, table);
	pass_two(fEntrada, fObjeto, table);

	fEntrada.close();
	fObjeto.close();

	return 0;
}
