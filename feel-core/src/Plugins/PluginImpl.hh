#ifndef PLUGINIMPL_H_
#define PLUGINIMPL_H_

#include "Plugin.hh"
#include "PluginProvider.hh"

class PluginImpl: public Plugin
{
public:
  PluginImpl(const string&& name, const string&& path_to_plugin);
  ~PluginImpl();

  PluginImpl(PluginImpl&&) = delete;
  PluginImpl(const PluginImpl&) = delete;
  PluginImpl& operator=(PluginImpl&&) = delete;
  PluginImpl& operator=(const PluginImpl&) = delete;

  string get_name() const noexcept;
  string get_path() const noexcept;

  std::vector<giid> get_giids() const noexcept;
  string read(const string&& giid) noexcept;
  bool write(const string&& giid, const string&& value) noexcept;

private:
  string          m_name;
  string          m_path;
  void*           m_plugin_dl; ///< Plugin handler to dlopen() APIs
  PluginProvider* m_plugin; 
};

#endif /* PLUGINIMPL_H_ */
