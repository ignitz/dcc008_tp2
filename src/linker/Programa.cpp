#include "Programa.hpp"

Programa::Programa ( std::string name, bool bWrite ) {
  std::string modMif, modSbtl;
  modMif  = name + ".mif";
  modSbtl = name + ".stbl";
  this->setName(name);
  this->size = 0;

  if (bWrite) { // Abre os arquivos pra leitura-escrita
    this->fileMif.open( modMif , std::ios::out | std::ios::in | std::ios::trunc );
    this->fileStbl.open( modSbtl , std::ios::out | std::ios::in | std::ios::trunc );
  }
  else { // Por padrão abre os arquivos pra leitura
    this->fileMif.open( modMif , std::ios::in );
    this->fileStbl.open( modSbtl , std::ios::in );
  }

  if (!this->fileMif.is_open() || !this->fileStbl.is_open()) {
    std::cerr << "Erro ao abrir " << this->getName() << std::endl;
    exit(EXIT_FAILURE);
  }

  // Lê a tabela de símbolos
  this->readTable();
}

Programa::Programa ( std::string name ) : Programa ( name, false ) {

}

Programa::~Programa () {
  this->fileMif.close();
  this->fileStbl.close();
}

std::string Programa::getName () {
  return this->name;
}

bool Programa::setName ( std::string name ) {
  this->name = name;
  return true; // TODO Quando pode dar false?
}

bool Programa::setVerbose ( bool bVerbose ) {
  this->bVerbose = bVerbose;
  return this->bVerbose;
}

bool Programa::readTable () {

  if (!this->fileStbl.is_open()) { // Verifica se o arquivo está aberto.
    std::cerr << "Erro em abrir o arquivo " << this->getName() << std::endl;
    return false;
  }

  std::string line;
  std::vector<std::string> fields;
  int state = 0;

  this->fileStbl.seekg(0, this->fileStbl.beg);
  while (getline(this->fileStbl, line)) {
    // Separa a linha em tokens
    boost::split(fields, line, boost::is_any_of("\t "), boost::token_compress_on);
    // Testando o iterador, acabei de aprender xD
    // É semelhante ao foreach no PHP
    for (auto const& field : fields) {
      switch (state) {
        case 0:
          if (field.compare("Size:") == 0)
          state++;
        break;

        case 1: // Lê o tamanho do módulo
          if ( field.substr(0, 2).compare("0x") == 0 ) {
            this->size = std::stoi ( field, nullptr, 0 );
            state++;
          }
          else {
            std::cerr <<
            "Erro em ler a tabela no campo " << field << "." << std::endl;
            std::cerr << "tentando continuar a leitura" << std::endl;
          }
        break;

        case 2: // Lendo tabelas de símbolos locais
          if ( field.compare("EXTERN:") == 0 ) { // Passa para o próximo estado
            state++;
          }
          else { // Aqui vou inserir os labels locais
            // TODO
          }
        break;

        case 3: // Lendo tabelas EXTERN
          // TODO
          // Aqui vou inserir os labels globais
        break;

        default: // Caso ocorra algo inexperado
          std::cerr << "Duendes alteraram a variável state para " <<
              state << "." << std::endl;
        break;
      } // end switch
    } // end for
  }

  return state == 3 ? true : false;
}

void Programa::printAllData () { // Imprime todo o conteúdo do módulo
  std::cout << "Nome:" << this->getName() << std::endl;
  std::cout << "----------------" << std::endl;
}
