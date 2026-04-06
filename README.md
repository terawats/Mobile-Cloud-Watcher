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

## Usage
...
