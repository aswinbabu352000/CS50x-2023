#include <cs50.h>
#include <stdio.h>

int convert(string s);

int main(void)
{
    string number = get_string("Enter a positive integer: ");

    printf("%i", convert(number));
}

int convert(string s)
{
    if(s[0] == '\0')
    {
        return;
    }

    int n = 0;

    n = s[strlen(s) - 1]

    s[strlen(s) - 1] = '\0'

    convert(s);

    return n;

}