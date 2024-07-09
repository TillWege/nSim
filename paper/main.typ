#import "template.typ": *

#show: project.with(
  title: "Simulation von Gravitation im Sonnensystem",
  subtitle: "Hochschule Rhein-Waal\nFakultät Kommunikation und Umwelt\nMethoden und Werkzeuge der modernen Astronomie",
  authors: (
    "Till Wegener (28891)",
  ),
  date: "9.July 2024",
)

#set heading(numbering: "1.1")
#show outline.entry.where(
  level: 1
): it => {
  v(10pt, weak: true)
  it
}

#v(20em)

#align(center)[#heading("Abstract", numbering: none, outlined: false)]

#block([Die Bewegung der Sterne und Planeten fasziniert die Menschheit schon seit Urzeiten. In diesem Paper wird eine Methode zur Simulation der Gravitation im Sonnensystem vorgestellt, welche physikalisch akkurate Daten verwendet, um die Bewegung der Planeten zu berechnen. Die Simulation wird in C++ implementiert und visualisiert. ])

Schlagwörter: _Simulation_, _Gravitation_, _Sonnensystem_, _C++_, _Visualisierung_

#pagebreak()

#outline(indent: 5%, fill: line(length: 100%, stroke: 0.5pt + rgb("#aaa")))

#pagebreak()

#set page(numbering: "1", number-align: center, columns: 2)


= Einleitung
Die Planeten in unserem Sonnensystem folgen wie alle anderen Objekt im Universum den physikalischen Grundgesetzen. Für die Bewegungen der Planeten ist primär die Gravitationskraft verantwortlich. Durch das verstehen dieser Kraft ist es uns möglich, die Bewegungen der Planeten zu simulieren. 

#block()

Dieses Paper verwendet diese Grundgesetze um eine Simulation zu entwickeln, welche die Bewegungen der Planeten im Sonnensystem berechnet. 

= Historischer Kontext

Im laufe der Zeit haben viele Physiker und Astronomen sich mit der Bewegung der Planeten beschäftigt. Über die Jahrhunderte wurden viele Modelle entwickelt welche nah am heutigen Stand der Wissenschaft sind. Im folgenden werden einige dieser Modelle vorgestellt.


#block([
  1. Im 16ten Jahrhundert entwickelte Nikolaus Kopernikus mit _Über die Umlaufbahnen der Himmelssphären_ @kopernicus das erste Modell, welches den heutigen Erkenntnissen nahe kommt. Er beschrieb die Bewegung der Planeten dadurch, dass alle Planeten aufeinander Gravitation ausüben.
])

#block([
  2. Zu beginn des 17ten Jahrhunderts entwickelte Johannes Kepler mit _Astronomia nova_ @kepler ein Modell, welches die Bewegung der Planeten genauer beschrieb. Seine Axiome besagten, dass die Planeten sich auf Ellipsen bewegen, und kKörper sich gegenseitig anziehen.
])

#block([
  3. 1644 entwickelte Isac Newton mit _Philosophiæ Naturalis Principia Mathematica_ @newton das Modell, welches bis heute zur Beschreibung der Gravitation verwendet wird. Seine Gravitationsgesetze beschreiben, wie sich Körper aufeinander anziehen.
])

= Mathematische Grundlagen

Wie durch Newton beschreiben, wirkt auf jeden Körper eine Gravitationskraft, welche durch die Masse der Körper und den Abstand zwischen den Körpern bestimmt wird. Die Gravitationskraft zwischen zwei Körpern kann durch die Formel

$ F_G = G * (m_1 * m_2)/r^2 $

beschrieben werden. Hierbei ist $F_G$ die resultierende Gravitationskraft, $G$ die Gravitationskonstante ($6.67*10^(-11)m^3/("kg"*s^2)$), $m_1$ und $m_2$ die Massen der Körper und $r$ der Abstand zwischen den Körpern. Die Kraft wirkt hierbei entlang der Verbindungslinie der Körper.

#linebreak()

