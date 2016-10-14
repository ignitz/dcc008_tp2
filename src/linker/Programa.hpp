#include <iostream>
#include <cstdio>
#include <string>
#include <vector>

/* Files management */
#include <iomanip>
#include <fstream>

class Programa {
private:
  std::string name;
  std::fstream file;
public:
  Programa(std::string, std::fstream);
  Programa(); // main
  virtual ~Programa ();

  std::string getName();
  bool setName( std::string );
  // std::string getFile( std::string );
  // bool setFile( std::string );
};
