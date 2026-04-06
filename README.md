# DIY Mobile Cloud Watcher with RF433MHz Remote Alarm
![CloudWatcherSet](https://github.com/user-attachments/assets/d34b2a57-2c2c-4f97-a8b2-dea6be46ad90)
A DIY Mobile Cloud Watcher with an RF433MHz remote alarm designed specifically for astrophotographers.

## Background
Thermal radiation from the ground is reflected back to Earth by clouds. By using an IR sky temperature sensor, we can estimate cloud cover based on temperature fluctuations. This project uses this concept to trigger an alarm before the rain actually arrives, giving you time to protect your equipment.

## Bill of Materials
Below are the parts required for this project:
| Items | Pictures | Details |
|------|------|-----|
| 1 | ![GY-906_MLX90614](https://github.com/user-attachments/assets/95e363bb-6645-4dce-9e59-8d72497c69e3) | IR Temperature sensor |
| 2 | ![ESP8266_with_18650_Battery-icon](https://github.com/user-attachments/assets/a86374e9-8ee1-4d51-a8a7-cf7b6455ac45) | ESP8266 with 18650 battery charger |
| 3 | ![RF433MHzDoorBell-icon](https://github.com/user-attachments/assets/9bbca971-e711-450b-a4a5-8abdde4c6816) | RF 433MHz remote door bell (3.3V battery only)|
| 4 | ![switch](https://github.com/user-attachments/assets/92e87060-ca88-429c-9ac8-df3b1fe13c8d) | Switch for power On/Off |
| 5 | ![18650_Battery](https://github.com/user-attachments/assets/f2965fbd-e28a-4e48-964b-2e0bf1440d3b) | 18650 Battery|
| 6 | ![CloudWatcher_inside-icon](https://github.com/user-attachments/assets/7b7591fc-2f43-46f2-b602-e12f63171c98) | IR Temperature sensor |

## Hardware Modification: ESP8266 Analog Input
The original ESP8266 A0 analog input is designed for a 1.0V maximum. To monitor the battery voltage (up to 5.0V), we need to modify the voltage divider.

### Change the original voltage divider resistor from 100kΩ to 56kΩ as shown in the schematic below.
![A0-mod](https://github.com/user-attachments/assets/b0ff055c-1296-44a2-a3c5-86b07ead31c6)

## Battery Monitoring
Connect the battery voltage to the A0 input to enable monitoring. You can check the real-time battery voltage and percentage via the web interface.
![ESP8266_A0_mod](https://github.com/user-attachments/assets/bf9b5e88-36b0-4f76-bb80-58d8fdfb9905)

## Wiring Diagrams
![Schemetic diagram](https://github.com/user-attachments/assets/d4fd0669-c84f-4a84-a33f-31c33b1997dc)

## Assembly Reference
### Wiring on A-side
![CloudWatcher_wiringA](https://github.com/user-attachments/assets/f856bf69-9d6d-4594-870c-1352c64c966d)

### Wiring on B-side
![CloudWatcher_wiringB](https://github.com/user-attachments/assets/4da3fdb8-354b-493e-b644-db7ac3d76c97)

### Battery Charging Port
The 18650 battery can be charged via the micro USB port. The charging status is indicated by the LED.

Green : Charging
![CloudWatcher_chargerPort](https://github.com/user-attachments/assets/c100f1e9-61e3-47fd-a40a-946203ff59bf)

## Configuration & Web Interface
1. Place the Cloud Watcher in an open area with a clear view of the sky.

2. Turn on the power and connect your device to the following WiFi:

      SSID     : "Cloud-Watcher-Panel"

      Password : "12345678"

3. Open your web browser and navigate to: http://192.168.0.1

### Monitoring & Setup
The web interface displays the current sky temperature and battery level. A fully charged 2000mAh battery typically lasts for 2-3 nights of continuous monitoring.

  * Sky Temperature Chart: After 3-5 minutes, the chart will display the temperature history.

  * Alarm Threshold: Set your temperature limit approximately 1-2 degrees higher than the baseline shown in the history chart to trigger the alarm.
![web_monitor_ticked](https://github.com/user-attachments/assets/d4af455b-b24c-4f38-b49d-0e8d922caa72)

### Alarm Triggering
When cloud cover is detected, the unit sends an RF433MHz signal to the doorbell receiver.

  * Testing: You can test the alarm by placing your hand over the sensor for 2-3 seconds.

  * Safety: Cloud detection typically occurs well before rain starts, providing a crucial early warning to save your expensive astrophotography gear.
![web_monitor](https://github.com/user-attachments/assets/71d54016-1bb4-41a3-bcac-a0b7b7389865)


Let enjoy and clear sky.

