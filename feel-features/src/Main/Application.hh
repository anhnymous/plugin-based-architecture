#ifndef APPLICATION_H_
#define APPLICATION_H_

#include <vector>
#include <memory>

#include "AppHandler.hh"

class Application
{
public:
  Application(Application&&) = delete;
  Application(const Application&) = delete;
  Application& operator=(Application&&) = delete;
  Application& operator=(const Application&) = delete;

  explicit Application(int argc, char** argv);
  ~Application() = default;

  int argc() const noexcept { return m_argc; }
  char** argv() const noexcept { return m_argv; }
  std::vector<std::shared_ptr<AppHandler>> handlers() const noexcept {
    return m_allHandlers;
  }
  std::shared_ptr<AppHandler> fallbackHandler() const noexcept {
    return m_fallbackHandler;
  }

  void useHandler(std::shared_ptr<AppHandler> handler) noexcept;
  void useFallbackHandler(std::shared_ptr<AppHandler> handler) noexcept;
  AppHandler::RetCode start();

private:
  int    m_argc;
  char** m_argv;

  std::vector<std::shared_ptr<AppHandler>> m_allHandlers;
  std::shared_ptr<AppHandler>              m_fallbackHandler;

  std::shared_ptr<AppHandler> getHandler();
};

#endif /* APPLICATION_H_ */
