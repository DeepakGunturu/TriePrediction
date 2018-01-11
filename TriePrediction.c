// Name: Deepak Kumar Gunturu
// PID: 3924312
// NID: de143131

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "TriePrediction.h"


// This function creates a new trie node when called and returns the root of the new trie
TrieNode *createTrieNode(void)
{
    int i;
    TrieNode *creator = malloc(sizeof(TrieNode));
    for (i = 0; i < 26; i++)
    {
        creator->children[i] = NULL;
    }
    creator->subtrie = NULL;
    creator->count = 0;
    
    return creator;
}

// This function is used to check for punctuation at the terminal positions of a given string
int checkPunctuation(char *str)
{
    
    int length = strlen(str);
    
    if(str[length-1] == '!' || str[length-1] == '?' || str[length-1] == '.')
        return 1;
 
    return 0;
}

// This function removes any puncutations in the string parameter that is passed and copies the new string without any punctuations in it
void stripPunctuators(char *str)
{
    int i,j,length;
    
    length = strlen(str);
    
    char temp[1026];
    
    j = 0;
    
    for(i = 0; i < length; i++)
    {
        if(isalpha(str[i]))
        {
            temp[j] = str[i];
            temp[j] = (tolower(temp[j]));
            j++;
        }
    }
    
    temp[j] = '\0';
    strcpy(str, temp);
}

// Inserts a particular string at the end of the trie
void insertToTrie(TrieNode *node, char *str, int i)
{
    int pos;
    int length = strlen(str);
    char next = str[i];

    if (i > length-1)
        return;
    
    pos = next - 'a';
    
    if (node->children[pos] == NULL)
    {
        node->children[pos] = createTrieNode();
    }
    
    if (i == length - 1)
    {
        node->children[pos]->count++;
    }
    
    
    insertToTrie(node->children[pos], str, i+1);
}


// Returns the terminal node of the end of the string if the string is in the trie
TrieNode *getNode(TrieNode *root, char *str)
{
    
    if (root == NULL || (str[0] == '\0' && root->count == 0))
        return NULL;
    
    else if (str[0] == '\0' && root->count > 0)
        return root;
    
    else
    {
        char temp = str[0];
        str++;
        int i = temp - 'a';
        
        if (root->children[i] == NULL)
            return NULL;
        
        
        return getNode(root->children[i], str);
    }
}

// This function takes in the corpus file as an argument and builds a trie out of that by removing the punctuations
TrieNode *buildTrie(char *filename)
{
    TrieNode *root;
    TrieNode *temp = NULL;
    
    char strIn[MAX_CHARACTERS_PER_WORD];
    char str[MAX_CHARACTERS_PER_WORD];
    char *str2;
    char prev[MAX_CHARACTERS_PER_WORD];
    
    FILE *fopener;
    fopener = fopen(filename, "r");
    
    root = createTrieNode();
    strcpy(prev, "");
    
    
    // Writing the corpus file into the string input that we collected
    while(fscanf(fopener, "%s", strIn) != EOF)
    {
        
        strcpy(str, strIn);
        stripPunctuators(str);
        
        
        // If there is any punctuation left over even after the punctuation is stripped from the string, this case executes
        if(checkPunctuation(strIn))
        {
            
            if(temp == NULL)
                insertToTrie(root, str, 0);
            
            else {
                insertToTrie(root, str, 0);
                
                if(temp->subtrie == NULL)
                    temp->subtrie = createTrieNode();
                
                insertToTrie(temp->subtrie, str, 0);
                temp = NULL;
            }
  
            if(checkPunctuation(prev))
            {
                strcpy(prev, strIn);
                continue;
            }
            
        }
       
        // Case for when the strip punctuation function works
        else
        {
  
            insertToTrie(root, str, 0);
            
            if(temp == NULL)
            {
                temp = getNode(root, str);
            }
            
            else
            {
           
                if(temp->subtrie == NULL)
                {
                    temp->subtrie = createTrieNode();
                }
                
               
                insertToTrie(temp->subtrie, str, 0);
         
                temp = getNode(root, str);
            }
        }
        
        
        strcpy(prev, strIn);
        
    }
    fclose(fopener);
    return root;
}

// This function was written by Dr. Szumlanski to act as a helper for printing a trie
void printTrieHelper(TrieNode *root, char *buffer, int k)
{
    int i;
    
    if (root == NULL)
        return;
    
    if (root->count > 0)
        printf("%s (%d)\n", buffer, root->count);
    
    buffer[k + 1] = '\0';
    
    for (i = 0; i < 26; i++) {
        buffer[k] = 'a' + i;
        
        printTrieHelper(root->children[i], buffer, k + 1);
    }
    
    buffer[k] = '\0';
}

// This function was written by Dr. Szumlanski to print a trie
void printTrie(TrieNode *root, int useSubtrieFormatting)
{
    char buffer[1026];
    
    if (useSubtrieFormatting) {
        strcpy(buffer, "- ");
        printTrieHelper(root, buffer, 2);
    }
    else {
        strcpy(buffer, "");
        printTrieHelper(root, buffer, 0);
    }
}

