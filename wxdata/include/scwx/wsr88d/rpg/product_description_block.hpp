#pragma once

#include <scwx/awips/message.hpp>

#include <cstdint>
#include <memory>

namespace scwx
{
namespace wsr88d
{
namespace rpg
{

class ProductDescriptionBlockImpl;

class ProductDescriptionBlock : public awips::Message
{
public:
   explicit ProductDescriptionBlock();
   ~ProductDescriptionBlock();

   ProductDescriptionBlock(const ProductDescriptionBlock&) = delete;
   ProductDescriptionBlock& operator=(const ProductDescriptionBlock&) = delete;

   ProductDescriptionBlock(ProductDescriptionBlock&&) noexcept;
   ProductDescriptionBlock& operator=(ProductDescriptionBlock&&) noexcept;

   int16_t  block_divider() const;
   float    latitude_of_radar() const;
   float    longitude_of_radar() const;
   int16_t  height_of_radar() const;
   int16_t  product_code() const;
   uint16_t operational_mode() const;
   uint16_t volume_coverage_pattern() const;
   int16_t  sequence_number() const;
   uint16_t volume_scan_number() const;
   uint16_t volume_scan_date() const;
   uint32_t volume_scan_start_time() const;
   uint16_t generation_date_of_product() const;
   uint32_t generation_time_of_product() const;
   uint16_t elevation_number() const;
   uint8_t  version() const;
   uint8_t  spot_blank() const;
   uint32_t offset_to_symbology() const;
   uint32_t offset_to_graphic() const;
   uint32_t offset_to_tabular() const;

   float    range() const;
   uint16_t range_raw() const;
   float    x_resolution() const;
   uint16_t x_resolution_raw() const;
   float    y_resolution() const;
   uint16_t y_resolution_raw() const;

   bool IsCompressionEnabled() const;

   size_t data_size() const override;

   bool Parse(std::istream& is) override;

   static constexpr size_t SIZE = 102u;

private:
   std::unique_ptr<ProductDescriptionBlockImpl> p;
};

} // namespace rpg
} // namespace wsr88d
} // namespace scwx
