#include <iostream>
#include <cstdio>
#include <string>
#include <vector>

#include "Linker.hpp"

#define MSG_NOME_PROGRAMA		"Ligador Wombat2 ver Dorico\n"
#define MSG_COPYRIGHT 			"(C) 2016 Yuri Niitsuma <ignitzhjfk@gmail.com>\n         Lucas Machado <lucaspedro.machado@gmail.com>\n         Nathalia Campos <nathalia.campos.braga@gmail.com>\n"
#define MSG_USO							"    Uso: linker programa.mif main modulo1 modulo2 ... moduloN [-v]\n"

int main(int argc, char const *argv[]) {

  if (argc < 3) {
    std::cout << MSG_NOME_PROGRAMA << std::endl;
    std::cout << MSG_COPYRIGHT << std::endl;
    std::cout << MSG_USO << std::endl;
    exit(EXIT_SUCCESS);
  }

  bool bVerbose = false;
  std::vector<std::string> sArgs;
  for (auto i = 1; i < argc; i++) {
    if (argv[i][0] != '-') {
      sArgs.push_back(argv[i]);
    }
    else if ( std::string(argv[i]).compare("-v") == 0) {
      bVerbose = true;
    }
    else { // É uma cilada Bino, não ter argumento válido
      std::cerr << "Comando " << argv[i] << "Inválido!" << std::endl;
      exit(EXIT_FAILURE);
    }
  }

  Linker linker(sArgs, bVerbose); // Inicia o linkador com os programas
  linker.appendFiles();
  linker.updateAddress();
  // - Atualizar tabelas dos símbolos
  // - Encontrar todos os externs e atualizar na table symbol global.
  // - Encontrar todas as ocorrências dos símbolos e atualizá-las.

  return 0;
}
