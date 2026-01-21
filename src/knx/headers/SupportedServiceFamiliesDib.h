#pragma once

#include "knx/headers/KnxStructure.h"
#include "knx/bytes/ByteBufferReader.h"
#include <cstdint>

enum class ServiceFamilyType : std::uint8_t {
  knxIpCore = 0x02,
  knxNetDeviceMgmt = 0x03,
  knxIpTunneling = 0x04,
  knxIpRouting = 0x05,
  knxIpRemoteDiagAndConfig = 0x07,
  unknown = 0xFE,
};

struct ServiceFamily {
  ServiceFamilyType type;
  std::uint8_t version;
  ServiceFamily() = default;
  ServiceFamily(ServiceFamilyType type, std::uint8_t version) :
    type{type}, version{version}{}
  [[nodiscard]] std::string toString() const;
};

class SupportedServiceFamiliesDib final : public KnxStructure {
public:
   static constexpr std::uint8_t TYPE = 0x02;
public:
  SupportedServiceFamiliesDib();
  explicit SupportedServiceFamiliesDib(std::vector<ServiceFamily> serviceFamilies);
  [[nodiscard]] std::vector<ServiceFamily> getServiceFamilies() const;

  static SupportedServiceFamiliesDib parse(ByteBufferReader& reader);
  void write(ByteBufferWriter& writer) const override;

private:
  std::vector<ServiceFamily> serviceFamilies;
};
