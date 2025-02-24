#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>


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
#define MIN_BET 2
#define MAX_BET 500

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
    config for players (player count and bot count)
*/
typedef struct {
    int numPlayers;
    int numBots;
} PlayerConfig;

PlayerConfig choosePlayer() {
    char input[20];
    int numPlayers = 0, numBots = 0;
    bool validInput = false;

    while (!validInput) {
        printf("Geben Sie die Anzahl der Spieler ein (1-%d): ", MAX_PLAYERS);
        fgets(input, sizeof(input), stdin);
        numPlayers = strtol(input, NULL, 10);

        if (numPlayers < 1 || numPlayers > MAX_PLAYERS) {
            printf("Ungueltige Eingabe! Die Spieleranzahl muss zwischen 1 und %d liegen.\n", MAX_PLAYERS);
            continue;
        }

        // if space
        if (numPlayers < MAX_PLAYERS) {
            printf("Wollen Sie noch Bots hinzufuegen? (Maximal %d moeglich): ", MAX_PLAYERS - numPlayers);
            fgets(input, sizeof(input), stdin);
            numBots = strtol(input, NULL, 10);

            if (numBots < 0 || (numPlayers + numBots) > MAX_PLAYERS) {
                printf("Ungueltige Anzahl von Bots! Maximal %d Bots erlaubt.\n", MAX_PLAYERS - numPlayers);
                continue;
            }
        }

        validInput = true;
    }

    // return player config struct
    PlayerConfig config = {numPlayers, numBots};
    return config;
}

// Initialize every player & bot balance
void Balance(int numPlayers, int numBots, int balancePlayers[][3]) {
    for (int i = 0; i < numPlayers+numBots; i++) {
        balancePlayers[i][0] = i + 1;  // Player-ID
        balancePlayers[i][1] = BALANCE_PER_PLAYER; //  Balance
        balancePlayers[i][2] = 0;  //  Bet
    }
}

// Initialize every bet
void playerBet(int balancePlayer[][3], int numPlayers, int numBots) {
    char input[20];

    // players place their bets
    for (int player = 0; player < numPlayers; player++) {
        do {
            printf("Wie viel Geld wollen Sie setzen, Spieler %d? (%d€ - %d€) ", player + 1, MIN_BET, MAX_BET);
            fgets(input, sizeof(input), stdin);
            input[strcspn(input, "\n")] = 0;
            balancePlayer[player][2] = strtol(input, NULL, 10);

            if (balancePlayer[player][2] < MIN_BET || balancePlayer[player][2] > MAX_BET || balancePlayer[player][2] > balancePlayer[player][1]) {
                printf("Ungueltiger Einsatz! Bitte setzen Sie zwischen %d€ und %d€, aber nicht mehr als Ihr Guthaben (%d€).\n", MIN_BET, MAX_BET, balancePlayer[player][1]);
            }
        } while (balancePlayer[player][2] < MIN_BET || balancePlayer[player][2] > MAX_BET || balancePlayer[player][2] > balancePlayer[player][1]);

        printf("Sie haben %d€ gesetzt.\n", balancePlayer[player][2]);
    }

    // bots place random bets
    for (int bot = numPlayers; bot < numBots + numPlayers; bot++) {
        if (balancePlayer[bot][1] < MIN_BET) {
            balancePlayer[bot][2] = 0;
            printf("Bot %d hat nicht genug Geld zum Setzen und setzt nichts.\n", bot + 1);
        } else {
            int maxBotBet = (balancePlayer[bot][1] < MAX_BET) ? balancePlayer[bot][1] : MAX_BET;
            balancePlayer[bot][2] = MIN_BET + rand() % (maxBotBet - MIN_BET + 1);
            printf("Bot %d hat %d€ gesetzt.\n", bot + 1, balancePlayer[bot][2]);
        }
    }
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
    Drawing cards and increasing the cardIndex
*/
void drawCard(Card *deck, int *cardIndex, Card hand[], int *cardCount) {
    hand[*cardCount] = deck[*cardIndex];
    (*cardCount)++;
    (*cardIndex)++;
}

/*
    Dealing the first 2 cards to the players and the dealer
    Cards of the player are shown, the dealer's second card is hidden
*/
void dealFirstCards(Card *deck, int numPlayers, int numBots, Card players[MAX_PLAYERS+1][TOTAL_CARDS], Card dealer[2], int playerCardCount[MAX_PLAYERS], int *cardIndex, int *dealerCardCount) {
    // Player
    for (int player = 0; player < (numPlayers + numBots); player++) {
        printf("-- Spieler %d:\n", player + 1);
        for (int card = 0; card < CARDS_PER_PLAYER; card++) {
            drawCard(deck, cardIndex, players[player], &playerCardCount[player]);
            printf("  %s %s\n", deck[*(cardIndex)-1].rank, deck[*(cardIndex)-1].suit);
        }
        printf("\n");
    }

    printf("Dealer:\n");
    for (int card = 0; card < CARDS_PER_PLAYER; card++) {
        drawCard(deck, cardIndex, dealer, dealerCardCount);
        if (card == 0) {
            printf(TEXT_RED "  (?)\n" TEXT_RESET);
        } else {
            printf("  %s %s\n", deck[*(cardIndex)-1].rank, deck[*(cardIndex)-1].suit);
        }
    }
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
    int aceCount = 0;

    for (int i = 0; i < numCards; i++) {
        if (strcmp(hand[i].rank, "Ass") == 0) {
            value += 11;
            aceCount++;
        } else if (strcmp(hand[i].rank, "Koenig") == 0 || strcmp(hand[i].rank, "Dame") == 0 || strcmp(hand[i].rank, "Bube") == 0) {
            value += 10;
        }
        else {
            value += atoi(hand[i].rank);
        }
    }

    while (value > 21 && aceCount > 0) {
        value -= 10;
        aceCount--;
    }

    return value;
}

/*
    Player's turn
    The player can decide to draw another card or to stop
    If the value of the hand is over 21, the player's turn is over
*/
void playerTurn(Card *deck, int *cardIndex, Card player[], int playerCardCount) {
    bool ziehen = true;

    while (ziehen) {
        int currentHandValue = handValue(player, playerCardCount);

        // Display the current hand and value
        printf("Ihre Hand:\n");
        printHand(player, playerCardCount);
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
                    drawCard(deck, cardIndex, player, &playerCardCount);

                    // calculate new hand value
                    currentHandValue = handValue(player, playerCardCount);
                    if (checkBust(currentHandValue)) {
                        break;
                    }
                } else if (furtherCards[0] == 'n') {
                    ziehen = false;  // player don't want new card
                }
            } else {
                printf("Bitte geben Sie eine gueltige Eingabe ('j' oder 'n') ein!\n");
            }
        }
    }

    // After the loop ends, print final hand and value
    printf("Ihre endgueltige Hand:\n");
    printHand(player, playerCardCount);
    printf(TEXT_RESET TEXT_BOLD " ==> %d\n", handValue(player, playerCardCount));

    if (blackjack(handValue(player, playerCardCount))) {
        printf(TEXT_RESET TEXT_GREEN TEXT_BLINKING "Blackjack!" TEXT_RESET "\n");
    }

    if (checkBust(handValue(player, playerCardCount)))
    {
        printf(TEXT_RESET TEXT_RED "BUST!" TEXT_RESET "\n");
    }
}

