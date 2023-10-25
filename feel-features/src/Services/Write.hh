#ifndef WRITE_H_
#define WRITE_H_

#include <string>
#include <memory>

#include <grpcpp/grpcpp.h>
using grpc::Channel;

#include "Write.grpc.pb.h"
using fcore::Write;

using string = std::string;

class WriteService
{
public:
  static WriteService& use_service();
  bool write(const string&& giid, const string&& value);

  ~WriteService() = default;

  WriteService(WriteService&&) = delete;
  WriteService(const WriteService&) = delete;
  WriteService& operator=(WriteService&&) = delete;
  WriteService& operator=(const WriteService&) = delete;

private:
  WriteService();
  std::shared_ptr<Channel> m_gChannel{nullptr};
  std::unique_ptr<Write::Stub> m_gService{nullptr};
};

#endif /* WRITE_H_ */
