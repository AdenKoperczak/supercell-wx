#include <scwx/qt/types/marker_types.hpp>

namespace scwx
{
namespace qt
{
namespace types
{

const std::vector<MarkerIconInfo>& getMarkerIcons()
{
   static std::vector<MarkerIconInfo> markerIcons = {};
   if (markerIcons.size() == 0)
   {
      markerIcons = {
         MarkerIconInfo(types::ImageTexture::LocationMarker, -1, -1),
         MarkerIconInfo(types::ImageTexture::LocationPinBlack, 8, 23),
         MarkerIconInfo(types::ImageTexture::LocationPinBlue, 8, 23),
         MarkerIconInfo(types::ImageTexture::LocationPinGreen, 8, 23),
         MarkerIconInfo(types::ImageTexture::LocationPinMagenta, 8, 23),
         MarkerIconInfo(types::ImageTexture::LocationPinOrange, 8, 23),
         MarkerIconInfo(types::ImageTexture::LocationPinPink, 8, 23),
         MarkerIconInfo(types::ImageTexture::LocationPinRed, 8, 23),
         MarkerIconInfo(types::ImageTexture::LocationPinWhite, 8, 23),
         MarkerIconInfo(types::ImageTexture::LocationPinYellow, 8, 23),
      };
   }

   return markerIcons;
}

} // namespace types
} // namespace qt
} // namespace scwx
