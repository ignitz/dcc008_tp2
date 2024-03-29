#include "passTwo.hpp"

#define String std::string

#define HEAD_MIF "DEPTH = 256;\nWIDTH = 8;\nADDRESS_RADIX = HEX;\nDATA_RADIX = BIN;\nCONTENT\nBEGIN\n\n"
#define END_MIF "END;\n"
// don't forget END;

void
pass_two(std::ifstream& file, std::fstream& objeto, TableOpcode& table, std::string sSaida) {
  bool bVerbose = table.verbose;
  using namespace std;
  if (bVerbose) cout << "*********************************\nDEBUG: Iniciando Pass_Two" << endl;

  std::string sSaidaTbl;

  sSaidaTbl = sSaida.substr(0, sSaida.find_last_of(".mif") - 3); // Retira extensão
  sSaidaTbl += ".stbl"; // Adiciona extensão pro símbolo

  fstream fTabela(sSaidaTbl, ios::out | ios::in | ios::trunc);
  if (!fTabela.is_open()) {
		cout << "Erro ao tentar abrir o arquivo " << sSaidaTbl << endl;
		exit(EXIT_FAILURE);
	}

  file.clear();
	file.seekg(0, file.beg); // Posiciona no inicio do arquivo novamente

  String line, output;  // campos da instrução
  vector<string> fields;        // tokens

  output += HEAD_MIF;

  table.location_counter = 0;

  while (getline(file, line)) {
    if (clearLine(line))
			continue;           // Se a linha não contiver instruções

    if (bVerbose) cout << "DEBUG: " << line << endl;

    boost::split(fields, line, // Separa a linha em tokens
          boost::is_any_of("\t "), boost::token_compress_on);

    output += table.get_line_mif(fields);
    transform(output.begin(), output.end(), output.begin(), ::toupper);
  }

  if (bVerbose) table.printSymbols();

  // Finaliza gravando em arquivo
  output += std::string("[" + int_to_hex(table.location_counter++) + "..FF]:  00000000;\n");
  output += END_MIF;
  objeto << output;

  // Grava a tabela de símbolos em um arquivo
  fTabela << "Size: 0x" << int_to_hex(table.location_counter - 1) << std::endl;
  fTabela << table.getTableSymbols();
  fTabela.close();
} // end passtwo
