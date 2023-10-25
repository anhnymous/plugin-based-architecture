#ifndef READSERVICEIMPL_H_
#define READSERVICEIMPL_H_

#include <grpcpp/grpcpp.h>
using grpc::Status;
using grpc::StatusCode;
using grpc::ServerContext;

#include "Read.grpc.pb.h"
using fcore::Read;
using fcore::ReadRequest;
using fcore::ReadResponse;

class ReadServiceImpl final: public Read::Service
{
public:
  ReadServiceImpl() = default;
  ~ReadServiceImpl() = default;

  ReadServiceImpl(ReadServiceImpl&&) = delete;
  ReadServiceImpl(const ReadServiceImpl&) = delete;
  ReadServiceImpl& operator=(ReadServiceImpl&&) = delete;
  ReadServiceImpl& operator=(const ReadServiceImpl&) = delete;

  Status grpc_read(ServerContext* context,
                   const ReadRequest* req,
                   ReadResponse* res) override;
};

#endif /* READSERVICEIMPL_H_ */
