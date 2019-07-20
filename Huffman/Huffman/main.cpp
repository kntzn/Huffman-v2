#include <stdlib.h>
#include <iostream>
#include <list>
#include <assert.h>

#define N_CHARS 256
#define EMPTY_NODE_CHAR -1
#define DIR_RIGHT 1
#define DIR_LEFT 0

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
    {
    bool operator() (const CharFreq* l, const CharFreq* r) const 
        {
        return l->freq < r->freq; 
        } 
    };

void printTree (CharFreq* parent, int depth = 0, bool mask [N_CHARS] = { });
void freeTree (CharFreq * root);

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
        // Sorts the nodes by their frequencies
        table.sort (FRQ_CMP ());

        // Saves the pointers to the rarest elements
        CharFreq* SonL = table.front ();
        table.pop_front ();
        CharFreq* SonR = table.front ();
        table.pop_front ();

        // And creates new element that replaces them 
        CharFreq *parent =  (CharFreq*) calloc (1, sizeof (CharFreq));

        SonL->ptr_to_root = parent;
        SonR->ptr_to_root = parent;

        parent->ptr_to_left = SonL;
        parent->ptr_to_right = SonR;
        
        parent->ch = EMPTY_NODE_CHAR;
        parent->freq = SonL->freq + SonR->freq;

        table.push_front (parent);
        }
    
    CharFreq *root = table.back ();
    
    printTree (root);

    
    freeTree (root);
    
    // TODO: Memory cleaner for tree
    // TODO: Codes saver (Array or smth)
    // TODO: string conversion

    // TODO: File Load/Save

    system ("pause");
    }



void printTree (CharFreq * parent, int depth, bool mask [N_CHARS])
    {
    // mmm, console graphics
    
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
    bool dir_code = DIR_LEFT;
    if (parent->ptr_to_root != nullptr &&
         parent->ptr_to_root->ptr_to_right == parent)
        dir_code = DIR_RIGHT;

    // Tabs according to the node depth
    //
    // If mask is inactive
    // Or if this node is the right hand of it's parent
    // Bar is being displayed
    for (int i = 0; i < depth; i++)
        if (!new_mask [i] ||
            (dir_code == DIR_RIGHT && i == depth - 1))
            std::cout << "     |";
        else
            std::cout << "      ";

    // Draws the joint (with direction code)
    std::cout << "__" << dir_code << "__";

    // Prints the leafs    
    if (parent->ptr_to_left == nullptr &&
        parent->ptr_to_right == nullptr)
        std::cout << '\"' << parent->ch << 
                     "\" (" << int (parent->ch) << ')' <<
                     " --- " << depth;
    
    std::cout << std::endl;

    // here console graphics ends


    
    
    
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

void freeTree (CharFreq * root)
    {
    
    if (root->ptr_to_left != nullptr)
        freeTree (root->ptr_to_left);
    if (root->ptr_to_right != nullptr)
        freeTree (root->ptr_to_right);
    
    // Do not free the char nodes (leafs)
    if (root->ptr_to_left != nullptr ||
        root->ptr_to_right != nullptr)
        free (root);
    }
