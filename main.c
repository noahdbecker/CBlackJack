#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/*
    Initialize constants for the deck of cards
*/
#define NUM_SUITS 4
#define NUM_RANKS 13
#define DECK_SIZE (NUM_SUITS * NUM_RANKS)
#define TOTAL_DECKS 6
#define TOTAL_CARDS (DECK_SIZE * TOTAL_DECKS)
#define MAX_PLAYERS 6
#define CARDS_PER_PLAYER 2

/*
    Define the structure of a card
*/
typedef struct {
    char *rank;
    char *suit;
    int value;
} Card;

/*
    Initialize the deck of cards
*/
const char *suits[NUM_SUITS] = {"Herz", "Karo", "Pik", "Kreuz"};
const char *ranks[NUM_RANKS] = {"2", "3", "4", "5", "6", "7", "8", "9", "10", "Bube", "Dame", "Koenig", "Ass"};
const int values[NUM_RANKS] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 10, 10, 10, 11};

/*
    Generating the decks
*/
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

/*
    Shuffling the cards by assigning each card a random generated index in the set of cards
*/
void shuffleDeck(Card *deck) {
    for (int i = 0; i < TOTAL_CARDS; i++) {
        int randomIndex = rand() % TOTAL_CARDS;
        Card temp = deck[i];
        deck[i] = deck[randomIndex];
        deck[randomIndex] = temp;
    }
}

/*
    Initiating the numbers of players
    < 1 and > the amount of maximum players (6) isn't possible
*/
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

/*
    Dealing the first 2 cards to the players and the dealer
    Cards of the player are shown, the dealer's second card is hidden
*/
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

/*
    Calculating the value of the hand
    Aces are counted as 11, if the value of the hand is over 21, the value of the ace is changed to 1
*/
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

/*
    Printing the hand of the player
*/
void printHand(Card hand[], int numCards) {
    for (int i = 0; i < numCards; i++) {
        printf("%s %s\n", hand[i].rank, hand[i].suit);
    }
}

/*
    Player's turn
    The player can decide to draw another card or to stop
    If the value of the hand is over 21, the player's turn is over
*/
void playerTurn(Card *deck, int *cardIndex, Card player[], int *playerCardCount) {
    char choice;
    do {
        printf("Ihre Hand:\n");
        printHand(player, *playerCardCount);
        printf("Aktueller Wert: %d\n", handValue(player, *playerCardCount));
        if (handValue(player, *playerCardCount) >= 21) {
            break;
        }
        printf("Wollen Sie eine weitere Karte ziehen? (j/n): ");
        scanf(" %c", &choice);
        if (choice == 'j') {
            player[*playerCardCount] = deck[*cardIndex];
            (*playerCardCount)++;
            (*cardIndex)++;
        }
    }
    while (choice == 'j');
}

/*
    Dealer's turn
    The dealer has to draw cards until the value of the hand is at least 17
*/
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

/*
    Main function
    The game is played by the players and the dealer
    The winner is determined by the value of the hand
*/
int main() {
    Card deck[TOTAL_CARDS];
    srand(time(NULL));

    // call function `initializeDeck` to initialize the deck of cards
    initializeDeck(deck);
    shuffleDeck(deck);

    // call function `choosePlayer` to choose the number of players
    int numPlayers = choosePlayer();

    // initialize the players and the dealer
    Card players[MAX_PLAYERS][TOTAL_CARDS];
    Card dealer[CARDS_PER_PLAYER];
    int dealerCardCount = CARDS_PER_PLAYER;

    // call function `dealCards` to deal the first 2 cards to the players and the dealer
    dealCards(deck, numPlayers, players, dealer);

    // Players' turns
    for (int player = 0; player < numPlayers; player++) {
        printf("Spieler %d ist am Zug:\n", player + 1);
        int playerCardCount = CARDS_PER_PLAYER;
        playerTurn(deck, &(int){CARDS_PER_PLAYER * numPlayers + CARDS_PER_PLAYER}, players[player], &playerCardCount);
        printf("\n");
    }

    // Dealer's turn
    printf("Dealer ist am Zug:\n");
    dealerTurn(deck, &(int){CARDS_PER_PLAYER * numPlayers + CARDS_PER_PLAYER}, dealer, &dealerCardCount);

    // Determine the winner
    for (int player = 0; player < numPlayers; player++) {
        int playerValue = handValue(players[player], TOTAL_CARDS);
        int dealerValue = handValue(dealer, dealerCardCount);
        printf("Spieler %d hat %d Punkte.\n", player + 1, playerValue);
        if (playerValue > 21) {
            printf("Spieler %d hat ueberkauft.\n", player + 1);
        } else if (dealerValue > 21) {
            printf("Dealer hat ueberkauft. Spieler %d gewinnt.\n", player + 1);
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
