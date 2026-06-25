# Code Coverage Crashkurs Demo

Dieses Repository begleitet den Code-Coverage-Crashkurs.

## Lernziele

Nach Durcharbeiten der Commit-Historie sollen die Teilnehmer:

1. verstehen, was Code Coverage ist und was nicht
2. die Unterschiede zwischen Function-, Line- und Branch-Coverage erklären können
3. verstehen, wie Coverage-Daten mit gcc/g++ erzeugt werden
4. die Rollen von gcc, gcov, lcov, genhtml und gcovr unterscheiden können
5. Coverage-Reports erzeugen und interpretieren können
6. erkennen, warum Testcode die Coverage-Kennzahlen verfälschen kann
7. verstehen, warum Coverage nur für Produktivcode ausgewertet werden sollte
8. die Aussagekraft von Function-, Line- und Branch-Coverage bewerten können
9. erkennen, dass hohe Coverage keine hohe Testqualität garantiert
10. typische Coverage-Fallstricke in Robotik- und ROS1-ähnlichen Projekten verstehen
11. Coverage-Reports für Zustandsautomaten, Fehlerpfade und Recovery-Logik interpretieren können
12. Coverage lokal mit lcov/genhtml erzeugen können
13. Coverage in CI-Pipelines integrieren können
14. die Vor- und Nachteile von lcov und gcovr für CI-Anwendungen verstehen
15. einfache Coverage-Gates mit gcovr konfigurieren können

## Lernpfad

Die Demo ist bewusst entlang einer Git-Historie aufgebaut.

Jeder Commit führt genau eine neue Idee ein und dokumentiert die dazugehörige Erkenntnis in dieser README.

Die empfohlene Reihenfolge ist:

```text
Setup
 ↓
Coverage-Instrumentierung
 ↓
Erster Coverage-Report
 ↓
Testcode verfälscht Kennzahlen
 ↓
Function Coverage
 ↓
Line Coverage
 ↓
Branch Coverage
 ↓
Coverage ≠ Testqualität
 ↓
Robotik-State-Machine
 ↓
ROS1-Callback-Fallstrick
 ↓
CI mit lcov
 ↓
CI mit gcovr
```

## Schritt-für-Schritt-Demo mit Git-Historie

Diese Demo ist so aufgebaut, dass Teilnehmer jederzeit über

```bash
git log --oneline --reverse
```

die Commit-Historie durchgehen und die einzelnen Schritte nachvollziehen können und mit

```bash
git checkout <commit-hash>
```

jeden Kursstand reproduzieren können.

## In der Demo verwendete Tools

1. `gcc`/`g++` – Compiler mit Coverage-Instrumentierung
2. `gcov` – Coverage-Datenanalyse-Tool von GNU
3. `lcov` – Erweiterung von gcov für HTML-Reports
4. `genhtml` – Tool zur Umwandlung von lcov-Daten in HTML
5. `gcovr` – Alternative zu lcov für Coverage-Reports, besonders in CI-Umgebungen
6. `cmake` – Build-System, um die Coverage-Instrumentierung zu konfigurieren
7. `make` – Build-Tool, um die Kompilierung und Testausführung zu automatisieren
8. `gtest` – Test-Framework für die Erstellung von Unit-Tests (optional, aber empfohlen)
9. `git` – Versionskontrollsystem, um die Demo-Historie zu verwalten
10. `bash` – Shell für die Ausführung von Build- und Coverage-Befehlen
11. CI-Tools (z.B. GitHub Actions, GitLab CI) – für die Integration von Coverage in Continuous Integration Pipelines

```bash
gcc --version
gcov --version
lcov --version
genhtml --version
gcovr --version
cmake --version
make --version
dpkg -l | grep libgtest-dev
git --version
bash --version
```

| Tool | Version |
|------|---------|
| gcc  | gcc (Ubuntu 9.4.0-1ubuntu1~20.04.2) 9.4.0 |
| gcov | gcov (Ubuntu 9.4.0-1ubuntu1~20.04.2) 9.4.0 |
| lcov | lcov: LCOV version 1.14 |
| genhtml | genhtml: LCOV version 1.14 |
| gcovr | gcovr 4.2 |
| cmake | cmake version 3.18.6 |
| make | GNU Make 4.2.1 |
| gtest | libgtest-dev:amd64 1.10.0-2 |
| git | git version 2.49.0 |
| bash | GNU bash, version 5.0.17(1)-release (x86_64-pc-linux-gnu) |

## 01 - Coverage-Instrumentierung

Coverage beginnt nicht beim Report, sondern beim Build.

Die Option `--coverage` sorgt dafür, dass gcc/g++ zusätzlichen Messcode erzeugt.

