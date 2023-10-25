#ifndef PLUGINMANAGER_H_
#define PLUGINMANAGER_H_

#include <string>
#include <memory>
#include <unordered_map>

#include "Plugin.hh"
using plugin_t = std::shared_ptr<Plugin>;

class PluginManager
{
public:
  ~PluginManager() = default;

  PluginManager(PluginManager&&) = delete;
  PluginManager(const PluginManager&) = delete;
  PluginManager& operator=(PluginManager&&) = delete;
  PluginManager& operator=(const PluginManager&) = delete;

  static PluginManager& getInstance();

  /**
   * Load a shared lib into a shared_ptr of PluginImpl 
   */
  plugin_t load(const string&& plugin_name, const string&& path_to_plugin);

  /**
   * To avoid unloading wrong plugin, we require both name and path to the plugin
   * we want to unload.
   */
  void unload(const string&& plugin_name, const string&& path_to_plugin);

  /**
   * Map a giid to a Plugin, store into internal DB m_plugin_registry
   */
  void store(const string&& giid, const plugin_t& p);

  /**
   * Look for the Plugin by a giid
   */
  plugin_t lookup(const string& giid);

private:
  PluginManager() = default;
  std::unordered_map<giid, plugin_t> m_plugin_registry;
};

#endif /* PLUGINMANAGER_H_ */
