/* In this program, the threads are playing the game Hangman.
 * One thread picks a random word from a list of words, then
 * all of them are trying to find it out by trying possible letters.
 */


#include <bits/stdc++.h>
#include <omp.h>

#define MAX_NUM_THREADS 64
#define MAX_WORDS 300000
#define MAX_WORD_LEN 50
#define NUM_LETTERS 26
#define LIVES 10

using namespace std;

int main (int argc, char **argv)
{
    if (argc > 1)
    {
        omp_set_num_threads(atoi(argv[1]));
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
    char choice[MAX_NUM_THREADS];
    bool ongoing = true;
    
    #pragma omp parallel default(none) shared(words, word, wordLen, letters, lives, used, choice, ongoing)
    {
        int numThreads = omp_get_num_threads();
        int threadId = omp_get_thread_num();
        /* rand() and srand() are not thread-safe => use rand_r() instead
         * Each thread has its own seed. */
        unsigned int seed = time(NULL) + threadId;
        
        #pragma omp single
        {
            word = words[rand_r(&seed) % words.size()];
            printf("The word is %s\n", word.c_str());
            wordLen = word.length();
            memset(letters, '-', wordLen);
            letters[wordLen] = '\0';
            printf("%s\n", letters);
            words.erase(std::remove_if(words.begin(), words.end(), [wordLen](string w){return ((int)w.length() != wordLen);}), words.end());
        }
        while (ongoing)
        {
            #pragma omp single
            {
                words.erase(std::remove_if(words.begin(), words.end(), [&letters](string w){
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
                }), words.end());
            }
            string w = words[rand_r(&seed) % words.size()];
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
            #pragma omp barrier
            #pragma omp single
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
                            lives = max(0, lives - 1);
                        }
                    }
                }
                printf("The players have %d lives left\n", lives);
                printf("Current status: %s\n", letters);
                ongoing = false;
                for (int i = 0; i < wordLen; ++i)
                {
                    if (letters[i] == '-')
                    {
                        ongoing = true;
                        break;
                    }
                }
                if (!ongoing)
                {
                    printf("The players succeeded in guessing the word %s\n", word.c_str());
                }
                else if (lives <= 0)
                {
                    printf("The players failed to guess the word %s\n", word.c_str());
                    ongoing = false;
                }
            }
        }
        printf("Thread %d finished\n", threadId);
    }
    
    return 0;
}
