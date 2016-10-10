#include "passOne.hpp"

// Primeira passada do montador
void
pass_one(std::ifstream& file, TableOpcode& table) {
	bool bVerbose = table.verbose;
	using namespace std;
	if (bVerbose) cout << "*********************************\nDEBUG: Iniciando Pass_One" << endl;

	string line, symbol, literal, strOpcode;  // campos da instrução
	int store_location;       // variáveis diversas
	vector<string> fields;                 // tokens

	while (getline(file, line)) { // obtenha uma linha de entrada
		if (clearLine(line))
			continue;           // Se a linha não contiver instruções

		if (bVerbose) cout << "DEBUG: " << line << endl;

		boost::split(fields, line, // Separa a linha em tokens
				boost::is_any_of("\t "), boost::token_compress_on);

		if (fields[0].back() == ':') { // Insere o simbolo na tabela
      // Prefixo "_" eh label
      store_location = (fields[0].front() == '_') ? table.location_counter : 0;
			fields[0].pop_back();

			if (table.checkSymbol(fields[0])) { // Verifica se label ja existe
				cerr << "Redeclaração do símbolo: " << fields[0] << endl;
				exit(EXIT_FAILURE);
			}
			else {
				table.insertSymbol(fields[0], store_location, fields[2]);
			}

			fields[0].push_back(':');
		} // end of while (getline(file, line))

		strOpcode = table.extract_opcode(fields);

		if (table.isOpcode(strOpcode)) {
			table.location_counter += 2;
		}
		else if (strOpcode.compare(".data")) {
			table.location_counter += 0;
		}
		else {
			cerr << "Opcode desconhecido: " << strOpcode << endl;
			exit(EXIT_FAILURE);
		}
	}

	// modifica symbol
	table.redefine(table.location_counter);

	table.insertSymbol("IO", 254); // Entrada e saida

	file.clear();
	file.seekg(0, file.beg); // Posiciona no inicio do arquivo novamente

}
