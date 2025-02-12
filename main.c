#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

#define NUM_SUITS 4
#define NUM_RANKS 13
#define DECK_SIZE (NUM_SUITS * NUM_RANKS)
#define TOTAL_DECKS 6
#define TOTAL_CARDS (DECK_SIZE * TOTAL_DECKS)
#define MAX_PLAYERS 6
#define CARDS_PER_PLAYER 2

typedef struct {
    char *rank;
    char *suit;
    int value;
} Card;

const char *suits[NUM_SUITS] = {"Herz", "Karo", "Pik", "Kreuz"};
const char *ranks[NUM_RANKS] = {"2", "3", "4", "5", "6", "7", "8", "9", "10", "Bube", "Dame", "Koenig", "Ass"};
const int values[NUM_RANKS] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 10, 10, 10, 11};

void initializeDeck(Card *deck) {
    int cardIndex = 0;
    for (int d = 0; d < TOTAL_DECKS; d++) {
        for (int i = 0; i < NUM_SUITS; i++) {
            for (int j = 0; j < NUM_RANKS; j++) {
                deck[cardIndex].suit = (char *)suits[i];
                deck[cardIndex].rank = (char *)ranks[j];
                deck[cardIndex].value = values[j];
                cardIndex++;
            }
        }
    }
}

void shuffleDeck(Card *deck) {
    for (int i = 0; i < TOTAL_CARDS; i++) {
        int randomIndex = rand() % TOTAL_CARDS;
        Card temp = deck[i];
        deck[i] = deck[randomIndex];
        deck[randomIndex] = temp;
    }
}

int choosePlayer() {
    int numPlayers;
    printf("Geben Sie die Anzahl der Spieler ein: ");
    scanf("%d", &numPlayers);
    if (numPlayers < 1 || numPlayers > MAX_PLAYERS) {
        printf("Die maximale Spieleranzahl ist %d.\n", MAX_PLAYERS);
        exit(0);
    }
    return numPlayers;
}

void dealCards(Card *deck, int numPlayers, Card players[MAX_PLAYERS][TOTAL_CARDS], Card dealer[2]) {
    int cardIndex = 0;
    for (int player = 0; player < numPlayers; player++) {
        printf("Spieler %d:\n", player + 1);
        for (int card = 0; card < CARDS_PER_PLAYER; card++) {
            players[player][card] = deck[cardIndex];
            printf("  %s %s\n", deck[cardIndex].rank, deck[cardIndex].suit);
            cardIndex++;
        }
        printf("\n");
    }

    printf("Dealer:\n");
    for (int card = 0; card < CARDS_PER_PLAYER; card++) {
        dealer[card] = deck[cardIndex];
        if (card == 0) {
            printf("  Verdeckte Karte\n");
        } else {
            printf("  %s %s\n", deck[cardIndex].rank, deck[cardIndex].suit);
        }
        cardIndex++;
    }
    printf("\n");
}

int handValue(Card hand[], int numCards) {
    int value = 0;
    int numAces = 0;
    for (int i = 0; i < numCards; i++) {
        value += hand[i].value;
        if (hand[i].value == 11) {
            numAces++;
        }
    }
    while (value > 21 && numAces > 0) {
        value -= 10;
        numAces--;
    }
    return value;
}

void printHand(Card hand[], int numCards) {
    for (int i = 0; i < numCards; i++) {
        printf("%s %s\n", hand[i].rank, hand[i].suit);
    }
}

void playerTurn(Card *deck, int *cardIndex, Card player[], int *playerCardCount) {
    char choice;
    bool ziehen = true;

    while (ziehen) {
        int currentHandValue = handValue(player, *playerCardCount);

        // Display the current hand and value
        printf("Ihre Hand:\n");
        printHand(player, *playerCardCount);
        printf("Aktueller Wert: %d\n", currentHandValue);

        // If the player's hand is already 21 or more, they can't draw further.
        if (currentHandValue >= 21) {
            break;
        }

        // Ask if the player wants to draw a new card
        printf("Wollen Sie eine weitere Karte ziehen? (j/n): ");
        scanf(" %c", &choice);

        if (choice == 'j') {
            // Draw a card and increment the count
            player[*playerCardCount] = deck[*cardIndex];
            (*playerCardCount)++;
            (*cardIndex)++;

            // After drawing a card, check if the player's hand value is 21 or more.
            currentHandValue = handValue(player, *playerCardCount);
            if (currentHandValue >= 21) {
                break;
            }
        } else if (choice == 'n') {
            ziehen = false;
        } else {
            printf("Bitte geben Sie eine richtige Eingabe ein!\n");
        }
    }

    // After the loop ends, print final hand and value
    printf("Ihre endgueltige Hand:\n");
    printHand(player, *playerCardCount);
    printf("Endgueltiger Wert: %d\n", handValue(player, *playerCardCount));
}



void dealerTurn(Card *deck, int *cardIndex, Card dealer[], int *dealerCardCount) {
    printf("Hand des Dealers:\n");
    printHand(dealer, *dealerCardCount);
    printf("Aktueller Wert: %d\n", handValue(dealer, *dealerCardCount));
    while (handValue(dealer, *dealerCardCount) < 17) {
        dealer[*dealerCardCount] = deck[*cardIndex];
        (*dealerCardCount)++;
        (*cardIndex)++;
        printf("Dealer zieht eine Karte:\n");
        printHand(dealer, *dealerCardCount);
        printf("Aktueller Wert: %d\n", handValue(dealer, *dealerCardCount));
    }
}

int main() {
    Card deck[TOTAL_CARDS];
    srand(time(NULL));

    initializeDeck(deck);
    shuffleDeck(deck);

    int numPlayers = choosePlayer();

    Card players[MAX_PLAYERS][TOTAL_CARDS];
    Card dealer[CARDS_PER_PLAYER];
    int dealerCardCount = CARDS_PER_PLAYER;

    dealCards(deck, numPlayers, players, dealer);

    for (int player = 0; player < numPlayers; player++) {
        printf("Spieler %d ist am Zug:\n", player + 1);
        int playerCardCount = CARDS_PER_PLAYER;
        playerTurn(deck, &(int){CARDS_PER_PLAYER * numPlayers + CARDS_PER_PLAYER}, players[player], &playerCardCount);
        printf("\n");
    }

    printf("Dealer ist am Zug:\n");
    dealerTurn(deck, &(int){CARDS_PER_PLAYER * numPlayers + CARDS_PER_PLAYER}, dealer, &dealerCardCount);

    for (int player = 0; player < numPlayers; player++) {
        int playerValue = handValue(players[player], TOTAL_CARDS);
        int dealerValue = handValue(dealer, dealerCardCount);
        printf("Spieler %d hat %d Punkte.\n", player + 1, playerValue);
        if (playerValue > 21) {
            printf("Spieler %d hat überkauft.\n", player + 1);
        } else if (dealerValue > 21) {
            printf("Dealer hat überkauft. Spieler %d gewinnt.\n", player + 1);
        } else if (playerValue > dealerValue) {
            printf("Spieler %d gewinnt.\n", player + 1);
        } else if (playerValue < dealerValue) {
            printf("Dealer gewinnt gegen Spieler %d.\n", player + 1);
        } else {
            printf("Spieler %d und der Dealer haben unentschieden.\n", player + 1);
        }
        printf("\n");
    }

    return 0;
}