Für ein 2-Körper-System kann die Bewegung der Körper durch diese Formel bestimmt werden. Die Gravitationskraft welche die Körper aufeinander ausüben, kann mit der Masse dieser verwendet werden, um die Beschleunigung der Körper mit $F = m * a$ zu bestimmen.

Für 2-Körper-Systeme ist es möglich, numerische Lösungen für die Bewegung der Körper zu finden.

#linebreak()

In mehr-Körper-Systemen wird die Bewegung der Körper durch die Summe der einzelnen wirkenden Gravitationskräfte bestimmt. Für Systeme mit 3 oder mehr Körpern ist es nicht möglich, analytische Lösungen zu finden. Wenn wir hierzu Lösungen finden wollen, müssen wir numerische Methoden verwenden.

Im folgenden wird eine Methode vorgestellt, welche Eulers Methode verwendet, um die Bewegung der Planeten im Sonnensystem zu simulieren. 

#linebreak()

Eulers Methode ist eine numerische Methode, welche einen Differentialgleichungssystem durch diskrete Schritte approximiert. Die Methode ist einfach zu implementieren, kann jedoch aufgrund ihrer funktionsweise ungenau sein.
Als erstes bestimmen wir unseren diskreten Zeitschritt $Delta t$. Danach bestimmen die Startwerte für die Position und Geschwindigkeit der Körper im Raum. 

#linebreak()

Nun können wir für alle $n$ Körper die Gravitationskräfte berechnen, welche auf die Körper wirken. In diesem Fall ist die Kraft $arrow(F_G)$ ein Vektor, welcher die Richtung und Stärke der Kraft angibt. Mit dieser Kraft können wir die Beschleunigung der Körper bestimmen. Mit $ arrow(a) = arrow(F_G) / m$ und $ arrow(v) = arrow(v) + arrow(a) * Delta t$ können wir nun die angepassten Geschwindigkeiten der Körper bestimmen. Die Geschwindigkeit ist in diesem Fall ein 3-Dimensionaler Vektor, welcher die Geschwindigkeit in $x$, $y$ und $z$ Richtung angibt. Für einen Körper kann nun die neue Position $arrow(r)$ im Raum mit $ arrow(r) = arrow(r) + arrow(v) * Delta t$ bestimmt werden.

#linebreak()

Wenn wir dieses Verfahren nun für alle Körper im System durchführen, können wir die Bewegung der Körper simulieren. Dieses Verfahren von hand durchzuführen ist sehr aufwendig, weshalb sich die implementation als Computerprogramm anbietet.

= Vorbereitung

== Datensätze <dataset>
Für die Simulation des Sonnensystems benötigen wir Daten über die Planeten und ihre diversen Satelliten. Die wichtigen Daten für die Simulation sind die Masse der Körper, die Position der Körper im Raum und die Geschwindigkeit der Körper. Diese Daten können aus diversen Quellen bezogen werden. Für die Planeten wurden die Daten aus dem Projekt _Nasa Data Scraper_ von _Devstronomy_ @planets auf Github verwendet. Das gleiche Projekt beinhaltet auch einen Datensatz für die Satelliten der Planeten. Dieser ist allerdings für unsere Simulation nicht geeignet, da keine Informationen zu Position und Geschwindigkeit der Satelliten enthalten sind.

#linebreak()

Die Daten der Satelliten wurden vom _Solar System Dynamics_ Projekt des _Jet Propulsion Laboratory_ der _NASA_ @ssd verwendet. Vom _SSD_ Projekt können wir sowohl die Physikalischen Daten der Satelliten @satellite_phys, als auch die orbitale Parameter der Satelliten @satellite_orb beziehen. Diese Daten beinhalten die _Semi Major Axis_ und weitere Parameter, welche für die Simulation benötigt werden.

== Wahl der Programmiersprache
Da für diese Simulation eine große Anzahl an Berechnungen durchgeführt werden muss, stellte die Wahl der Programmiersprache eine wichtige Entscheidung dar. Für diese Simulation wurde deshalb C++ gewählt. C++ ist eine schnelle kompilierte Sprache. 

