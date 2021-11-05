#include <scwx/common/products.hpp>

#include <unordered_map>

namespace scwx
{
namespace common
{

static const std::unordered_map<Level2Product, std::string> level2Name_ {
   {Level2Product::Reflectivity, "REF"},
   {Level2Product::Velocity, "VEL"},
   {Level2Product::SpectrumWidth, "SW"},
   {Level2Product::DifferentialReflectivity, "ZDR"},
   {Level2Product::DifferentialPhase, "PHI"},
   {Level2Product::CorrelationCoefficient, "RHO"},
   {Level2Product::ClutterFilterPowerRemoved, "CFP"},
   {Level2Product::Unknown, "?"}};

static const std::unordered_map<Level2Product, std::string> level2Description_ {
   {Level2Product::Reflectivity, "Reflectivity"},
   {Level2Product::Velocity, "Velocity"},
   {Level2Product::SpectrumWidth, "Spectrum Width"},
   {Level2Product::DifferentialReflectivity, "Differential Reflectivity"},
   {Level2Product::DifferentialPhase, "Differential Phase"},
   {Level2Product::CorrelationCoefficient, "Correlation Coefficient"},
   {Level2Product::ClutterFilterPowerRemoved, "Clutter Filter Power Removed"},
   {Level2Product::Unknown, "?"}};

const std::string& GetLevel2Name(Level2Product product)
{
   return level2Name_.at(product);
}

const std::string& GetLevel2Description(Level2Product product)
{
   return level2Description_.at(product);
}

const Level2Product GetLevel2Product(const std::string& name)
{
   auto result = std::find_if(
      level2Name_.cbegin(),
      level2Name_.cend(),
      [&](const std::pair<Level2Product, std::string>& pair) -> bool {
         return pair.second == name;
      });

   if (result != level2Name_.cend())
   {
      return result->first;
   }
   else
   {
      return Level2Product::Unknown;
   }
}

} // namespace common
} // namespace scwx
