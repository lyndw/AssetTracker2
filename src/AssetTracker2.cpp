// Derived heavily from AddaFruits AssetTracker library
// Modified by Lynd Wieman to work with AssetTracker 2 gps receiver
// At this writing I am not modifying the accelerator detection part, 
// I haven't tested the accerator detector yet.

#include "AssetTracker2.h"
#include "math.h"

//----------------- Tracker ----------------//

UbloxM8Q_GPS gps = UbloxM8Q_GPS();
Adafruit_LIS3DH accel = Adafruit_LIS3DH(A2);

uint8_t internalANT[]={0xB5,0x62,0x06,0x13,0x04,0x00,0x00,0x00,0xF0,0x7D,0x8A,0x2A};
uint8_t externalANT[]={0xB5,0x62,0x06,0x13,0x04,0x00,0x01,0x00,0xF0,0x7D,0x8B,0x2E};

AssetTracker2::AssetTracker2() {

}

void AssetTracker2::begin() {
    accel.begin(LIS3DH_DEFAULT_ADDRESS);

    // Default to 5kHz low-power sampling
    accel.setDataRate(LIS3DH_DATARATE_LOWPOWER_5KHZ);

    // Default to 4 gravities range
    accel.setRange(LIS3DH_RANGE_4_G);

}

float AssetTracker2::readLat() {
    return gps.latitude;
}

float AssetTracker2::readLon() {
    return gps.longitude;
}

float AssetTracker2::readLatDeg() {
    return gps.latitudeDegrees;
}

float AssetTracker2::readLonDeg() {
    return gps.longitudeDegrees;
}

float AssetTracker2::getGpsAccuracy() {
  return (float)gps.horzAcc;
}

float AssetTracker2::getGpsHeadingMot() {
  return (float)gps.headingMot;
}

float AssetTracker2::getGpsHeadingVeh() {
  return (float)gps.headingVeh;
}

bool AssetTracker2::getGpsHeadingVehValid() {
  return gps.headVehValid;
}

uint32_t AssetTracker2::getHaccuracy() {
  return gps.horzAcc;
}
uint32_t AssetTracker2::getVaccuracy() {
  return gps.vertAcc;
}
int32_t AssetTracker2::getVelN() {
  return gps.velN;
}
int32_t AssetTracker2::getVelE() {
  return gps.velE;
}
int32_t AssetTracker2::getVelD() {
  return gps.velD;
}
int32_t AssetTracker2::getOdoTrip() {
  return gps.odoTrip;
}
int32_t AssetTracker2::getOdoTotal() {
  return gps.odoTotal;
}
int32_t AssetTracker2::getGspeed() {
  return gps.gSpeed;
}

uint8_t AssetTracker2::getHour(){
  return gps.hour;
}

uint8_t AssetTracker2::getMinute(){
  return gps.minute;
}

uint8_t AssetTracker2::getSeconds(){
  return gps.seconds;
}

int32_t AssetTracker2::getMilliseconds(){
  return gps.milliseconds;
}

uint16_t AssetTracker2::getYear(){
  return gps.year;
}

uint8_t AssetTracker2::getMonth(){
  return gps.month;
}

uint8_t AssetTracker2::getDay(){
  return gps.day;
}

uint8_t AssetTracker2::getSatellites(){
  return gps.satellites;
}

uint8_t AssetTracker2::getFixQuality(){
  return gps.fixquality;
}

float AssetTracker2::getSpeed(){
  return gps.speed;
}

float AssetTracker2::getGeoIdHeight(){
  return gps.geoidheight;
}

float AssetTracker2::getAltitude(){
  return gps.altitude;
}

String AssetTracker2::readLatLon() {
    String latLon = String::format("%f,%f",gps.latitudeDegrees,gps.longitudeDegrees);
    return latLon;
}

void AssetTracker2::gpsOn() {
    enum ubxReturn err;
    // Power to the GPS is controlled by a FET connected to D6
    pinMode(D6,OUTPUT);
    digitalWrite(D6,LOW);
    Serial1.begin(9600);
    delay(10);
    err = ubxPENDING;
    while (err != ubxSUCCESS){
	if (err != ubxPENDING ){
	    Serial.print("gps.begin failed: ");
	    Serial.println(err);
	    return;
	}
	err=gps.begin();
    }
    Serial.println("GPS started");
}

void AssetTracker2::gpsOff() {
    digitalWrite(D6,HIGH);
}

bool AssetTracker2::gpsInitComplete() {
    return gps.initComplete;
}

bool AssetTracker2::gpsFix() {
    return gps.fix;
}

bool AssetTracker2::gpsDateValid() {
    return gps.dateValid;
}

bool AssetTracker2::gpsTimeValid() {
    return gps.timeValid;
}

bool AssetTracker2::antennaInternal(){

  for(uint8_t i=0;i<12;i++)
    {
        Serial1.write(internalANT[i]); //send the command to gps modul
    }
  return true;

}

bool AssetTracker2::antennaExternal(){

  for(uint8_t i=0;i<12;i++)
    {
        Serial1.write(externalANT[i]); //send the command to gps modul
    }
  return true;

}

void AssetTracker2::updateGPS() {
    gps.read();
}

void AssetTracker2::gpsRate(uint16_t measurement, int navigation) {
    gps.setRate(measurement, navigation);
}

void AssetTracker2::gpsResetODO(void) {
    gps.resetODO();
}

void AssetTracker2::gpsHoldThresh(int speed, uint16_t distance) {
    gps.setThresh(speed, distance);
}

int AssetTracker2::readX() {
    accel.read();
    return accel.x;
}

int AssetTracker2::readY() {
    accel.read();
    return accel.y;
}

int AssetTracker2::readZ() {
    accel.read();
    return accel.z;
}

// Read XYZ all at once instead of taking a new measurement between each
void AssetTracker2::readXYZ(int *x, int *y, int *z){
    accel.read();
    *x = accel.x;
    *y = accel.y;
    *z = accel.z;
}


int AssetTracker2::readXYZmagnitude() {
    accel.read();
    int magnitude = sqrt((accel.x*accel.x)+(accel.y*accel.y)+(accel.z*accel.z));
    return magnitude;
}

bool AssetTracker2::setupLowPowerWakeMode(uint8_t movementThreshold) {
	return accel.setupLowPowerWakeMode(movementThreshold);
}

uint8_t AssetTracker2::clearAccelInterrupt() {
	return accel.clearInterrupt();
}
