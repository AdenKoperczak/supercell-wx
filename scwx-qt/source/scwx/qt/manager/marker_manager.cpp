#include <scwx/qt/manager/marker_manager.hpp>
#include <scwx/qt/types/marker_types.hpp>
#include <scwx/qt/util/color.hpp>
#include <scwx/qt/util/json.hpp>
#include <scwx/qt/main/application.hpp>
#include <scwx/util/logger.hpp>

#include <filesystem>
#include <shared_mutex>
#include <vector>
#include <string>

#include <QStandardPaths>
#include <boost/json.hpp>
#include <boost/asio/post.hpp>
#include <boost/asio/thread_pool.hpp>

namespace scwx
{
namespace qt
{
namespace manager
{

static const std::string logPrefix_ = "scwx::qt::manager::marker_manager";
static const auto        logger_    = scwx::util::Logger::Create(logPrefix_);

static const std::string kNameName_      = "name";
static const std::string kIconName_      = "icon";
static const std::string kLatitudeName_  = "latitude";
static const std::string kLongitudeName_ = "longitude";
static const std::string kIconColorName_ = "icon-color";

class MarkerManager::Impl
{
public:
   class MarkerRecord;

   explicit Impl(MarkerManager* self) : self_ {self} {}
   ~Impl() { threadPool_.join(); }

   std::string                                markerSettingsPath_ {};
   std::vector<std::shared_ptr<MarkerRecord>> markerRecords_ {};

   MarkerManager* self_;

   boost::asio::thread_pool threadPool_ {1u};
   std::shared_mutex        markerRecordLock_ {};

   void                          InitializeMarkerSettings();
   void                          ReadMarkerSettings();
   void                          WriteMarkerSettings();
   std::shared_ptr<MarkerRecord> GetMarkerByName(const std::string& name);
};

class MarkerManager::Impl::MarkerRecord
{
public:
   MarkerRecord(const types::MarkerInfo& info) :
      markerInfo_ {info}
   {
   }

   const types::MarkerInfo& toMarkerInfo()
   {
      return markerInfo_;
   }

   types::MarkerInfo markerInfo_;

   friend void tag_invoke(boost::json::value_from_tag,
                          boost::json::value&                  jv,
                          const std::shared_ptr<MarkerRecord>& record) {
      jv = {{kNameName_, record->markerInfo_.name},
            {kIconName_, record->markerInfo_.iconName},
            {kLatitudeName_, record->markerInfo_.latitude},
            {kLongitudeName_, record->markerInfo_.longitude},
            {kIconColorName_,
             util::color::ToArgbString(record->markerInfo_.iconColor)}};
   }

   friend MarkerRecord tag_invoke(boost::json::value_to_tag<MarkerRecord>,
                                  const boost::json::value& jv)
   {
      static const std::string defaultIconName =
         types::getMarkerIcons()[0].name;
      static const std::string defaultIconColor = "#ffaaaaaa";
      const boost::json::object& jo = jv.as_object();

      std::string iconName;
      std::string iconColor;

      if (jo.contains(kIconName_) && jo.at(kIconName_).is_string())
      {
         iconName = boost::json::value_to<std::string>(jv.at(kIconName_));
      }
      else
      {
         iconName = defaultIconName;
      }

      if (jo.contains(kIconColorName_) && jo.at(kIconColorName_).is_string())
      {
         iconColor = boost::json::value_to<std::string>(jv.at(kIconColorName_));
      }
      else
      {
         iconColor = defaultIconColor;
      }

      return MarkerRecord(types::MarkerInfo(
         boost::json::value_to<std::string>(jv.at(kNameName_)),
         iconName,
         boost::json::value_to<double>(jv.at(kLatitudeName_)),
         boost::json::value_to<double>(jv.at(kLongitudeName_)),
         util::color::ToRgba8PixelT(iconColor)));
   }
};

void MarkerManager::Impl::InitializeMarkerSettings()
{
   std::string appDataPath {
      QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)
         .toStdString()};

   if (!std::filesystem::exists(appDataPath))
   {
      if (!std::filesystem::create_directories(appDataPath))
      {
         logger_->error("Unable to create application data directory: \"{}\"",
                        appDataPath);
      }
   }

