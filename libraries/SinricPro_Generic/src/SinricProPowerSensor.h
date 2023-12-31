/****************************************************************************************************************************
  SinricProPowerSensor.h - Sinric Pro Library for boards

  Based on and modified from SinricPro libarary (https://github.com/sinricpro/)
  to support other boards such as SAMD21, SAMD51, Adafruit's nRF52 boards, Teensy, SAM DUE, STM32, etc.

  Built by Khoi Hoang https://github.com/khoih-prog/SinricPro_Generic
  Licensed under MIT license 

  Copyright (c) 2019 Sinric. All rights reserved.
  Licensed under Creative Commons Attribution-Share Alike (CC BY-SA)

  This file is part of the Sinric Pro (https://github.com/sinricpro/)

  Version: 2.8.2
  
  Version Modified By   Date      Comments
  ------- -----------  ---------- -----------
  2.4.0   K Hoang      21/05/2020 Initial porting to support SAMD21, SAMD51 nRF52 boards, such as AdaFruit Itsy-Bitsy,
                                  Feather, Gemma, Trinket, Hallowing Metro M0/M4, NRF52840 Feather, Itsy-Bitsy, STM32, etc.
  2.5.1   K Hoang      02/08/2020 Add support to STM32F/L/H/G/WB/MP1. Add debug feature, examples. Restructure examples.
                                  Sync with SinricPro v2.5.1: add Speaker SelectInput, Camera. Enable Ethernetx lib support.
  2.6.1   K Hoang      15/08/2020 Sync with SinricPro v2.6.1: add AirQualitySensor, Camera Class.
  2.7.0   K Hoang      06/10/2020 Sync with SinricPro v2.7.0: Added AppKey, AppSecret and DeviceId classes and RTT function.
  2.7.4   K Hoang      12/11/2020 Sync with SinricPro v2.7.4. Add WIO Terminal support and examples
  2.8.0   K Hoang      10/12/2020 Sync with SinricPro v2.8.0. Add examples. Use std::queue instead of QueueList. SSL Option.
  2.8.1   K Hoang      02/06/2021 Add support to RP2040 using Arduino-mbed or arduino-pico core with WiFiNINA or Ethernet
  2.8.2   K Hoang      20/07/2021 Add support to WT32_ETH01 (ESP32 + LAN8720A)
 *****************************************************************************************************************************/

#ifndef _SINRIC_PRO_POWERSENSOR_H_
#define _SINRIC_PRO_POWERSENSOR_H_

#include "SinricProDevice.h"

/**
   @class SinricProPowerSensor
   @brief Device to report power usage
 *  */
class SinricProPowerSensor :  public SinricProDevice
{
  public:
    SinricProPowerSensor(const DeviceId &deviceId);
    
    // From v2.5.1
    String getProductType() 
    {
      return SinricProDevice::getProductType() + String("POWER_SENSOR"); 
    }
    //////
    
    // event
    bool sendPowerSensorEvent(float voltage, float current, float power = -1.0f, float apparentPower = -1.0f, float reactivePower = -1.0f, float factor = -1.0f, String cause = "PERIODIC_POLL");
    
  private:
    unsigned long startTime = 0;
    unsigned long lastPower = 0;
    
  protected:
    float getWattHours(unsigned long currentTimestamp);
};

SinricProPowerSensor::SinricProPowerSensor(const DeviceId &deviceId) : SinricProDevice(deviceId) {}

/**
   @brief Send PowerSensor event to SinricPro Server
   @param   voltage       voltage
   @param   current       current
   @param   power         (optional) if not provided, it is calculated automaticly (power = voltage * current)
   @param   apparentPower (optional) if not provided it is set to -1
   @param   reactivePower (optional) if not provided it is set to -1
   @param   factor        (optional) if not provided it is set to -1 \n if apparentPower is provided, factor is calculated automaticly (factor = power / apparentPower)
   @param   cause         (optional) Reason why event is sent (default = `"PERIODIC_POLL"`)
   @return  the success of sending the event
   @retval  true          event has been sent successfully
   @retval  false         event has not been sent, maybe you sent to much events in a short distance of time
 **/
bool SinricProPowerSensor::sendPowerSensorEvent(float voltage, float current, float power, float apparentPower, float reactivePower, float factor, String cause)
{
  DynamicJsonDocument eventMessage = prepareEvent(deviceId, "powerUsage", cause.c_str());
  JsonObject event_value = eventMessage["payload"]["value"];

  if (power == -1)
    power = voltage * current;

  if (apparentPower != -1)
    factor = power / apparentPower;

  unsigned long currentTimestamp = getTimestamp();

  event_value["startTime"]     = startTime;
  event_value["voltage"]       = voltage;
  event_value["current"]       = current;
  event_value["power"]         = power;
  event_value["apparentPower"] = apparentPower;
  event_value["reactivePower"] = reactivePower;
  event_value["factor"]        = factor;
  event_value["wattHours"]     = getWattHours(currentTimestamp);

  startTime = currentTimestamp;
  lastPower = power;
  
  return sendEvent(eventMessage);
}

float SinricProPowerSensor::getWattHours(unsigned long currentTimestamp)
{
  if (startTime)
    return (currentTimestamp - startTime) * lastPower / 3600.0f;

  return 0;
}

#endif    //_SINRIC_PRO_POWERSENSOR_H_
