#import "@preview/charged-ieee:0.1.0": ieee

// #show: ieee.with(
//   title: [Simulation von Gravitation im Sonnensystem],
//   abstract: [Die Bewegung der Sterne und Planeten fasziniert die Menschheit schon seit Urzeiten. In diesem Paper wird eine Methode zur Simulation der Gravitation im Sonnensystem vorgestellt, welche physikalisch akkurate Daten verwendet, um die Bewegung der Planeten zu berechnen. Die Simulation wird in C++ implementiert und visualisiert. 
//   ],
//   authors: (
//     (
//       name: "Till Wegener (28891)",
//       location: [Moers, Deutschland],
//       email: "till.wegener@hsrw.org",
//       organization: "Hochschule Rhein-Waal\nFakultät Kommunikation und Umwelt\nMethoden und Werkzeuge der modernen Astronomie"
//     ),
//   ),

//   index-terms: ("Simulation", "Gravitation", "Sonnensystem", "C++", "Visualisierung"),
//   bibliography: bibliography("sources.yml", title: "Quellen", style: "ieee"),
// )

// #outline()

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

#block([Die Bewegung der Sterne und Plan
eten fasziniert die Menschheit schon seit Urzeiten. In diesem Paper wird eine Methode zur Simulation der Gravitation im Sonnensystem vorgestellt, welche physikalisch akkurate Daten verwendet, um die Bewegung der Planeten zu berechnen. Die Simulation wird in C++ implementiert und visualisiert. ])

Schlagwörter: _Simulation_, _Gravitation_, _Sonnensystem_, _C++_, _Visualisierung_

#pagebreak()

#outline(indent: 5%, fill: line(length: 100%, stroke: 0.5pt + rgb("#aaa")))

#pagebreak()

#set page(numbering: "1", number-align: center, columns: 2)


= Einleitung
Die Planeten in unserem Sonnensystem folgen wie alle anderen Objekt im Universum den Phsyikalischen Grundgesetzen. Für die Bewegungen der Planeten ist primär die Gratitationskraft verantwortlich. Durch das verstehen dieser Kraft ist es uns möglich, die Bewegungen der Planeten zu simulieren. 

#block()

Dieses Paper verwendet diese Grundgestze um eine Simulation zu entwickeln, welche die Bewegungen der Planeten im Sonnensystem berechnet. 

= Historischer Kontext

Im laufe der Zeit haben viele Phsyiker und Astronomen sich mit der Bewegung der Planeten beschäftigt. Über die Jarhunderte wurden viele Modelle entwickelt welche nah am heutigen Stand der Wissenschaft sind. Im folgenden werden einige dieser Modelle vorgestellt.


#block([
  1. Im 16ten Jahrhundert entwickelte Nikolaus Kopernikus mit _Über die Umlaufbahnen der Himmelssphären_ @kopernicus das erste Modell, welches den heutigen erkenntnissen nahe kommt. Er beschrieb die Bewegung der Planeten dardurch, dass alle Planeten aufeinander Gravitation ausüben.
])

#block([
  2. Zu beginn des 17ten Jahrhundertes entwickelte Johannes Kepler mit _Astronomia nova_ @kepler ein Modell, welches die Bewegung der Planeten genauer beschrieb. Seine Axiome besaten, dass die Planeten sich auf Ellipsen bewegen, und körper sich gegenseitig anziehen.
])

#block([
  3. 1644 entwickelte Isac Newton mit _Philosophiæ Naturalis Principia Mathematica_ @newton das Modell, welches bis heute zur beschreibung der Gravitation verwendet wird. Seine Gravitationsgesetze beschreiben, wie sich Körper aufeinander anziehen.
])

= Mathematische Grundlagen

Wie durch Newton beschreiben, wirkt auf jeden Körper eine Gravitationskraft, welche durch die Masse der Körper und den Abstand zwischen den Körpern bestimmt wird. Die Gravitationskraft zwischen zwei Körpern kann durch die Formel

$ F_G = G * (m_1 * m_2)/r^2 $

beschrieben werden. Hierbei ist $F_G$ die resultierende Gravitationskraft, $G$ die Gravitationskonstante ($6.67*10^(-11)m^3/("kg"*s^2)$), $m_1$ und $m_2$ die Massen der Körper und $r$ der Abstand zwischen den Körpern. Die Kraft wirket hierbei entlang der Verbindungslinie der Körper.

