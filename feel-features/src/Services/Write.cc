#include "Write.hh"
#include "FFeatureConfig.hh"

using grpc::Status;
using grpc::ClientContext;
using fcore::WriteRequest;
using fcore::WriteResponse;

WriteService& WriteService::use_service()
{
  static WriteService instance;
  return instance;
}

bool WriteService::write(const string&& giid, const string&& value)
{
  WriteRequest req;
  WriteResponse res;
  ClientContext cctx;
  req.set_giid(giid);
  req.set_write_value(value);
  Status s = m_gService->grpc_write(&cctx, req, &res);
  if (s.ok()) {
    return true;
  } else {
    return false;
  }
}

WriteService::WriteService()
{
  auto server_addr = string(FCore_SERVER_HOST) + ":" + string(FCore_SERVER_PORT);
  m_gChannel = grpc::CreateChannel(server_addr, grpc::InsecureChannelCredentials());
  m_gService.reset(Write::NewStub(m_gChannel).release());
}
