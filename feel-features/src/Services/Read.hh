#ifndef READ_H_
#define READ_H_

#include <string>
#include <memory>

#include <grpcpp/grpcpp.h>
using grpc::Channel;

#include "Read.grpc.pb.h"
using fcore::Read;

using string = std::string;

class ReadService
{
public:
  static ReadService& use_service();
  string read(const string&& giid);

  ~ReadService() = default;

  ReadService(ReadService&&) = delete;
  ReadService(const ReadService&) = delete;
  ReadService& operator=(ReadService&&) = delete;
  ReadService& operator=(const ReadService&) = delete;

private:
  ReadService();
  std::shared_ptr<Channel> m_gChannel{nullptr};
  std::unique_ptr<Read::Stub> m_gService{nullptr};
};

#endif /* READ_H_ */
