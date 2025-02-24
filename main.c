#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

#ifdef _WIN32
    #include <windows.h>
    #define SLEEP(ms) Sleep(ms)
#else
    #include <unistd.h>
    #define SLEEP(ms) usleep((ms) * 1000)
#endif



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
#define TEXT_INVERT "\x1b[7m"
#define TEXT_RESET "\x1b[0m"



/*
    input validators for:
    * "reset"
    * characters (y/n)
    * range of numbers (e.g. numbers of players 1-6)
*/

// reset validator
void resetValidation(char *input) {
    input[strcspn(input, "\n")] = '\0';
    if (strcmp(input, "RESET") == 0 || strcmp(input, "reset") == 0) {
        exit(0);
    }
}

// character validator
bool characterValidation(char input, const char validValues[], const size_t length) {
    for (size_t i = 0; i < length; i++) {
        if (input == validValues[i]) {
            return true;
        }
    }
    return false;
}
bool getValidatedChar(const char *prompt, char allowedChars[], const size_t length, char *output) {
    char input[10];

    while (true) {
        printf("%s", prompt);
        fgets(input, sizeof(input), stdin);
        input[strcspn(input, "\n")] = '\0';

        resetValidation(input);

        if (strlen(input) == 1 && characterValidation(input[0], allowedChars, length)) {
            *output = input[0];
            return true;
        }

        printf(TEXT_RESET TEXT_RED "Ungültige Eingabe! Bitte geben Sie " TEXT_RESET "'%c' " TEXT_RED "oder " TEXT_RESET "'%c' " TEXT_RED "ein.\n" TEXT_RESET, allowedChars[0], allowedChars[1]);
    }
}

// number validator
bool intValidation(const int input, const int min, int max) {
    return input >= min && input <= max;
}
bool getValidatedInt(const char *prompt, const int min, const int max, int *output) {
    char input[20];

    while (true) {
        printf("%s", prompt);
        fgets(input, sizeof(input), stdin);

        resetValidation(input);

        char *invalid;
        int value = strtol(input, &invalid, 10);

        if (*invalid != '\0' && *invalid != '\n') {
            printf(TEXT_RESET TEXT_RED "Ungültige Eingabe! Bitte geben Sie eine gültige Zahl ein.\n" TEXT_RESET);
            continue;
        }

        if (!intValidation(value, min, max)) {
            printf(TEXT_RESET TEXT_RED "Zahl außerhalb des gültigen Bereichs! Erlaubt: " TEXT_RESET "%d " TEXT_RED "bis " TEXT_RESET "%d" TEXT_RED ".\n" TEXT_RESET, min, max);
            continue;
        }

        *output = value;
        return true;
    }
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
        const int randomIndex = rand() % TOTAL_CARDS;
        const Card temp = deck[i];
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
    int numPlayers = 0, numBots = 0;
    bool validPlayers = false;

    getValidatedInt("\nGeben Sie die Anzahl der Spieler ein (1-6): ", 1, MAX_PLAYERS, &numPlayers);

    // if space for bots is available
    if (numPlayers < MAX_PLAYERS) {
        char prompt[100];
        snprintf(prompt, sizeof(prompt), "Wollen Sie noch Bots hinzufügen? (Maximal %d möglich): ", MAX_PLAYERS - numPlayers);
        getValidatedInt(prompt, 0, MAX_PLAYERS - numPlayers, &numBots);
    }

    // return player config struct
    return (PlayerConfig){numPlayers, numBots};
}



/*
    initialize bet for every player with default values
    * id (starting from 1)
    * 5000€ (BALANCE_PER_PLAYER) default balance
    * bet of 0
*/
void Balance(const int numPlayers, const int numBots, int balancePlayers[][3]) {
    for (int i = 0; i < numPlayers+numBots; i++) {
        balancePlayers[i][0] = i + 1;  // Player-ID
        balancePlayers[i][1] = BALANCE_PER_PLAYER; //  Balance
        balancePlayers[i][2] = 0;  //  Bet
    }
}



