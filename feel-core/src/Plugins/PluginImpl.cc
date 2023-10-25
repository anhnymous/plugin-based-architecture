#include <dlfcn.h>
#include <stdexcept>

#include "PluginImpl.hh"

PluginImpl::PluginImpl(const string&& name, const string&& path_to_plugin)
{
  m_name = name;
  m_path = path_to_plugin;
  m_plugin_dl = nullptr;
  m_plugin = nullptr;

  m_plugin_dl = dlopen(path_to_plugin.c_str(), RTLD_NOW);
  if (!m_plugin_dl) {
    throw std::runtime_error("Failed to load plugin using dlopen");
  }

  dlerror();
  char* error = NULL;
  create_f create_plugin = (create_f)dlsym(m_plugin_dl, "create");
  if ((error = dlerror()) != NULL) {
    dlclose(m_plugin_dl);
   throw std::runtime_error("Failed to load symbol 'create'"); 
  } else {
    m_plugin = create_plugin();
  }
}

PluginImpl::~PluginImpl()
{
  dlerror();
  char* error = NULL;
  destroy_f destroy_plugin = (destroy_f)dlsym(m_plugin_dl, "destroy");
  if ((error = dlerror()) != NULL) {
    dlclose(m_plugin_dl);
  } else {
    destroy_plugin(m_plugin);
    m_plugin = nullptr;
    dlclose(m_plugin_dl);
    m_plugin_dl = nullptr;
  }
}
  
string PluginImpl::get_name() const noexcept
{
  return m_name;
}

string PluginImpl::get_path() const noexcept
{
  return m_path;
}

std::vector<giid> PluginImpl::get_giids() const noexcept
{
  return m_plugin->get_giids();
}

string PluginImpl::read(const string&& giid) noexcept
{
  return m_plugin->read(std::move(giid));
}

bool PluginImpl::write(const string&& giid, const string&& value) noexcept
{
  return m_plugin->write(std::move(giid), std::move(value));
}