Wichtig:

- gcc/g++ kompiliert und instrumentiert
- beim Kompilieren entstehen `.gcno`-Dateien
- beim Ausführen entstehen `.gcda`-Dateien
- erst danach können gcov/lcov die Daten auswerten

Merksatz:

> gcc setzt die Messpunkte. Tests erzeugen die Laufzeitdaten.

## 02 - Erster Produktivcode

Die Funktion `decideMode()` enthält drei fachliche Pfade:

- `AUTO`
- `MANUAL`
- `ERROR`

Damit eignet sie sich gut, um später Function, Line und Branch Coverage zu vergleichen.

Noch gibt es keine Tests. Also gibt es auch keine aussagekräftige Runtime-Coverage.

## 03 - Erster Test

Der erste Test deckt nur den Happy Path ab:

```text
sensor_ok = true
manual_mode = false
=> AUTO
```

Nicht getestet sind:
```text
sensor_ok = false => ERROR
manual_mode = true => MANUAL
```

Erkenntnis:

> Ein einzelner Test kann eine Funktion ausführen, ohne alle fachlichen Pfade zu prüfen.

## 04 - Erster Coverage-Report

Dieser Report sammelt zunächst alles, was im Build-Verzeichnis Coverage-Daten erzeugt hat.

Das umfasst aktuell sowohl:

- Produktivcode unter `src/`
- Testcode unter `test/`

Befehle:

```bash
mkdir build
cd build
cmake ..
make
ctest --output-on-failure
../scripts/generate_coverage_all.sh
```

Erkenntnis:

> Coverage entsteht erst nach Build und Testausführung.

## 05 - Aha-Moment: Testcode verfälscht Coverage

Der Report `coverage_all_report` enthält auch Dateien aus `test/` und `usr`.

Das ist problematisch, weil Testdateien oft sehr hohe Coverage haben:
Sie werden ja beim Testlauf direkt ausgeführt.

Dadurch kann die Gesamtkennzahl besser aussehen, als die Abdeckung des Produktivcodes wirklich ist.

Falsche Frage:

> Wie gut führen die Tests sich selbst aus?

Richtige Frage:

> Wie gut decken die Tests den Produktivcode ab?

Erkenntnis:

> Coverage-Kennzahlen sind nur sinnvoll, wenn klar ist, welche Dateien gemessen werden.

## 06 - Produktivcode isolieren

Der zweite Report filtert Testcode heraus:

```bash
lcov --remove coverage.info '/usr/*' '*/test/*' -o coverage_productive.info
```

Jetzt betrachten wir nur noch den Produktivcode.

Vergleich:

- `coverage_all_report`: enthält Produktivcode und Testcode
- `coverage_productive_report`: enthält nur Produktivcode

Erkenntnis:

> Der produktive Coverage-Wert ist meist niedriger, aber ehrlicher.

## 07 - Function Coverage

Mit nur einem Test kann `decideMode()` bereits 100 % Function Coverage erreichen.

Warum?

Die Funktion wurde mindestens einmal betreten.

Aber nicht getestet sind weiterhin:

- `MANUAL`
- `ERROR`

Erkenntnis:

> Function Coverage ist oft wenig aussagekräftig. Sie sagt nur, ob eine Funktion aufgerufen wurde, nicht ob ihre Logik vollständig getestet wurde.

## 08 - Line Coverage verbessern

Der zweite Test deckt den `MANUAL`-Pfad ab.

Dadurch werden mehr Zeilen im Produktivcode ausgeführt.

Weiterhin fehlt aber der Fehlerpfad:

```text
sensor_ok = false => ERROR
```

Erkenntnis:

> Line Coverage ist besser als Function Coverage, weil sie zeigt, welche Zeilen tatsächlich ausgeführt wurden.

## 09 - Line Coverage kann vollständig wirken

Mit AUTO, MANUAL und ERROR sind alle sichtbaren Rückgabepfade von `decideMode()` getestet.

Der Report kann jetzt sehr gute Line Coverage zeigen.

Aber daraus folgt noch nicht automatisch:

- dass jede Entscheidung vollständig verstanden wurde
- dass komplexere boolesche Bedingungen gut getestet sind
- dass Tests fachlich stark sind

Erkenntnis:

> Viele Teams hören bei hoher Line Coverage zu früh auf.


## 10 - Branch Coverage einführen

Nachdem klar ist, welche Dateien gemessen werden sollen, führen wir Branch Coverage als zusätzliche Metrik ein.

Bisherige Fragen:

- Function Coverage: Welche Funktionen wurden betreten?
- Line Coverage: Welche Zeilen wurden ausgeführt?