/*
    getting every bet from every player
*/
void playerBet(int balancePlayer[][3], const int numPlayers, const int numBots) {
    int bet;

    // player bet
    for (int player = 0; player < numPlayers; player++) {
        do {
            char prompt[100];
            snprintf(prompt, sizeof(prompt), "\nWie viel Geld wollen Sie setzen, Spieler %d? (%d€ - %d€ | Guthaben: %d€): ", player + 1, MIN_BET, MAX_BET, balancePlayer[player][1]);

            getValidatedInt(prompt, MIN_BET, MAX_BET, &bet);

            if (bet > balancePlayer[player][1]) {
                printf("Ungültiger Einsatz! Sie haben nur %d€ Guthaben.\n", balancePlayer[player][1]);
            }
        } while (bet < MIN_BET || bet > MAX_BET || bet > balancePlayer[player][1]);

        balancePlayer[player][2] = bet;
        printf("Sie (Spieler %d) haben " TEXT_RESET TEXT_BOLD "%d€ " TEXT_RESET "gesetzt.\n\n", player + 1, balancePlayer[player][2]);
    }

    // bots place random bets
    for (int bot = numPlayers; bot < numBots + numPlayers; bot++) {
        if (balancePlayer[bot][1] < MIN_BET) {
            balancePlayer[bot][2] = 0;
            printf("Bot %d hat nicht genug Geld zum Setzen und setzt nichts.\n", bot + 1);
        } else {
            int maxBotBet = (balancePlayer[bot][1] < MAX_BET) ? balancePlayer[bot][1] : MAX_BET;
            balancePlayer[bot][2] = MIN_BET + rand() % (maxBotBet - MIN_BET + 1);
            printf("Bot %d hat " TEXT_RESET TEXT_BOLD "%d€ " TEXT_RESET "gesetzt.\n", bot - numPlayers + 1, balancePlayer[bot][2]);
        }
    }
}



/*
    Printing the hand of the player
*/
void printHand(Card hand[], const int numCards) {
    for (int i = 0; i < numCards; i++) {
        printf("  %s %s\n", hand[i].rank, hand[i].suit);
        SLEEP(250);
    }
}



/*
    Drawing cards and increasing the cardIndex
*/
void drawCard(const Card *deck, int *cardIndex, Card hand[], int *cardCount) {
    hand[*cardCount] = deck[*cardIndex];
    (*cardCount)++;
    (*cardIndex)++;
}



/*
    Dealing the first 2 cards to the players and the dealer
    Cards of the player are shown, the dealer's second card is hidden
*/
void dealFirstCards(const Card *deck, const int numPlayers, const int numBots, Card players[MAX_PLAYERS+1][TOTAL_CARDS], Card dealer[2], int playerCardCount[MAX_PLAYERS], int *cardIndex, int *dealerCardCount) {
    // Player
    for (int player = 0; player < (numPlayers + numBots); player++) {
        if (player < numPlayers) {
            printf("-- Spieler %d:\n", player + 1);
        } else {
            printf("-- Bot %d:\n", player - numPlayers + 1);
        }
        for (int card = 0; card < CARDS_PER_PLAYER; card++) {
            SLEEP(250);
            drawCard(deck, cardIndex, players[player], &playerCardCount[player]);
            printf("  %s %s\n", deck[*(cardIndex)-1].rank, deck[*(cardIndex)-1].suit);
            SLEEP(250);
        }

        if (player != numPlayers - 1) {
            SLEEP(1000);
        }

        printf("\n");
    }

    printf("════════════════════\n");
    printf("Dealer:\n");
    SLEEP(500);
    for (int card = 0; card < CARDS_PER_PLAYER; card++) {
        drawCard(deck, cardIndex, dealer, dealerCardCount);
        if (card == 0) {
            printf(TEXT_RED "  (?)\n" TEXT_RESET);
        } else {
            printf("  %s %s\n", deck[*(cardIndex)-1].rank, deck[*(cardIndex)-1].suit);
        }
        SLEEP(250);
    }
    printf("════════════════════\n\n");
}



