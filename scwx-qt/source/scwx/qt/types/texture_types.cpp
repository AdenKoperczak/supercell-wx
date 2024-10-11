#include <scwx/qt/types/texture_types.hpp>

#include <unordered_map>

namespace scwx
{
namespace qt
{
namespace types
{

struct TextureInfo
{
   std::string name_ {};
   std::string path_ {};
};

static const std::unordered_map<ImageTexture, TextureInfo> imageTextureInfo_ {
   {ImageTexture::CardinalPoint24,
    {"images/cardinal-point-24", ":/res/icons/flaticon/cardinal-point-24.png"}},
   {ImageTexture::Compass24,
    {"images/compass-24", ":/res/icons/flaticon/compass-24.png"}},
   {ImageTexture::Crosshairs24,
    {"images/crosshairs-24", ":/res/textures/images/crosshairs-24.png"}},
   {ImageTexture::Cursor17,
    {"images/cursor-17", ":/res/textures/images/cursor-17.png"}},
   {ImageTexture::Dot3, {"images/dot-3", ":/res/textures/images/dot-3.png"}},
   {ImageTexture::LocationMarker,
    {"images/location-marker", ":/res/textures/images/location-marker.svg"}},
   {ImageTexture::LocationPinBlack,
    {"images/location-pin-black",
     ":/res/icons/font-awesome-6/location-pin-black.svg"}},
   {ImageTexture::LocationPinBlue,
    {"images/location-pin-blue",
     ":/res/icons/font-awesome-6/location-pin-blue.svg"}},
   {ImageTexture::LocationPinGreen,
    {"images/location-pin-green",
     ":/res/icons/font-awesome-6/location-pin-green.svg"}},
   {ImageTexture::LocationPinMagenta,
    {"images/location-pin-magenta",
     ":/res/icons/font-awesome-6/location-pin-magenta.svg"}},
   {ImageTexture::LocationPinOrange,
    {"images/location-pin-orange",
     ":/res/icons/font-awesome-6/location-pin-orange.svg"}},
   {ImageTexture::LocationPinPink,
    {"images/location-pin-pink",
     ":/res/icons/font-awesome-6/location-pin-pink.svg"}},
   {ImageTexture::LocationPinRed,
    {"images/location-pin-red",
     ":/res/icons/font-awesome-6/location-pin-red.svg"}},
   {ImageTexture::LocationPinWhite,
    {"images/location-pin-white",
     ":/res/icons/font-awesome-6/location-pin-white.svg"}},
   {ImageTexture::LocationPinYellow,
    {"images/location-pin-yellow",
     ":/res/icons/font-awesome-6/location-pin-yellow.svg"}},
   {ImageTexture::MapboxLogo,
    {"images/mapbox-logo", ":/res/textures/images/mapbox-logo.svg"}},
   {ImageTexture::MapTilerLogo,
    {"images/maptiler-logo", ":/res/textures/images/maptiler-logo.svg"}}};

static const std::unordered_map<LineTexture, TextureInfo> lineTextureInfo_ {
   {LineTexture::Default1x7,
    {"lines/default-1x7", ":/res/textures/lines/default-1x7.png"}},
   {LineTexture::TestPattern,
    {"lines/test-pattern", ":/res/textures/lines/test-pattern.png"}}};

const std::string& GetTextureName(ImageTexture imageTexture)
{
   return imageTextureInfo_.at(imageTexture).name_;
}

const std::string& GetTextureName(LineTexture lineTexture)
{
   return lineTextureInfo_.at(lineTexture).name_;
}

const std::string& GetTexturePath(ImageTexture imageTexture)
{
   return imageTextureInfo_.at(imageTexture).path_;
}

const std::string& GetTexturePath(LineTexture lineTexture)
{
   return lineTextureInfo_.at(lineTexture).path_;
}

} // namespace types
} // namespace qt
} // namespace scwx