Neue Frage:

- Branch Coverage: Welche Entscheidungswege wurden ausgeführt?

Erkenntnis:

> Branch Coverage zeigt, ob Bedingungen in beide Richtungen getestet wurden.

## 11 - Robotik-nahe State Machine

Robotiksoftware enthält häufig Zustandslogik:

- Betriebszustände
- Fehlerzustände
- Recovery-Pfade
- Fallback-Verhalten

Die Funktion `nextState()` ist bewusst einfach, zeigt aber ein typisches Muster.

Erkenntnis:

> Zustandsautomaten sind ein natürlicher Anwendungsfall für Branch Coverage.


## 12 - Unvollständige State-Machine-Tests

Die Tests decken zwei Transitionen ab:

```text
IDLE + kein Hindernis => DRIVING
DRIVING + Hindernis => ERROR
```

Bewusst nicht getestet ist:

```text
ERROR + reset => RECOVERY
```

Erwarteter Aha-Moment im Report:

- Function Coverage kann hoch sein
- Line Coverage kann hoch wirken
- Branch Coverage zeigt fehlende Entscheidungswege

Erkenntnis:

> Branch Coverage ist besonders wertvoll für Fehler- und Recovery-Pfade.


## 13 - Recovery-Pfad abdecken

Der fehlende Recovery-Pfad wird ergänzt:

```text
ERROR + reset => RECOVERY
```

Der Report sollte nun zeigen, dass die relevante Entscheidungslogik besser abgedeckt ist.

Erkenntnis:

> Branch Coverage lenkt den Blick auf fachlich wichtige Übergänge, nicht nur auf ausgeführte Zeilen.

## 14 - Coverage ist keine Testqualität

Ein Test kann Code ausführen, ohne sinnvolle Assertions zu enthalten.

Beispiel:

```cpp
nextState(RobotState::RECOVERY, false, false);
SUCCEED();
```

Dieser Test erhöht Coverage, prüft aber kein Verhalten.

Erkenntnis:

> Coverage misst Ausführung, nicht Testqualität.


## 15 - Tests müssen Verhalten prüfen

Der execution-only Test wurde entfernt.

Gute Tests prüfen beobachtbares Verhalten:

- Rückgabewerte
- Zustandsänderungen
- Ausgaben
- Fehlersignale

Erkenntnis:

> Gute Coverage ist nur dann nützlich, wenn die Tests auch fachlich relevante Assertions enthalten.


## 16 - ROS1-ähnlicher Callback-Code

Der `SafetyMonitor` simuliert typische ROS1-Callback-Logik.

In echten ROS1-Projekten wäre `laserCallback()` z. B. ein Subscriber Callback.

Erkenntnis:

> Callback-Code wird nur abgedeckt, wenn der Callback tatsächlich ausgelöst wird.


## 17 - Callback-Fallstrick

Der Test erzeugt ein Objekt und prüft den Initialzustand.

Aber der Callback wird nicht aufgerufen.

Nicht abgedeckt:

```cpp
laserCallback(true);
```

ROS1-Übertragung:

```text
Node startet
Subscriber ist registriert
Callback-Code bleibt trotzdem ungetestet
```

Erkenntnis:

> Node startet ≠ Callback getestet.

## 18 - Callback aktiv stimulieren

Der Callback wird nun direkt ausgelöst:

```cpp
monitor.laserCallback(true);
```

In echten ROS1-Tests entspricht das z. B.:

- Nachricht publizieren
- Callback ausführen lassen
- Zustand oder Ausgabe prüfen

Erkenntnis:

> ROS-Kommunikation muss im Test aktiv stimuliert und validiert werden.

## 19 - Coverage-Daten bereinigen

Coverage-Daten sind Build-Artefakte.

Vor einem sauberen Neuversuch können sie gelöscht werden:

```bash
../scripts/clean_coverage.sh
```

Erkenntnis:

> Alte `.gcda`-Dateien können Coverage-Ergebnisse verfälschen.


## 20 - CI mit lcov

Die lokal verwendete Coverage-Kette kann auch in CI laufen:

```text
Build → Test → lcov → genhtml → HTML-Artefakt
```

Vorteile:

- gleiche Toolchain wie lokal
- HTML-Report ist gut für manuelle Analyse
- lcov/genhtml sind unter Ubuntu leicht verfügbar

Nachteile:

- XML-Reports für CI-Systeme müssen separat erzeugt werden
- Excludes werden schnell unübersichtlich
- Branch Coverage muss explizit aktiviert werden
- Quality Gates sind nicht besonders komfortabel

Erkenntnis:

> lcov ist sehr gut für HTML-Reports, aber in CI oft etwas sperrig.