/*
    Check if the value of the hand is 21
*/
bool blackjack(const int value) {
    return value == 21;
}



/*
    Check if the value of the final hand is over 21 (bust)
*/
bool checkBust(const int value)
{
    return value > 21;
}



/*
    Calculating the value of the hand
    Aces are counted as 11, if the value of the hand is over 21, the value of the ace is changed to 1
*/
int handValue(Card hand[], const int numCards) {
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
void playerTurn(const Card *deck, int *cardIndex, Card player[], int *playerCardCount) {
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

        bool validInput = false;
        while (!validInput) {
            char jn[] = {'j', 'n'};
            char choice;

            getValidatedChar("Wollen Sie eine weitere Karte ziehen? (j/n)", jn, sizeof(jn) / sizeof(jn[0]), &choice);
            validInput = true;

            if (choice == 'j') {
                drawCard(deck, cardIndex, player, playerCardCount);

                currentHandValue = handValue(player, *playerCardCount);
                if (checkBust(currentHandValue)) {
                    break;
                }
            } else if (choice == 'n') {
                ziehen = false;
            }
        }
    }

    SLEEP(500);

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
    Bot turn
*/
void botTurn(const Card *deck, int *cardIndex, Card bot[], int *botCardCount) {
        int botHandValue = handValue(bot, *botCardCount);
        printf("Hand des Bots:\n");
        printHand(bot, *botCardCount);
        printf(TEXT_RESET TEXT_BOLD "Aktueller Wert: %d\n" TEXT_RESET, handValue(bot, *botCardCount));

        SLEEP(500);

        while (botHandValue < 17) {
            printf("\nBot zieht eine Karte:\n");
            drawCard(deck, cardIndex, bot, botCardCount);
            printHand(bot, *botCardCount);
            botHandValue = handValue(bot, *botCardCount);
            printf(TEXT_RESET TEXT_BOLD "Aktueller Wert: %d\n" TEXT_RESET, handValue(bot, *botCardCount));
            SLEEP(500);
        }
}



/*
    Dealer's turn
    The dealer has to draw cards until the value of the hand is at least 17
*/
void dealerTurn(const Card *deck, int *cardIndex, Card *dealer, int *dealerCardCount) {
    printf("Hand des Dealers:\n");
    printHand(dealer, *dealerCardCount);
    printf(TEXT_RESET TEXT_BOLD "Aktueller Wert: %d\n\n" TEXT_RESET, handValue(dealer, *dealerCardCount));

    SLEEP(500);
    while (handValue(dealer, *dealerCardCount) < 17) {
        drawCard(deck, cardIndex, dealer, dealerCardCount);
        printf("Dealer zieht eine Karte:\n");
        printHand(dealer, *dealerCardCount);
        printf(TEXT_RESET TEXT_BOLD "Aktueller Wert: %d\n\n" TEXT_RESET, handValue(dealer, *dealerCardCount));
        SLEEP(500);
    }
}



/*
    updating the players balance based on the results of the game
*/
void updateBalance(int player, int playerValue, int dealerValue, int playerCardCount, int dealerCardCount, int balancePlayers[][3]) {
    const int bet = balancePlayers[player][2];

    // check if player has blackjack (2 cards and value of 21)
    if (playerCardCount == 2 && blackjack(playerValue)) {
        // if player and dealer = blackjack: lose
        if (dealerCardCount == 2 && blackjack(dealerValue)) {
            balancePlayers[player][1] -= bet;
            printf(TEXT_RESET "Neue Balance: " TEXT_BOLD "%d€ " TEXT_RESET TEXT_RED "(-%d€)\n" TEXT_RESET, balancePlayers[player][1], bet);
        } else {
            const int gewinn = (int)round(bet * 1.5);  // pays 3 to 2
            balancePlayers[player][1] += gewinn;
            printf(TEXT_RESET "Neue Balance: " TEXT_BOLD "%d€ " TEXT_RESET TEXT_GREEN "(+%d€)\n" TEXT_RESET, balancePlayers[player][1], gewinn);
        }
    }

    // player looses
    else if (playerValue > 21) {
        balancePlayers[player][1] -= bet;
        printf(TEXT_RESET "Neue Balance: " TEXT_BOLD "%d€ " TEXT_RESET TEXT_RED "(-%d€)\n" TEXT_RESET, balancePlayers[player][1], bet);
    }
    else if (playerValue < dealerValue) {
        balancePlayers[player][1] -= bet;
        printf(TEXT_RESET "Neue Balance: " TEXT_BOLD "%d€ " TEXT_RESET TEXT_RED "(-%d€)\n" TEXT_RESET, balancePlayers[player][1], bet);
    }

    // player wins against dealer
    else if (dealerValue > 21) {
        balancePlayers[player][1] += bet;  // pays double
        printf(TEXT_RESET "Neue Balance: " TEXT_BOLD "%d€ " TEXT_RESET TEXT_GREEN "(+%d€)\n" TEXT_RESET, balancePlayers[player][1], bet);
    }
    else if (playerValue > dealerValue) {
        balancePlayers[player][1] += bet;
        printf(TEXT_RESET "Neue Balance: " TEXT_BOLD "%d€ " TEXT_RESET TEXT_GREEN "(+%d€)\n" TEXT_RESET, balancePlayers[player][1], bet);
    }

    // draw - money back
    else {
        balancePlayers[player][1] += 0;
        printf(TEXT_RESET "Balance bleibt bei " TEXT_BOLD "%d€ " TEXT_RESET TEXT_YELLOW "(Einsatz zurück)\n" TEXT_RESET, balancePlayers[player][1]);
    }

    // reset bet
    balancePlayers[player][2] = 0;
}



/*
    Determining the winner on basis of the hand value
*/
void determineWinner(Card players[MAX_PLAYERS + 1][TOTAL_CARDS], const int numPlayers, const int numBots, Card dealer[], const int dealerCardCount, int playerCardCount[MAX_PLAYERS], int balancePlayers[][3]) {
    int dealerValue = handValue(dealer, dealerCardCount);
    for (int player = 0; player < (numPlayers + numBots); player++) {
        const char *playerType = (player < numPlayers) ? "Spieler" : "Bot";
        int playerNumber = (player < numPlayers) ? (player + 1) : (player - numPlayers + 1);

        int playerValue = handValue(players[player], playerCardCount[player]);
        printf("%s %d hat " TEXT_BOLD_UNDERLINE "%d Punkte%s" TEXT_RESET ".\n", playerType, playerNumber, playerValue, blackjack(playerValue) ? TEXT_RESET " (Blackjack)" : "");

        if (playerValue > 21) {
            printf("%s %d hat " TEXT_RED "ueberkauft.\n" TEXT_RESET, playerType, playerNumber);
        } else if (dealerValue > 21) {
            printf("Dealer hat ueberkauft. %s %d " TEXT_GREEN "gewinnt.\n" TEXT_RESET, playerType, playerNumber);
        } else if (playerValue > dealerValue) {
            printf("%s %d " TEXT_GREEN "gewinnt.\n" TEXT_RESET, playerType, playerNumber);
        } else if (playerValue < dealerValue) {
            printf(TEXT_RED "Dealer gewinnt gegen %s %d.\n" TEXT_RESET, playerType, playerNumber);
        } else {
            printf("%s %d und der Dealer haben " TEXT_YELLOW "unentschieden.\n" TEXT_RESET, playerType, playerNumber);
        }
        updateBalance(player, playerValue, dealerValue, playerCardCount[player], dealerCardCount, balancePlayers);

        SLEEP(500);
        printf("\n");
    }
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
    The game is played by the players (and bots) and the dealer
*/
int main() {
    srand(time(NULL));
    bool playing = true;

    Card deck[TOTAL_CARDS];
    initializeDeck(deck); // call function `initializeDeck` to initialize the deck of cards

    const PlayerConfig config = choosePlayer(); // call function `choosePlayer` to choose the number of players
    const int numPlayers = config.numPlayers;
    const int numBots = config.numBots;
    int totalPlayers = numPlayers + numBots;

    int playerCardCount[MAX_PLAYERS+1] = {0};
    int dealerCardCount = 0;
    int cardIndex = 0;

    // init balance: define array size and init default balances
    int balancePlayers[totalPlayers][3];
    Balance(numPlayers, numBots, balancePlayers);



    // MAIN GAME
    while (playing) {
        Card dealer[50];
        Card players[MAX_PLAYERS+1][TOTAL_CARDS];

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
            printf("▃▅▆█ 웃 %d █▆▅▃\n", player + 1);
            SLEEP(250);
            printf(TEXT_BOLD_UNDERLINE "Spieler %d ist am Zug:\n" TEXT_RESET, player + 1);
            SLEEP(250);
            playerTurn(deck, &cardIndex, players[player], &playerCardCount[player]);

            printf("\n");
            SLEEP(1000);
            printf("\n");
            SLEEP(1000);
            printf("\n\n");
        }

        for (int bot = numPlayers; bot < (numPlayers + numBots); bot++) {
            printf("▃▅▆█ 🤖 %d █▆▅▃\n", bot + 1 - numPlayers);
            SLEEP(250);
            printf(TEXT_BOLD_UNDERLINE "Bot %d ist am Zug:\n" TEXT_RESET, bot + 1 - numPlayers);
            SLEEP(250);
            botTurn(deck, &cardIndex, players[bot], &playerCardCount[bot]);

            printf("\n");
            SLEEP(1000);
            printf("\n");
            SLEEP(1000);
            printf("\n\n");
        }


        // Dealer's turn
        printf("\n▃▅▆█ 🤵‍♂️ Dealer █▆▅▃\n");
        SLEEP(250);
        printf(TEXT_RESET TEXT_BOLD_UNDERLINE "Dealer ist am Zug:\n" TEXT_RESET);
        SLEEP(250);
        dealerTurn(deck, &cardIndex, dealer, &dealerCardCount);

        printf("\n");
        SLEEP(1000);
        printf("\n");
        printf("════ Spielauswertung ════\n");
        printf("*****************\n");
        SLEEP(250);
        printf(TEXT_RESET "Dealer: " TEXT_BOLD_UNDERLINE "%d Punkte\n" TEXT_RESET, handValue(dealer, dealerCardCount)); // Dealer summary
        SLEEP(250);
        printf("*****************\n\n");

        determineWinner(players, numPlayers, numBots, dealer, dealerCardCount, playerCardCount, balancePlayers); // Determine the winner
        printf("════════════════════\n");

        SLEEP(1500);

        // play again?
        char allowedCharacters[] = {'j', 'n'};
        char choice;
        getValidatedChar("Wollen Sie erneut spielen (j/n)? ", allowedCharacters, sizeof(allowedCharacters) / sizeof(allowedCharacters[0]), &choice);
        playing = (choice == 'j');

        if (choice == 'j') {
            printf("\n\n\n\n\n\n\n\n\n");
        }
    }

    printf("\n\n" TEXT_INVERT "═════════════════════════════════════════════" TEXT_RESET "\n");
    printf("     " TEXT_BLINKING "❤️" TEXT_RESET " Vielen Dank für's Spielen! " TEXT_BLINKING "❤️" TEXT_RESET "      \n");
    printf("Made by Jan Kruske, Felix Schulz, Noah Becker\n");
    printf(TEXT_INVERT "═════════════════════════════════════════════" TEXT_RESET "\n\n");

    printf("\nDas Spiel wird in 10 Sekunden beendet.\n");

    SLEEP(10000);

    return 0;
}