#linebreak()

Für ein 2-Körper-System kann die Bewegung der Körper durch diese Formel bestimmt werden. Die Gravitationskraft welche die Körper aufeinander ausüben, kann mit der Masse dieser verwendet werden, um die Beschleunigung der Körper mit $F = m * a$ zu bestimmen.

Für 2-Körper-Systeme ist es möglich, numerische Lösungen für die Bewegung der Körper zu finden.

#linebreak()

In mehr-Körper-Systemen wird die Bewegung der Körper durch die Summe der einzelnen wirkenden Gravitationskräfte bestimmt. Für Systeme mit 3 oder mehr Körpern ist es nicht möglich, analytische Lösungen zu finden. Wenn wir hierzu Lösungen finden wollen, müssen wir numerische Methoden verwenden.

Im folgenden wird eine Methode vorgestellt, welche Eulers Methode verwendet, um die Bewegung der Planeten im Sonnensystem zu simulieren. 

#linebreak()

Eulers Methode ist eine numerische Methode, welche einen Differentialgleichungssystem durch diskrete Schritte approximiert. Die Methode ist einfach zu implementieren, kann jedoch aufgrund ihrer funktionsweise ungenau sein.
Als erstes bestimmen wir unseren diskreten Zeitschritt $Delta t$. Dannach bestimmen die Startwerte für die Position und Geschwindigkeit der Körper im Raum. 

#linebreak()

Nun können wir für alle $n$ Körper die Gravitationskräfte berechnen, welche auf die Körper wirken. In diesem Fall ist die Kraft $arrow(F_G)$ ein Vektor, welcher die Richtung und Stärke der Kraft angibt. Mit dieser Kraft können wir die Beschleunigung der Körper bestimmen. Mit $ arrow(a) = arrow(F_G) / m$ und $ arrow(v) = arrow(v) + arrow(a) * Delta t$ können wir nun die angepassten Geschwindigkeiten der Körper bestimmen. Die Geschwindigkeit ist in diesem Fall ein 3-Dimensionaler Vektor, welcher die Geschwindigkeit in $x$, $y$ und $z$ Richtung angibt. Für einen Körper kann nun die neue Position $arrow(r)$ im Raum mit $ arrow(r) = arrow(r) + arrow(v) * Delta t$ bestimmt werden.

#linebreak()

Wenn wir dieses Verfahren nun für alle Körper im System durchführen, können wir die Bewegung der Körper simulieren. Dieses Verfahren von hand durchzuführen ist sehr aufwendig, weshalb sich die implementation als Computerprogramm anbietet.

= Vorbereitung

== Datensätze <dataset>
Für die Simulation des Sonnensytems benötigen wir Daten über die Planeten und ihre diversen Satellieten. Die wichtigen Daten für die Simulation sind die Masse der Körper, die Position der Körper im Raum und die Geschwindigkeit der Körper. Diese Daten können aus diversen Quellen bezogen werden. Für die Planeten wurden die Daten aus dem Projekt _Nasa Data Scraper_ von _Devstronomy_ @planets auf github verwendet. Das gleiche Projekt beinhält auch einen Datensatz für die Satellieten der Planeten. Dieser ist allerdings für unsere Simulation nicht geeignet, da keine Informationen zu Position und Geschwindigkeit der Satellieten enthalten sind.

#linebreak()

Die Daten der Satellieten wurden vom _Solar System Dynamics_ Projekt des _Jet Propulsion Laboratory_ der _NASA_ @ssd verwendet. Vom _SSD_ Projekt können wir sowohl die Phsyikalischen Daten der Satellieten @satellite_phys, als auch die orbitale Parameter der Satellieten @satellite_orb beziehen. Diese Daten beinhalten die _Semi Major Axis_ und weitere Parameter, welche für die Simulation benötigt werden.

== Wahl der Programmiersprache
Da für diese Simulation eine große Anzahl an Berechnungen durchgeführt werden muss, stellte die Wahl der Programmiersprache eine wichtige Entscheidung dar. Für diese Simulation wurde deshalb C++ gewählt. C++ ist eine schnelle kompilierte Sprache. 

