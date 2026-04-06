# DIY Mobile Cloud watcher with RF433MHz remote alarm
![CloudWatcherSet](https://github.com/user-attachments/assets/d34b2a57-2c2c-4f97-a8b2-dea6be46ad90)
DIY Mobile Cloud watcher with RF433MHz remote alarm for astrophotographer 

## Lists of parts
Below is a lists of parts that are required by this project
| Items | Pictures | Details |
|------|------|-----|
| 1 | ![GY-906_MLX90614](https://github.com/user-attachments/assets/95e363bb-6645-4dce-9e59-8d72497c69e3) | IR Temperature sensor |
| 2 | ![ESP8266_with_18650_Battery-icon](https://github.com/user-attachments/assets/a86374e9-8ee1-4d51-a8a7-cf7b6455ac45) | ESP8266 with 18650 battery charger |
| 3 | ![RF433MHzDoorBell-icon](https://github.com/user-attachments/assets/9bbca971-e711-450b-a4a5-8abdde4c6816) | RF 433MHz remote door bell (3.3V battery only)|
| 4 | ![switch](https://github.com/user-attachments/assets/92e87060-ca88-429c-9ac8-df3b1fe13c8d) | Switch for power On/Off |
| 5 | ![18650_Battery](https://github.com/user-attachments/assets/f2965fbd-e28a-4e48-964b-2e0bf1440d3b) | 18650 Battery|
| 6 | ![CloudWatcher_inside-icon](https://github.com/user-attachments/assets/7b7591fc-2f43-46f2-b602-e12f63171c98) | IR Temperature sensor |

## ESP8266 Analog input maximiun voltage modification
According to origibal A0 alalog input volated are design for 1.0V maximiun input, but we have needed to use for battery voltage moniter.
Then we have to change valus of original volatge divider resister from 100k to 56k as below schemetic.
![A0-mod](https://github.com/user-attachments/assets/b0ff055c-1296-44a2-a3c5-86b07ead31c6)

## We have to connect battery voltage to A0 input for battery voltage moniter.
Battery voltage voltage and percentage are monitoring by web access.
![ESP8266_A0_mod](https://github.com/user-attachments/assets/bf9b5e88-36b0-4f76-bb80-58d8fdfb9905)

## Completed Wiring Daiagram
![Schemetic diagram](https://github.com/user-attachments/assets/d4fd0669-c84f-4a84-a33f-31c33b1997dc)
### Wiring on A-side
![CloudWatcher_wiringA](https://github.com/user-attachments/assets/f856bf69-9d6d-4594-870c-1352c64c966d)

### Wiring on B-side
![CloudWatcher_wiringB](https://github.com/user-attachments/assets/4da3fdb8-354b-493e-b644-db7ac3d76c97)

### Micro USB battery charger port
18650 battery can be charged by micro USB and charging status are indicated by LED color status.
Green : Charging 
![CloudWatcher_chargerPort](https://github.com/user-attachments/assets/c100f1e9-61e3-47fd-a40a-946203ff59bf)

## Web access and alarm setup 
Put Mobile Cloud watcher unit on a location that sensor can see a clear sky then turn on power switch and connect to wifi SSID as below

SSID     : "Cloud-Watcher-Panel"

Password : "12345678"

After wifi connected, open browser and access to http://192.168.0.1 for web monitor/setup.
Below is a sample screen, you can see Sky temperature that reflect to amount of cloud and current battery level.
Fully charged 2000mhr battery can be used for 2-3 night monitoring.
After 3-5min monitoring, sky temperature chart will be show a history of sky temperare.
Please setup sky teperature limit line for trigger an alarm level by 1-2 degree from history chart.
![web_monitor](https://github.com/user-attachments/assets/71d54016-1bb4-41a3-bcac-a0b7b7389865)

When cloudy happen Mobile Cloud watcher unit will send a RF433Mhz to trigger a door bell alarm to receiver.
(Without cloud, you can test alarm trigger by put your hand over sensor for 2-3sec.)
Cloudy will be happen befor actual rain come and safe for your expensive astrophotography equipment.
![web_monitor_ticked](https://github.com/user-attachments/assets/d4af455b-b24c-4f38-b49d-0e8d922caa72)
Let enjoy and clear sky.

