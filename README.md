# SDA-5708   Raspberry

Linux Treiber für den SDA-5708-24 Display von Siemens. Diese Anzeige hat 8 Zeichen, jedes Zeichen besteht aus 5x7 Punktmartizen. Datenblatt konnte nicht gefunden werden, die Information auf der Seite "http://www.sbprojects.com/knowledge/footprints/sda5708.php" stimmen. 

Der Font (font5x7.h) für die 5x7 Matrix ist aus der Webseite "http://sunge.awardspace.com/glcd-sd/node4.html" genommen worden.

## Pin Belegung

| SDA-5708        | Raspberry  | 
| ------------- |:-------------|
| 1 Vcc	|	2  Vcc |
| 2 Load	|	26 GPIO7 |
| 3 Data	|	24 GPIO8 |
| 4 SDCLK	|	12 GPIO18 |
| 5 RESET	|	16 GPIO23 |
| 6 GND	|	6  GND |
