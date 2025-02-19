#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <string.h>


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
    Text Styling
*/
#define TEXT_BOLD_UNDERLINE "\e[1;4m"
#define TEXT_BOLD "\e[1m"
#define TEXT_GREEN "\e[32m"
#define TEXT_YELLOW "\e[33m"
#define TEXT_RED "\e[31m"
#define TEXT_BLINKING "\e[5m"
#define TEXT_RESET "\e[0m"

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
void dealCards(Card *deck, int numPlayers, Card players[MAX_PLAYERS+1][TOTAL_CARDS], Card dealer[2]) {
    int cardIndex = 0;
    for (int player = 0; player < numPlayers; player++) {
        printf("-- Spieler %d:\n", player + 1);
        for (int card = 0; card < CARDS_PER_PLAYER; card++) {
            players[player][card] = deck[cardIndex];
            printf("  %s %s\n", deck[cardIndex].rank, deck[cardIndex].suit);
            cardIndex++;
        }
        printf("\n");
    }

    printf("════════════════════\n");
    printf("Dealer:\n");
    for (int card = 0; card < CARDS_PER_PLAYER; card++) {
        dealer[card] = deck[cardIndex];
        if (card == 0) {
            printf(TEXT_RED "  (?)\n" TEXT_RESET);
        } else {
            printf("  %s %s\n", deck[cardIndex].rank, deck[cardIndex].suit);
        }
        cardIndex++;
    }
    printf("════════════════════\n\n");
}

/*
    Check if the value of the hand is 21
*/
bool blackjack(int value) {
    return value == 21;
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
        printf("  %s %s\n", hand[i].rank, hand[i].suit);
    }
}

/*
    Player's turn
    The player can decide to draw another card or to stop
    If the value of the hand is over 21, the player's turn is over
*/
void playerTurn(Card *deck, int *cardIndex, Card player[], int *playerCardCount) {
    char choice[10];
    bool ziehen = true;

    while (ziehen) {
        int currentHandValue = handValue(player, *playerCardCount);

        // Display the current hand and value
        printf("Ihre Hand:\n");
        printHand(player, *playerCardCount);
        printf(TEXT_RESET TEXT_BOLD "Aktueller Wert: %d\n\n" TEXT_RESET, currentHandValue);

        // If the player's hand is already 21 or more, they can't draw further.        
        if (currentHandValue >= 21) {
            break;
        }

        // Ask if the player wants to draw a new card
        printf("Wollen Sie eine weitere Karte ziehen? (j/n): ");
        if (fgets(choice, sizeof(choice), stdin) == NULL) {
            printf("Fehler bei der Eingabe.\n");
            continue;
        }

        // Zeilenumbruch entfernen
        choice[strcspn(choice, "\n")] = '\0';

        if (strcmp(choice, "j") == 0) {
            // Karte ziehen und Zähler erhöhen
            player[*playerCardCount] = deck[*cardIndex];
            (*playerCardCount)++;
            (*cardIndex)++;

            // Handwert erneut berechnen
            currentHandValue = handValue(player, *playerCardCount);
            if (currentHandValue >= 21) {
                break;
            }
        } else if (strcmp(choice, "n") == 0) {
            ziehen = false;  // Spieler möchte keine Karte mehr
        } else {
            printf("Bitte geben Sie eine richtige Eingabe ein!\n");
        }
    }

    // After the loop ends, print final hand and value
    printf("Ihre endgueltige Hand:\n");
    printHand(player, *playerCardCount);
    printf(TEXT_RESET TEXT_BOLD " ==> %d\n", handValue(player, *playerCardCount));

    if (blackjack(handValue(player, *playerCardCount))) {
        printf(TEXT_RESET TEXT_GREEN TEXT_BLINKING "Blackjack!" TEXT_RESET "\n");
    }
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
    bool playing = true;

    Card deck[TOTAL_CARDS];
    srand(time(NULL));

    // call function `initializeDeck` to initialize the deck of cards
    initializeDeck(deck);

    // call function `choosePlayer` to choose the number of players
    int numPlayers = choosePlayer();

    // initialize the players and the dealer
    Card players[MAX_PLAYERS+1][TOTAL_CARDS];
    Card dealer[CARDS_PER_PLAYER];
    int dealerCardCount = CARDS_PER_PLAYER;

    while (playing) {
        // shuffle deck new every time
        shuffleDeck(deck);

        printf("\n────────────────────\n");

        // call function `dealCards` to deal the first 2 cards to the players and the dealer
        dealCards(deck, numPlayers, players, dealer);

        // Players' turns
        for (int player = 0; player < numPlayers; player++) {
            printf("▃▅▆█ 웃 %d █▆▅▃\n", player + 1);
            printf(TEXT_BOLD_UNDERLINE "Spieler %d ist am Zug:\n" TEXT_RESET, player + 1);
            int playerCardCount = CARDS_PER_PLAYER;
            playerTurn(deck, &(int){CARDS_PER_PLAYER * numPlayers + CARDS_PER_PLAYER}, players[player], &playerCardCount);
            printf("\n");
        }

        // Dealer's turn
        printf("Dealer ist am Zug:\n");
        dealerTurn(deck, &(int){CARDS_PER_PLAYER * numPlayers + CARDS_PER_PLAYER}, dealer, &dealerCardCount);

        printf("\n\n");
        printf("════════════════════\n\n");

        // Determine the winner
        for (int player = 0; player < numPlayers; player++) {
            int playerValue = handValue(players[player], TOTAL_CARDS);
            int dealerValue = handValue(dealer, dealerCardCount);
            printf("Spieler %d hat " TEXT_BOLD_UNDERLINE "%d Punkte%s" TEXT_RESET ".\n", player + 1, playerValue, blackjack(playerValue) ? TEXT_RESET " (Blackjack)" : "");
            if (playerValue > 21) {
                printf("Spieler %d hat " TEXT_RED "ueberkauft.\n" TEXT_RESET, player + 1);
            } else if (dealerValue > 21) {
                printf("Dealer hat ueberkauft. Spieler %d " TEXT_GREEN "gewinnt.\n" TEXT_RESET, player + 1);
            } else if (playerValue > dealerValue) {
                printf("Spieler %d " TEXT_GREEN "gewinnt.\n" TEXT_RESET, player + 1);
            } else if (playerValue < dealerValue) {
                printf(TEXT_RED "Dealer gewinnt gegen Spieler %d.\n" TEXT_RESET, player + 1);
            } else {
                printf("Spieler %d und der Dealer haben " TEXT_YELLOW "unentschieden.\n" TEXT_RESET, player + 1);
            }
            printf("\n");
        }

        printf("════════════════════\n");

        char choice = '\0';
        while (choice != 'j' && choice != 'n') {
            printf("\nWollen Sie erneut spielen? (j/n) ");
            scanf(" %c", &choice);

            if (choice == 'j') {
                playing = true;
            } else if (choice == 'n') {
                playing = false;
            } else {
                printf("Bitte geben Sie eine Eingabe ein!\n");
                choice = '\0';
            }
        }
    }

    printf("\n\n═════════════════════════════════════\n");
    printf("  ❤️ Vielen Dank für's spielen! ❤️");
    printf("\n═════════════════════════════════════\n\n");
    return 0;
}
