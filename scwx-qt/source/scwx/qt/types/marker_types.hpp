#pragma once

#include <scwx/qt/types/texture_types.hpp>

#include <string>
#include <vector>
#include <cstdint>

namespace scwx
{
namespace qt
{
namespace types
{

struct MarkerInfo
{
   MarkerInfo(const std::string& name,
              const std::string& iconName,
              double             latitude,
              double             longitude) :
       name {name},
       latitude {latitude},
       longitude {longitude},
       iconName {iconName}
   {
   }

   std::string name;
   double      latitude;
   double      longitude;
   std::string iconName;
};


struct MarkerIconInfo {
   explicit MarkerIconInfo(types::ImageTexture texture,
                           std::int32_t        hotX,
                           std::int32_t        hotY) :
      name(types::GetTextureName(texture)),
      path(types::GetTexturePath(texture)),
      hotX(hotX),
      hotY(hotY)
   {
   }

   std::string name;
   std::string path;
   std::int32_t hotX;
   std::int32_t hotY;
};

const std::vector<MarkerIconInfo>& getMarkerIcons();

} // namespace types
} // namespace qt
} // namespace scwx
