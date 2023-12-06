#include <cs50.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <math.h>
//prototype
int count_letters(string text);
int count_words(string text);
int count_sentences(string text);

int main(void)
{
    int l, w, s, R;
    float L, S, index;
    string text;
    text = get_string("Text:");
    l = count_letters(text);
    w = count_words(text);
    s = count_sentences(text);

    L = (100 * (float)l) / (float)w;
    S = ((float)s * 100) / (float)w;
    //the Coleman-Liau index
    index = 0.0588 * L - 0.296 * S - 15.8;
    R = round(index);
//outputting the grade
    if (R < 1)
    {
        printf("Before Grade 1\n");
    }
    else if (R >= 1 && R <= 15)
    {
        printf("Grade %d\n", R);
    }
    else
    {
        printf("Grade 16+\n");
    }
    return 0;

}
//counting letters
int count_letters(string text)
{
    int i = 0, k = 0;
    while (text[i] != '\0')
    {
        if (isalpha(text[i]) != 0)
        {
            k++;
        }
        i++;
    }

    return k;
}
//counting words
int count_words(string text)
{
    int k = 0, j = 0, i;
    while (text[j] == ' ')
    {
        j++;
    }
    if (text[j + 1] == '\0')
    {
        return 0;
    }
    else
    {

        for (i = j; i < strlen(text) - 1; i++)
        {
            if (text[i] == ' ' && text [i + 1] != ' ')
            {
                k++;

            }
        }
        return k + 1;
    }

}
//counting sentences
int count_sentences(string text)
{
    int k = 0, i;
    for (i = 0; i < strlen(text); i++)
    {
        if ((text[i] == '.' || text[i] == '!' || text[i] == '?')  && (text[i + 1] != '.' || text[i + 1] != '!' || text[i + 1] != '?'))
        {
            k++;

        }
    }
    return k;

}
