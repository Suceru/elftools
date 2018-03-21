#include <map>
#include <string>

using SymbolTable = std::map<std::string, uint64_t>;

class ElfHandler{
protected:
  ElfHandler(){};

public:
  virtual const SymbolTable& getSymbolTable() const = 0;
};
