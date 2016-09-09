#define _CRT_SECURE_NO_WARNINGS 1

#include "CommentConvert.h"

StateType state;

void DoCppState(FILE *read, FILE *write)
{
    int first = fgetc(read), second = 0;
    switch (first)
    {
        case '\n':
            fputc(first, write);
            state = NULL_STATE;
            break;
            
        case EOF:
            //fputc(first, write);
            state = END_STATE;
            break;
        default:
            fputc(first, write);
            break;
    }
}

void DoCState(FILE*read, FILE* write)
{
    int first = fgetc(read), second = 0,third=0;
    switch (first)
    {
        case '*':
            second = fgetc(read);
            if (second == '/')
            {
                third = fgetc(read);
                if (third == '\n')
                    fputc(third, write);
                else
                {
                    ungetc(third, read);
                    fputc('\n', write);
                }
                state = NULL_STATE;
            }
            else if (second == '*')
            {
                ungetc(second, read);
                fputc(first, write);
            }
            else
            {
                fputc(first, write);
                fputc(second, write);
            }
            break;
            
        case '\n':
            fputc(first, write);
            fputc('/', write);
            fputc('/', write);
            break;
            
        case EOF:
            //fputc(first, write);
            state = END_STATE;
            break;
            
        default:
            fputc(first, write);
            break;
    }
}

void DoNullState(FILE *read, FILE *write)
{
    int first = fgetc(read), second = 0;
    
    switch (first)
    {
        case '/':
            second = fgetc(read);
            if (second == '/')
            {
                state = CPP_STATE;
                fputc(first, write);
                fputc(second, write);
                //
                
            }
            else if (second == '*')
            {
                fputc(first, write);
                fputc('/', write);
                state = C_STATE;
            }
            else
            {
                fputc(first, write);
                fputc(second, write);
            }
            break;
        case EOF:
            fputc(first, write);
            state = END_STATE;
            break;
        default:
            fputc(first, write);
            break;
    }
}

void ConvertWork(FILE *read, FILE *write)
{
    state = NULL_STATE;
    
    while (state != END_STATE)
    {
        switch (state)
        {
            case NULL_STATE:
                DoNullState(read, write);
                break;
            case C_STATE:
                DoCState(read, write);
                break;
            case CPP_STATE:
                DoCppState(read, write);
                break;
        }
    }
}

void CommentConvert()
{
    FILE *pRead, *pWrite;
    
    pRead = fopen(INPUTFILENAME, "r");
    if (pRead == NULL)
    {
        perror("INPUTFILENAME");
        system("pause");
        exit(EXIT_FAILURE);
    }
    
    pWrite = fopen(OUTPUTFILENAME, "w");
    if (pWrite == NULL)
    {
        perror(OUTPUTFILENAME);
        fclose(pRead);
        system("pause");
        exit(EXIT_FAILURE);
    }
    
    ConvertWork(pRead, pWrite);
    fclose(pRead);
    fclose(pWrite);
}