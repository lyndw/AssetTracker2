// hold off connecting to cell/netowrk/cloud. See below.
SYSTEM_MODE(MANUAL)

#include "AssetTracker2.h"


// Creating an AssetTracker named 't' for us to reference
AssetTracker2 t = AssetTracker2();

// A FuelGauge for checking on the battery state
FuelGauge fuel;

String gpsPosition = "Not fixed";

String buf; // for publishing values


int gpsPublish(String command);
int batteryStatus(String command);
int showAll(String command);
int postValue(String cmd);

// Run the GPS off a timer interrupt.
// read all bytes available, if an entire message was received,
// parse it store the data for access by the get routines.
void callGPS() {
     t.updateGPS();
}
// The period is based on the baud rate of the serial port
// connected to the gps.
Timer timer(50, callGPS);


void setup() {
    // Opens up a Serial port so you can listen over USB
    Serial.begin(9600);

    // These should be registered before connecting
    Particle.variable("pos", &gpsPosition, STRING);

    Particle.function("batt", batteryStatus);
    Particle.function("gpsRate", gpsRate);
    Particle.function("resetODO", resetODO);
    Particle.function("gpsPublish", gpsPublish);
    Particle.function("showAll", showAll);
    Particle.function("postValue", postValue);

    // Wait to allow particle serial monitor to get connected
    // This lets you see the version info from the ublox receiver
    delay(10000);

    // enabling gps while cell service is connecting doesn't work because
    // of conflict for cpu time.
    // Turn on gps then turn on cell service. Doing it in the reverse
    // order doesn't work

    // Enable the GPS module. 
    t.gpsOn();

    // connect to the cloud
    Particle.connect();

    // Start reading from the gps
    timer.start();
}

unsigned long lastPublish = 0;
unsigned long delaymillis = 10000;

void loop() {
    unsigned long now;
    now = millis();
    if ((now - lastPublish) > delaymillis ){
	gpsPublish("dummy");
	Serial.println(t.readLatLon());
	lastPublish = millis();
    }
    Particle.process();
}

