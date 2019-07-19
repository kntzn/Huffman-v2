#include <stdlib.h>
#include <iostream>

#define N_CHARS 256

struct CharFreq
    {
    unsigned long freq;
    char ch;
    };

int CompareCharFreq (const void * left, const void * right);

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

    std::qsort (freq_table, N_CHARS, sizeof (CharFreq), CompareCharFreq);


    // Outputs the table
    for (int i = 0; i < N_CHARS; i++)
        { 
        if (freq_table [i].freq)
            std::cout << freq_table [i].ch << " (" << int ( freq_table [i].ch) << ") - " << freq_table [i].freq << std::endl;
        }

    system ("pause");
    }

int CompareCharFreq (const void * left, const void * right)
    {
    const CharFreq* obj_left = (CharFreq*) left;
    const CharFreq* obj_right = (CharFreq*) right;

    return (obj_left->freq > obj_right->freq) ? -1 : 1;
    }
