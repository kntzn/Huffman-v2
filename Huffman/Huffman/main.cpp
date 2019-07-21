#include <stdlib.h>
#include <iostream>
#include <list>
#include <vector>
#include <assert.h>

#define N_CHARS 256
#define MAX_DEPTH N_CHARS
#define EMPTY_NODE_CHAR -1
#define DIR_RIGHT 1
#define DIR_LEFT 0

struct CharTreeNode
    {
    unsigned long freq;
    char ch;
    CharTreeNode* ptr_to_root;
    CharTreeNode* ptr_to_left;
    CharTreeNode* ptr_to_right;
    std::vector <bool> code;

    CharTreeNode ():
        freq (0),
        ch (-1),
        ptr_to_left (nullptr),
        ptr_to_right (nullptr),
        ptr_to_root (nullptr)
        { /*BVVUUUUUUUS'*/ }
    };

struct FRQ_CMP
    {
    bool operator() (const CharTreeNode* l, const CharTreeNode* r) const 
        {
        return l->freq < r->freq; 
        } 
    };

void printTree (CharTreeNode* parent, int depth = 0, bool mask [MAX_DEPTH] = { });

void freeTree (CharTreeNode * root);
void saveCodes (CharTreeNode * root, std::vector <bool> &root_code);


int main ()
    { 
    char string [] = "beep boop beer!"; 
    unsigned long len = 16;

    // Sets up freq_table and sets chars
    CharTreeNode* freq_table = (CharTreeNode*) calloc (N_CHARS, sizeof (CharTreeNode));
    if (!freq_table) 
        exit (1);
    for (int i = 0; i < N_CHARS; i++)
        freq_table [i].ch = i;

    // Counts frequency and fills the table
    for (int i = 0; i < len; i++)
        freq_table [string [i]].freq++;

    // Counts used chars
    // And saves them to table
    std::list <CharTreeNode*> table;
    int chars_used = 0;
    for (int i = 0; i < N_CHARS; i++)
        if (freq_table [i].freq)
            {
            chars_used++;
            table.push_back (freq_table + i);
            }

    std::cout << "Chars used: " << chars_used << "/" << N_CHARS << std::endl;

    // Builds the tree
    while (table.size () != 1)
        {
        // Sorts the nodes by their frequencies
        table.sort (FRQ_CMP ());

        // Saves the pointers to the rarest elements
        CharTreeNode* SonL = table.front ();
        table.pop_front ();
        CharTreeNode* SonR = table.front ();
        table.pop_front ();

        // And creates new element that replaces them 
        CharTreeNode *parent =  (CharTreeNode*) calloc (1, sizeof (CharTreeNode));
        if (!parent)
            exit (1);

        SonL->ptr_to_root = parent;
        SonR->ptr_to_root = parent;

        parent->ptr_to_left = SonL;
        parent->ptr_to_right = SonR;
        
        parent->ch = EMPTY_NODE_CHAR;
        parent->freq = SonL->freq + SonR->freq;

        table.push_front (parent);
        }
    CharTreeNode *root = table.back ();
    
    printTree (root);

    saveCodes (root, std::vector <bool> (0));

    // TODO: Codes saver (Array or smth)
    // TODO: string conversion

    // TODO: File Load/Save

    // Frees the memory
    freeTree (root);
    free (freq_table);
    
    system ("pause");
    }



void printTree (CharTreeNode * parent, int depth, bool mask [MAX_DEPTH])
    {
    // mmm, console graphics
    
    // Creates new copy of the mask
    bool new_mask [MAX_DEPTH] = { };
    for (int i = 0; i < MAX_DEPTH; i++)
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
        printTree (parent->ptr_to_left, depth + 1, new_mask);
    
    // Right-hand node
    new_mask [depth] = true;
    if (parent->ptr_to_right != nullptr)
        printTree (parent->ptr_to_right, depth + 1, new_mask);
    }

void freeTree (CharTreeNode * root)
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

void saveCodes (CharTreeNode * root, std::vector <bool> &root_code)
    {
    if (root->ptr_to_left != NULL)
        {
        root_code.push_back (DIR_LEFT);
        saveCodes (root->ptr_to_left, root_code);
        }
    if (root->ptr_to_right != NULL)
        {
        root_code.push_back (DIR_RIGHT);
        saveCodes (root->ptr_to_right, root_code);
        }

    if (root->ptr_to_left == NULL && root->ptr_to_right == NULL)
        {
        root->code = root_code;
        }

    if (root_code.size ())
        root_code.pop_back ();
    }
