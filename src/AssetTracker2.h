// Derived heavily from AddaFruits AssetTracker library
// Modified by Lynd Wieman to work with AssetTracker 2 gps receiver
// At this writing I am not modifying the accelerator detection part
#ifndef AssetTracker2_h
#define AssetTracker2_h

#include "application.h"
#include "math.h"

#include "UbloxM8Q_GPS.h"
#include "Adafruit_LIS3DH.h"


class AssetTracker2 {

 public:

  AssetTracker2();

  void
    begin(void),
    updateGPS(void),
    gpsRate(uint16_t measurement, int navigation),
    gpsHoldThresh(int speed, uint16_t distance),
    gpsResetODO(void),
    gpsOn(void),
    gpsOff(void),
    readXYZ(int *x, int *y, int *z);
  int
    readX(void),
    readY(void),
    readZ(void),
    readXYZmagnitude(void);
  float
    readLat(void),
    readLon(void),
    readLatDeg(void),
    readLonDeg(void),
    getGpsAccuracy(void),
    getGpsHeadingMot(void),
    getGpsHeadingVeh(void),
    getSpeed(),
    getGeoIdHeight(),
    getAltitude();
  bool
    antennaInternal(void),
    antennaExternal(void),
    gpsInitComplete(void),
    gpsDateValid(void),
    gpsTimeValid(void),
    getGpsHeadingVehValid(void),
    gpsFix(void);
  char
    checkGPS(void);
  String
    readLatLon(void);
  bool
    setupLowPowerWakeMode(uint8_t movementThreshold = 16);
  uint8_t
    getHour(),
    getMinute(),
    getSeconds(),
    getMonth(),
    getDay(),
    getSatellites(),
    getFixQuality(),
    clearAccelInterrupt();
  uint16_t
    getYear();
  uint32_t
    getHaccuracy(),
    getVaccuracy();
  int32_t
    getVelN(),
    getVelE(),
    getVelD(),
    getMilliseconds(),
    getOdoTrip(),
    getOdoTotal(),
    getGspeed();

 private:

};

#endif // AssetTracker2_h
