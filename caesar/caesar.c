#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>

int only_digits(string s);
int rotate(char c, int n);

int main(int argc, string argv[])
{
    if (argc == 2)
    {
        if (only_digits(argv[1]) == true)
        {
            int key = atoi(argv[1]);

            string plain_text = get_string("plaintext: ");

            printf("ciphertext: ");

            int j = 0;

            while (plain_text[j] != '\0')
            {
                printf("%c", rotate(plain_text[j], key));
                j++;
            }

            printf("\n");
        }
        else if ((only_digits(argv[1]) == false))
        {
            printf("Usage: ./caesar key\n");

            return 1;
        }
    }
    else
    {
        printf("Usage: ./caesar key\n");

        return 1;
    }
}

int only_digits(string s)
{
    int i = 0;
    int digits = 0;
    while (s[i] != '\0')
    {
        if (s[i] >= '0' && s[i] <= '9')
        {

            digits = digits + 0;
        }
        else
        {

            digits = digits + 1;
        }
        i++;
    }

    if (digits == 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

int rotate(char c, int n)
{
    int p = 0;
    char newc = 0;

    if (c >= 'a' && c <= 'z')
    {
        c = c - 96; // -96 to convert a from 97 to 1, b to 2...
        newc = 96 + c + n - (((c + n) / 26) * 26);
    }
    else if (c >= 'A' && c <= 'Z')
    {
        c = c - 64; // -64 to convert A from 65 to 1, b to 2...
        newc = 64 + c + n - (((c + n) / 26) * 26);
    }
    else // if its not an alphabet
    {
        newc = c;
    }
    return newc;
}