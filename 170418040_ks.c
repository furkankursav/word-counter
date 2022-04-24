#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

// function prototypes.
void IncreaseCounter(int*, int);
bool IsFileValid(FILE*);
void ListDetailsFromFile(char*);
void PrintDetails(char* fileName, int lineCount, int wordCount, int charCount, bool bPrintLines, bool bPrintWords, bool bPrintChars);
bool StartsWith(const char* pre, const char* str);

// Total Counters.
int totalLineCount = 0;
int totalWordCount = 0;
int totalCharCount = 0;

// Default we will not print any details.
bool bPrintLines = false;
bool bPrintWords = false;
bool bPrintChars = false;


// word counter flags.
#define OUT 0
#define IN 1

int main (int argc, char* argv[]) 
{
    int ParamCount = 0;

    // First we're counting how many parameters given
    for(int i = 1; i < argc; i++)
    {
        if(StartsWith("-", argv[i]))
        {
            ParamCount++;
        }

        else
        {
            break;
        }
    }

    // If user did not give any parameters, we'll write all details.
    if(ParamCount == 0)
    {
        bPrintLines = true;
        bPrintWords = true;
        bPrintChars = true;
    }

    // but user gived parameters, check what tey are
    else
    {
        for(int i = 1; i < argc; i++)
        {
            if(StartsWith("-", argv[i]))
            {
                // -l means print line count
                if(strcmp(argv[i], "-l") == 0)
                    bPrintLines = true;

                // -w means print word count
                else if(strcmp(argv[i], "-w") == 0)
                    bPrintWords = true;

                // -c means print char count
                else if(strcmp(argv[i], "-c") == 0)
                    bPrintChars = true;

                // invalid parameter, determinate program.
                else
                {
                    fprintf(stderr, "unknown parameteter '%s'\n", argv[i]);
                    return 0;
                }
            }
            
        }
    }
    
    // If user did not give any txt file, then read from standart input
    if(argc < 2)
    {
        ListDetailsFromFile("-stdin-");
        return 0;
    }

    // iterating all given arguments.
    for(int i = 1; i < argc; i++)
    {
        if(StartsWith("-", argv[i]))
        {
            continue;
        }

        else if(strcmp(argv[i], "+") == 0)
        {
            ListDetailsFromFile("-stdin-");
        }
            
        else
        {
            ListDetailsFromFile(argv[i]);
        }
    }

    printf("\n");

    // if txt file count greater than 2
    if(argc - ParamCount > 2)
    {
        // then print total counters.
        PrintDetails("total", totalLineCount, totalWordCount, totalCharCount, bPrintLines, bPrintWords, bPrintChars);
    }

    exit(0);
}

bool StartsWith(const char* pre, const char* str)
{
    size_t lenpre = strlen(pre);
    size_t lenstr = strlen(str);

    return lenstr < lenpre ? false : memcmp(pre, str, lenpre) == 0;
}

void IncreaseCounter(int* InNumber, int amount)
{
    (*InNumber) += amount;
}

/*
 * returns true if given file is valid
 * @param InFile, file to check.
 */
bool IsFileValid(FILE* InFile)
{
    return !(InFile == NULL);
}

/*
 * Calculating lines, words and chars counts.
 * @param fileName, file name or give -stdin- if you want read from standart input.
*/
void ListDetailsFromFile(char* fileName)
{

    // trying to open file in read mode
    FILE* fp;

    if(strcmp(fileName, "-stdin-") == 0)
    {
        fp = stdin;
    }

    else
    {
        fp = fopen(fileName, "r");
    }
    
    

    // checking file succesfully opened?
    if(IsFileValid(fp) == false) 
    {
        // if it was not, then continue with next file
        fprintf(stderr, "'%s' file could not open!\n", fileName);
        return;
    }
    
    // current character
    int ch;

    // line, word and character counters.
    int charCount = 0;
    int wordCount = 0;
    int lineCount = 0;

    // word count flag
    int state = IN;

    // if file opened succesfully then start counting. 
    do 
    {
        // getting current char with fgetc()
        ch = fgetc(fp);

        // if current character is space setting our flag to 0
        if(isspace(ch))
        {
            state = OUT;
        }

        // if our flag is 0 then increase word counter
        else if(state == OUT)
        {
            state = IN;
            IncreaseCounter(&wordCount, 1);
        }

        // if this character is end of the line or new line 
        if (ch == '\n')
        {
            // then increase line counter.
            IncreaseCounter(&lineCount, 1);
        }

        // if we reached end of the file
        if(feof(fp)) 
        {
            // then break.
            break;
        }

        // increasing character counter after every charater.
        IncreaseCounter(&charCount, 1);
        
    } while(1);

    
    
    
    // closing file.
    if(strcmp(fileName, "-stdin-") != 0)
        fclose(fp);

    // increasing total counters.
    IncreaseCounter(&totalCharCount, charCount);
    IncreaseCounter(&totalWordCount, wordCount);
    IncreaseCounter(&totalLineCount, lineCount);

    PrintDetails(fileName, lineCount, wordCount, charCount, bPrintLines, bPrintWords, bPrintChars);
    
}

/*
 * Printing details with given parameters.
 * @param fileName, name of file
 * @param lineCount, count of lines
 * @param wordCount, count of words
 * @param charCount, count of chars
 * @param bPrintLines, pass true if you want print lineCount
 * @param bPrintWords, pass true if you want print wordCount
 * @param bPrintChars, pass true if you want print charCount
*/
void PrintDetails(char* fileName, int lineCount, int wordCount, int charCount, bool bPrintLines, bool bPrintWords, bool bPrintChars)
{

    printf("%s ", fileName);

    if(bPrintLines)
    {
        printf("%d ", lineCount);
    }

    if(bPrintWords)
    {
        printf("%d ", wordCount);
    }

    if(bPrintChars)
    {
        printf("%d ", charCount);
    }

    printf("\n");
}