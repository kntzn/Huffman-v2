#include <stdlib.h>
#include <iostream>
#include <list>
#include <assert.h>

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
    { bool operator()(const CharFreq* l, const CharFreq* r) const { return l->freq < r->freq; } };

int CompareCharFreq (const void * left, const void * right);

void printTree (CharFreq* parent, int depth = 0, bool mask [N_CHARS] = { });

int main ()
    { 
    char string [] = "beep boop beer!";
    unsigned long len = 16;

    // Sets up freq_table and sets chars
    CharFreq freq_table [N_CHARS];
    for (int i = 0; i < N_CHARS; i++)
        freq_table [i].ch = i;

    // Counts frequency
    for (int i = 0; i < len; i++)
        freq_table [string [i]].freq++;

    // Counts used chars
    // And saves them to table
    std::list <CharFreq*> table;
    int chars_used = 0;
    for (int i = 0; i < N_CHARS; i++)
        if (freq_table [i].freq)
            {
            chars_used++;
            table.push_back (freq_table + i);
            }


    std::cout << "Chars used: " << chars_used << std::endl;

    
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
        
        parent->ch = -1;
        parent->freq = SonL->freq + SonR->freq;

        table.push_front (parent);

        }
    
    CharFreq *root = table.back ();
    
    printTree (root);
    



    system ("pause");
    }

int CompareCharFreq (const void * left, const void * right)
    {
    const CharFreq* obj_left = (CharFreq*) left;
    const CharFreq* obj_right = (CharFreq*) right;

    return (obj_left->freq > obj_right->freq) ? -1 : 1;
    }

void printTree (CharFreq * parent, int depth, bool mask [])
    {
    // Creates new copy of the mask
    bool new_mask [N_CHARS] = { };
    for (int i = 0; i < N_CHARS; i++)
        {
        if (depth)
            new_mask [i] = mask [i];
        else 
            new_mask [i] = 0;
        }


    // Gets the direction code (0 or 1)
    bool dir_code = 0;
    if (parent->ptr_to_root != nullptr &&
         parent->ptr_to_root->ptr_to_right == parent)
        dir_code = 1;

    // Tabs according to the node depth
    //
    // If mask is inactive
    // Or if this node is the right hand of it's parent
    // Bar is being displayed
    for (int i = 0; i < depth; i++)
        if (!new_mask [i] ||
            (dir_code == 1 && i == depth - 1))
            std::cout << "     |";
        else
            std::cout << "      ";

    std::cout << "__" << dir_code << "__";

    // Prints the leafs    
    if (parent->ptr_to_left == nullptr &&
        parent->ptr_to_right == nullptr)
        std::cout << '\"' << parent->ch << 
                     "\" (" << int (parent->ch) << ')' <<
                     " --- " << depth << std::endl;
    // or joints
    else
        std::cout << std::endl;

    // Just to be on the safe side
    assert (0 <= depth && depth < 256);
        
    // Left-hand node
    if (parent->ptr_to_left != nullptr)
        { 
        printTree (parent->ptr_to_left, depth + 1, new_mask);
        }
    // Right-hand node
    new_mask [depth] = true;
    if (parent->ptr_to_right != nullptr)
        printTree (parent->ptr_to_right, depth + 1, new_mask);

    }
