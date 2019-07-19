#include <stdlib.h>
#include <iostream>

#define N_CHARS 256

int main ()
    { 
    char string [] = "beep boop beer!";
    unsigned long len = 16;

    int freq_table [N_CHARS] = { };
    for (int i = 0; i < len; i++)
        freq_table [string [i]]++;

    for (int i = 0; i < N_CHARS; i++)
        { 
        if (freq_table [i])
            std::cout << char (i) << " (" << i << ") - " << freq_table [i] << std::endl;
        }

    system ("pause");
    }