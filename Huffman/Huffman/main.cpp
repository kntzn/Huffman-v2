﻿#include <stdlib.h>
#include <iostream>
#include <list>
#include <deque>
#include <vector>
#include <assert.h>
#include <ctime>
#include "FileIO.h"
#include "sarray.h"
#include "sort.h"

#define N_CHARS 256
#define MAX_DEPTH N_CHARS
#define EMPTY_NODE_CHAR 255
#define DIR_RIGHT 1
#define DIR_LEFT 0

struct CharTreeNode
    {
    unsigned long freq;
    unsigned char ch;
    CharTreeNode* ptr_to_root;
    CharTreeNode* ptr_to_left;
    CharTreeNode* ptr_to_right;
    
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
        return l->freq > r->freq; 
        } 
    };

void printTable (CharTreeNode freq_table [N_CHARS], sarray <bool, MAX_DEPTH> codes [N_CHARS]);
void printTree (CharTreeNode* parent, int depth = 0, bool mask [MAX_DEPTH] = { });
void printStringCodeVector (std::vector <bool> string_code);
void printOutputStringInt (std::string output_string);

CharTreeNode* buildTree (CharTreeNode freq_table [N_CHARS]);
void saveCodes (CharTreeNode * root,
                sarray <bool, MAX_DEPTH> codes [N_CHARS],
                sarray <bool, MAX_DEPTH> &root_code);
void convert (std::string &output_string,
               char* string, long len,
               sarray <bool, MAX_DEPTH> codes [N_CHARS]);
void freeTree (CharTreeNode * root);

int main ()
    {
    clock_t start = clock ();

    // String and fileIO setup
    char* string = nullptr;
    long len = 0;
    FileIO file;
    file.fastLoad ("Валентин-mini.txt", string, len);
    //len++;

    /*
    #ifdef (O1)
        for (int i = 0; i < len; i++)
            if ('A' <= string [i] && string [i] <= 'Z')
                string [i] = tolower (string [i]);
    #endif*/

    // Sets up freq_table and sets chars
    CharTreeNode* freq_table = (CharTreeNode*) calloc (N_CHARS, sizeof (CharTreeNode));
    assert (freq_table);
    for (int i = 0; i < N_CHARS; i++)
        {
        freq_table [i].ch   = i;
        freq_table [i].freq = 1;
        }

    // Converts bits array to string
    std::string output_string;
    char last_char = 0;
    int reg = 0;

    std::cout << __LINE__ << std::endl;

    
    for (int i = 0; i < len; i++)
        {
        //std::cout << i << '/' << len << ' ';

        unsigned char curr_char = string [i];

        //std::cout << curr_char << ' ';

        // Updates the table
        freq_table [curr_char].freq++;


        // Builds the tree
        CharTreeNode* root = buildTree (freq_table); // 9600 ms // Insertion sort
        
       
        //printTree (root);                         

        //std::cout << std::endl;

        sarray <bool, MAX_DEPTH> codes [N_CHARS] = { };
        saveCodes (root, codes, sarray <bool, N_CHARS> ()); // ~0 ms
        //printTable (freq_table, codes);


        size_t curr_char_code_size = codes [curr_char].size ();

        //std::cout << curr_char_code_size << std::endl;
        // ~ 0 ms
        
        for (int j = 0; j < curr_char_code_size; j++)
            {
            //std::cout << codes [curr_char] [j] << ' ';

            last_char |= codes [curr_char] [j]; // 2900 ms
            if (reg != 7)
                {
                last_char <<= 1; // 350ms
                reg++;
                }
            else
                {
                output_string.push_back (last_char); // 400ms
                last_char = 0;
                reg = 0;
                }
            }

        //printOutputStringInt (output_string);
        
        // frees the tree
        //freeTree (root);
        }
    
    if (reg != 0)
        {
        last_char <<= (7 - reg);
        output_string.push_back (last_char);
        }

    // Saves output_string
    file.fastSave ("output.txt", output_string.c_str (), output_string.size ());

    // Frees the memory
    free (freq_table);
    
    std::cout << clock () - start << " ms" << std::endl;
    std::cout << "Compression: " << 100 - output_string.size () * 100 / len << "%" << std::endl;

    system ("pause");
    }

