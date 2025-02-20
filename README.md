# CBlackJack

<table>
<tr><th>Autoren</th><td>Jan Kruske, Felix Schulz, Noah Becker</td></tr>
<tr><th>Dozent</th><td>Dr. Jonas Fritzsch</td></tr>
<tr><th>Studiengang/Kurs</th><td>B. Sc. Informatik – Cyber Security TINF24F</td></tr>
<tr><th>Anlass</th><td>Entwurf und Implementierung einer Anwendung in C als Programmierprojekt</td></tr>
<tr><th>Bearbeitungszeitraum</th><td>05.02.25 - 24.02.2025</td></tr>
<tr><th>Abgabedatum</th><td>24.02.2025</td></tr>
</table>

Dieses Projekt ist eine Simulation des Kartenspiels Blackjack. Es ermöglicht bis zu 6 Spielern, gegen einen Dealer anzutreten und das Spiel zu spielen. Das Projekt wurde in C entwickelt und verwendet ANSI Escape-Sequenzen für die Textformatierung in der Konsole.

## Inhaltsverzeichnis
1. [Projektbeschreibung](#projektbeschreibung)
2. [Installation](#installation)
3. [Nutzung](#nutzung)
4. [Ordnerstruktur](#ordnerstruktur)
5. [Funktionen](#funktionen)
6. [Autoren](#autoren)

## Projektbeschreibung
Dieses Projekt ist eine einfache Konsolenanwendung, die das Blackjack-Spiel für mehrere Spieler simuliert. Die Spieler können Karten ziehen, und der Dealer spielt gemäß den traditionellen Blackjack-Regeln. Das Ziel ist es, so nah wie möglich an 21 Punkte zu kommen, ohne diese zu überschreiten.

## Installation
1. **Repository klonen**:
   ```bash
   git clone https://github.com/dein-benutzername/Blackjack-Simulation.git

## Anwenderdokumentation
### Einführung
Willkommen zur Blackjack-Simulation! Dieses Spiel ermöglicht es Ihnen, gegen den Dealer in einer klassischen Partie Blackjack anzutreten. Folgen Sie den untenstehenden Anweisungen zur Installation und Nutzung.

Installation
Repository klonen: Zuerst müssen Sie das GitHub-Repository auf Ihren Computer klonen:

bash
git clone https://github.com/dein-benutzername/Blackjack-Simulation.git
In das Projektverzeichnis wechseln: Navigieren Sie in das Projektverzeichnis:

bash
cd Blackjack-Simulation
Projekt kompilieren: Kompilieren Sie das Projekt mit dem Befehl:

bash
make
Nutzung
Programm starten: Um das Spiel zu starten, führen Sie das kompilierte Programm aus:

bash
./blackjack
Anzahl der Spieler eingeben: Geben Sie die Anzahl der Spieler ein, die am Spiel teilnehmen möchten (1 bis 6).

Karten ziehen oder stehen bleiben: Während des Spiels werden Sie aufgefordert, ob Sie eine weitere Karte ziehen möchten. Geben Sie 'j' (ja) oder 'n' (nein) ein, um Ihre Entscheidung zu treffen.

Ergebnisse überprüfen: Am Ende des Spiels werden die Ergebnisse angezeigt. Sie können entscheiden, ob Sie eine weitere Runde spielen möchten.

Beispiel
Hier ist ein Beispiel, wie Sie das Programm ausführen und nutzen:

plaintext
$ ./blackjack
Geben Sie die Anzahl der Spieler ein: 3
-- Spieler 1:
  10 Herz
  Dame Pik

-- Spieler 2:
  7 Karo
  8 Kreuz

-- Spieler 3:
  Ass Herz
  4 Pik

════════════════════
Dealer:
  (?)
  9 Herz
════════════════════

▃▅▆█ 웃 1 █▆▅▃
Spieler 1 ist am Zug:
Ihre Hand:
  10 Herz
  Dame Pik
Aktueller Wert: 20

Wollen Sie eine weitere Karte ziehen? (j/n)? n
Ihre endgueltige Hand:
  10 Herz
  Dame Pik
 ==> 20

▃▅▆█ 웃 2 █▆▅▃
Spieler 2 ist am Zug:
...
FAQ
Was ist das Ziel des Spiels? Das Ziel ist es, eine Hand mit einem Wert so nahe wie möglich an 21 zu erreichen, ohne diesen Wert zu überschreiten.

Was passiert, wenn ich über 21 komme? Wenn Ihre Hand einen Wert von mehr als 21 erreicht, haben Sie sich überkauft und verlieren automatisch die Runde.

Entwick dokumentation
Einführung
Diese Entwicklerdokumentation bietet eine detaillierte Übersicht über die technische Umsetzung der Blackjack-Simulation. Sie beschreibt die Struktur des Codes, die verwendeten Funktionen und die wichtigsten Implementierungsdetails.

Projektstruktur
plaintext
Blackjack-Simulation/
├── src/
│   ├── main.c
│   └── helper.c
├── docs/
│   ├── entwicklerdokumentation.md
│   └── anwenderdokumentation.md
├── Makefile
├── README.md
└── .gitignore
Quellcode
Der gesamte Quellcode befindet sich im src/ Verzeichnis. Die wichtigsten Dateien sind:

main.c: Enthält die Hauptlogik des Spiels, einschließlich der Spielschleife, Spielereingabe und Kartenausgabe.

helper.c: Enthält Hilfsfunktionen zur Unterstützung der Hauptlogik.

Wichtige Funktionen
*initializeDeck(Card deck): Initialisiert das Kartendeck mit den 52 Standardkarten, multipliziert mit der Anzahl der Decks.

*shuffleDeck(Card deck): Mischt das Kartendeck mit einem einfachen Zufallsalgorithmus.

choosePlayer(): Fragt die Anzahl der Spieler ab und validiert die Eingabe.

*dealCards(Card deck, int numPlayers, Card players[MAX_PLAYERS+1][TOTAL_CARDS], Card dealer[2]): Teilt die ersten zwei Karten an die Spieler und den Dealer aus.

**playerTurn(Card *deck, int cardIndex, Card player[], int playerCardCount): Führt den Zug eines Spielers durch, wobei der Spieler entscheiden kann, ob er weitere Karten ziehen möchte.

**dealerTurn(Card *deck, int cardIndex, Card dealer[], int dealerCardCount): Führt den Zug des Dealers durch, wobei der Dealer Karten zieht, bis der Wert seiner Hand mindestens 17 erreicht.

handValue(Card hand[], int numCards): Berechnet den aktuellen Wert einer Hand, wobei Asse flexibel als 1 oder 11 gezählt werden.

Beispiele und Tests
Die Datei main.c enthält Beispiele und Testfälle zur Verifizierung der Funktionen.

Abhängigkeiten
Standard-C-Bibliotheken (stdio.h, stdlib.h, time.h, stdbool.h, string.h)

Makefile zum Kompilieren des Projekts

Kompilierung und Ausführung
Um das Projekt zu kompilieren, verwenden Sie den Befehl make im Hauptverzeichnis. Dies generiert die ausführbare Datei blackjack, die Sie zur Ausführung des Spiels verwenden können.