== Wahl von Hilfsbibliotheken
Zur Visualisierung unserer Simulation benötigen wir eine Bibliothek, welche das darstellen von 3D-Objekten ermöglicht. Hierzu wurde die Bibliothek _Raylib_ @Raylib verwendet. Raylib ist eine C-Bibliothek, welche das erstellen von 2D und 3D Anwendungen ermöglicht. Neben der Darstellung von 3D-Objekten war es auch noch wichtig, dass wir einfache Interface-Elemente anzeigen können. Hierzu wird die Bibliothek _Dear ImGui_ @Imgui verwendet. ImGui ist eine C++ Bibliothek, welche in der Spiele-entwicklung verwendet wird, um einfache Interface-Elemente anzuzeigen. Um diese beiden Bibliotheken zusammen zu verwenden, wurde das Projekt _rlImGui_ @Rlimgui verwendet.

Die Bibliotheken _ImGui_ und _rlImGui_ wurden als Submodule in das Projekt eingebunden. _Raylib_ wird während der Kompilierung des Projektes heruntergeladen und verwendet. Hierzu wurde das Programm _Cmake_ @cmake verwendet. Cmake ist ein Build-System, welches es ermöglicht, C und C++ Projekte plattformunabhängig zu kompilieren.

= Implementierung

== Grundlegendes

Die Implementierung der Simulation folgt dem zuvor beschriebenen Ablauf. In der _Main_-Funktion der Simulation werden zuerst die nötigen Bibliotheken initialisiert. Für dieses Projekt war es nötig, einen eigenen _Vektor_-Datentypen zu verwenden. Die Gründe hierzu, werden in @32bit erläutert. Dieser _SciVec3_ verwendet 64-Bit gleitkomma-Zahlen, um möglichst präzise Berechnungen zu ermöglichen. Der Datentyp und die dazugehörigen Funktionen zur Addition, Subtraktion, Multiplikation und Skalierung können in _SciVec3.h_ gefunden werden. Die Körper des Sonnensystems werden mithilfe eines Datentypen _Body_ abgebildet. In _Body.h_ wird das _struct_ _Body_ wie folgt definiert:

#figure([```cpp
struct Body
{
  std::string name;
  double radius;
	double mass;
	Color color{};
	SciVec3 position;
  SciVec3 velocity;
	bool isPlanet;

	int satelliteCount = 0;
	std::string satelliteNames[64] = {""};

