#include "SupportedServiceFamiliesDib.h"
#include "KnxStructure.h"
#include <cstdint>
#include <iostream>

SupportedServiceFamiliesDib::SupportedServiceFamiliesDib()
    : KnxStructure(SupportedServiceFamiliesDib::TYPE), serviceFamilies{} {}

SupportedServiceFamiliesDib::SupportedServiceFamiliesDib(
    std::vector<ServiceFamily> serviceFamilies)
    : KnxStructure(SupportedServiceFamiliesDib::TYPE),
      serviceFamilies{serviceFamilies} {}

std::string ServiceFamily::toString() const {
  std::string result{};
  switch (type) {
  case ServiceFamilyType::KnxIpCore:
    result = "Knx Net ip Core";
    break;
  case ServiceFamilyType::KnxNetDeviceMgmt:
    result = "Knx Net ip device mgt";
    break;
  case ServiceFamilyType::KnxIpTunneling:
    result = "Knx Net ip tunneling";
    break;
  case ServiceFamilyType::KnxIpRouting:
    result = "Knx Net ip routing";
    break;
  case ServiceFamilyType::KnxIpRemoteDiagAndConfig:
    result = "Knx Net ip remote diag and config";
    break;
  case ServiceFamilyType::Unknown:
    result = "Unknown Type";
    break;
  }
  result += ", version: " + std::to_string(version);
  return result;
}

static ServiceFamilyType toEnum(std::uint8_t value) {
  switch (value) {
  case static_cast<std::uint8_t>(ServiceFamilyType::KnxIpCore):
    return ServiceFamilyType::KnxIpCore;
  case static_cast<std::uint8_t>(ServiceFamilyType::KnxIpRouting):
    return ServiceFamilyType::KnxIpRouting;
  case static_cast<std::uint8_t>(ServiceFamilyType::KnxIpTunneling):
    return ServiceFamilyType::KnxIpTunneling;
  case static_cast<std::uint8_t>(ServiceFamilyType::KnxNetDeviceMgmt):
    return ServiceFamilyType::KnxNetDeviceMgmt;
  case static_cast<std::uint8_t>(ServiceFamilyType::KnxIpRemoteDiagAndConfig):
    return ServiceFamilyType::KnxIpRemoteDiagAndConfig;
  default:
    return ServiceFamilyType::Unknown;
  }
}

void SupportedServiceFamiliesDib::write(ByteBufferWriter &writer) const {}

SupportedServiceFamiliesDib
SupportedServiceFamiliesDib::parse(ByteBufferReader &reader) {
  auto [length, type] = KnxStructure::parse(reader);
  SupportedServiceFamiliesDib result{};
  auto size = length / 2;
  result.serviceFamilies.reserve(size);
  for (int i = 0; i < size; i++) {
    auto serviceType = toEnum(reader.readUint8());
    auto version = reader.readUint8();
    result.serviceFamilies.emplace_back(serviceType, version);
  }
  return result;
}

std::vector<ServiceFamily>
SupportedServiceFamiliesDib::getServiceFamilies() const {
  return serviceFamilies;
}
