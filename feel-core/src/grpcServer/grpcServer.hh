#ifndef GRPCSERVER_H_
#define GRPCSERVER_H_

#include <string>
#include <functional>

#include <grpcpp/server.h>
#include <grpcpp/grpcpp.h>

using string = std::string;

class grpcServer
{
public:
  grpcServer() = default;
  ~grpcServer() = default;

  grpcServer(grpcServer&&) = delete;
  grpcServer(const grpcServer&) = delete;
  grpcServer& operator=(grpcServer&&) = delete;
  grpcServer& operator=(const grpcServer&) = delete;

  grpcServer& set_ipv4(const string& ipv4) noexcept;
  grpcServer& set_port(const string& port) noexcept;

  void start();
  bool is_running() const noexcept;
  void stop() noexcept;

private:
  string m_server_ipv4;
  string m_server_port;

  inline static std::function<void(grpc::Server*)> gsd = [](grpc::Server* s) { (void)s; };
  std::unique_ptr<grpc::Server, decltype(gsd)> m_server_core{nullptr, gsd};
  std::atomic<bool> m_server_running{false};
};

#endif /* GRPCSERVER_H_ */
