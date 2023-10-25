#ifndef WRITESERVICEIMPL_H_
#define WRITESERVICEIMPL_H_

#include <grpcpp/grpcpp.h>
using grpc::Status;
using grpc::StatusCode;
using grpc::ServerContext;

#include "Write.grpc.pb.h"
using fcore::Write;
using fcore::WriteRequest;
using fcore::WriteResponse;

class WriteServiceImpl final: public Write::Service
{
public:
  WriteServiceImpl() = default;
  ~WriteServiceImpl() = default;

  WriteServiceImpl(WriteServiceImpl&&) = delete;
  WriteServiceImpl(const WriteServiceImpl&) = delete;
  WriteServiceImpl& operator=(WriteServiceImpl&&) = delete;
  WriteServiceImpl& operator=(const WriteServiceImpl&) = delete;

  Status grpc_write(ServerContext* context,
                    const WriteRequest* req,
                    WriteResponse* res) override;
};

#endif /* WRITESERVICEIMPL_H_ */
