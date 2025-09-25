import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import uart, sensor
from esphome.const import (
    UNIT_CELSIUS,
    ICON_THERMOMETER,
    ICON_WATER,
    DEVICE_CLASS_TEMPERATURE,
)


DEPENDENCIES = ["uart"]
AUTO_LOAD = ["sensor"]
CODEOWNERS = ["@Lafayette"]

gasboard7500opc_ns = cg.esphome_ns.namespace("gasboard7500opc")
Gasboard7500OPCSensor = gasboard7500opc_ns.class_(
    "Gasboard7500OPCSensor", cg.Component, uart.UARTDevice
)

CONFIG_SCHEMA = (
    cv.Schema(
        {
            cv.GenerateID(): cv.declare_id(Gasboard7500OPCSensor),
            cv.Required("gas_flow"): sensor.sensor_schema(
                unit_of_measurement="L/min",  # Use string, NOT constant
                icon=ICON_WATER,
                accuracy_decimals=2,
            ),
            cv.Required("gas_temperature"): sensor.sensor_schema(
                unit_of_measurement=UNIT_CELSIUS,
                icon=ICON_THERMOMETER,
                device_class=DEVICE_CLASS_TEMPERATURE,
                accuracy_decimals=1,
            ),
        }
    )
    .extend(uart.UART_DEVICE_SCHEMA)
    .extend(cv.COMPONENT_SCHEMA)
)


async def to_code(config):
    var = cg.new_Pvariable(config[cv.GenerateID()])
    await cg.register_component(var, config)
    await uart.register_uart_device(var, config)

    sens_flow = await sensor.new_sensor(config["gas_flow"])
    cg.add(var.gas_flow.set(sens_flow))

    sens_temp = await sensor.new_sensor(config["gas_temperature"])
    cg.add(var.gas_temperature.set(sens_temp))
