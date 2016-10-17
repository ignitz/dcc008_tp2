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
public:
  bool bVerbose;
  std::fstream fileMif;
  std::fstream fileStbl;

  Programa ( std::string , bool );
  Programa ( std::string );
  virtual ~Programa ();

  std::string getName ();
  bool setName ( std::string );
  bool setVerbose ( bool );
  // std::string getFile( std::string );
  // bool setFile( std::string );
};
