#include "gasboard7500opc.h"
#include "esphome/core/log.h"

namespace esphome {
namespace gasboard7500opc {

static const char *const TAG = "gasboard7500opc";

void Gasboard7500OPCSensor::setup() {
  uint8_t command[] = {0x11, 0x01, 0x07, 0xE7};
  this->write_array(command, sizeof(command));
  ESP_LOGD(TAG, "Sent response mode toggle: %02X %02X %02X %02X", command[0], command[1], command[2], command[3]);
}

void Gasboard7500OPCSensor::loop() {
  while (available() >= 12) {
    if (peek() != 0x16) {
      read(); // skip invalid byte
      continue;
    }

    uint8_t data[12];
    for (int i = 0; i < 12; i++) {
      data[i] = read();
    }

    if (data[2] != 0x01) {
      ESP_LOGW(TAG, "Unknown CMD: 0x%02X", data[2]);
      continue;
    }

    uint8_t checksum = 0;
    for (int i = 0; i < 11; i++) {
      checksum += data[i];
    }
    checksum = 256 - checksum;

    if (data[11] != checksum) {
      ESP_LOGW(TAG, "Checksum mismatch: expected 0x%02X, got 0x%02X", checksum, data[11]);
      continue;
    }

    uint16_t flow_raw = (data[5] << 8) | data[6];
    uint16_t temp_raw = (data[7] << 8) | data[8];

    float flow = flow_raw / 100.0f;
    float temperature = temp_raw / 10.0f;

    gas_flow->publish_state(flow);
    gas_temperature->publish_state(temperature);

    ESP_LOGI(TAG, "Flow: %.2f L/min, Temp: %.1f °C", flow, temperature);
  }
}

}  // namespace gasboard7500opc
}  // namespace esphome
