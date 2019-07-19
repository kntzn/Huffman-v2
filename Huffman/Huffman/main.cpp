#include <stdlib.h>
#include <iostream>

#define N_CHARS 256

struct CharFreq
    {
    unsigned long freq;
    char ch;
    };

int main ()
    { 
    char string [] = "beep boop beer!";
    unsigned long len = 16;

    // Sets up freq_table and sets chars
    CharFreq freq_table [N_CHARS] = { };
    for (int i = 0; i < N_CHARS; i++)
        freq_table [i].ch = i;

    // Counts frequency
    for (int i = 0; i < len; i++)
        freq_table [string [i]].freq++;


    for (int i = 0; i < N_CHARS; i++)
        { 
        if (freq_table [i].freq)
            std::cout << char (i) << " (" << i << ") - " << freq_table [i].freq << std::endl;
        }

    system ("pause");
    }