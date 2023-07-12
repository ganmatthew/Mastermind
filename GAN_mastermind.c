/*
    Mastermind Machine Project
    Name        : John Matthew Ong Gan
    Section     : S12B
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

// game - game functions and computations
// display - game interfaces
// config - game settings
// input - user input variables
// ctr - counter variables

void displayDots(int numberOfLines) // generates a fixed number of dots on the display
{
	int i;
	for (i = 1; i <= numberOfLines; i++)
	{
		printf(".");
	}
	printf("\n");
}

void displayGameTitle() // ASCII artwork of the game title
{
	printf("\n.##.....##....###.....######..########.########.########..##.....##.####.##....##.########.");
	printf("\n.###...###...##.##...##....##....##....##.......##.....##.###...###..##..###...##.##.....##");
	printf("\n.####.####..##...##..##..........##....##.......##.....##.####.####..##..####..##.##.....##");
	printf("\n.##.###.##.##.....##..######.....##....######...########..##.###.##..##..##.##.##.##.....##");
	printf("\n.##.....##.#########.......##....##....##.......##...##...##.....##..##..##..####.##.....##");
	printf("\n.##.....##.##.....##.##....##....##....##.......##....##..##.....##..##..##...###.##.....##");
	printf("\n.##.....##.##.....##..######.....##....########.##.....##.##.....##.####.##....##.########.\n\n");
}

void displayWin(int *gameWinningPlayer, int *ctrGameAttempt) // message that is displayed when a player wins
{
    system("cls");
    displayDots(90);
    printf("Congratulations! Player %d guessed the code in ", --*gameWinningPlayer);
    if (*ctrGameAttempt == 1)
        printf("%d attempt!\n", *ctrGameAttempt);
    else
        printf("%d attempts!\n", *ctrGameAttempt);
    displayDots(90);
}

void displayDraw(int gameRandomCode) // message that is displayed when no players win
{
    system("cls");
    displayDots(90);
    printf("Nobody guessed the code! The code was %d.\n", gameRandomCode);
}

int gameCtrMatches(int inputGuess, int codeFrag) // counts how many matches can be found of each score match type
{
    int ctrMatches;
    int inputFrag = inputGuess;
    for (ctrMatches = 0; inputFrag > 0; inputFrag /= 10)
    {
        //printf("ctr: %d  input: %d  code: %d\n", ctrMatches, inputFrag, codeFrag); // debugging purposes
        if (inputFrag % 10 == codeFrag)
        {
            ctrMatches++;
        }
    }
    return ctrMatches;
}

int gameScoreTypeA(int inputGuess, int gameRandomCode) // identifies Type A score matches
{
    int localScoreA;
    int inputFrag = inputGuess;
    int codeFrag = gameRandomCode;

    for (localScoreA = 0; inputFrag > 0; inputFrag /= 10)
    {
        //printf("ctr: %d  input: %d  code: %d\n", localScoreA, inputFrag, codeFrag); // debugging purposes
        if (inputFrag % 10 == codeFrag % 10)
        {
            localScoreA++;
        }
        codeFrag /= 10;
    }
    return localScoreA;
}

int gameScoreTypeB(int inputGuess, int gameRandomCode, int localScoreA) // identifies Type B score matches
{
    int ctrScoreB, a, b;
    int localScoreB = 0;

    for (ctrScoreB = 0; ctrScoreB < 10; ctrScoreB++)
    {
        a = gameCtrMatches(inputGuess, ctrScoreB);
        b = gameCtrMatches(gameRandomCode, ctrScoreB);

        //printf("ctr: %d  input: %d  code: %d\n", ctrScoreB, inputFrag, codeFrag); // debugging purposes
        if (a > b) // if
        {
            //printf("Type B condition 1\n); // debugging purposes
            localScoreB += b;
        }
        else if (a <= b) // if
        {
            //printf("Type B condition 2\n); // debugging purposes
            localScoreB += a;
        }
    }

    localScoreB -= localScoreA;
    if (localScoreB < 0)
    {
        localScoreB = -localScoreB;
    }

    return localScoreB;
}

int gameVerifyBool(char inputChar, char *settingDir) // function used to check for invalid inputs into boolean input interfaces
{
    switch (inputChar)
    {
    case 'y':
    case 'Y':
        inputChar = 'Y';
        *settingDir = inputChar;
        return 1;
    case 'N':
    case 'n':
        inputChar = 'N';
        *settingDir = inputChar;
        return 1;
    default:
        system("cls");
        displayDots(90);
        printf("[Error] Please enter a valid input.\n");
        displayDots(90);
        return 0;
    }
}

int gameVerifySettings(int *settingDir) // function used to check for invalid inputs into numerical settings menu input interfaces
{
    int inputSelect;
    if (scanf("%d", &inputSelect) == 0) // a scanf returns 1 if numeric
    {
        fflush(stdin); // discards the scanned input, otherwise it will overflow
        displayDots(90);
        printf("[Error] Please enter a valid input.\n");
        displayDots(90);
        return 1;
    }
    else
    {
        *settingDir = inputSelect;
        return 0;
    }
}

void gameScanInput(int inputGuess, int inputCheck, int gameRandomCode, int *gameRoundScoreA, int *gameRoundScoreB ) // handles error checking, input checking, and use of cheat codes in the game itself
{
    int gameCheatCode = 0; // false when the cheat code is not active

    switch (inputGuess)
    {
        case 10302000: // cheat code that shows the secret code to the player
            if (inputGuess == 10302000)
            {
                gameCheatCode = 1;
                inputGuess = -2; // forces an error to avoid the input from being parsed
            }
        default:
            if (inputCheck != 1 || inputGuess < 1) // will error if non-numeric (a scanf returns 1 if numeric) or negative
            {
                fflush(stdin); // discards the scanned input, otherwise it will overflow
                //system("cls");
                displayDots(90);
                printf("[Error] Please enter a valid input.\n");
                if (inputGuess == -2) // displays additional lines only during the error produced from getting the cheat code
                {
                    displayDots(90);
                }
                inputCheck = -1;
            }
    }

    switch (gameCheatCode)
    {
        case 1:
            printf("[Error] The code is %d.\n", gameRandomCode);
            break;
    }

    *gameRoundScoreA = gameScoreTypeA(inputGuess, gameRandomCode);
    *gameRoundScoreB = gameScoreTypeB(inputGuess, gameRandomCode, *gameRoundScoreA);
    inputCheck = 0; // resets the input checking value
}


int gameGenerateCode(int configNumOfDigits) // generates a random code and repeats the code generation if the code does not pass the designated tests
{
    int ctrNonZero, ctrGen, randomCode, testCode, testNum;
    int moduloParam = 9;
    int addParam = 1; 
	int notValidRandom = 1; // becomes 0 when the generated code passes all tests
    for (ctrGen = 1; ctrGen <= configNumOfDigits - 1; ctrGen++)
    {
		addParam *= 10;
	}
    moduloParam *= addParam;
    while (notValidRandom)
	{
        for (ctrNonZero = 0; ctrNonZero < 10; ctrNonZero++) // loops to get a random number until all digits are non-zeroes
        {
			randomCode = rand() % moduloParam + addParam;   // uses modulo to extract digits, format is based on number of digits of the modulo and addition parameters
			//printf("%d\n", randomCode);
		}
        testNum = addParam;

        while (testNum >= 1)
        {
			testCode = randomCode;
            testCode %= (testNum * 10);
            testCode /= testNum;
            if (testCode == 0) // goes back to generate a new code if any digit is zero
			{
                testNum = 0;
            }
            if (testCode != 0 && testNum <= 1) // stop loop if no digits are zero in the running loop
            {
				notValidRandom = 0;
			}
            testNum /= 10;
        }
	}
	return randomCode;
}

void gameDisplayCode(int configNumOfDigits, int configNumOfPlayers, int *gameWinningPlayer, int *ctrGamePlayer, int *ctrGameAttempt, int *configNumOfAttempts, int *gameRoundScoreA, int *gameRoundScoreB) // handles the main game interface that the user primarily sees on the display
{
    char endMenuOption;         // option that is given after the game to continue playing or not
    int endMenuStatus = 0;      // status that determines whether to keep the game running
    int inputGuess, inputCheck;

    while (endMenuStatus != 2)  // keeps the game running until the user asks to go back to main menu
    {
        system("cls");
        endMenuStatus = 0;
        int gameRandomCode = gameGenerateCode(configNumOfDigits);

        printf("\t\t\t\tInput\t\tA\tB\n");
        displayDots(90);
        do  // keeps running the game until an endgame condition is reached
        {
            for (*ctrGamePlayer = 1; *ctrGamePlayer <= configNumOfPlayers; ++*ctrGamePlayer)
            {
                //int gameRandomCode; scanf("%d", &gameRandomCode); // for debugging
                //printf("Code: %d\n", gameRandomCode); // for debugging

                if (*ctrGameAttempt == 0) // prevents game attempt counter from becoming negative
                {
                    *ctrGameAttempt = 1;
                }

                printf("Player %d, Attempt %d: ", *ctrGamePlayer, *ctrGameAttempt);
                inputCheck = scanf("%d", &inputGuess);
                gameScanInput(inputGuess, inputCheck, gameRandomCode, gameRoundScoreA, gameRoundScoreB);
                
                if (inputCheck != 1) // bounces the input back to the player who inputted an invalid value
                {
                    --*ctrGamePlayer;
                    --*ctrGameAttempt;
                    if (*ctrGamePlayer < 1) // prevents game player counter from becoming negative
                        *ctrGamePlayer = 1;

                    if (*ctrGameAttempt < 1) // prevents game attempt counter from becoming negative
                        *ctrGameAttempt = 1;
                }

                if (*ctrGameAttempt >= 0) // shows the previous input information only after the first input
                {
                    if (*ctrGameAttempt == 0) // prevents game attempt counter from going negative
                        ++*ctrGameAttempt;

                    if (inputCheck != 1) // corrects the game attempt counter during error input
                        ++*ctrGameAttempt;

                    printf("Player %d\tAttempt %d\t%d\t\t%d\t%d\n\n", *ctrGamePlayer, *ctrGameAttempt, inputGuess, *gameRoundScoreA, *gameRoundScoreB);
                }
            }
            ++*ctrGameAttempt;

            if (inputCheck != 1)
                ++*ctrGameAttempt;

        } while (*gameRoundScoreA < 4 && *ctrGameAttempt < *configNumOfAttempts);

        *gameWinningPlayer = *ctrGamePlayer;

        if (*gameRoundScoreA == 4) // shows winning message when winning condition is reached
        {
            displayWin(gameWinningPlayer, ctrGameAttempt);
        }
        else if (*gameRoundScoreA < 4) // shows draw message when draw condition is reached
        {
            displayDraw(gameRandomCode);
        }

        while (endMenuStatus < 1) // keeps asking for input until a valid input is entered
        {
            printf("\nPlay again?\n\n[Y] Start a new game\n[N] Go back to main menu\n");
            printf("Choose an option: ");
            scanf(" %c", &endMenuOption);
            switch (endMenuOption)
            {
                case 'Y':
                case 'y':
                    endMenuStatus = 1; // starts a new game
                    system("cls");
                    break;
                case 'N':
                case 'n':
                    endMenuStatus = 2; // goes back to menu
                    break;
                default:
                    //system("cls");
                    displayDots(90);
                    fflush(stdin); // discards the scanned input, otherwise it will overflow
                    printf("[Error] Please enter a valid input.\n");
                    break;
            }
        }
        *ctrGameAttempt = 0;
        *gameRoundScoreA = 0;
        *gameRoundScoreB = 0;
    }
}

void displaySettings(int *configNumOfPlayers, int *configNumOfAttempts, int *configNumOfDigits) // settings menu
{
	int inputSettingsOption = 0;
	while(inputSettingsOption != 4) // keeps the settings up until the user enters the corresponding number for exiting settings
	{
		system("cls");
		displayDots(90);
		printf("Mastermind Game Settings\n");
        printf("[1] Change number of players\t\t\tcurrent: %d\n", *configNumOfPlayers);
        printf("[2] Change maximum number of attempts\t\tcurrent: %d\n", *configNumOfAttempts);
        printf("[3] Change maximum number of digits\t\tcurrent: %d\n", *configNumOfDigits);
        printf("[4] Exit Settings\n");
		printf("Choose an option: ");
		scanf("%d", &inputSettingsOption);
		displayDots(90);
		switch (inputSettingsOption)
		{
			case 1:
				do
				{
					printf("Change number of players\ndefault: 2\tcurrent: %d\tnew: ", *configNumOfPlayers);
                } while ( gameVerifySettings(configNumOfPlayers) );
                break;
			case 2:
				do
				{
					printf("Change maximum number of attempts\ndefault: 10\tcurrent: %d\tnew: ", *configNumOfAttempts);
                } while ( gameVerifySettings(configNumOfAttempts) );
                break;
			case 3:
				do
				{
					printf("Change maximum number of digits\ndefault: 4\tcurrent: %d\tnew: ", *configNumOfDigits);
                } while ( gameVerifySettings(configNumOfDigits) );
                break;
			case 4:
				system("cls");
				break;
			default:
				system("cls");
				fflush(stdin); // discards the scanned input, otherwise it will overflow
				printf("[Error] Please enter a valid input.\n");
				break;
		}
	}
}

int main() // main menu
{
	srand( time(0) );               // uses current time as a seed for a random number
	int gameStatus = 1;             // stopped = 0,	main menu = 1, ingame = 2, settings = 3
	int gameRoundScoreA = 0;        // per-round Type A match scoring variable
    int gameRoundScoreB = 0;        // per-round Type B match scoring variable
    int gameWinningPlayer;          // storage variable for the winning player number
	int ctrGameAttempt = 0;         // counter for per-game attempts listing
    int ctrGamePlayer = 1;          // counter for per-game player switching
    int inputMenuOption;            // storage variable for the menu selection opton

    int configNumOfPlayers = 2;     // default setting for number of players
	int configNumOfAttempts = 10;   // default setting for number of attempts
	int configNumOfDigits = 4;      // default setting for number of digits
	
	system("cls");

	while(gameStatus != 0)
	{
		system("cls");
		displayGameTitle();
		printf("Welcome to Mastermind!\n[1] Play Game\n[2] Game Settings\n[3] Exit Game\nChoose an option: ");
		scanf("%d", &inputMenuOption);
		switch(inputMenuOption)
		{
			case 1:
				gameStatus = 2;
				system("cls");
                gameDisplayCode(configNumOfDigits, configNumOfPlayers, &gameWinningPlayer, &ctrGamePlayer, &ctrGameAttempt, &configNumOfAttempts, &gameRoundScoreA, &gameRoundScoreB);
                break;
			case 2:
				gameStatus = 3;
				system("cls");
				displaySettings(&configNumOfPlayers, &configNumOfAttempts, &configNumOfDigits);
				break;
			case 3:
				gameStatus = 0;
				system("cls");
				break;
			default:
				fflush(stdin); // discards the scanned input, otherwise it will overflow
				system("cls");
				printf("\n[Error] Please enter a valid input.\n");
				break;
		}
	}
    displayDots(90);
    printf("\nUntil next time...\n\nMatthew Gan\nCCPROG1-S12B\n\nWritten on December 7, 2019\n\n");
    displayDots(90);
	
	return 0;
}