void printTable (CharTreeNode freq_table [N_CHARS], sarray <bool, MAX_DEPTH> codes [N_CHARS])
    { 
    std::cout << std::endl << std::endl;

    for (int i = 0; i < N_CHARS; i++)
        if (freq_table [i].freq)
            {
            std::cout << "'" << freq_table [i].ch << "'\t (" <<
                int (freq_table [i].ch) << ")\t freq: " <<
                freq_table [i].freq << "\t code: ";

            
            for (int j = 0; j < codes [i].size (); j++)
                std::cout << codes [i][j];

            std::cout << std::endl;
            }
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
void printStringCodeVector (std::vector<bool> string_code)
    {
    for (int i = 0; i < string_code.size (); i++)
        std::cout << int (string_code [i]) << " ";
    std::cout << std::endl;
    }
void printOutputStringInt (std::string output_string)
    {
    for (int i = 0; i < output_string.size (); i++)
        std::cout << int (output_string [i]) << " ";
    std::cout << std::endl;
    }

int cmp_freq (const void* left, const void* right)
    {
    return (int (((CharTreeNode*) right)->freq) - int (((CharTreeNode*) left)->freq));
    }

void QuickSort (CharTreeNode* data [], int left, int right)
    {
    int CurL = left;
    int CurR = right;
    CharTreeNode* CurMid = data [(left + right) / 2];

    while (CurL <= CurR)
        {
        while (data [CurL]->freq > CurMid->freq) CurL++;
        while (data [CurR]->freq < CurMid->freq) CurR--;

        if (CurL <= CurR)
            {
            if (data [CurL]->freq < data [CurR]->freq)
                {
                auto temp_CurL = data [CurL];
                data [CurL]    = data [CurR];
                data [CurR]    = temp_CurL;
                }

            CurL++;
            CurR--;
            }

        if (CurL > CurR) break;
        }

    if (CurL < right) QuickSort (data, CurL, right);
    if (left < CurR)  QuickSort (data, left, CurR);
    }
CharTreeNode * buildTree (CharTreeNode freq_table [N_CHARS])
    {
    // Saves used chars in table
    //std::list <CharTreeNode*> table; // 9500ms

    sarray <CharTreeNode*, N_CHARS> table;

    for (int i = 0; i < N_CHARS; i++)
        if (freq_table [i].freq)
            table.push_back (freq_table + i);
    
    // Builds the tree
    while (table.size () != 1)
        {
        // Sorts the nodes by their frequencies
        //table.sort (FRQ_CMP ());
        
        
        QuickSort (table.data (), 0, table.size () - 1); // 2000 ms


        // Saves the pointers to the rarest elements
        CharTreeNode* SonL = table.back ();
        table.pop_back ();
        CharTreeNode* SonR = table.back ();
        table.pop_back ();

        // And creates new element that replaces them 
        CharTreeNode *parent = new CharTreeNode;// calloc (1, sizeof (CharTreeNode));
        if (!parent)
            exit (1);


        SonL->ptr_to_root = parent;
        SonR->ptr_to_root = parent;

        parent->ptr_to_left = SonL;
        parent->ptr_to_right = SonR;

        parent->ch = EMPTY_NODE_CHAR;
        parent->freq = SonL->freq + SonR->freq;

        table.push_back (parent);
        }

    

    return table.back ();
    }
void saveCodes (CharTreeNode * root, 
                sarray <bool, MAX_DEPTH> codes [N_CHARS],
                sarray <bool, MAX_DEPTH> &root_code)
    {
    if (root->ptr_to_left != NULL)
        {
        root_code.push_back (DIR_LEFT);
        saveCodes (root->ptr_to_left, codes, root_code);
        }
    if (root->ptr_to_right != NULL)
        {
        root_code.push_back (DIR_RIGHT);
        saveCodes (root->ptr_to_right, codes, root_code);
        }

    if (root->ptr_to_left == NULL && root->ptr_to_right == NULL)
        {
        codes [root->ch] = root_code;

        if (!root_code.size ())
            codes [root->ch].push_back (0);
        }

    if (root_code.size ())
        root_code.pop_back ();
    }
void convert (std::string &output_string,
               char* string, long len,
               sarray <bool, MAX_DEPTH> codes [N_CHARS])
    {
    char last_char = 0;
    int reg = 0;

    for (int i = 0; i < len; i++)
        {
        unsigned char curr_char = string [i];

        size_t curr_char_code_size = codes [curr_char].size ();

        
        for (int j = 0; j < curr_char_code_size; j++)
            {
            
            last_char |= codes [curr_char] [j]; // 2900 ms
            if (reg != 7)
                {
                last_char <<= 1; // 350ms
                reg++;
                }
            else
                {
                output_string.push_back (last_char); // 400ms
                last_char = 0;
                reg = 0;
                }
            }
        
        }
    if (reg != 0)
        {
        last_char <<= (7 - reg);
        output_string.push_back (last_char);
        }
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