   markerSettingsPath_ = appDataPath + "/location-markers.json";
}

void MarkerManager::Impl::ReadMarkerSettings()
{
   logger_->info("Reading location marker settings");

   boost::json::value markerJson = nullptr;
   {
      std::unique_lock lock(markerRecordLock_);

      // Determine if marker settings exists
      if (std::filesystem::exists(markerSettingsPath_))
      {
         markerJson = util::json::ReadJsonFile(markerSettingsPath_);
      }

      if (markerJson != nullptr && markerJson.is_array())
      {
         // For each marker entry
         auto& markerArray = markerJson.as_array();
         markerRecords_.reserve(markerArray.size());
         for (auto& markerEntry : markerArray)
         {
            try
            {
               MarkerRecord record =
                  boost::json::value_to<MarkerRecord>(markerEntry);

               if (!record.markerInfo_.name.empty())
               {
                  markerRecords_.emplace_back(
                     std::make_shared<MarkerRecord>(record.markerInfo_));
               }
            }
            catch (const std::exception& ex)
            {
               logger_->warn("Invalid location marker entry: {}", ex.what());
            }
         }

         logger_->debug("{} location marker entries", markerRecords_.size());
      }
   }

   Q_EMIT self_->MarkersUpdated();
}

void MarkerManager::Impl::WriteMarkerSettings()
{
   logger_->info("Saving location marker settings");

   std::shared_lock lock(markerRecordLock_);
   auto markerJson = boost::json::value_from(markerRecords_);
   util::json::WriteJsonFile(markerSettingsPath_, markerJson);
}

std::shared_ptr<MarkerManager::Impl::MarkerRecord>
MarkerManager::Impl::GetMarkerByName(const std::string& name)
{
   for (auto& markerRecord : markerRecords_)
   {
      if (markerRecord->markerInfo_.name == name)
      {
         return markerRecord;
      }
   }

   return nullptr;
}

MarkerManager::MarkerManager() : p(std::make_unique<Impl>(this))
{

   boost::asio::post(p->threadPool_,
                     [this]()
                     {
                        try
                        {
                           p->InitializeMarkerSettings();

                           // Read Marker settings on startup
                           main::Application::WaitForInitialization();
                           p->ReadMarkerSettings();

                           Q_EMIT MarkersInitialized(p->markerRecords_.size());
                        }
                        catch (const std::exception& ex)
                        {
                           logger_->error(ex.what());
                        }
                     });
}

MarkerManager::~MarkerManager()
{
   p->WriteMarkerSettings();
}

size_t MarkerManager::marker_count()
{
   return p->markerRecords_.size();
}

std::optional<types::MarkerInfo> MarkerManager::get_marker(size_t index)
{
   std::shared_lock lock(p->markerRecordLock_);
   if (index >= p->markerRecords_.size())
   {
      return {};
   }
   std::shared_ptr<MarkerManager::Impl::MarkerRecord>& markerRecord =
      p->markerRecords_[index];
   return markerRecord->toMarkerInfo();
}

void MarkerManager::set_marker(size_t index, const types::MarkerInfo& marker)
{
   {
      std::unique_lock lock(p->markerRecordLock_);
      if (index >= p->markerRecords_.size())
      {
         return;
      }
      std::shared_ptr<MarkerManager::Impl::MarkerRecord>& markerRecord =
         p->markerRecords_[index];
      markerRecord->markerInfo_ = marker;
   }
   Q_EMIT MarkerChanged(index);
   Q_EMIT MarkersUpdated();
}

void MarkerManager::add_marker(const types::MarkerInfo& marker)
{
   {
      std::unique_lock lock(p->markerRecordLock_);
      p->markerRecords_.emplace_back(std::make_shared<Impl::MarkerRecord>(marker));
   }
   Q_EMIT MarkerAdded();
   Q_EMIT MarkersUpdated();
}

void MarkerManager::remove_marker(size_t index)
{
   {
      std::unique_lock lock(p->markerRecordLock_);
      if (index >= p->markerRecords_.size())
      {
         return;
      }

      p->markerRecords_.erase(std::next(p->markerRecords_.begin(), index));
   }

   Q_EMIT MarkerRemoved(index);
   Q_EMIT MarkersUpdated();
}

void MarkerManager::move_marker(size_t from, size_t to)
{
   {
      std::unique_lock lock(p->markerRecordLock_);
      if (from >= p->markerRecords_.size() || to >= p->markerRecords_.size())
      {
         return;
      }
      std::shared_ptr<MarkerManager::Impl::MarkerRecord>& markerRecord =
         p->markerRecords_[from];

      if (from == to) {}
      else if (from < to)
      {
         for (size_t i = from; i < to; i++)
         {
            p->markerRecords_[i] = p->markerRecords_[i + 1];
         }
         p->markerRecords_[to] = markerRecord;
      }
      else
      {
         for (size_t i = from; i > to; i--)
         {
            p->markerRecords_[i] = p->markerRecords_[i - 1];
         }
         p->markerRecords_[to] = markerRecord;
      }
   }
   Q_EMIT MarkersUpdated();
}

std::shared_ptr<MarkerManager> MarkerManager::Instance()
{
   static std::weak_ptr<MarkerManager> markerManagerReference_ {};
   static std::mutex                   instanceMutex_ {};

   std::unique_lock lock(instanceMutex_);

   std::shared_ptr<MarkerManager> markerManager =
      markerManagerReference_.lock();

   if (markerManager == nullptr)
   {
      markerManager           = std::make_shared<MarkerManager>();
      markerManagerReference_ = markerManager;
   }

   return markerManager;
}

} // namespace manager
} // namespace qt
} // namespace scwx
