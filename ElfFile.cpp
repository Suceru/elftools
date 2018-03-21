#include "ElfFile.hpp"
#include "ElfHandler.hpp"

#include <functional>
#include <vector>

#include <iostream>

#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>
#include <errno.h>

#include <elf.h>
#include <fcntl.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>

using namespace std;

ElfFile::ElfFile(const std::string& filename){
  _filename = filename;

  const auto elffile = _filename.c_str();

  int fd = open(elffile, O_RDONLY);
  if(fd == -1){
    fprintf(stderr, "Could not open elf file %s! - ErrorNr: %d\n", elffile, errno);
    exit(EXIT_FAILURE);
  }

  struct stat statbuf;
  if(fstat(fd, &statbuf) == -1){
    fprintf(stderr, "Could not resolve file size! - ErrorNr: %d\n", errno);
    exit(EXIT_FAILURE);
  }

  _memorysize = static_cast<size_t>(statbuf.st_size);
  _memory = static_cast<char*>(mmap(NULL, _memorysize, PROT_READ, MAP_PRIVATE, fd, 0));
  if(_memory == MAP_FAILED){
    fprintf(stderr, "Could not map file in memory! - ErrorNr: %d\n", errno);
    exit(EXIT_FAILURE);
  }
  
  if(close(fd) == -1){
    fprintf(stderr, "Could not close elf file %s! - ErrorNr: %d\n", elffile, errno);
    exit(EXIT_FAILURE);
  }
}

ElfFile::~ElfFile(){
  if(munmap(_memory, _memorysize) == -1){
    fprintf(stderr, "Could not unmap memory! - ErrorNr: %d\n", errno);
    exit(EXIT_FAILURE);
  }
}

ElfFile::ArchClass ElfFile::getArchClass() const{
  switch(_memory[4]){
  case ELFCLASS32:
    return ArchClass::Bit32;
  case ELFCLASS64:
    return ArchClass::Bit64;
  default:
    return ArchClass::Unknown;
  }
}

bool ElfFile::checkFile() const{
  if(_memory[0] != 0x7F ||
     _memory[1] != 'E'  ||
     _memory[2] != 'L'  ||
     _memory[3] != 'F')
    return false;

  return true;
}

const char* ElfFile::getMemory() const{
  return _memory;
}

namespace {

template <typename ElfHeader, typename ProgramHeader, typename SectionHeader>
class ElfHandlerImpl : public ElfHandler {
public:
  ElfHandlerImpl(const char* memory) :
    ElfHandler(),
    _memory(memory),
    _elfHeader(*reinterpret_cast<const ElfHeader*>(memory)),
    _programHeaders(reinterpret_cast<const ProgramHeader*>(&memory[_elfHeader.e_phoff]),
		    reinterpret_cast<const ProgramHeader*>(&memory[_elfHeader.e_phoff + _elfHeader.e_phentsize * (_elfHeader.e_phnum + 1)])),
    _sectionHeaders(reinterpret_cast<const SectionHeader*>(&memory[_elfHeader.e_shoff]),
		    reinterpret_cast<const SectionHeader*>(&memory[_elfHeader.e_shoff + _elfHeader.e_shentsize * (_elfHeader.e_shnum + 1)]))
  {};

  virtual const SymbolTable& getSymbolTable() const{
    return _symbolTable;
  };
  
private:
  const char* const  _memory;
  const ElfHeader&   _elfHeader;
  const std::vector<std::reference_wrapper<const ProgramHeader>> _programHeaders;
  const std::vector<std::reference_wrapper<const SectionHeader>> _sectionHeaders;

  SymbolTable _symbolTable;
};

}

unique_ptr<ElfHandler> ElfFile::getHandler() const{
  using ElfHandler32 = ElfHandlerImpl<Elf32_Ehdr, Elf32_Phdr, Elf32_Shdr>;
  using ElfHandler64 = ElfHandlerImpl<Elf64_Ehdr, Elf64_Phdr, Elf64_Shdr>;

  const auto archClass = getArchClass();
  if(archClass == ElfFile::ArchClass::Bit32)
    return make_unique<ElfHandler32>(_memory);
  else if(archClass == ElfFile::ArchClass::Bit64)
    return make_unique<ElfHandler64>(_memory);
  else
    throw;
}
