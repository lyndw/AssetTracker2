// hold off connecting to cell/netowrk/cloud. See below.
SYSTEM_MODE(MANUAL)

#include "AssetTracker2.h"

int transmittingData = 1;

// Used to keep track of the last time we published data
long lastPublish = 0;

// Minutes between publishing
unsigned int delayMinutes = 1;

// Creating an AssetTracker named 't' for us to reference
AssetTracker2 t = AssetTracker2();

// A FuelGauge named 'fuel' for checking on the battery state
FuelGauge fuel;

String gpsPosition = "Not fixed";

// All the data in one string
// "lat, lon, acc, alt, spd, batV, bat%"
String buf;

int transmitMode(String command);
int gpsPublish(String command);
int batteryStatus(String command);

// Read the gps data from the timer interrupt. This avoids
// having to call the read routine in loop(). Calling from loop()
// is subject to delays by particle processing between calls
// to loop() and results in overrun errors from the gps.
// This method avoids that problem.
void callGPS(){
    t.updateGPS();
}
Timer timer(50, callGPS);

void setup() {
    // Opens up a Serial port so you can listen over USB
    Serial.begin(9600);

    // Wait to allow particle serial monitor to get connected
    delay(10000);

    // enabling gps while cell service is connecting doesn't work because
    // of conflict for cpu time.
    // Turn on gps then turn on cell service. Doing it in the reverse
    // order doesn't work

    // Enable the GPS module. 
    t.gpsOn();


    // These functions are useful for remote diagnostics. Read more below.
    // These can be registered before connecting
    Particle.function("tmode", transmitMode);
    Particle.function("batt", batteryStatus);

    Particle.variable("pos", &gpsPosition, STRING);

    Particle.connect();

    // You can also register functions after connection
    Particle.function("gpsRate", gpsRate);
    Particle.function("gpsPublish", gpsPublish);

    timer.start();
}

void loop() {
 
    // is it time to publish?
    if (millis() - lastPublish > delayMinutes*60*1000) {
        // Remember when we published
        lastPublish = millis();

        Serial.println(gpsPosition);

        // we should only publish data if there's a fix
        if (t.gpsFix()) {
            // Only publish if we're in transmittingData mode 1;
            if (transmittingData) {
                // "lat, lon, acc, alt, spd, batV, bat%"
                buf = String::format("%f, %f, +/-%fft, %f, %f, %f, %f",
                         t.readLatDeg(),
                         t.readLonDeg(),
                         (((float)t.getHaccuracy())/304.8), // integer mm to float feet
                         t.getAltitude()/304.8,             // mm to feet
                         t.getSpeed(),
                         fuel.getVCell(),
                         fuel.getSoC());


                Particle.publish("loc", buf, 60, PRIVATE);
                gpsPosition = buf;
            }
            // report the data over serial
            Serial.println(t.readLatLon());
        }
    }
    // Must call this explicitly when using MANUAL mode
    Particle.process();
}

// Allows you to remotely change whether a device is publishing to the cloud
// or is only reporting data over Serial. Saves data when using only Serial!
// Change the default at the top of the code.
int transmitMode(String command) {
    transmittingData = atoi(command);
    return 1;
}
// Allows changing the measurement rate
int gpsRate(String command) {
    uint16_t rate = atoi(command);
    int nav = atoi(command.substring(command.indexOf(' ')));
    Serial.print("rate: ");
    Serial.print(rate);
    Serial.print(" nav: ");
    Serial.println(nav);
    t.gpsRate(rate, nav);
    return 1;
}

// Actively ask for a GPS reading if you're impatient. Only publishes if there's
// a GPS fix, otherwise returns '0'
int gpsPublish(String command) {
    if (t.gpsFix()) {
        Particle.publish("G", t.readLatLon(), 60, PRIVATE);

        // uncomment next line if you want a manual publish to reset delay counter
        // lastPublish = millis();
        return 1;
    } else {
      return 0;
    }
}

// Lets you remotely check the battery status by calling the function "batt"
// Triggers a publish with the info (so subscribe or watch the dashboard)
// and also returns a '1' if there's >10% battery left and a '0' if below
int batteryStatus(String command){
    // Publish the battery voltage and percentage of battery remaining
    // if you want to be really efficient, just report one of these
    // the String::format("%f.2") part gives us a string to publish,
    // but with only 2 decimal points to save space
    Particle.publish("B",
          "v:" + String::format("%.2f",fuel.getVCell()) +
          ", %" + String::format("%.2f",fuel.getSoC()),
          60, PRIVATE
    );
    // if there's more than 10% of the battery left, then return 1
    if (fuel.getSoC()>10){ return 1;}
    // if you're running out of battery, return 0
    else { return 0;}
}

