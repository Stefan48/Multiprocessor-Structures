/* In this program, the processes are playing the game Hangman.
 * Process 0 picks a random word from a list of words and the
 * others are trying to find it out by trying possible letters.
 */


#include <bits/stdc++.h>
#include <mpi.h>

#define MAX_WORDS 300000
#define NUM_LETTERS 26
#define LIVES 10

using namespace std;

int main (int argc, char **argv)
{
    MPI_Init(&argc, &argv);
    
    int numProc, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &numProc);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    
    ifstream in("wordlist.txt");
    vector<string> words;
    string w;
    while (in >> w)
    {
        words.push_back(w);
    }
    in.close();
    
    if (rank == 0)
    {
        srand(time(NULL));
        
        string word = words[rand() % words.size()];
        cout << "The word is " << word << '\n';
        int wordLen = word.length();
        MPI_Bcast(&wordLen, 1, MPI_INT, 0, MPI_COMM_WORLD);
        char letters[wordLen + 1];
        memset(letters, '-', wordLen);
        letters[wordLen] = '\0';
        cout << letters << "\n";
        int lives = LIVES;
        bool used[NUM_LETTERS] = { false };
        char letter;
        
        while (true)
        {
            for (int i = 1; i < numProc; ++i)
            {
                MPI_Recv(&letter, 1, MPI_CHAR, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                cout << "Player " << i << " picked letter " << letter << '\n';
                if ((letter >= 'a' && letter <= 'z') || (letter >= 'A' && letter <= 'Z'))
                {
                    if (letter >= 'a')
                    {
                        letter -= ('a' - 'A');
                    }
                    if (!used[letter - 'A'])
                    {
                        used[letter - 'A'] = true;
                        bool exists = false;
                        for (int j = 0; j < wordLen; ++j)
                        {
                            if (word.at(j) == letter)
                            {
                                exists = true;
                                letters[j] = letter;
                            }
                        }
                        if (!exists)
                        {
                            lives = max(0, lives - 1);
                        }
                    }
                }
            }
            cout << "The players have " << lives << " lives left\n";
            cout << "Current status: " << letters << '\n';
            MPI_Bcast(&lives, 1, MPI_INT, 0, MPI_COMM_WORLD);
            MPI_Bcast(letters, wordLen, MPI_CHAR, 0, MPI_COMM_WORLD);
            MPI_Bcast(used, NUM_LETTERS, MPI_CXX_BOOL, 0, MPI_COMM_WORLD);
            bool ongoing = false;
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
                cout << "The players succeeded in guessing the word " << word << '\n';
                break;
            }
            else if (lives <= 0)
            {
                cout << "The players failed to guess the word " << word << '\n';
                break;
            }
        } 
    }
    else
    {
        srand(time(NULL) + rank);
        int wordLen;
        MPI_Bcast(&wordLen, 1, MPI_INT, 0, MPI_COMM_WORLD);
        words.erase(std::remove_if(words.begin(), words.end(), [wordLen](string w){return ((int)w.length() != wordLen);}), words.end());
        char letters[wordLen + 1];
        memset(letters, '-', wordLen);
        letters[wordLen] = '\0';
        int lives;
        bool used[NUM_LETTERS] = { false };
        char letter;
    
        while (true)
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
            string word = words[rand() % words.size()];
            cout << "Player " << rank << " thinking of " << word << "\n";
            for (int i = 0; i < (int)word.length(); ++i)
            {
                if (!used[word.at(i) - 'A'])
                {
                    letter = word.at(i);
                    break;
                }
            }
            MPI_Send(&letter, 1, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
            MPI_Bcast(&lives, 1, MPI_INT, 0, MPI_COMM_WORLD);
            MPI_Bcast(letters, wordLen, MPI_CHAR, 0, MPI_COMM_WORLD);
            MPI_Bcast(used, NUM_LETTERS, MPI_CXX_BOOL, 0, MPI_COMM_WORLD);
            bool ongoing = false;
            for (int i = 0; i < wordLen; ++i)
            {
                if (letters[i] == '-')
                {
                    ongoing = true;
                    break;
                }
                
            }
            if (!ongoing || lives <= 0)
            {
                cout << "Player " << rank << " finished\n";
                break;
            }
        } 
    }
    
    MPI_Finalize();
    return 0;
}
