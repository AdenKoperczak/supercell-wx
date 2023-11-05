#include <scwx/qt/map/radar_range_layer.hpp>
#include <scwx/qt/types/layer_types.hpp>
#include <scwx/qt/util/geographic_lib.hpp>
#include <scwx/util/logger.hpp>

#include <glm/glm.hpp>

namespace scwx
{
namespace qt
{
namespace map
{

static const std::string logPrefix_ = "scwx::qt::map::radar_range_layer";
static const auto        logger_    = scwx::util::Logger::Create(logPrefix_);

static std::shared_ptr<QMapLibreGL::Feature>
GetRangeCircle(float range, QMapLibreGL::Coordinate center);

void RadarRangeLayer::Add(std::shared_ptr<QMapLibreGL::Map> map,
                          float                             range,
                          QMapLibreGL::Coordinate           center,
                          const QString&                    before)
{
   static const QString layerId = QString::fromStdString(types::GetLayerName(
      types::LayerType::Data, types::DataLayer::RadarRange));

   logger_->debug("Add()");

   if (map->layerExists(layerId))
   {
      map->removeLayer(layerId);
   }
   if (map->sourceExists("rangeCircleSource"))
   {
      map->removeSource("rangeCircleSource");
   }

   std::shared_ptr<QMapLibreGL::Feature> rangeCircle =
      GetRangeCircle(range, center);

   map->addSource(
      "rangeCircleSource",
      {{"type", "geojson"}, {"data", QVariant::fromValue(*rangeCircle)}});
   map->addLayer(
      {{"id", layerId}, {"type", "line"}, {"source", "rangeCircleSource"}},
      before);
   map->setPaintProperty(layerId, "line-color", "rgba(128, 128, 128, 128)");
}

void RadarRangeLayer::Update(std::shared_ptr<QMapLibreGL::Map> map,
                             float                             range,
                             QMapLibreGL::Coordinate           center)
{
   std::shared_ptr<QMapLibreGL::Feature> rangeCircle =
      GetRangeCircle(range, center);

   map->updateSource("rangeCircleSource",
                     {{"data", QVariant::fromValue(*rangeCircle)}});
}

static std::shared_ptr<QMapLibreGL::Feature>
GetRangeCircle(float range, QMapLibreGL::Coordinate center)
{
   const GeographicLib::Geodesic& geodesic(
      util::GeographicLib::DefaultGeodesic());

   constexpr float angleDelta  = 0.5f;
   constexpr float angleDeltaH = angleDelta / 2.0f;

   float angle = -angleDeltaH;

   QMapLibreGL::Coordinates geometry;

   for (uint16_t azimuth = 0; azimuth <= 720; ++azimuth)
   {
      double latitude;
      double longitude;

      geodesic.Direct(center.first,
                      center.second,
                      angle,
                      range * 1000.0f,
                      latitude,
                      longitude);

      geometry.append({latitude, longitude});

      angle += angleDelta;
   }

   std::shared_ptr<QMapLibreGL::Feature> rangeCircle =
      std::make_shared<QMapLibreGL::Feature>(
         QMapLibreGL::Feature::LineStringType,
         std::initializer_list<QMapLibreGL::CoordinatesCollection> {
            std::initializer_list<QMapLibreGL::Coordinates> {geometry}});

   return rangeCircle;
}

} // namespace map
} // namespace qt
} // namespace scwx
