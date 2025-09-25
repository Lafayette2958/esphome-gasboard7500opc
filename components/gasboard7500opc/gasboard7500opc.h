#pragma once

#include "esphome/core/component.h"
#include "esphome/components/uart/uart.h"
#include "esphome/components/sensor/sensor.h"

namespace esphome {
namespace gasboard7500opc {

class Gasboard7500OPCSensor : public Component, public uart::UARTDevice {
 public:
  sensor::Sensor *gas_flow = new sensor::Sensor();
  sensor::Sensor *gas_temperature = new sensor::Sensor();

  explicit Gasboard7500OPCSensor(uart::UARTComponent *parent) : uart::UARTDevice(parent) {}

  float get_setup_priority() const override { return setup_priority::BUS; }

  void setup() override;

  void loop() override;
};

}  // namespace gasboard7500opc
}  // namespace esphome
