#ifndef PLUGIN_H_
#define PLUGIN_H_

#include <vector>
#include <string>

using giid = std::string;
using string = std::string;

class Plugin
{
public:
  Plugin() = default;
  virtual ~Plugin() = default;

  Plugin(Plugin&&) = delete;
  Plugin(const Plugin&) = delete;
  Plugin& operator=(Plugin&&) = delete;
  Plugin& operator=(const Plugin&) = delete;

  /**
   * Return the Plugin's name
   */
  virtual string get_name() const noexcept = 0;
  
  /**
   * Return the absolute path to shared lib (plugin)
   */
  virtual string get_path() const noexcept = 0;

  /**
   * Return the list of all giids supported by this Plugin
   */
  virtual std::vector<giid> get_giids() const noexcept = 0;

  /**
   * Generic "Read" interface/service provided by this Plugin
   */
  virtual string read(const string&& giid) noexcept = 0;

  /**
   * Generic "Write" interface/service provided by this Plugin
   */
  virtual bool write(const string&& giid, const string&& value) noexcept = 0;
};

#endif /* PLUGIN_H_ */
