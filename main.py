import random

class Blackjack:
    def __init__(self):
        self.player_cards = [random.randint(2, 13), random.randint(2, 13)]
        self.dealer_cards = [random.randint(2, 13), random.randint(2, 13)]
        self._adjust_face_cards()

    def _adjust_face_cards(self):
        for i in range(len(self.player_cards)):
            if self.player_cards[i] > 13:
                self.player_cards[i] -= 13
        for i in range(len(self.dealer_cards)):
            if self.dealer_cards[i] > 13:
                self.dealer_cards[i] -= 13
        self._convert_face_cards()

    def _convert_face_cards(self):
        for i in range(len(self.player_cards)):
            if self.player_cards[i] > 10:
                self.player_cards[i] = 10
        for i in range(len(self.dealer_cards)):
            if self.dealer_cards[i] > 10:
                self.dealer_cards[i] = 10

    def show_cards(self):
        print(f"P1: {self.player_cards} - Summe: {sum(self.player_cards)}")
        print(f"Dealer: [{self.dealer_cards[0]}, ?]")

    def hit(self):
        card = random.randint(2, 13)
        card = 10 if card > 10 else card  # Bildkarten als 10 zählen
        self.player_cards.append(card)
        return sum(self.player_cards)

    def dealer_turn(self):
        while sum(self.dealer_cards) < 17:
            card = random.randint(2, 13)
            card = 10 if card > 10 else card
            self.dealer_cards.append(card)
        print(f"Dealer: {self.dealer_cards} - Summe: {sum(self.dealer_cards)}")

    def play(self):
        self.show_cards()

        while sum(self.player_cards) <= 21:
            print("Hit or Stand? (h/s)")
            choice = input().strip().lower()
            if choice == 'h':
                player_total = self.hit()
                print(f"P1: {self.player_cards} - Summe: {player_total}")
                if player_total > 21:
                    print("Bust! You Lose!")
                    return
            elif choice == 's':
                break
            else:
                print("Invalid Input! Please choose 'h' for Hit or 's' for Stand.")

        self.dealer_turn()

        player_total = sum(self.player_cards)
        dealer_total = sum(self.dealer_cards)

        if dealer_total > 21:
            print("Dealer Bust! You Win!")
        elif player_total > dealer_total:
            print("You Win!")
        elif player_total < dealer_total:
            print("You Lose!")
        else:
            print("It's a Tie!")


def main():
    while True:
        game = Blackjack()
        game.play()
        print("Möchten Sie ein weiteres Spiel starten? Geben Sie 'end' ein, um zu beenden oder drücken Sie Enter, um fortzufahren.")
        choice = input().strip().lower()
        if choice == 'end':
            break

# Hauptprogramm
if __name__ == "__main__":
    main()
