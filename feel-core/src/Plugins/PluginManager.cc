#include "PluginImpl.hh"
#include "PluginManager.hh"

PluginManager& PluginManager::getInstance()
{
  static PluginManager instance;
  return instance;
}
  
plugin_t PluginManager::load(const string&& plugin_name, const string&& path_to_plugin)
{
  try {
    auto ret =  std::make_shared<PluginImpl>(std::move(plugin_name),
                                             std::move(path_to_plugin));
    return ret;
  } catch (const std::runtime_error& e) {
    return nullptr;
  }
}

void PluginManager::unload(const string&& plugin_name, const string&& path_to_plugin)
{
  auto plugin = this->load(std::move(plugin_name), std::move(path_to_plugin));
  auto giids = plugin->get_giids();
  for (const auto& intf : giids) {
    m_plugin_registry.erase(intf);
  }
}

void PluginManager::store(const string&& giid, const plugin_t& p)
{
  m_plugin_registry[giid] = p;
}

plugin_t PluginManager::lookup(const string& giid)
{
  auto finding = m_plugin_registry.find(giid);
  if (finding != m_plugin_registry.end()) {
#ifdef DEBUG
  fprintf(stdout, "[DEBUG] lookup(%s): Found plugin: %p\n",
          giid.c_str(), (void*)finding->second.get());
#endif
    return finding->second;
  } else {
#ifdef DEBUG
    fprintf(stdout, "[DEBUG] lookup(%s): Not found plugin\n", giid.c_str());
#endif
    return nullptr;
  }
}
