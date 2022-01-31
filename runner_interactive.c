#include "wordle_solver.c"

int getRandomSecret()
{

    srand(time(0));
    int num = rand() % maxWords;
    return num;
}

void solveInteractively()
{

    char feedBack[len + 1];

    char *newGuess = initialize();

    int attempts = 1;

    while (attempts < 20)
    {

        printf("Next Guess is : %.*s\n", len, newGuess);
        printf("Enter Feedback (1:wrong char, 2: incorrect Position, 3:correct position , 4:invalid guess) : ");
        scanf("%[^\n]%*c", feedBack);

        printf("%.*s : %.*s\n", len, newGuess, len, feedBack);

        if (feedBack[0] == '4')
        {
            newGuess = getNextGuess(feedBack);
            continue;
        }

        int done = 1;

        for (int i = 0; i < len; i++)
            if (feedBack[i] != '3')
                done = 0;
        if (done == 1)
        {
            printf("!!%.*s!! Solved in %d attemps\n\n", len, newGuess, attempts);
            break;
        }

        newGuess = getNextGuess(feedBack);

        attempts++;
    }
}

int main()
{
    solveInteractively();
}