# Das (Glücks-)Spiel Blackjack in C



<table>
<tr><th>Autoren</th><td>Jan Kruske, Felix Schulz, Noah Becker</td></tr>
<tr><th>Dozent</th><td>Dr. Jonas Fritzsch</td></tr>
<tr><th>Studiengang/Kurs</th><td>B. Sc. Informatik – Cyber Security, TINF24F</td></tr>
<tr><th>Anlass</th><td>Entwurf und Implementierung einer Anwendung in C als Programmierprojekt</td></tr>
<tr><th>Bearbeitungszeitraum</th><td>05.02.25 - 24.02.2025</td></tr>
<tr><th>Abgabedatum</th><td>24.02.2025</td></tr>
<tr><th>Präsentation</th><td>26.02.2025</td></tr>
</table>

## Inhaltsverzeichnis
1. [Projektbeschreibung](#projektbeschreibung)
2. [Installation](#installation)
3. [Anwenderdokumentation](#anwenderdokumentation)
4. [Entwicklerdokumentation](#entwicklerdokumentation)

## Projektbeschreibung
Dieses Projekt simuliert das Kartenspiel Blackjack. Es ermöglicht bis zu 6 Spielern, gegen einen Dealer anzutreten. Spieler können Wetteinsätze zwischen 2 € und 500 € platzieren. Das Spiel folgt den klassischen Blackjack-Regeln, wobei der Spieler gewinnt, indem er näher an 21 kommt als der Dealer, ohne den Wert zu überschreiten. Der Dealer trifft Entscheidungen basierend auf einem eigenem Algorythmus, welcher vorgibt eine Karte nachzuziehen, wenn der Wert der eigenen Karten unter 17 liegt.

## Anwenderdokumentation
### Installation
1. **Repository klonen**:
   ```bash
   git clone https://github.com/noahdbecker/CBlackJack.git
2. **In das Projektverzeichnis navigieren**:
   ```bash
   cd CBlackJack
3. **Main.exe ausführen**:
   ```bash
   main.exe

### Spielverlauf
Nach dem Ausführen der Main.exe muss die Anzahl der Spieler und Spielerbots von 1 bis 6 festgelegt werden. Danach hat jeder Spieler die Möglichkeit seinen Einsatz zwischen 2€ und 500€ zu platzieren. Jeder Spieler bekommt jetzt seine Spielkarten zugeteilt und Spieler 1 darf sich entscheiden, ob er mit der HIT-Aktion ('j') noch eine weitere Karte ziehen will oder, ob er mit der Stand-Aktion ('n') seinen Zug beendet und seine Karten behält. Sofern der Spieler durch das Ziehen einer Karte den Wert 21 überschreitet, wird findet ein BUST statt, durch welchen der Spieler ausscheidet. 

Dies findet für die Anzahl der ausgewählten Spieler statt. Nun beginnt der Dealer so lange zu ziehen, bis er den Wert 17 überschreitet. Daraufhin wird ausgewertet, welcher der Spieler näher an der 21 dran sind, als der Dealer und welche Spieler den Wert überkauft haben. Gewinnt ein Spieler gegen den Dealer, erhält er seinen Einsatz verdoppelt zurück. Im Fall eines Gleichstands erhält der Spieler seinen Einsatz zurück. Bei einem Verlust verliert der Spieler seinen Einsatz. Jederzeit besteht die Möglichkeit durch einen RESET das Spiel zu beenden.

Sollte der Compiler die Ausgabe des Textstyles nicht richtig darstellen, besteht die Möglichkeit auf das Safetyfile zuzugreifen, welches auf die Textstyles verzichtet, um dem Nutzer in jedem Fall ein angenehmes Spielerlebnis zu garantieren.

## Entwicklerdokumentation
Zu Beginn werden die nötigen Bobliotheken importiert, die Konstanten definiert und die Text-Styles eingeführt. Anschließend werden Kartendecks erstellt und gemischt. Die Abfrage erfolgt, um die Anzahl der Spieler und die Anzahl der Spielerbots, falls möglich, hinzuzufügen. 

Es folgt die Initialisierung der 'balance' für jeden Spieler und Spielerbot. Jeder Spieler wird durchiteriert und bekommt seine Karten zugewiesen. Anschließend werden dem Dealer auch zwei Karten zugewiesen und alle Karten werden auf der Konsole ausgegeben, die zweite Karte des Dealers bleibt verborgen.
Es folgt die erste Prüfung, ob der Wert der Karten schon bei 21 oder drüber ists, sodass ein BUST vorliegt.

Der Wert der Karten wird so berechnet, dass, sollte der Spieler ein Ass haben, der Wert zugunsten des Spielers entweder den Wert 1 oder 11 annimmt. Der Wert wird anschlißend auf den Konsole ausgegeben.

Der Spielzug der Spieler ist so aufgebaut, dass, angefangen bei Spieler 1, jeder die Möglichkeit hat, sich für HIT ('j') oder STAND ('n') zu entscheiden und somit noch eine weitere Karte zu erhalten oder den Spielzug zu beenden. Es wird geprüft, ob der Spieler hierbei einen BUST erreicht und der Spielzug beendet.

Sobald jeder Spieler seine Züge beendet hat, wird die finale Hand der Spieler auf der Konsole ausgegeben und der Dealer beginnt mit seinem Spielzug. Er zieht neue Karten bis er den Wert 17 überschreitet und beendet dann seinen Spielzug. Zuletzt wird ausgewertet, welcher Spieler näher an der 21 dran ist als der Dealer.

Die Main-Funktion initialisiert die Karten, Spieler und die 'balance', bestimmt den Spielverlauf und prüft am Ende, ob der Spieler ein neues Spiel starten möchte.

Aufgrund der Nutzung verschiedener Textstyles die nicht mit jedem Compiler funktionieren, wurde ein alternatives Safetyfile erstellt, das auf die aufwendigen Textstyles verzichtet, um Fehlercodes zu vermeiden und eine saubere Ausgabe zu erzeugen.
