#include <string>

#include "Read.hh"
#include "Write.hh"
#include "PluginX.hh"

#define NOK_INT -1

// TODO this should return std::optional<int>
int PluginX::xxx_read()
{
  auto retval = ReadService::use_service().read("giid/001");
  if (!retval.empty()) {
    return std::stoi(retval);
  } else {
    return NOK_INT; 
  }
}

bool PluginX::xxx_write(int value)
{
  return WriteService::use_service().write("giid/002", std::to_string(value));
}
