#include <grpcpp/health_check_service_interface.h>
#include <grpcpp/ext/proto_server_reflection_plugin.h>

#include "grpcServer.hh"
#include "ReadServiceImpl.hh"
#include "WriteServiceImpl.hh"

using grpc::ServerBuilder;

grpcServer& grpcServer::set_ipv4(const string& ipv4) noexcept
{
  m_server_ipv4 = ipv4;
  return *this;
}

grpcServer& grpcServer::set_port(const string& port) noexcept
{
  m_server_port = port;
  return *this;
}

void grpcServer::start()
{
  grpc::EnableDefaultHealthCheckService(true);
  grpc::reflection::InitProtoReflectionServerBuilderPlugin();

  ServerBuilder builder;
  ReadServiceImpl read_service;
  WriteServiceImpl write_service;
  builder.RegisterService(&read_service).RegisterService(&write_service);

  auto server_addr = m_server_ipv4 + ":" + m_server_port;
  builder.AddListeningPort(server_addr, grpc::InsecureServerCredentials());
  m_server_core.reset(builder.BuildAndStart().release());
  m_server_running = true;
  m_server_core->Wait();
}
  
bool grpcServer::is_running() const noexcept
{
  return m_server_running;
}

void grpcServer::stop() noexcept
{
  m_server_core->Shutdown();
  m_server_running = false;
  m_server_core.reset();
}
