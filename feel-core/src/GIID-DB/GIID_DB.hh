#ifndef GIID_DB_H_
#define GIID_DB_H_

#include "PluginManager.hh"

class GIID_DB
{
public:
  ~GIID_DB() = default;

  GIID_DB(GIID_DB&&) = delete;
  GIID_DB(const GIID_DB&) = delete;
  GIID_DB& operator=(GIID_DB&&) = delete;
  GIID_DB& operator=(const GIID_DB&) = delete;

  static GIID_DB& getInstance();
  
  /**
   * Build GIID DB which is a map from giids to Plugins
   */
  bool build_db() noexcept;

  /**
   * Find the shared ptr to Plugin that support Interface giid
   */
  plugin_t lookup(const string& giid) const noexcept;

private:
  GIID_DB();
  PluginManager& m_plugin_mgr;
};

#endif /* GIID_DB_H_ */
