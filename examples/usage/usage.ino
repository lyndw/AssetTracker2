// Example usage for AssetTracker2 library by Lynd Wieman.
// Must initialize GPS before connecting to Particle cloud.
SYSTEM_MODE(MANUAL)

#include "AssetTracker2.h"

// Initialize objects from the lib
AssetTracker2 assetTracker2;

// Read the gps data from the timer interrupt.
void callGPS(){
    t.updateGPS();
}
Timer timer(50, callGPS);
void setup() {
    // Call functions on initialized library objects that require hardware
    assetTracker2.begin();
    assetTracker2.gpsOn();
    Particle.connect();
    timer.start();
}

void loop() {
    // Use the library's initialized objects and functions
    float lat = assetTracker2.readLatDeg();
    float lon = assetTracker2.readLonDeg();
    Particle.process();
}
