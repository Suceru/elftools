#include <map>
#include <memory>
#include <string>

class ElfHandler;

class ElfFile {

 public:
  enum class ArchClass{
    Unknown = 0,
    Bit32   = 1,
    Bit64   = 2,
  };

  ElfFile(const std::string& filename);
  ~ElfFile();

  bool checkFile() const;
  ArchClass getArchClass() const;

  const char* getMemory() const;
  std::unique_ptr<ElfHandler> getHandler() const;

 private:
  std::string _filename;
  size_t      _memorysize;
  char*       _memory;
};