== Wahl von Hilfsbibliotheken
Zur Visualisierung unserer Simulation benötigen wir eine Bibliothek, welche das darstellen von 3D-Objekten ermöglicht. Hierzu wurde die Bibliothek _Raylib_ @Raylib verwendet. Raylib ist eine C-Bibliothek, welche das erstellen von 2D und 3D Anwendungen ermöglicht. Neben der Darstellung von 3D-Objekten war es auch noch wichtig, dass wir einfache Interface-Elemente anzeigen können. Hierzu wird die Bibliothek _Dear ImGui_ @Imgui verwendet. ImGui ist eine C++ Bibliothek, welche in der Spieleentwicklung verwendet wird, um einfache Interface-Elemente anzuzeigen. Um diese beiden Bibliotheken zusammen zu verwedne, wurde das Projekt _rlImGui_ @Rlimgui verwendet.

Die Bibliotheken _ImGui_ und _rlImGui_ wurden als Submodule in das Projekt eingebunden. _Raylib_ wird während der kompilierung des Projektes heruntergeladen und verwendet. Hierzu wurde das Programm _Cmake_ @cmake verwendet. Cmake ist ein Build-System, welches es ermöglicht, C und C++ Projekte plattformunabhängig zu kompilieren.

= Implementierung

== Grundlegendes

Die Implementierung der Simulation folgt dem zuvor beschriebenen Ablauf. In der _Main_-Funktion der Simulation werden zuerst die nötigen Bibliotheken initialisiert. Für dieses Projekt war es nötig, einen eigenen _Vektor_-Datentypen zu verwenden. Die Gründe hierzu, werden in @32bit erläutert. Dieser _SciVec_ verwendet 64-Bit gleitkomma-Zahlen, um möglichst preziese Berechnungen zu ermöglichen. Der Datentyp und die dazugehörigen Funktionen zur Addition, Subtraktion, Multiplikation und Skalierung können in _SciVec.h_ geufunden werden. Die Körper des Sonnensystems werden mithilfe eines Datentypens _Body_ abgebildet. In _Body.h_ wird das _struct_ _Body_ wie folgt definiert:

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
Die Felder _radius_, _mass_, _postion_, und _velocity_ sind für die Simulation am wichtigesten. Für jedes Objekt wird außerdem ein _Name_, eine _Farbe_, und eine Liste an Satllieten gespeichert. Die Simulation verwaltet eine danamische Liste (_std::vector_) an _Body_-Objekten. Diese Liste stellt den aktuellen zustand der Simulation da. Um diese Liste nun mit Daten zu füllen, müssen wir unsere Datensätze einladen. 

== Laden der Datensätze

Die Datei _Loader.h_ beinhaltet die Funktionen, welche die Datensätze aus @dataset lädt. Die Funktion _loadPlanets_ lädt die _planets.csv_-Datei aus dem _Assets_-Ordner. Zuerst erstellt diese Funktion allerdings die Sonne, welche nicht im Datensatz enthalten ist. Hierfür wurden die wichtigsten Werte in _Consts.h_ definiert. Dannach iteriert die Funktion über die Zeilen der CSV-Datei und erstellt für jede Zeile ein _Body_-Objekt. Hierbei ist es wichtig, die einzelnen Werte in die Korrekten Einheiten umzuwandeln. Für diese Simulation wurde die Entscheidung getroffen, alle Werte in SI-Einheiten zu speichern. Insbesonders für die Datenwerte _Mass_ war es nötig, 64-Bit Gleitkommazahlen zu verwenden. Um die Visualisierung besser zu gestalten, wurden allen Planeten eine individuelle Farbe zugeordnet. 

Neben der bereits genannten Daten, war die bestimmung der initialen Position und Geschwindigkeit der Körper eine wichtige Entscheidung. Der Datensatz der Planeten beinhält sowohl Werte für den _Aphelion_, den _Perihelion_, die _Inclination_ und die _Exzentrizität_ der Planeten. Des Weiteren wurde ein Wert für die durchschnittliche _orbital_velocity_ angegeben.

// TODO Richtig implmentieren und dann beschreiben

#block()

