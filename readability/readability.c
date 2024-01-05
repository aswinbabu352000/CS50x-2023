#include <cs50.h>
#include <ctype.h>
#include <math.h>
#include <stdio.h>

int count_letters(string s);
int count_words(string s);
int count_sentences(string s);

int main(void)
{
    // TODO ask user for a text input
    string text = get_string("Text: ");

    float L = ((float) (count_letters(text) / (float) count_words(text) * 100));
    float S = ((float) (count_sentences(text) / (float) count_words(text) * 100));

    float index = round(0.0588 * L - 0.296 * S - 15.8);

    if (index < 0)
        printf("Before Grade 1\n");
    else if (index > 16)
        printf("Grade 16+\n");
    else
        printf("Grade %i\n", (int) index);
}

int count_letters(string s)
{
    int letters = 0;
    int i = 0;

    while (s[i] != '\0')
    {
        if (isalpha(s[i]))
            letters++;
        i++;
    }
    return letters;
}

int count_words(string s)
{
    int words = 0;
    int i = 0;

    while (s[i] != '\0')
    {
        if (isspace(s[i]) && (isalpha(s[i + 1]) || ispunct(s[i + 1])))
            words++;
        i++;
    }

    if (words > 0)
        return words + 1;
    else
        return 0;
}

int count_sentences(string s)
{
    int sentences = 0;
    int i = 0;
    while (s[i] != '\0')
    {
        if (((s[i]) == '.' || (s[i]) == '?' || (s[i]) == '!') && (isspace(s[i + 1]) || s[i + 1] == '\0'))
            sentences++;
        i++;
    }
    return sentences;
}