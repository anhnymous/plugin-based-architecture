#include "GIID_DB.hh"
#include "FCoreLogger.hh"
#include "ReadServiceImpl.hh"

Status ReadServiceImpl::grpc_read(ServerContext* context,
                                  const ReadRequest* req,
                                  ReadResponse* res)
{
  if (!context || !req || !res) {
    return Status(StatusCode::FAILED_PRECONDITION, "", "");
  }

  if (!req->has_giid()) {
    res->set_read_success(false);
    res->set_read_value("");
    return Status(StatusCode::FAILED_PRECONDITION, "Missing GIID", "");
  }

  auto giid = req->giid();
  LOG_INFO("FCore: ReadServiceImpl: giid = %s", giid.c_str());

  auto plugin = GIID_DB::getInstance().lookup(giid);
  if (plugin) {
    auto result = plugin->read(std::move(giid));
    res->set_giid(giid);
    res->set_read_success(true);
    res->set_read_value(result);
    return Status::OK;
  } else {
    LOG_WARNING("FCore: ReadServiceImpl: there is no Plugin support giid '%s'",
                 giid.c_str());
    res->set_giid(giid);
    res->set_read_success(false);
    res->set_read_value("");
    return Status::OK;
  }
}