/*
    Bot turn
*/
void botTurn(Card *deck, int *cardIndex, Card bot[], int *botCardCount) {
        printf("Hand des Bots:\n");
        printHand(bot, *botCardCount);
        int botHandValue = handValue(bot, *botCardCount);
        printf("Aktueller Wert: %d\n", botHandValue);
        while (botHandValue < 17) {
            drawCard(deck, cardIndex, bot, botCardCount);
            printf("Aktueller Wert: %d\n", handValue(bot, *botCardCount));
            botHandValue = handValue(bot, *botCardCount);
            printf("Bot zieht eine Karte:\n");
            printHand(bot, *botCardCount);
            printf("Aktueller Wert: %d\n", handValue(bot, *botCardCount));
        }
}

/*
    Dealer's turn
    The dealer has to draw cards until the value of the hand is at least 17
*/
void dealerTurn(Card *deck, int *cardIndex, Card *dealer, int *dealerCardCount) {
    printf("Hand des Dealers:\n");
    printHand(dealer, *dealerCardCount);
    printf("Aktueller Wert: %d\n", handValue(dealer, *dealerCardCount));
    while (handValue(dealer, *dealerCardCount) < 17) {
        drawCard(deck, cardIndex, dealer, dealerCardCount);
        printf("Dealer zieht eine Karte:\n");
        printHand(dealer, *dealerCardCount);
        printf("Aktueller Wert: %d\n", handValue(dealer, *dealerCardCount));
    }
}
/*
    Determining the winner on basis of the hand value
*/
void determineWinner(Card players[MAX_PLAYERS + 1][TOTAL_CARDS], int numPlayers, int numBots, Card dealer[], int dealerCardCount, int playerCardCount[MAX_PLAYERS]) {
    int dealerValue = handValue(dealer, dealerCardCount);
    for (int player = 0; player < (numPlayers + numBots); player++) {
        int playerValue = handValue(players[player], playerCardCount[player]);
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


// Balance development
void balanceDevelopment(Card players[MAX_PLAYERS + 1][TOTAL_CARDS], int player, int balancePlayers[][3], Card *dealer, int dealerCardCount, int playerCardCount[MAX_PLAYERS]) {
    int playerValue = handValue(players[player], playerCardCount[player]);
    int dealerValue = handValue(dealer, dealerCardCount);
    int bet = balancePlayers[player][2];

    // check if player has blackjack (2 cards and value of 21)
    if (playerCardCount[player] == 2 && blackjack(playerValue)) {
        int gewinn = (int)round(bet * 1.5);  // pays 3 to 2
        balancePlayers[player][1] += gewinn;
        printf("Spieler %d hat ein BLACKJACK mit den ersten 2 Karten! Neue Balance: %d€ (Gewinn: +%d€)\n", player + 1, balancePlayers[player][1], gewinn);
    }
    // player wins against dealer
    else if (dealerValue > 21 || playerValue > dealerValue) {
        balancePlayers[player][1] += bet;  // pays double
        printf("Spieler %d gewinnt! Neue Balance: %d€ (+%d€)\n", player + 1, balancePlayers[player][1], bet);
    }
    // player looses
    else if (playerValue < dealerValue) {
        balancePlayers[player][1] -= bet;
        printf("Spieler %d verliert! Neue Balance: %d€ (-%d€)\n", player + 1, balancePlayers[player][1], bet);
    }
    // draw - money back
    else {
        balancePlayers[player][1] += bet;
        printf("Spieler %d unentschieden! Balance bleibt bei %d€ (Einsatz zurück)\n", player + 1, balancePlayers[player][1]);
    }
    // reset bet
    balancePlayers[player][2] = 0;
}



/*
    Reset Game
*/
void resetGame(Card players[MAX_PLAYERS+1][TOTAL_CARDS], int playerCardCount[MAX_PLAYERS], Card dealer[], int *dealerCardCount, int *cardIndex) {
    for (int i = 0; i <= MAX_PLAYERS; i++) {
        playerCardCount[i] = 0;
        for (int j = 0; j < TOTAL_CARDS; j++) {
            players[i][j].rank = NULL;
            players[i][j].suit = NULL;
            players[i][j].value = 0;
        }
    }

    *dealerCardCount = 0;
    *cardIndex = 0;
}

/*
    Main function
    The game is played by the players and the dealer
*/
int main() {
    srand(time(NULL));
    bool playing = true;

    Card deck[TOTAL_CARDS];
    initializeDeck(deck); // call function `initializeDeck` to initialize the deck of cards

    PlayerConfig config = choosePlayer(); // call function `choosePlayer` to choose the number of players
    int numPlayers = config.numPlayers;
    int numBots = config.numBots;
    int totalPlayers = numPlayers + numBots;



    Card players[MAX_PLAYERS+1][TOTAL_CARDS];
    Card dealer[20];
    int playerCardCount[MAX_PLAYERS] = {0};
    int dealerCardCount = 0;
    int cardIndex = 0;

    // array: balance for players
    int balancePlayers[totalPlayers][3];

    // init balance
    Balance(numPlayers, numBots, balancePlayers);


    while (playing) {
        // reset the game at the beginning
        resetGame(players, playerCardCount, dealer, &dealerCardCount, &cardIndex);

        // shuffle deck new every time
        shuffleDeck(deck);
        playerBet(balancePlayers, numPlayers, numBots);

        printf("\n────────────────────\n");

        // call function `dealFirstCards` to deal the first 2 cards to the players and the dealer
        dealFirstCards(deck, numPlayers, numBots, players, dealer, playerCardCount, &cardIndex, &dealerCardCount);

        // Players' turns
        for (int player = 0; player < numPlayers; player++) {
            printf("%d\n", player + 1);
            printf(TEXT_BOLD_UNDERLINE "Spieler %d ist am Zug:\n" TEXT_RESET, player + 1);
            playerTurn(deck, &cardIndex, players[player], playerCardCount[player]);

            printf("\n");
        }

        for (int bot = numPlayers; bot < (numPlayers + numBots); bot++) {
            printf("%d\n", bot + 1 - numPlayers);
            printf(TEXT_BOLD_UNDERLINE "Bot %d ist am Zug:\n" TEXT_RESET, bot + 1 - numPlayers);
            botTurn(deck, &cardIndex, players[bot], &playerCardCount[bot]);
            printf("\n");
        }


        // Dealer's turn
        printf("Dealer ist am Zug:\n");
        int deckIndex = CARDS_PER_PLAYER * (numPlayers + numBots) + CARDS_PER_PLAYER;
        dealerTurn(deck, &cardIndex, dealer, &dealerCardCount);


        printf("\n\n");
        printf(TEXT_RESET "Dealer: " TEXT_BOLD_UNDERLINE "%d Punkte\n" TEXT_RESET, handValue(dealer, dealerCardCount)); // Dealer summary

        determineWinner(players, numPlayers, numBots, dealer, dealerCardCount, playerCardCount); // Determine the winner

        printf("\n\n");
        printf("\n");

        for (int player = 0; player < (numPlayers + numBots); player++) {
            balanceDevelopment(players, player, balancePlayers, dealer, dealerCardCount, playerCardCount); // Balance development
            printf("Spieler %d hat %d€\n\n", player+1, balancePlayers[player][1]);



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
                printf("Ungueltige Eingabe! Bitte geben Sie entweder 'j' oder 'n' ein.\n");
            }
        }
    }


    printf("Vielen Dank für's spielen!"); // thx for playing 


    for (int i = 0; i < numPlayers; i++) {
        free(balancePlayers[i]);
    }
    free(balancePlayers);

    return 0;
}