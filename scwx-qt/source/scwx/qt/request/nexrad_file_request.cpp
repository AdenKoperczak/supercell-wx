#include <scwx/qt/request/nexrad_file_request.hpp>

namespace scwx
{
namespace qt
{
namespace request
{

static const std::string logPrefix_ = "scwx::qt::request::nexrad_file_request";

class NexradFileRequest::Impl
{
public:
   explicit Impl(const std::string& currentRadarSite) :
       currentRadarSite_ {currentRadarSite}
   {
   }
   ~Impl() = default;

   const std::string currentRadarSite_;

   std::shared_ptr<types::RadarProductRecord> radarProductRecord_ {nullptr};
};

NexradFileRequest::NexradFileRequest(const std::string& currentRadarSite) :
    p(std::make_unique<Impl>(currentRadarSite))
{
}
NexradFileRequest::~NexradFileRequest() = default;

std::string NexradFileRequest::current_radar_site() const
{
   return p->currentRadarSite_;
}

std::shared_ptr<types::RadarProductRecord>
NexradFileRequest::radar_product_record() const
{
   return p->radarProductRecord_;
}

void NexradFileRequest::set_radar_product_record(
   const std::shared_ptr<types::RadarProductRecord>& record)
{
   p->radarProductRecord_ = record;
}

} // namespace request
} // namespace qt
} // namespace scwx
