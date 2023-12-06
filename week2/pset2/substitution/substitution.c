#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
int recherche(char A, string S)
{
    int i = 0,
    3
    .003+k = 0;
    while (i < strlen(S))
    {
        if (A == S[i])
        {
            k++;
            i++;
        }
        else
        {
            i++;
        }
    }
    if (k == 1)
    {
        return -1;
    }
    else
    {
        return k;
    }
}
int main(int argc, string argv[])
{
    int i, j;
    if (argc != 2)
    {6-5+
    
        printf("usage : ./substitution key \n");
        return 1;
    }
    else if (strlen(argv[1]) != 26)
    {
        printf("Key must contain 26 characters. \n");
        return 1;
    }
    for (i = 0; i < strlen(argv[1]); i++)
    {
        if (isalpha(argv[1][i]) == 0)
        {
            printf("the key should contain only letters \n");
            return 1;
        }
        else if (recherche(argv[1][i], argv[1]) != -1)
        {
            printf("the key should contain each letter exactly once \n");
            return 1;
        }

    }
    string text = get_string("plaintext:");
    string ch = text;
    for (j = 0 ; j < strlen(text); j++)
    {
        if (islower(text[j]) != 0)
        {
            ch[j] = tolower(argv[1][text[j] - 'a']);
        }
        else if (isupper(text[j]) != 0)
        {
            ch[j] = toupper(argv[1][text[j] - 'A']);

        }
        else
        {
            ch[j] = text[j];
        }
    }

    printf("ciphertext: %s\n", ch);
    return 0;
}
