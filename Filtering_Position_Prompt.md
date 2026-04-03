Du Bist mein Technischer Berater und hilfst mir dabei die optimale technische lösung für mein Problem zufinden. 

Umgebung:
Ich baue einen Roboter. Dieser bewegt sich sehr schnell mit omni direktionalem antrieb auf einem bekanntem feld. Der Roboter ist nicht immer gleich ausgerichtet und dreht sich bis zu +- 90° hin und her (von 0 achse am Feldorientiert) Dieses Feld ist ca. 2m breit und 3 meter lang. Das feld hat Wände auf allen seiten und eine linie welche nicht überfahren werden darf auf jeder seite 10cm vor der Wand.
Die Start Position ist nicht allgemein bekannt. auch die Start Orientierung nicht. Diese sollte vor dem start auf Knopfdruck gemessen werden.

Sensoren : 
- "moussensor" ein sensor wie in einer komputer maus. Mit diesem kann sehr grob die aktuelle Geschwindigkeit sowie die bewegungsrichtung bestimmt werden. Der sensor hat eine Update frequenc von ungefähr 500Hz.

- Kammera: es gibt 2 sich gegenüberliegende Kameras mit blob Erkennung. die eine ist nach vorn ausgerichtet und die andere nach hinten. Beide haben einen Sichtwinkel von maximal 110°. Die Kammeras erkennen zwei farbige Rechtecke jeweils an beiden enden des feldes.  Die kammeras liefern ca. 20 FPS 

- Accelerometer: Ich nutze einen BNO055 als Accelerometer und als Gyroskop ( ca. alle 50Hz neue daten ) 

- 4 Ultraschallsensoren: in jede der 4 Richtungen habe ich auf dem Roboter einen SRF08 US Sensor. Diese liefern alle 10Hz neue daten. diese sind allerdings in der Entfernung sehr ungenau und haben starke Probleme sobald der messwinkel zur wand größer als +- 15° beträgt. 

- Liniensensoren der Roboter besitzt 32 farbsensoren an der Unterseite um die 10cm vor der wand verlaufende Linie zu erkennen. Diese sensoren werden auch mit ca. 50Hz abgefragt. 

Problemstellung:
Ich benötige eine exacte und vorallem schnelle positions Auskunft im betrieb. obwohl sich der Roboter mit mehr als 1.3m/s bewegen kann, darf er die Linie nicht überfahren deshalb muss immer genau wissen wo er ist und wie weit von der wand entfernt. Die sensoren haben alle sehr ungenaue oder langsamme Messdaten. ( Lidar darf ich leider nicht nutzen. ) Wie könnte ich die daten kombinieren bzw. Rechnerisch filter/kombinieren um eine absolute Position zu generieren.

was wären deine Lösungsansätze.