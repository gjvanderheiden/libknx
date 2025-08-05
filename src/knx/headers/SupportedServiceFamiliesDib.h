#pragma once

#include "knx/headers/KnxStructure.h"
#include "knx/bytes/ByteBufferReader.h"
#include <cstdint>


enum class ServiceFamilyType : std::uint8_t {
  KnxIpCore = 0x02,
  KnxNetDeviceMgmt = 0x03,
  KnxIpTunneling = 0x04,
  KnxIpRouting = 0x05,
  KnxIpRemoteDiagAndConfig = 0x07,
  Unknown = 0xFE,
};

struct ServiceFamily {
  ServiceFamilyType type;
  std::uint8_t version;
  ServiceFamily() = default;
  ServiceFamily(ServiceFamilyType type, std::uint8_t version) :
    type{type}, version{version}{}
  std::string toString() const;
};

class SupportedServiceFamiliesDib final : public KnxStructure {
public:
   static constexpr std::uint8_t TYPE = 0x02;
public:
  SupportedServiceFamiliesDib();
  SupportedServiceFamiliesDib(std::vector<ServiceFamily> serviceFamilies);
  std::vector<ServiceFamily> getServiceFamilies() const;

  static SupportedServiceFamiliesDib parse(ByteBufferReader& reader);
  void write(ByteBufferWriter& writer) const override;

private:
  std::vector<ServiceFamily> serviceFamilies;
};
