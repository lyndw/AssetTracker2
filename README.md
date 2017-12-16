# AssetTracker2

A Particle library for Asset Tracker 2. This library requires the UbloxM8Q_GPS 
driver for the GPS receiver used in Asset Tracker 2, and the libraries
Adadfruit_LIS3DH and Adafruit_Sensor which were required for the first Asset
Tracker hardware also.


## Usage

Connect the Particle Asset Tracker 2 hardware, add the AssetTracker2,
the UbloxM8Q_GPS, the Adafruit_LIS3SDH, and the Adafruit_Sensor libraries
to your project and follow the example of one of the examples provided with
this library. 
Only the 0_GPS_Features and 1_GPS_Features examples were modified. The other
examples are unchanged from what was included with the original AssetTracker
library.


See the [examples](examples) folder for more details.

## Documentation

When I first used the AssetTracker 2 hardware I tried the AssetTracker
and Adafruit_GPS drivers which were written for a different GPS receiver.
Although they worked to parse the NMEA data that the ublox receiver
generates by default, they used the wrong protocol for configuration and
so all commands were ignored.
Also, to get the most benefit from the ublox receiver the ubx protocol must
be used. So, I wrote this driver.
Rather than support two protocols and suffer with the complexities involved
I only implemented the ubx protocol. When the receiver is initialized
the NMEA (ASCII) sentences are turned off and ubx protocol messages are
enabled.

The ublox driver was written to be as compatible as possible with the
AssetTracker driver but the AssetTracker library had dependencies on the 
non-ublox GPS receiver that was used for AssetTracker 1.
I have created the AssetTracker2 library to remove the dependencies, but
the AssetTracker2 is now inextricably depedendent on the ublox driver.

The external interface presented by AssetTracker and AssetTracker2 are 
similar but access methods that were specific to NMEA have been removed.
 
There is a timing problem on startup that is related to trying to initialize
the GPS receiver while the cell, network, and cloud connections are being
initialized by the particle system software. The symptom is the GPS
receiver doesn't get configured correctly and so the driver never 
receives ubx messages immediately after initialization. 

You can avoid the startup problem by starting in SYSTEM_MODE(MANUAL),
then call Particle.connect() after the GPS receiver is running. See
examples 0 and 1 for details.

This version of the drivers runs the gps access off a timer interrupt 
instead of calling it repeatedly in loop(). This works much better without 
any lost data.

Boiler plate from Particle that I didn't remove:
## Contributing

Here's how you can make changes to this library and eventually contribute those changes back.

To get started, [clone the library from GitHub to your local machine](https://help.github.com/articles/cloning-a-repository/).

Change the name of the library in `library.properties` to something different. You can add your name at then end.

Modify the sources in <src> and <examples> with the new behavior.

To compile an example, use `particle compile examples/usage` command in [Particle CLI](https://docs.particle.io/guide/tools-and-features/cli#update-your-device-remotely) or use our [Desktop IDE](https://docs.particle.io/guide/tools-and-features/dev/#compiling-code).

After your changes are done you can upload them with `particle library upload` or `Upload` command in the IDE. This will create a private (only visible by you) library that you can use in other projects. Do `particle library add AssetTracker2_myname` to add the library to a project on your machine or add the AssetTracker2_myname library to a project on the Web IDE or Desktop IDE.

At this point, you can create a [GitHub pull request](https://help.github.com/articles/about-pull-requests/) with your changes to the original library. 

If you wish to make your library public, use `particle library publish` or `Publish` command.

## LICENSE
This software was mostly written by someone else. I make no claim to license
or copyright.