Die Funktion _loadSatallites_ funktioniert ähnlich wie _loadPlanets_. Hierbei wird die Dateien _satellites_data.csv_ und _satellites_orbit.csv_ kombiniert, um die Satellieten der Planeten zu erstellen. Für jeden Satelliet wird zuerst der _Parent_-Körper gesucht. Dannach werden die weiteren Planetaren informationen bestimmt. Dieser Datensatz weißt die Besonderheit auf, dass nicht die Masse der Objekte angegeben ist, sondern der _Standard Gravitational Parameter_ $mu$. Dieser Wert ist definiert als $mu = G * m$, wobei $G$ die Gravitationskonstante ist. Um die Masse der Satellieten zu bestimmen, wird der Wert $mu$ durch $G$ geteilt. 

Die bestimmung der initialen Position und Geschwindigkeit der Satellieten ist etwas komplizierter. Hierzu müssen wir nicht nur die Orbitalen Parameter des Satllieten, sondern auch die des Parent-Körpers verwenden.

// TODO Richtig implmentieren und dann beschreiben

== Aufteilung von Simulation und Visualisierung
Die Simulation und Visualisierung sind zwei verschiedene Aufgaben mit unterschiedlichen Zielen. Die Visualisierung in geregelten Abstände den Zustand der Simulation darstellen. Dieser Vorgang ist größtenteils abhängig von der Grafikkarte. Die Simulation hingegen ist abhängig von der CPU. Ideallerweise sind die beiden Prozesse soweit wie möglich voneinander getrennt. Dies wurde in diesem Projekt so realisiert, dass die Simulation in einem getrennten _Thread_ läuft. Die Visualisierung hingegen läuft im _Main_-Thread der Anwendung. Beide Threads greifen auf die selbe Liste an Daten zu, weshalb es nötig war, die Zugriffe auf diese Liste zu synchronisieren. Um eine möglichst geringe Latenz für den Nutzer zu ermöglichen, hat die Visualisierung die priämre Kontrolle über die Liste. Sie kann angeben wann die Simulation auf die Liste zugreifen kann, indem die Simulation temporär pausiert wird.

== Implementieren der Simulation
Die Simualtion ist der Kern der Anwendung. Um möglichst große Zeiträume simluieren zu können, ist es also nötig, diesen Vorgang möglichst effizient zu implementieren. Die Simulation wird in der Funktion _simulate_ in _main.cpp_ implementiert. Diese Funktion wird in einem eigenen _Thread_ ausgeführt. Dieser Thread arbeitet komplett unabhängig von der Visualisierung. Solang die Simulation nicht pausiert ist, entwedet durch den Nutzer, oder durch die Visualisierung, wird die Simulation in einer Schleife ausgeführt. Im folgenden wird eine vereinfachte Version der Funktion _simulate_ gezeigt:

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

Diese Funktion wird solange in einer Schleife ausgeführt, bis die Anwendung geschlossen wird. Nach dem starten des 3-D-Renderers wird der Hintergrund gelöscht. Als nächstes wird die _ClipPlane_@clipplane angepasst. Dies ist der Abstand von Geometrie zur Kamera ab welchem Objekte nichtmehr dargestellt werden. Aufgrund der großen Werte welche duch das verwenden von physikalisch akkuraten Werten auftreten, ist es nötig diesen Wert anhan der aktuellen Kameraeinstellungen anzupassen.

Nachdem diese Vorbereitungen getroffen wurden, werden die einzelnen planetarischen Objekte gezeichnet. Die Funktion _DrawBody_ nimmt die aktuelle Position des Körpers und stellt diesen mithilfe einer gefärbten Kugel in 3D dar.

#figure([
  #image("./assets/sun.jpg", width: 80%)
], caption: "Sonne in nSim")

Diese Methode kann nun verwendet werden um die Planeten und ihre Satellieten zu visualisieren. Ein Phänomen welches aufgrund dieser Art der Visualisierung auftritt, ist das die relative Bewegung und Position von Körpern zueinander schwer zu erkennen ist.

#figure([
  #image("./assets/no_trails.jpg", width: 80%)
], caption: "Erde und Mond") <no_trails>

Wie in @no_trails zu erkennen ist, gibt uns diese Visualisierung keine Anhaltspunkte für die Orientierung und Bewegung der Planeten. Ein weiters Problem war es, dass aufgrund der Skalierung der Visualisierung einige Satellieten sehr klein, und damit schwer zu erkennen wurden. Um diese Problematiken zu verringern wurden drei weitere Visualisierungen hinzugefügt.

