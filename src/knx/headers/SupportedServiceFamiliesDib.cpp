#include "SupportedServiceFamiliesDib.h"
#include "KnxStructure.h"
#include <cstdint>
#include <utility>

SupportedServiceFamiliesDib::SupportedServiceFamiliesDib()
    : KnxStructure(SupportedServiceFamiliesDib::TYPE) {}

SupportedServiceFamiliesDib::SupportedServiceFamiliesDib(
    std::vector<ServiceFamily> serviceFamilies)
    : KnxStructure(SupportedServiceFamiliesDib::TYPE),
      serviceFamilies{std::move(serviceFamilies)} {}

std::string ServiceFamily::toString() const {
  std::string result{};
  switch (type) {
  case ServiceFamilyType::knxIpCore:
    result = "Knx Net ip Core";
    break;
  case ServiceFamilyType::knxNetDeviceMgmt:
    result = "Knx Net ip device mgt";
    break;
  case ServiceFamilyType::knxIpTunneling:
    result = "Knx Net ip tunneling";
    break;
  case ServiceFamilyType::knxIpRouting:
    result = "Knx Net ip routing";
    break;
  case ServiceFamilyType::knxIpRemoteDiagAndConfig:
    result = "Knx Net ip remote diag and config";
    break;
  case ServiceFamilyType::unknown:
    result = "Unknown Type";
    break;
  }
  result += ", version: " + std::to_string(version);
  return result;
}
namespace {
ServiceFamilyType toEnum(std::uint8_t value) {
  switch (value) {
  case static_cast<std::uint8_t>(ServiceFamilyType::knxIpCore):
    return ServiceFamilyType::knxIpCore;
  case static_cast<std::uint8_t>(ServiceFamilyType::knxIpRouting):
    return ServiceFamilyType::knxIpRouting;
  case static_cast<std::uint8_t>(ServiceFamilyType::knxIpTunneling):
    return ServiceFamilyType::knxIpTunneling;
  case static_cast<std::uint8_t>(ServiceFamilyType::knxNetDeviceMgmt):
    return ServiceFamilyType::knxNetDeviceMgmt;
  case static_cast<std::uint8_t>(ServiceFamilyType::knxIpRemoteDiagAndConfig):
    return ServiceFamilyType::knxIpRemoteDiagAndConfig;
  default:
    return ServiceFamilyType::unknown;
  }
}
} // namespace


SupportedServiceFamiliesDib
SupportedServiceFamiliesDib::parse(ByteBufferReader &reader) {
  auto [length, type] = KnxStructure::parse(reader);
  SupportedServiceFamiliesDib result{};
  auto size = (length - 1) / 2;
  result.serviceFamilies.reserve(size);
  for (int i = 0; i < size; i++) {
    auto serviceType = toEnum(reader.readUint8());
    auto version = reader.readUint8();
    result.serviceFamilies.emplace_back(serviceType, version);
  }
  return result;
}

void SupportedServiceFamiliesDib::write(ByteBufferWriter &writer) const {
  writeKnxStructure(writer, (serviceFamilies.size() * 2) + 2);
  for(auto const &serviceFamily : serviceFamilies) {
    writer.writeUint8(static_cast<std::uint8_t>(serviceFamily.type));
    writer.writeUint8(serviceFamily.version);
  }
}

std::vector<ServiceFamily>
SupportedServiceFamiliesDib::getServiceFamilies() const {
  return serviceFamilies;
}