// Reset the trip odometer
int resetODO(String command) {
    t.gpsResetODO();
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

// Publish Latitude and Longitude
int gpsPublish(String command) {
    Particle.publish("G", t.readLatLon(), 60, PRIVATE);

    return 1;
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

// This shows how to access all the (current) values but isn't
// too useful otherwise since the Asset Tracker 2 hardware can't
// see satellites with the serial cable connected.
// It is useful to run this with the serial cable connetected to see
// the names of the values you can read, then disconnect the serial 
// connection, put the gps out where it can see satellites. You can 
// then call the postValue routine passing it the name of one of these 
// vales.
// For details on all values see the ublox spec:
// https://www.u-blox.com/sites/default/files/products/documents/u-blox8-M8_ReceiverDescrProtSpec_(UBX-13003221)_Public.pdf
int showAll(String cmd)
{

    bool initComplete = t.gpsInitComplete();
    uint8_t hour = t.getHour(); // Time is UTC
    uint8_t minute = t.getMinute();
    uint8_t seconds = t.getSeconds();
    uint8_t month = t.getMonth();
    uint8_t day = t.getDay();
    uint16_t year = t.getYear();
    uint16_t milliseconds = t.getMilliseconds(); // Since last measurement
    float latitude = t.readLat(); // degrees
    float longitude = t.readLon(); // degrees
    float latitudeDegrees = t.readLatDeg(); // degrees
    float longitudeDegrees = t.readLonDeg(); // degrees
    float geoidheight = t.getGeoIdHeight(); // Height above ellipsoid, mm
    float altitude = t.getAltitude(); // Height above mean sea level, mm
    float speed = t.getSpeed();  // m/s
    uint8_t fixquality = t.getFixQuality(); // 0:no fix,1:dead rec,2:2d,3:3d,4:gnss+dead rec,5:time only
    uint8_t satellites = t.getSatellites(); // how many
    uint32_t horzAcc = t.getHaccuracy(); // estimate, mm
    uint32_t vertAcc = t.getVaccuracy(); // estimate, mm
    int32_t velN = t.getVelN(); // North velocity, mm/s
    int32_t velE = t.getVelE(); // East velocity, mm/s
    int32_t velD = t.getVelD(); // Down velocity, mm/s
    int32_t gSpeed = t.getGspeed(); // Ground speed, mm/s
    int32_t odoTrip = t.getOdoTrip(); // m
    int32_t odoTotal = t.getOdoTotal(); // m



    Serial.print("initComplete: ");
    Serial.println(initComplete);
    Serial.print("hour: ");
    Serial.println(hour);
    Serial.print("minute: ");
    Serial.println(minute);
    Serial.print("seconds: ");
    Serial.println(seconds);
    Serial.print("month: ");
    Serial.println(month);
    Serial.print("day: ");
    Serial.println(day);
    Serial.print("year: ");
    Serial.println(year);
    Serial.print("milliseconds: ");
    Serial.println(milliseconds);
    Serial.print("latitude: ");
    Serial.println(latitude);
    Serial.print("longitude: ");
    Serial.println(longitude);
    Serial.print("latitudeDegrees: ");
    Serial.println(latitudeDegrees);
    Serial.print("longitudeDegrees: ");
    Serial.println(longitudeDegrees);
    Serial.print("geoidheight: ");
    Serial.println(geoidheight);
    Serial.print("altitude: ");
    Serial.println(altitude);
    Serial.print("speed: ");
    Serial.println(speed);
    Serial.print("fixquality: ");
    Serial.println(fixquality);
    Serial.print("satellites: ");
    Serial.println(satellites);
    Serial.print("horzAcc: ");
    Serial.println(horzAcc);
    Serial.print("vertAcc: ");
    Serial.println(vertAcc);
    Serial.print("velN: ");
    Serial.println(velN);
    Serial.print("velE: ");
    Serial.println(velE);
    Serial.print("velD: ");
    Serial.println(velD);
    Serial.print("gSpeed: ");
    Serial.println(gSpeed);
    Serial.print("odoTrip: ");
    Serial.println(odoTrip);
    Serial.print("odoTotal: ");
    Serial.println(odoTotal);

    return 1;
}

// Type the name of the value you want to see into the console function
// argument and it will be published. 
// See showAll() above for names of values and units.
int postValue(String cmd)
{

    bool initComplete = t.gpsInitComplete();
    uint8_t hour = t.getHour();
    uint8_t minute = t.getMinute();
    uint8_t seconds = t.getSeconds();
    uint8_t month = t.getMonth();
    uint8_t day = t.getDay();
    uint16_t year = t.getYear();
    uint16_t milliseconds = t.getMilliseconds();
    float latitude = t.readLat();
    float longitude = t.readLon();
    float latitudeDegrees = t.readLatDeg();
    float longitudeDegrees = t.readLonDeg();
    float geoidheight = t.getGeoIdHeight();
    float altitude = t.getAltitude();
    float speed = t.getSpeed();
    uint8_t fixquality = t.getFixQuality();
    uint8_t satellites = t.getSatellites();
    uint32_t horzAcc = t.getHaccuracy();
    uint32_t vertAcc = t.getVaccuracy();
    int32_t velN = t.getVelN();
    uint32_t velE = t.getVelE();
    uint32_t velD = t.getVelD();
    int32_t gSpeed = t.getGspeed();
    int32_t odoTrip = t.getOdoTrip();
    int32_t odoTotal = t.getOdoTotal();




    // This implementation is a bit crude but it was easy to implement
    // and simple to read.
    if (cmd == "initComplete"){
        buf = String::format("%d", initComplete);
    } else if (cmd == "hour"){
        buf = String::format("%d", hour);
    } else if (cmd == "minute"){
        buf = String::format("%d", minute);
    } else if (cmd == "seconds"){
        buf = String::format("%d", seconds);
    } else if (cmd == "month"){
        buf = String::format("%d", month);
    } else if (cmd == "day"){
        buf = String::format("%d", day);
    } else if (cmd == "year"){
        buf = String::format("%d", year);
    } else if (cmd == "milliseconds"){
        buf = String::format("%d", milliseconds);
    } else if (cmd == "latitude"){
        buf = String::format("%f", latitude);
    } else if (cmd == "longitude"){
        buf = String::format("%f", longitude);
    } else if (cmd == "latitudeDegrees"){
        buf = String::format("%f", latitudeDegrees);
    } else if (cmd == "longitudeDegrees"){
        buf = String::format("%f", longitudeDegrees);
    } else if (cmd == "geoidheight"){
        buf = String::format("%f", geoidheight);
    } else if (cmd == "altitude"){
        buf = String::format("%f", altitude);
    } else if (cmd == "speed"){
        buf = String::format("%f", speed);
    } else if (cmd == "fixquality"){
        buf = String::format("%d", fixquality);
    } else if (cmd == "satellites"){
        buf = String::format("%d", satellites);
    } else if (cmd == "horzAcc"){
        buf = String::format("%lu", horzAcc);
    } else if (cmd == "vertAcc"){
        buf = String::format("%lu", vertAcc);
    } else if (cmd == "velN"){
        buf = String::format("%ld", velN);
    } else if (cmd == "velE"){
        buf = String::format("%ld", velE);
    } else if (cmd == "velD"){
        buf = String::format("%ld", velD);
    } else if (cmd == "gSpeed"){
        buf = String::format("%lu", gSpeed);
    } else if (cmd == "odoTrip"){
        buf = String::format("%lu", odoTrip);
    } else if (cmd == "odoTotal"){
        buf = String::format("%lu", odoTotal);
    } else {
        buf = "Don't know that one. Check spelling.";
    }
    Particle.publish("V", buf, 60, PRIVATE);
    

    return 1;
}
