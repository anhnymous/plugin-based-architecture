#include "Application.hh"

Application::Application(int argc, char** argv): m_argc(argc), m_argv(argv) { }

void Application::useHandler(std::shared_ptr<AppHandler> handler) noexcept
{
  if (handler) m_allHandlers.emplace_back(handler);
}

void Application::useFallbackHandler(std::shared_ptr<AppHandler> handler) noexcept
{
  if (handler) {
    m_fallbackHandler = handler;
    m_allHandlers.emplace_back(handler);
  }
}

AppHandler::RetCode Application::start()
{
  auto handler = getHandler();
  return handler->start(m_argc, m_argv);
}

std::shared_ptr<AppHandler> Application::getHandler()
{
  for (auto handler : m_allHandlers) {
    if (handler->isMatchedArguments(m_argc, m_argv)) {
      return handler;
    }
  }
  return m_fallbackHandler;
}
