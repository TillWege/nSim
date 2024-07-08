#import "@preview/charged-ieee:0.1.0": ieee

#show: ieee.with(
  title: [Simulation von Gravitation im Sonnensystem],
  abstract: [Die Bewegung der Sterne und Planeten fasziniert die Menschheit schon seit Urzeiten. In diesem Paper wird eine Methode zur Simulation der Gravitation im Sonnensystem vorgestellt, welche physikalisch akkurate Daten verwendet, um die Bewegung der Planeten zu berechnen. Die Simulation wird in C++ implementiert und visualisiert. 
  ],
  authors: (
    (
      name: "Till Wegener (28891)",
      location: [Moers, Deutschland],
      email: "till.wegener@hsrw.org",
      organization: "Hochschule Rhein-Waal\nFakultät Kommunikation und Umwelt\nMethoden und Werkzeuge der modernen Astronomie"
    ),
  ),

  index-terms: ("Simulation", "Gravitation", "Sonnensystem", "C++", "Visualisierung"),
  bibliography: bibliography("sources.yml", title: "Quellen", style: "ieee"),
)

#outline()


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

Die Implementierung der Simulation folgt dem zuvor beschriebenen Ablauf. In der _Main_-Funktion der Simulation werden zuerst die nötigen Bibliotheken initialisiert. Für dieses Projekt war es nötig, einen eigenen _Vektor_-Datentypen zu verwenden. Die Gründe Hierzu werden in @32bit erläutert. Dieser _SciVec_ verwendet 64-Bit gleitkomma-Zahlen, um möglichst preziese Berechnungen zu ermöglichen. Der Datentyp und die dazugehörigen Funktionen zur Addition, Subtraktion, Multiplikation und Skalierung können in _SciVec.h_ geufunden werden. Die Körper des Sonnensystems werden mithilfe eines Datentypens _Body_ abgebildet. In _Body.h_ wird das _struct_ _Body_ wie folgt definiert:

```cpp
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
```
Die Felder _radius_, _mass_, _postion_, und _velocity_ sind für die Simulation am wichtigesten. Für jedes Objekt wird außerdem ein _Name_, eine _Farbe_, und eine Liste an Satllieten gespeichert.

Die Datei _loader.h_ beinhaltet die Logik, welche die Datensätze aus @dataset lädt.

= Probleme <problems>

== 32-Bit gleitkommazahlen <32bit>

== Finden von passenden Datensätzen

== Bestimmen von Initialwerten

= Ergebnisse

= Diskussion