	...
}
```], caption: "Definition des _Body_-Datentyps")
Die Felder _radius_, _mass_, _postion_, und _velocity_ sind für die Simulation am wichtigsten. Für jedes Objekt wird außerdem ein _Name_, eine _Farbe_, und eine Liste an Satelliten gespeichert. Die Simulation verwaltet eine dynamische Liste (_std::vector_) an _Body_-Objekten. Diese Liste stellt den aktuellen zustand der Simulation da. Um diese Liste nun mit Daten zu füllen, müssen wir unsere Datensätze einladen. 

== Laden der Datensätze

Die Datei _Loader.h_ beinhaltet die Funktionen, welche die Datensätze aus @dataset lädt. Die Funktion _loadPlanets_ lädt die _planets.csv_-Datei aus dem _Assets_-Ordner. Zuerst erstellt diese Funktion allerdings die Sonne, welche nicht im Datensatz enthalten ist. Hierfür wurden die wichtigsten Werte in _Consts.h_ definiert. Danach iteriert die Funktion über die Zeilen der CSV-Datei und erstellt für jede Zeile ein _Body_-Objekt. Hierbei ist es wichtig, die einzelnen Werte in die Korrekten Einheiten umzuwandeln. Für diese Simulation wurde die Entscheidung getroffen, alle Werte in SI-Einheiten zu speichern. Insbesondere für die Datenwerte _Mass_ war es nötig, 64-Bit Gleitkommazahlen zu verwenden. Um die Visualisierung besser zu gestalten, wurden allen Planeten eine individuelle Farbe zugeordnet. 

Neben der bereits genannten Daten, war die Bestimmung der initialen Position und Geschwindigkeit der Körper eine wichtige Entscheidung. Der Datensatz der Planeten beinhaltet sowohl Werte für den _Aphelion_, den _Perihelion_, die _Inclination_ und die _Exzentrizität_ der Planeten. Des Weiteren wurde ein Wert für die durchschnittliche _orbital_velocity_ angegeben.

// TODO Richtig implmentieren und dann beschreiben

#block()

Die Funktion _loadSatallites_ funktioniert ähnlich wie _loadPlanets_. Hierbei wird die Dateien _satellites_data.csv_ und _satellites_orbit.csv_ kombiniert, um die Satelliten der Planeten zu erstellen. Für jeden Satellit wird zuerst der _Parent_-Körper gesucht. Danach werden die weiteren Planetaren Informationen bestimmt. Dieser Datensatz weißt die Besonderheit auf, dass nicht die Masse der Objekte angegeben ist, sondern der _Standard Gravitational Parameter_ $mu$. Dieser Wert ist definiert als $mu = G * m$, wobei $G$ die Gravitationskonstante ist. Um die Masse der Satelliten zu bestimmen, wird der Wert $mu$ durch $G$ geteilt. 

Die Bestimmung der initialen Position und Geschwindigkeit der Satelliten ist etwas komplizierter. Hierzu müssen wir nicht nur die Orbitalen Parameter des Satelliten, sondern auch die des Parent-Körpers verwenden.

// TODO Richtig implmentieren und dann beschreiben

== Aufteilung von Simulation und Visualisierung
Die Simulation und Visualisierung sind zwei verschiedene Aufgaben mit unterschiedlichen Zielen. Die Visualisierung in geregelten Abstände den Zustand der Simulation darstellen. Dieser Vorgang ist größtenteils abhängig von der Grafikkarte. Die Simulation hingegen ist abhängig von der CPU. Idealerweise sind die beiden Prozesse soweit wie möglich voneinander getrennt. Dies wurde in diesem Projekt so realisiert, dass die Simulation in einem getrennten _Thread_ läuft. Die Visualisierung hingegen läuft im _Main_-Thread der Anwendung. Beide Threads greifen auf die selbe Liste an Daten zu, weshalb es nötig war, die Zugriffe auf diese Liste zu synchronisieren. Um eine möglichst geringe Latenz für den Nutzer zu ermöglichen, hat die Visualisierung die primäre Kontrolle über die Liste. Sie kann angeben wann die Simulation auf die Liste zugreifen kann, indem die Simulation temporär pausiert wird.

== Implementieren der Simulation
Die Simulation ist der Kern der Anwendung. Um möglichst große Zeiträume simulieren zu können, ist es also nötig, diesen Vorgang möglichst effizient zu implementieren. Die Simulation wird in der Funktion _simulate_ in _main.cpp_ implementiert. Diese Funktion wird in einem eigenen _Thread_ ausgeführt. Dieser Thread arbeitet komplett unabhängig von der Visualisierung. Solang die Simulation nicht pausiert ist, entweder durch den Nutzer, oder durch die Visualisierung, wird die Simulation in einer Schleife ausgeführt. Im folgenden wird eine vereinfachte Version der Funktion _simulate_ gezeigt:

#figure([```cpp
void simulate()
{
  if(simulationRunning)
  {
    for(body1 in bodies)
    {
      for(body2 in bodies)
      {
        if(body1 == body2)
          continue;
        
        Force = calculate_gravity(body1, body2);

        body1.acceleration += Force / body1.mass;
        body2.acceleration -= Force / body2.mass;

        body1.velocity += body1.acceleration * delta_t;
        body2.velocity += body2.acceleration * delta_t;

        body1.position += body1.velocity * delta_t;
        body2.position += body2.velocity * delta_t;
      }
    }
  }
}
```], caption: "Vereinfachte Version der Funktion simulate")

Die Funktion folgt dem zuvor definierten Aufbau. Für jeden Körper wird die Gravitationskraft berechnet, welche auf den Körper wirkt. Mit dieser Kraft wird die Beschleunigung des Körpers bestimmt. Mit der Beschleunigung wird die Geschwindigkeit, und damit die Position des Körpers bestimmt. Dieser Vorgang wird für alle Körper in der Liste _bodies_ durchgeführt.

Wichtig ist hierbei, dass die Berechnung von der globalen variable _delta_t_ abhängig ist. Diese Variable bestimmt den Zeitschritt der Simulation. Je kleiner dieser Wert ist, desto genauer ist die Simulation, jedoch auch langsamer. Je nach verfügbarer Rechenleistung kann dieser Wert auf bis zu 1s pro iteration gesetzt werden. Mehr hierzu in @results.

== Implementieren der Visualisierung

Mithilfe der Visualisierung können wir den momentanen Zustand der Simulation darstellen. Die primäre _draw_-loop der Simulation findet innerhalb der _main_-Funktion der Anwendung statt. Eine vereinfachte Version dieser Funktion sieht wie folgt aus:

#figure([```cpp
void draw()
{
  BeginDrawing3D();
  ClearBackground(BLACK);
  UpdateClipPlane();

  simulationRunning = false;
  for(body in bodies)
  {
    DrawBody(body);
  }
  simulationRunning = true;

  UpdateCamera();
  DrawInterface();

  EndDrawing3D();
}
```], caption: "Vereinfachte Version der Funktion draw")

Diese Funktion wird solange in einer Schleife ausgeführt, bis die Anwendung geschlossen wird. Nach dem starten des 3-D-Renderers wird der Hintergrund gelöscht. Als nächstes wird die _ClipPlane_@clipplane angepasst. Dies ist der Abstand von Geometrie zur Kamera ab welchem Objekte nichtmehr dargestellt werden. Aufgrund der großen Werte welche durch das verwenden von physikalisch akkuraten Werten auftreten, ist es nötig diesen Wert anhand der aktuellen Kameraeinstellungen anzupassen.

Nachdem diese Vorbereitungen getroffen wurden, werden die einzelnen planetarischen Objekte gezeichnet. Die Funktion _DrawBody_ nimmt die aktuelle Position des Körpers und stellt diesen mithilfe einer gefärbten Kugel in 3D dar.

#figure([
  #image("./assets/sun.jpg", width: 80%)
], caption: "Sonne in nSim")

Diese Methode kann nun verwendet werden um die Planeten und ihre Satelliten zu visualisieren. Ein Phänomen welches aufgrund dieser Art der Visualisierung auftritt, ist das die relative Bewegung und Position von Körpern zueinander schwer zu erkennen ist.

#figure([
  #image("./assets/no_trails.jpg", width: 80%)
], caption: "Erde und Mond") <no_trails>

Wie in @no_trails zu erkennen ist, gibt uns diese Visualisierung keine Anhaltspunkte für die Orientierung und Bewegung der Planeten. Ein weiteres Problem war es, dass aufgrund der Skalierung der Visualisierung einige Satelliten sehr klein, und damit schwer zu erkennen wurden. Um diese Problematiken zu verringern wurden drei weitere Visualisierungen hinzugefügt.

1. Astronomisches Gitter

#figure([
  #image("./assets/grid.jpg", width: 80%)
], caption: "Sonne mit astronomischem Gitter") <grid>

Um ein besseres Gefühl für die relative Position und Rotation von Punkten im Raum zu geben, wurde ein optionales Gitter zur Visualisierung hinzugefügt. Dieses Gitter weißt eine Auflösung von $1/100 "AU"$, oder einem einhundertstel der Astronomischen Einheit da. Über die Einstellung kann auch die Größte des Gitters kontrolliert werden. Das Gitter wird zentriert über dem aktuell fokussierten Körper angezeigt.

2. Transparente Selektoren für Satelliten

#figure([
  #image("./assets/selector.jpg")
], caption: "Mond mit transparenten Selektor") <selector>

@selector Zeigt unseren Mond in Grau mit einem größeren halb-transparenten Selektor darüber. Der Selektor ermöglicht es, auch die kleinren planetarischen Objekte zu selektieren. Die Größe des Selektors ist dabei immer noch abhängig von dem Radius des Objektes, damit die Skalierung trotzdem akkurat bleibt.

3. Historische Pfade

#figure([
  #image("./assets/trails.jpg", width: 80%)
], caption: "Erde und Mond mit historischen Pfaden")

Um die Bewegung der Objekte besser zu visualisieren werden die vergangenen Positionen gespeichert und in Form einer 3D-Kurve durch den Raum angezeigt. So ist es beispielsweise möglich zu sehen, wie der Mond sich relativ zur Erde bewegt.

== Implementierung des Interfaces

Um mit der Simulation zu interagieren wurden einfache Interface-Elemente benötigt. Die Interface-Elemente wurden in Form von verschiedenen _ImGui-Fenstern_ realisiert. Die folgenden Fenster wurden für die Simulation implementiert:

+ Graphics Debugger: #linebreak() Fenster zum einstellen der grafischen Visualisierung. Hier können Dinge wie das Gitter, die Selektoren und Wireframe-Rendering aktiviert werden
+ Camera Settings: #linebreak() Fenster welches die Aktuellen Parameter der Camera anzeigt.
+ Simulation Settings: #linebreak() Fenster welches die Konfiguration des Zeitschrittes erlauben. Über dieses Fenster kann auch die Simulation pausiert werden. Des Weiteren wird angezeigt, wie viele Sekunden, Minuten, Stunden und Tage bisher in der Simulation vergangen sind.
+ Focus Select: #linebreak() Fenster welches verwendet werden kann, um den aktuell fokussierten Körper zu wechseln.

== Implementierung der Kamera
Die Kamera selbst ist mithilfe von sphärischen Koordinaten relativ zum aktuell fokussierten Körper ausgerichtet. Es ist möglich, mithilfe der Maus den horizontalen und vertikalen Winkel der Kamera relativ zum Körper anzupassen. Mithilfe des Mausrads kann der Abstand $r$ der Kamera zu diesem Kontrolliert werden. Die Kamera dieser Simulation richtet sich am aktuell fokussierten Planeten aus. Dementsprechend ist es nötig, die Kamera in jedem Durchgang der Visualisierungsfunktion zu aktualisieren. Dabei übernimmt die Kamera zuerst die Position des aktuell fokussierten Objektes. Danach wird anhand der sphärischen Koordinaten $theta$ und $phi$ wie folgt die relative Position ermittelt:

$Delta_x = r * sin(theta) * cos(phi)$

$Delta_y = r * cos(theta)$

$Delta_z = r * sin(theta) * cos(phi)$

Die finalen Koordinaten der Kamera können dann mit

$x = x_"planet" + Delta_x$

$y = y_"planet" + Delta_y$

$z = z_"planet" + Delta_z$


bestimmt werden. Hierbei ist wichtig zu beachten, dass _OpenGL_, und damit auch _Raylib_, ein rechtshändiges Koordinatensystem verwenden, in welchem $y$ und nicht wie typisch $z$ die vertikale Achse ist.

= Besonderheiten und Lösungen <problems>
Die Simulation weißt eine vielzahl von Besonderheiten auf, welche die Implementation beeinflussen. Der wichtigste Faktor hierbei ist die Größte der verschiedenen verwendeten Werte. Da die Werte in SI-Einheiten vorliegen, sind die Werte für die Positionen und Geschwindigkeiten sehr groß. Die Masse der Sonne beispielsweise beträgt $1.989 * 10^30"kg"$. Die Distanz zwischen der Erde und der Sonne liegt bei $1.495 * 10^11m$. Aufgrund dieser großen Werte ist es nötig, die Visualisierung zu skalieren. Hierzu wurde der Faktor 1:1000000 gewählt. Dieser Faktor wird für alle Größen innerhalb der Visualisierung verwendet. Die Berechnung der Simulation findet allerdings trotzdem mit den ungekürzten Werten statt.

== 32-Bit Gleitkommazahlen <32bit>
Die Größenordnung der Werte führt dazu, dass wir Gleitkommazahlen verwenden müssen, um diese akkurat darzustellen. Eine reguläre 64-Bit Zahl hat einen Wertebereich von $-2^63$ bis $2^63-1$ oder von $plus.minus 9.22 * 10^18$. Damit wäre es nicht möglich, die meisten Berechnungen durchzuführen. Gleitkommazahlen nach IEEE-754@ieee können größere Zahlen darstellen. 32Bit Gleitkommazahlen haben einen Wertebereich von $plus.minus 3.40*10^38$. Dieser Wertebereich ist groß genug um unsere Berechnungen durchzuführen. Für Gleitkommazahlen ist allerdings nicht nur der Wertebereich, sondern auch die Genauigkeit wichtig. 32-Bit Gleitkommazahlen haben eine Genauigkeit von 7 Stellen. Für unsere Berechnungen ist dies nicht ausreichend. Deshalb wurde ein eigener Datentyp _SciVec3_ implementiert. Dieser Datentyp verwendet 64-Bit Gleitkommazahlen. Diese bieten eine Genauigkeit von 15 Stellen. Der Datentyp _SciVec3_ wird für alle Berechnungen innerhalb der Simulation verwendet. Nur für die Darstellung wird in den von _Raylib_ verwendeten _Vector3_-Datentypen umgewandelt.

== Floating Origin <floating_origin>
Auch nach den Anpassungen in @32bit treten allerdings noch Probleme bei der Visualisierung auf. Unsere skalierten Werte sind häufig länger als 7 Stellen. Dies führt aufgrund des Aufbaus von IEEE-754 Gleitkommazahlen, dass Genauigkeitsprobleme sichtbar werden. In diesem Fall können wir nicht wie zuvor den Datentypen anpassen, da moderne Grafikschnittstellen wie _OpenGL_ nur 32-Bit Gleitkommazahlen unterstützen. 

Um dieses Problem zu lösen, wurde das Konzept des _Floating Origin_ eingeführt. Hierbei werden alle Objekte relativ zum aktuell fokussierten Körper dargestellt. Dies führt dazu, dass der fokussierten Bereich der Visualisierung immer im genausten Bereich der Gleitkommazahlen dargestellt werden. Die Logik hierfür ist wie folgt:

+ Die Position des aktuell fokussierten Körpers wird als Ursprung der Welt gesetzt.
+ Beim anzeigen der Körper wird die Position des fokussierten Körpers von der Position des Körpers abgezogen.
+ Die Kamera wird relativ zum fokussierten Körper positioniert.

Da die gesamte Positionierung und Berechnung weiterhin mithilfe des _SciVec3_ in 64-Bit ausgeführt wird, sind die Berechnungen weiterhin akkurat. Erst nachdem diese Berechnungen durchgeführt wurden, werden die Werte in 32-Bit Gleitkommazahlen umgewandelt.

= Ergebnisse <results>
Ziel unserer Simulation war es, dass wir die Bewegung der Planeten simulieren und in 3D visualisieren können. Hierzu wurde das Programm _nSim_, ein n-Body Simulator, entwickelt.

#figure(
  image("./assets/nsim.png"),
  caption: "nSim Simulation des inneren Sonnensystems"
) <nsim1>

@nsim1 Zeigt einen weiten Blick auf das innere Sonnensystem. Hier werden die Umlaufbahnen des Merkurs, der Venus, der Erde und des Mars um die Sonne sichtbar.

#figure(
  image("./assets/nsim1.png"),
  caption: "nSim Simulation von Jupiter"
) <nsim2>

@nsim2 Zeigt die Simulation des Jupiter und seiner vier größten Monde. Hierbei ist zu erkennen, wie die Monde um den Jupiter kreisen.

_nSim_ ermöglicht es, innerhalb kürzester Zeit die Bewegung der Planeten über Tage oder sogar Jahre hinweg zu simulieren. Hierzu werden diverse Open-Source Projekte verwendet. Der Code für dieses Projekt können auf _Github_ @github eingesehen werden.

= Diskussion

#bibliography("sources.yml", title: "Quellen", style: "ieee")