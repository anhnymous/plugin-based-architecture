#include <stdio.h>

#include "PluginX.hh"
#include "PluginY.hh"
#include "Interfaces.hh"
  
int Interfaces::xxx_read()
{
  return PluginX::xxx_read();
}
  
bool Interfaces::xxx_write(int value)
{
  return PluginX::xxx_write(value);
}
  
float Interfaces::yyy_read()
{
  return PluginY::yyy_read();
}

bool Interfaces::yyy_write(float value)
{
  return PluginY::yyy_write(value);
}
