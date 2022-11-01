/* In this program, the threads are playing the game Hangman.
 * One thread picks a random word from a list of words, then
 * all of them are trying to find it out by trying possible letters.
 */


#include <bits/stdc++.h>
#include <pthread.h>
#include <sys/sysinfo.h>

#define MAX_NUM_THREADS 64
#define MAX_WORDS 300000
#define MAX_WORD_LEN 50
#define NUM_LETTERS 26
#define LIVES 10
#define MASTER 0

using namespace std;

struct ThreadArgs
{
    int numThreads;
    int threadId;
    vector<string> *words;
    string word;
    int wordLen;
    char *letters;
    int *lives;
    bool *used;
    char *choice;
    bool *ongoing;
    pthread_barrier_t *barrier;
};

void *threadFunction(void *arg)
{
    ThreadArgs *args = (ThreadArgs*)arg;
    int numThreads = args->numThreads;
    int threadId = args->threadId;
    vector<string> *words = args->words;
    string word = args->word;
    int wordLen = args->wordLen;
    char *letters = args->letters;
    int *lives = args->lives;
    bool *used = args->used;
    char *choice = args->choice;
    bool *ongoing = args->ongoing;
    pthread_barrier_t *barrier = args->barrier;
    /* rand() and srand() are not thread-safe => use rand_r() instead
      * Each thread has its own seed. */
    unsigned int seed = time(NULL) + args->threadId;
    
    while (*ongoing)
    {
        if (threadId == MASTER)
        {
            words->erase(std::remove_if(words->begin(), words->end(), [&letters](string w){
                bool ok = true;
                for (int i = 0; i < (int)w.length(); ++i)
                {
                    if (letters[i] != '-' && w.at(i) != letters[i])
                    {
                        ok = false;
                        break;
                    }
                }
                return !ok;
            }), words->end());
        }
        pthread_barrier_wait(barrier);
        string w = (*words)[rand_r(&seed) % words->size()];
        char letter;
        printf("Player %d thinking of %s\n", threadId, w.c_str());
        for (int i = 0; i < (int)w.length(); ++i)
        {
            if (!used[w.at(i) - 'A'])
            {
                letter = w.at(i);
                break;
            }
        }
        choice[threadId] = letter;
        printf("Player %d picked letter %c\n", threadId, letter);
        pthread_barrier_wait(barrier);
        if (threadId == MASTER)
        {
            for (int i = 0; i < numThreads; ++i)
            {
                if (choice[i] >= 'a')
                {
                    choice[i] -= ('a' - 'A');
                }
                if (!used[choice[i] - 'A'])
                {
                    used[choice[i] - 'A'] = true;
                    bool exists = false;
                    for (int j = 0; j < wordLen; ++j)
                    {
                        if (word.at(j) == choice[i])
                        {
                            exists = true;
                            letters[j] = choice[i];
                        }
                    }
                    if (!exists)
                    {
                        *lives = max(0, *lives - 1);
                    }
                }
            }
            printf("The players have %d lives left\n", *lives);
            printf("Current status: %s\n", letters);
            *ongoing = false;
            for (int i = 0; i < wordLen; ++i)
            {
                if (letters[i] == '-')
                {
                    *ongoing = true;
                    break;
                }
            }
            if (*ongoing == false)
            {
                printf("The players succeeded in guessing the word %s\n", word.c_str());
            }
            else if (*lives <= 0)
            {
                printf("The players failed to guess the word %s\n", word.c_str());
                *ongoing = false;
            }
        }
        pthread_barrier_wait(barrier);
    }
    printf("Thread %d finished\n", threadId);
    pthread_exit(NULL);
}

int main (int argc, char **argv)
{
    int numThreads;
    if (argc > 1)
    {
        numThreads = atoi(argv[1]);
    }
    else
    {
        numThreads = get_nprocs();
    }
    
    ifstream in("wordlist.txt");
    vector<string> words;
    string w;
    while (in >> w)
    {
        words.push_back(w);
    }
    in.close();
    
    string word;
    int wordLen;
    char letters[MAX_WORD_LEN];
    int lives = LIVES;
    bool used[NUM_LETTERS] = { false };
    char choice[numThreads];
    bool ongoing = true;
    unsigned int seed = time(NULL);
    
    word = words[rand_r(&seed) % words.size()];
    printf("The word is %s\n", word.c_str());
    wordLen = word.length();
    memset(letters, '-', wordLen);
    letters[wordLen] = '\0';
    printf("%s\n", letters);
    words.erase(std::remove_if(words.begin(), words.end(), [wordLen](string w){return ((int)w.length() != wordLen);}), words.end());
    
    pthread_t threads[numThreads-1];
    ThreadArgs threadArgs[numThreads];
    pthread_barrier_t barrier;
    pthread_barrier_init(&barrier, NULL, numThreads);
    for (int i = 0; i < numThreads; ++i)
    {
        threadArgs[i].numThreads = numThreads;
        threadArgs[i].threadId = i;
        threadArgs[i].words = &words;
        threadArgs[i].word = word;
        threadArgs[i].wordLen = wordLen;
        threadArgs[i].letters = letters;
        threadArgs[i].lives = &lives;
        threadArgs[i].used = used;
        threadArgs[i].choice = choice;
        threadArgs[i].ongoing = &ongoing;
        threadArgs[i].barrier = &barrier;
    }
    for (int i = 1; i < numThreads; ++i)
    {
        pthread_create(&threads[i-1], NULL, threadFunction, &threadArgs[i]);
    }
    threadFunction(&threadArgs[0]);
    for (int i = 0; i < numThreads - 1; ++i)
    {
        pthread_join(threads[i], NULL);
    }
    
    return 0;
}
