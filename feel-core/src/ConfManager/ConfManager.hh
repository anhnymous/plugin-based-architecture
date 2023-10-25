#ifndef CONFMANAGER_H_
#define CONFMANAGER_H_

#include <mutex>
#include <memory>
#include <vector>
#include <string>
#include <cstdint>

using string = std::string;

class ConfManager
{
public:
  static std::shared_ptr<ConfManager> load();
  static std::shared_ptr<ConfManager> reload();
  ~ConfManager() = default;

  inline string& logDir() const noexcept {
    return m_logDir;
  }

  inline string& logLevel() const noexcept {
    return m_logLevel;
  }

  inline uint32_t& logSizeLimit() const noexcept {
    return m_logSizeLimit;
  }

private:
  static std::shared_ptr<ConfManager> m_confManager;
  static std::mutex                   m_mutex;
  static string                       m_logDir;
  static string                       m_logLevel;
  static uint32_t                     m_logSizeLimit; // bytes

  explicit ConfManager();
};

#endif /* CONFMANAGER_H_ */
