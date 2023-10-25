#include "Read.hh"
#include "FFeatureConfig.hh"

using grpc::Status;
using grpc::ClientContext;

using fcore::ReadRequest;
using fcore::ReadResponse;

ReadService& ReadService::use_service()
{
  static ReadService instance;
  return instance;
}

string ReadService::read(const string&& giid)
{
  ReadRequest req;
  ReadResponse res;
  ClientContext cctx;
  req.set_giid(giid);
  Status s = m_gService->grpc_read(&cctx, req, &res);
  if (s.ok()) {
    return res.read_value();
  } else {
    return "";
  }
}

ReadService::ReadService()
{
  auto server_addr = string(FCore_SERVER_HOST) + ":" + string(FCore_SERVER_PORT);
  m_gChannel = grpc::CreateChannel(server_addr, grpc::InsecureChannelCredentials());
  m_gService.reset(Read::NewStub(m_gChannel).release());
}
