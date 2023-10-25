#include "GIID_DB.hh"
#include "FCoreLogger.hh"
#include "FCoreConfig.hh"

GIID_DB& GIID_DB::getInstance()
{
  static GIID_DB instance;
  return instance;
}

bool GIID_DB::build_db() noexcept
{
  auto xp = m_plugin_mgr.load(PLUGIN_X, PLUGIN_X_PATH);
  if (!xp) {
    LOG_ERROR("FCore: failed to load Plugin X");
    return false;
  }

  auto yp = m_plugin_mgr.load(PLUGIN_Y, PLUGIN_Y_PATH);
  if (!yp) {
    LOG_ERROR("FCore: failed to load Plugin Y");
    return false;
  }

  m_plugin_mgr.store("giid/001", xp);
  m_plugin_mgr.store("giid/002", xp);
  m_plugin_mgr.store("giid/003", yp);
  m_plugin_mgr.store("giid/004", yp);
  return true;
}
  
plugin_t GIID_DB::lookup(const string& giid) const noexcept
{
  return m_plugin_mgr.lookup(giid);
}
  
GIID_DB::GIID_DB(): m_plugin_mgr{PluginManager::getInstance()} { }