// This helper function is used to take a trie and check the most frequently occuring string in the trie
void mostFrequentWordhelper(TrieNode *root, char *str, int *strCount, char *str2, int k, int *init)
{
    if (root == NULL)
        return;
    
    int i;
    
    if (root->count > 0)
    {
        
        if(*init == 0)
        {
            strcpy(str, str2);
            *strCount = root->count;
            
            *init = 1;
        }
        
        
        if(root->count > *strCount)
        {
           
            *strCount = root->count;
            strcpy(str, str2);
            
        }
        
    }
    
    str2[k + 1] = '\0';
    
    for (i = 0; i < 26; i++)
    {
        str2[k] = 'a' + i;
        
        mostFrequentWordhelper(root->children[i], str, strCount, str2, k + 1, init);
    }
    str2[k] = '\0';
}

// This function checks the trie for the most frequently occuring string in the Trie and copies the string into the second parameter
void getMostFrequentWord(TrieNode *root, char *str)
{
    if(root == NULL)
    {
        strcpy(str,"");
        return;
    }
    
    int temp1,temp2,*strCount, *i;
    
    char sampStr[MAX_CHARACTERS_PER_WORD];
    char stringer[MAX_CHARACTERS_PER_WORD];

    TrieNode *end = getNode(root, str);
    
    i = &temp2;
    strCount = &temp1;
    *i = 0;

    if(end == NULL || end->subtrie == NULL)
        return;
    
    mostFrequentWordhelper(end->subtrie, sampStr, strCount, stringer, 0, i);
    strcpy(str, sampStr);
    
}

// Returns the terminal node of the end of the string if the string is in the trie
int containsWord(TrieNode *root, char *str)
{
    if(root == NULL || (str[0] == '\0' && root->count == 0))
        return 0;
    
   
    if(getNode(root,str) != NULL)
        return 1;
    
    else
        return 0;
}

// Helper function to add the number of times a substring occurs in a string in a trie
int SumOfChildren(TrieNode *root)
{
    if(root == NULL)
        return 0;
    
    int i, sum = 0;
    
    sum = root->count;
    
    for(i = 0; i < 26; i++)
    {
        sum += SumOfChildren(root->children[i]);
    }
    
    return sum;
    
}

// This function returns the number of strings that begin with the specified string str
int prefixCount(TrieNode *root, char *str)
{
    if(root == NULL)
        return 0;
    
    if(str == NULL || str[0] == '\0')
        return SumOfChildren(root);
    
    return prefixCount(root->children[str[0]-'a'], str+1);
}

// This function is a helper fumction that takes the corpus file as a command line input and writes it into the main function using test prediction commands like "@" and "!"
void mainFuctInputFileReader(char *filename, TrieNode *root)
{
    int j, length, i = 0;
    TrieNode *end = NULL, *node = NULL;
    char temp[MAX_CHARACTERS_PER_WORD];
    char inputTest[MAX_CHARACTERS_PER_WORD];
    char input[MAX_CHARACTERS_PER_WORD];
    
    FILE *file;
    file = fopen(filename, "r");
    
    while(fscanf(file, "%s", inputTest) != EOF)
    {
        
        // Case for command line argument "!"
        if(strcmp(inputTest, "!") == 0)
        {
            printTrie(root, 0);
            
        }
        
        // Case for command line argument "@"
        else if(strcmp(inputTest, "@") == 0)
        {
            
            fscanf(file, "%s", inputTest);
            fscanf(file, "%d", &i);
            printf("%s", inputTest);
            
            strcpy(input, inputTest);
            stripPunctuators(input);
            
       
            end = getNode(root, input);
            
            
            // If node to be searched points to NULL
            if(end == NULL)
            {
                printf("\n");
                continue;
            }
            
            getMostFrequentWord(root, input);
            
            strcpy(temp, input);
            printf(" %s", temp);
            
            if(i > 1)
            {
                
                for(j = 1; j < i; j++)
                {
                    
                    end = getNode(root, temp);
                    
                    if(end->subtrie == NULL)
                    {
                        break;
                    }
                    
                    getMostFrequentWord(root, temp);
                    printf(" %s", temp);
                    
                }
            }
            
            printf("\n");
            
        }
        
        // Case for when a node does not point to NULL
        else
        {
            printf("%s\n", inputTest);
            length = strlen(inputTest);
            
            
            for(i = 0; i < length; i++)
            {
                inputTest[i] = tolower(inputTest[i]);
            }
            
            
            
            if(getNode(root, inputTest) == NULL)
            {
                printf("(INVALID STRING)\n");
                continue;
            }
            node = getNode(root, inputTest);
            if(node->subtrie == NULL)
            {
                printf("(EMPTY)\n");
                continue;
            }
            printTrie(node->subtrie, 1);
        }
    }
    
    fclose(file);
}

// This function frees up all the memory associated with a trie and returns NULL
TrieNode *destroyTrie(TrieNode *root)
{
    int i;
    
    for(i = 0; i < 26; i++)
    {
        if (root->children[i] != NULL)
            destroyTrie(root->children[i]);
    }
        
    if (root->subtrie != NULL)
        destroyTrie(root->subtrie);
    
    free(root);
    return NULL;
}

// The difficulty rating of the program
double difficultyRating(void)
{
    return 4.2;
}
// Number of hours spent on the assignment
double hoursSpent(void)
{
    return 12.5;
}

// The main function is used here for the corpus test cases and the mainFuctInputFileReader reads from the corpus files as command line arguments
int main(int argc, char **argv)
{
    
    TrieNode *root = buildTrie(argv[1]);
    
    mainFuctInputFileReader(argv[2], root);
    
    root = destroyTrie(root);
    
   
    return 0;
}