1. Astronomisches Gitter

#figure([
  #image("./assets/grid.jpg", width: 80%)
], caption: "Sonne mit astronomischem Gitter") <grid>

Um ein besseres Gefühl für die relative Position und Rotation von Punkten im Raum zu geben, wurde ein optionales Gitter zur Visualisierung hinzugefügt. Dieses Gitter weißt eine Auflösung von $1/100 "AU"$, oder einem einhundertstel der Astronomischen Einheit da. Über die Einstellung kann auch die Größte des Gitters kontrolliert werden. Das Gitter wird zentriert über dem aktuell fokussierten Körper angezeigt.

2. Transparente Selektoren für Satellieten

#figure([
  #image("./assets/selector.jpg")
], caption: "Mond mit transparenten Selektor") <selector>

@selector Zeigt unseren Mond in Grau mit einem größeren halb-transparenten Slektor dadrüber. Der Selektor ermöglicht es, auch die kleinern planetarischen Objekte zu selektieren. Die Größe des Selektors ist dabei immernoch abhängig von dem Radius des Objektes, damit die Skalierung trotzdem akkurat bleibt.

3. Historische Pfade

#figure([
  #image("./assets/trails.jpg", width: 80%)
], caption: "Erde und Mond mit historischen Pfaden")

Um die Bewegung der Objekte besser zu visualisieren werden die vergangenen Positionen gespeichert und in Form einer 3D-Kurve durch den Raum angezeigt. So ist es beispielsweise möglich zu sehen, wie der Mond sich relativ zur Erde bewegt.

== Implementierung des Interfaces

Um mit der Simulation zu interagieren wurden einfache Interface-Elemente benötigt. Die Interface-Elemente wurden in Form von verschiedenen _ImGui-Fenstern_ realisiert. Die folgenden Fenster wurden für die Simulation implementiert:

+ Graphics Debugger: #linebreak() Fenster zum einstellen der grafischen Visualisierung. Hier können Dinge wie das Gitter, die Selektoren und Wireframe-Rendering aktiviert werden
+ Camera Settings: #linebreak() Fenster welches die Aktuellen Parameter der Camera anzeigt.
+ Simulation Settings: #linebreak() Fenster welches die Konfiguration des Zeitschrittes erlauben. Über dieses Fenster kann auch die Simulation pausiert werden. Des Weiteren wird angezeigt, wie viele Sekunden, Minuten, Stunden und Tage bisher in der Simualtion vergangen sind.
+ Focus Select: #linebreak() Fenster welches verwendet werden kann, um den aktuell fokussierten Körper zu wechseln.

== Implementierung der Kamera
Die Kamera selbst ist mithilfe von spherischen Koordinaten relativ zum aktuell fokussierten Körper ausgerichtet. Es ist möglich, mithilfe der Maus den horizontalen und vertikalen Winkel der Kamera relativ zum Körper anzupassen. Mithilfe des Mausrads kann der Abstand $r$ der Kamera zu diesem Kontrolliert werden. Die Kamera dieser Simulation richtet sich am aktuell fokussierten Planeten aus. Dementsprechen ist es nötig, die Kamera in jedem durchgang der Visualisierungs-Funktion zu aktualisieren. Dabei übernimmt die Kamera zuerst die Position des aktuell fokussierten Objektes. Dannach wird anhand der spherischen Koordinaten $theta$ und $phi$ wie folgt die relative Position ermittelt:

$Delta_x = r * sin(theta) * cos(phi)$

$Delta_y = r * cos(theta)$

$Delta_z = r * sin(theta) * cos(phi)$

Die finalen Koordinaten der Kamera können dann mit

$x = x_"planet" + Delta_x$

$y = y_"planet" + Delta_y$

$z = z_"planet" + Delta_z$


bestimmt werden. Hierbei ist wichtig zu beachten, dass _OpenGL_, und damit auch _Raylib_, ein rechtshändiges Koordinatensystem verwenden, in welchem $y$ und nicht wie typisch $z$ die vertikale Achse ist.

= Besonderheiten und Lösungen <problems>

== 32-Bit gleitkommazahlen <32bit>

== Skalierung der Visualisierung <scale>

== Floating Origin <floating_origin>

= Ergebnisse <results>

= Diskussion

#bibliography("sources.yml", title: "Quellen", style: "ieee")