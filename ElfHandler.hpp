#include <string>

class ElfHandler {

 public:
  enum class ArchClass{
    Unknown = 0,
    Bit32   = 1,
    Bit64   = 2,
  };

  ElfHandler(const std::string& filename);
  ~ElfHandler();

  bool checkFile();
  ArchClass getArchClass();
  
 private:
  std::string _filename;
  size_t      _memorysize;
  char*       _memory;
};
