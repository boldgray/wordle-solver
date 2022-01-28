#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int sign = -1;

const int len = 5;
const int maxWords = 30000;

int characterLimit[26];
int correctChars[26];
int wordMap[26];

int masterMap[26];

int wrongChars[26];

int incorrectPositions[26][len];
char finalGuess[len];

char feedBack[len];
int feedBackMap[26];
int guessMap[26];
int secretMap[26];


int wordValid[maxWords];

char wordList[maxWords][len];

int checkIfWordContainsChar(char word[], char ch) {
    for(int i=0; i<len; i++)
        if(word[i]==ch)
            return 1;
    return 0;
}


int checkIncorrectPosition(int incPos[][len], char ch, int i) {
    for(int j=0; j<len; j++)
        if(incPos[ch-'a'][j]==i)
            return 1;
    return 0;
}

void getAllWordListFromCSV(){
     FILE * fp;
    char * line = NULL;
    size_t len = 0;
    ssize_t read;

    fp = fopen("./word_list.txt", "r");
    if (fp == NULL)
        exit(EXIT_FAILURE);
    int wn = 0;
    while (wn < maxWords && (read = getline(&line, &len, fp)) != -1) {
        //printf("Retrieved line of length %zu:\n", read);
        //printf("%s", line);
        for(int i=0; i<len; i++)
        {
            wordList[wn][i] = line[i];
        }
        wn++;
    }

    fclose(fp);
    if (line)
        free(line);
}

void initialize() {
    for(int i=0; i<26; i++) {
        characterLimit[i] = -1;
        correctChars[i] = 0;
        masterMap[i] = 0;
        wrongChars[i] = 0;
        
        for(int j=0; j<len; j++)
            incorrectPositions[i][j] = -1;
    }
    
    for(int i=0; i<len; i++)
        finalGuess[i] = 0;
    
    for(int i=0; i<maxWords; i++)
        wordValid[i] = 1;
    
    getAllWordListFromCSV();	
}

int getRandomSecret() {

    srand(time(0));
    int num = rand() % maxWords;
    return num;
}

int getMin(int a, int b){
    if(a<b)
        return a;
    else
        return b;
}

void solveRandom(){
		
    initialize();
    
    char * secret = wordList[getRandomSecret()];
    //secret = "gabriel";
    
    
    for (int i=0; i<len; i++) {
        char c =  secret[i];
        masterMap[c-'a']++;
    }


    int attemps = 0;
    
    char * guess;
    
    int wordsRemained = maxWords;

    while (wordsRemained > 0) {


        for (int w=0; w<maxWords; w++) {
            
            if(wordValid[w]==0)
                continue;

            char * word = wordList[w];

            for(int i=0; i<26; i++)
                wordMap[i] = 0;

            for (int i=0; i<len; i++) {
                char c =  word[i];
                wordMap[c-'a']++;
            }

            int qualify = 1;
            for (int i=0; i<26; i++)
                if (wordMap[i] < correctChars[i]
                        ||  characterLimit[i] != -1 && wordMap[i] > characterLimit[i]) {
                    qualify = 0;
                    break;
                }
            if (qualify==0) {
                wordValid[w] = 0;
                continue;
            }

            for (int i=0; i<26; i++)
                
                if (wrongChars[i]==1 && checkIfWordContainsChar(word, (char) ('a'+i))==1) {
                    qualify = 0;
                    break;
                }
            if (qualify==0) {
                wordValid[w] = 0;
                continue;
            }

            for (int i = 0; i < len; i++) {
                if (checkIncorrectPosition(incorrectPositions, word[i], i)==1) {
                    qualify = 0;
                    break;
                }

            }

            if (qualify==0) {
                wordValid[w] = 0;
                continue;
            }

            for (int i = 0; i < len; i++) {
                if (finalGuess[i] != 0 && finalGuess[i] != word[i]) {
                    qualify = 0;
                    break;
                }
            }

            if (qualify==0) {
                wordValid[w] = 0;
                continue;
            }
            guess = word;
            wordValid[w] = 0;
            break;
        }
        
        
        for(int i=0; i<26; i++) {
            feedBackMap[i] = 0;
            guessMap[i] = 0;
            secretMap[i] = 0;
        }
        for (int i=0; i<len; i++) {
            char c = guess[i];
            guessMap[c-'a']++;
        }


        for(int i=0; i<len; i++)
        {
            if(secret[i]==guess[i]) {
                feedBack[i] = '3';
                feedBackMap[secret[i]-'a']++;
            }
        }
        
        for(int i=0; i<len; i++)
        {
            if(masterMap[guess[i]-'a']==0) {
                feedBack[i] = '1';
            }else if(secret[i]!=guess[i]) {
                if(feedBackMap[guess[i]-'a'] < masterMap[guess[i]-'a']){
                feedBack[i] = '2';
                feedBackMap[guess[i]-'a']++;
                }else {
                    feedBack[i] = '1';
                }
            }
        }
        
        
        
        for (int i = 0; i < len; i++) {
            char c = guess[i];
            if (feedBack[i] == '2' || feedBack[i] == '3')
                secretMap[c-'a']++;
        }

        for (int i = 0; i < len; i++) {
            char c = guess[i];
            if (secretMap[c-'a']>0) {
                correctChars[c-'a'] = getMin(guessMap[c-'a'], secretMap[c-'a']);
                if (feedBack[i] == '3') {
                    finalGuess[i] = c;
                    
                    for(int j=0; j<len; j++)
                        if(incorrectPositions[c-'a'][j]==i)
                            incorrectPositions[c-'a'][j] = -1;
                    
                } else {
                    for(int j=0; j<len; j++)
                        if(incorrectPositions[c-'a'][j]==-1) {
                            incorrectPositions[c-'a'][j] = i;
                            break;
                        }
                }

                if (guessMap[c-'a'] > secretMap[c-'a'])
                    characterLimit[c-'a'] =  secretMap[c-'a'];

            } else
                wrongChars[c-'a'] = 1;

        }
        attemps++;
        
        printf("%.*s\n", len, guess);


        int done = 1;
        
        
        
        for (int i=0; i<len; i++)
            if (finalGuess[i] == 0)
                done = 0;
        if (done==1)
            break;

    }

    printf("!!%.*s!! Solved in %d attemps\n\n", len, guess, attemps);

}


int main()
{
    printf("\n");
    solveRandom();
}