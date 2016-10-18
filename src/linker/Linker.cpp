#include "Linker.hpp"

Linker::Linker( std::vector<std::string> sArgs ) {
  this->bVerbose = false;
  this->iNumArgs = sArgs.size();

  // Checar se o arquivo já existe
  this->output = new Programa(sArgs[0], true);
  sArgs.erase(sArgs.begin());

  this->mainProg = new Programa(sArgs[0]);
  sArgs.erase(sArgs.begin());

  for(auto const& value: sArgs) {
    this->modulos.push_back(new Programa(value));
  }
}

Linker::Linker( std::vector<std::string> sArgs, bool bVerbose ) : Linker(sArgs) {
  this->bVerbose = bVerbose;
  if (this->bVerbose) {
    std::cout << "Setando verbose nos módulos" << std::endl;
    this->output->setVerbose(bVerbose);
    this->mainProg->setVerbose(bVerbose);
    for(auto const& modulo: this->modulos) {
      modulo->setVerbose(bVerbose);
    }
    std::cout << "Imprimindo a tabela original (sem deslocamento):" << std::endl;
    this->printAllData();
  }
}

Linker::~Linker() {
  if (this->bVerbose) std::cout << "Deletando mainProg." << std::endl;
  delete mainProg;
  if (this->bVerbose) std::cout << "Deletando output." << std::endl;
  delete output;
  if (this->bVerbose) std::cout << "Deletando módulos:" << std::endl;
  for(auto const& modulo: this->modulos) {
    if (this->bVerbose) std::cout << modulo->getName() << std::endl;
    delete modulo;
  }
}

void
Linker::appendOutput() {
  auto fields = this->mainProg->getFileMif();
  this->output->writeFileMif(fields[0][0]);
  for (auto& linefields : fields) {
    for (auto& field : linefields)
      this->output->writeFileMif(field);
  }
}

void
Linker::updateAddress () {
  if (this->mainProg->getSize() % 2 != 0) // Faz um alinhamento para par bytes
    this->mainProg->setSize(this->mainProg->getSize() + 1);
  int sumSize = this->mainProg->getSize();

  for (auto const& modulo : this->modulos) {
    if (modulo->getSize() % 2 != 0) // Faz um alinhamento para par bytes
      modulo->setSize(modulo->getSize() + 1);
    modulo->translatePositionLocal(sumSize);
    sumSize += modulo->getSize();
  }

  // TODO Atualizar os extern todos
  // Então, a ideia é procurar por todas as tabelas até achar e daí atualizar o valor correto

  if (this->bVerbose) {
    std::cout <<
    std::endl <<
    "Imprimindo as tabelas após as atualizações:" << std::endl;
    this->printAllData();
  }
}

void
Linker::printAllData () { // Imprime todo o conteúdo do linkador
  std::cout << "Executando printAllData do Linker" << std::endl;
  std::cout << "- Arquivo de saída MIF" << std::endl;
  this->output->printAllData();
  std::cout << "- Main" << std::endl;
  this->mainProg->printAllData();
  std::cout << "- Módulos" << std::endl;
  for (auto const& modulo : this->modulos) {
    modulo->printAllData();
  }
}
