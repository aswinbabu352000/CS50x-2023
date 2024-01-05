#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

int check_key(string s);
char substitute(char c, string k);
int main(int argc, string argv[])
{
    if (argc == 2)
    {
        if (strlen(argv[1]) == 26)
        {
            if (check_key(argv[1]))
            {
                string plaintext = get_string("plaintext:  ");

                printf("ciphertext: ");

                int l = 0;
                while (plaintext[l] != '\0')
                {
                    // toupper to make the key uniform (non case-sensitive)
                    printf("%c", substitute(plaintext[l], argv[1]));
                    l++;
                }
                printf("\n");
            }
            else
            {
                printf("Key must be 26 different alphabets.\n");
                return 1;
            }
        }
        else
        {
            printf("Key must contain 26 characters.\n");
            return 1;
        }
    }
    else
    {
        printf("Usage: ./substitution key\n");
        return 1;
    }
}

int check_key(string s)
{
    int i = 0;

    while (s[i] != '\0')
    {
        int j = 0;
        int counter = 0;

        if (isalpha(s[i])) // checking if all char is alphabet
        {
            while (j != 26)
            {
                if (s[i] == s[j])
                {
                    counter += 1;
                }
                j++;

                if (counter > 1) // Counter = 1 means it is equal to itself only, Counter > 1 = there is a duplicate
                {
                    return false;
                }
            }
            i++;
        }
        else
        {
            return false;
        }
    }
    return true;
}

char substitute(char p, string k)
{
    int new_char = 0;
    int d = 0;

    if (isalpha(p))
    {
        if (isupper(p)) // Plaintext char is UPPER
        {
            new_char = toupper(k[p - 65]);
        }
        else if (!isupper(p)) // Plaintext char is LOWER
        {
            new_char = tolower(k[p - 97]);
        }

        return (char) new_char;
    }
    else
    {
        return p;
    }
}