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
    CharFreq* freq_table = (CharFreq*) calloc (N_CHARS, sizeof (CharFreq));
    for (int i = 0; i < N_CHARS; i++)
        freq_table [i].ch = i;

    // Counts frequency
    for (int i = 0; i < len; i++)
        freq_table [string [i]].freq++;

    // Sorts the table
    qsort (freq_table, N_CHARS, sizeof (CharFreq), CompareCharFreq);

    // Counts used chars
    int chars_used = 0;
    while (freq_table [chars_used].freq)
        chars_used++;
    std::cout << "Chars used: " << chars_used << std::endl;

    // Removes unused chars from table
    CharFreq* freq_table_sorted = (CharFreq*) calloc (chars_used, sizeof (CharFreq));
    for (int i = 0; i < chars_used; i++)
        freq_table_sorted [i] = freq_table [i];
    free (freq_table);

    // Outputs the table
    for (int i = 0; i < chars_used; i++)
        { 
        std::cout << freq_table_sorted [i].ch << " (" << int (freq_table_sorted [i].ch) << ") - " << freq_table_sorted [i].freq << std::endl;
        }

    system ("pause");
    }

int CompareCharFreq (const void * left, const void * right)
    {
    const CharFreq* obj_left = (CharFreq*) left;
    const CharFreq* obj_right = (CharFreq*) right;

    return (obj_left->freq > obj_right->freq) ? -1 : 1;
    }
