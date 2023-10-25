#include "GIID_DB.hh"
#include "FCoreLogger.hh"
#include "WriteServiceImpl.hh"

Status WriteServiceImpl::grpc_write(ServerContext* context,
                                    const WriteRequest* req,
                                    WriteResponse* res)
{
  if (!context || !req || !res) {
    return Status(StatusCode::FAILED_PRECONDITION, "", "");
  }

  if (!req->has_giid()) {
    res->set_write_success(false);
    return Status(StatusCode::FAILED_PRECONDITION, "Missing GIID", "");
  }

  if (!req->has_write_value()) {
    res->set_write_success(false);
    return Status(StatusCode::FAILED_PRECONDITION, "Missing Write value", "");
  }

  auto giid = req->giid();
  auto value = req->write_value();
  LOG_INFO("FCore: WriteServiceImpl: giid = %s, write value = %s",
            giid.c_str(), value.c_str());

  auto plugin = GIID_DB::getInstance().lookup(giid);
  if (plugin) {
    auto result = plugin->write(std::move(giid), std::move(value));
    res->set_write_success(result);
    res->set_giid(giid);
    return Status::OK;
  } else {
    LOG_WARNING("FCore: WriteServiceImpl: there is no Plugin support giid '%s'",
                 giid.c_str());
    res->set_giid(giid);
    res->set_write_success(false);
    return Status::OK;
  }
}

















