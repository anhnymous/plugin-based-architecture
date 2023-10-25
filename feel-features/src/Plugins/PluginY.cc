#include <string>

#include "Read.hh"
#include "Write.hh"
#include "PluginY.hh"

#define NOK_FLOAT -1

// TODO this should return std::optional<float>
float PluginY::yyy_read()
{
  auto retval = ReadService::use_service().read("giid/003");
  if (!retval.empty()) {
    return std::stof(retval);
  } else {
    return NOK_FLOAT; 
  }
}

bool PluginY::yyy_write(float value)
{
  return WriteService::use_service().write("giid/004", std::to_string(value));
}
