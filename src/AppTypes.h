#ifndef APPTYPES_H
#define APPTYPES_H

#include <ShiftRegister74HC595.h>
// #include <FlowMeter.h>

// flow sensor
// FlowMeter *flowMeter;
// const FlowSensorProperties FHKCS_1mm_0deg = {0.4, 39.7, {1.0, 0.995, 0.9925, 0.992, 0.9925, 0.994, 0.995, 0.9975, 1.00125, 1.006}};



struct NTC
{
  float cold_ntc;
  float hot_ntc;
  float ntc3;
};

struct TempConfig
{
  float ChillerBucketMax;
  float ChillerBucketMin;
  int HotTankMax;
  int HotTankMin;
};

#endif