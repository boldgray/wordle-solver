#include "wordle_solver.c"


int getRandomSecret() {

    srand(time(0));
    int num = rand() % maxWords;
    return num;
}

void solveRandom(){
		
    

    int masterMap[26];

     for(int i=0; i<26; i++) {
            masterMap[i] = 0;
    }

    char feedBack[len];
    int feedBackMap[26];

    char * newGuess = initialize();
    
    
    //char * secret = wordList[getRandomSecret()];
    char * secret = "scout";
    
    for (int i=0; i<len; i++) {
        char c =  secret[i];
        masterMap[c-'a']++;
    }

   

    int attempts = 1;
    
    
    

    
    while (attempts < 10) {

         for(int i=0; i<26; i++) {
            feedBackMap[i] = 0;
        }
        


        for(int i=0; i<len; i++)
        {
            if(secret[i]==newGuess[i]) {
                feedBack[i] = '3';
                feedBackMap[secret[i]-'a']++;
            }
        }
        
        for(int i=0; i<len; i++)
        {
            if(masterMap[newGuess[i]-'a']==0) {
                feedBack[i] = '1';
            }else if(secret[i]!=newGuess[i]) {
                if(feedBackMap[newGuess[i]-'a'] < masterMap[newGuess[i]-'a']){
                feedBack[i] = '2';
                feedBackMap[newGuess[i]-'a']++;
                }else {
                    feedBack[i] = '1';
                }
            }
        }

        printf("%.*s : %.*s\n", len, newGuess, len, feedBack);

        int done = 1;
        
        for (int i=0; i<len; i++)
            if (newGuess[i] != secret[i])
                done = 0;
        if (done==1){
            printf("\n!!%.*s!! Solved in %d attemps\n\n", len, newGuess, attempts);
            break;
        }


       
        
        attempts++;
        
         newGuess = getNextGuess(feedBack);


    }

    

}

int main()
{
    printf("\n");
    solveRandom();
}