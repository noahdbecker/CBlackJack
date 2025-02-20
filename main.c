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
#define BALANCE_PER_PLAYER 5000

/*
    Text Styling
*/
#define TEXT_UNDERLINE "\x1b[4m"
#define TEXT_BOLD_UNDERLINE "\x1b[1;4m"
#define TEXT_BOLD "\x1b[1m"
#define TEXT_GREEN "\x1b[32m"
#define TEXT_YELLOW "\x1b[33m"
#define TEXT_RED "\x1b[31m"
#define TEXT_BLINKING "\x1b[5m"
#define TEXT_RESET "\x1b[0m"

/*
    Input validator (y/n)
*/
bool inputValidation(char *input) {
    return (strlen(input) == 1 && (*input == 'j' || *input == 'n'));
}


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
    char input[20];
    int numPlayers;

    bool validInput = false;

    while (!validInput) {
        printf("Geben Sie die Anzahl der Spieler ein: ");
        fgets(input, sizeof(input), stdin);
        numPlayers = strtol(input, NULL, 10);

        if (numPlayers > 0 && numPlayers < MAX_PLAYERS) {
            validInput = true;
        } else if (numPlayers < 1 || numPlayers > MAX_PLAYERS) {
            printf("Die Spieleranzahl muss zwischen 1 und %d liegen.\n", MAX_PLAYERS);
        }
    }

    return numPlayers;
}
// Initialize every player balance

void playerBalance(int numPlayers) {
    for (int i = 1; i <= numPlayers; i++) {
        int Balance[numPlayers][3] = { i, BALANCE_PER_PLAYER,0 };
    }
}
void playerBet(int Balance[][], int numPlayers) {
    char input[20];
    for (int player = 1; player < numPlayers; player++) {
        printf("Wie viel Geld wollen sie setzen Spieler %d?", player);
        fgets(input, sizeof(input), stdin);
        Balance[player-1][2] = strtol(input, NULL, 10);
        printf("Sie haben %d€ gesetzt.", Balance[player-1][2]);
    }

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
    Check if the value of the final hand is over 21 (bust)
*/
bool checkBust(int value)
{
    return value > 21;
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
    bool ziehen = true;

    while (ziehen) {
        int currentHandValue = handValue(player, *playerCardCount);

        // Display the current hand and value
        printf("Ihre Hand:\n");
        printHand(player, *playerCardCount);
        printf(TEXT_RESET TEXT_BOLD "Aktueller Wert: %d\n\n" TEXT_RESET, currentHandValue);

        // If the player's hand is already 21 or more, they can't draw further.        
        if (blackjack(currentHandValue) || checkBust(currentHandValue)) {
            break;
        }

        char furtherCards[10];
        bool validInput = false;

        while (!validInput) {
            printf("Wollen Sie eine weitere Karte ziehen? (j/n)? ");
            scanf("%9s", furtherCards);
            while (getchar() != '\n');

            if (inputValidation(furtherCards)) {
                validInput = true;

                if (furtherCards[0] == 'j') {
                    // draw card and increase card count
                    player[*playerCardCount] = deck[*cardIndex];
                    (*playerCardCount)++;
                    (*cardIndex)++;

                    // calculate hand value
                    currentHandValue = handValue(player, *playerCardCount);
                    if (checkBust(currentHandValue)) {
                        break;
                    }
                } else if (furtherCards[0] == 'n') {
                    ziehen = false;  // player don't want new card
                }
            } else {
                printf("Bitte geben Sie eine gültige Eingabe ('j' oder 'n') ein!\n");
            }
        }
    }

    // After the loop ends, print final hand and value
    printf("Ihre endgueltige Hand:\n");
    printHand(player, *playerCardCount);
    printf(TEXT_RESET TEXT_BOLD " ==> %d\n", handValue(player, *playerCardCount));

    if (blackjack(handValue(player, *playerCardCount))) {
        printf(TEXT_RESET TEXT_GREEN TEXT_BLINKING "Blackjack!" TEXT_RESET "\n");
    }

    if (checkBust(handValue(player, *playerCardCount)))
    {
        printf(TEXT_RESET TEXT_RED "BUST!" TEXT_RESET "\n");
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
//     The winner is determined by the value of the hand
void determineWinner(Card players[MAX_PLAYERS + 1][TOTAL_CARDS], int numPlayers, Card dealer[], int dealerCardCount) {
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
}
/*
    Main function
    The game is played by the players and the dealer
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
    playerBalance(numPlayers);

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

        // Dealer summary
        printf("*****************\n");
        printf(TEXT_RESET "Dealer: " TEXT_BOLD_UNDERLINE "%d Punkte\n" TEXT_RESET, handValue(dealer, dealerCardCount));
        printf("*****************\n\n");

        // Determine the winner
        determineWinner(players, numPlayers, dealer, dealerCardCount);


        printf("════════════════════\n");


        /*
            play again?
        */
        char input[10];
        bool validInput = false;

        while (!validInput) {
            printf("Wollen Sie erneut spielen (j/n)? ");
            scanf("%9s", input);
            while (getchar() != '\n');

            if (inputValidation(input)) {
                playing = (input[0] == 'j');
                validInput = true;
            } else {
                printf("Ungültige Eingabe! Bitte geben Sie entweder 'j' oder 'n' ein.\n");
            }
        }
    }

    /*
        thx for playing ❤️
    */
    printf("\n\n═════════════════════════════════════\n");
    printf("  ❤️ Vielen Dank für's spielen! ❤️");
    printf("\n═════════════════════════════════════\n\n");
    return 0;
}
