#include <stdlib.h>
#include <iostream>
#include <list>

#define N_CHARS 256

struct CharFreq
    {
    unsigned long freq;
    char ch;
    CharFreq* ptr_to_root;
    CharFreq* ptr_to_left;
    CharFreq* ptr_to_right;

    CharFreq ():
        freq (0),
        ch (-1),
        ptr_to_left (nullptr),
        ptr_to_right (nullptr),
        ptr_to_root (nullptr)
        { /*BVVUUUUUUUS'*/ }
    };

struct FRQ_CMP
    { bool operator()(const CharFreq* l, const CharFreq* r) const { return l->freq > r->freq; } };

int CompareCharFreq (const void * left, const void * right);

void printTree (int depth, CharFreq* parent);

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
    std::list <CharFreq*> table;
    for (int i = 0; i < chars_used; i++)
        table.push_back (freq_table + i);
    

    // Builds the tree
    while (table.size () != 1)
        {
        table.sort (FRQ_CMP ());

        for (auto i : table)
            std::cout << i->ch << " (" << int (i->ch) << ") - " << i->freq << std::endl;

        std::cout << std::endl;

        CharFreq* SonL = table.front ();
        table.pop_front ();
        CharFreq* SonR = table.front ();
        table.pop_front ();

        CharFreq *parent = new CharFreq;

        SonL->ptr_to_root = parent;
        SonR->ptr_to_root = parent;

        parent->ptr_to_left = SonL;
        parent->ptr_to_right = SonR;
        
        parent->ch = 0;
        parent->freq = SonL->freq + SonR->freq;

        table.push_front (parent);

        }
    
    CharFreq *root = table.back ();
    
    printTree (0, root);
    



    system ("pause");
    }

int CompareCharFreq (const void * left, const void * right)
    {
    const CharFreq* obj_left = (CharFreq*) left;
    const CharFreq* obj_right = (CharFreq*) right;

    return (obj_left->freq > obj_right->freq) ? -1 : 1;
    }

void printTree (int depth, CharFreq * parent)
    {
    
    if (parent->ptr_to_left != nullptr)
        printTree (depth + 1, parent->ptr_to_left);
    if (parent->ptr_to_right != nullptr)
        printTree (depth + 1, parent->ptr_to_right);

    for (int i = 0; i < depth; i++)
        std::cout << "    ";

    std::cout << parent->ch << std::endl;
    }
