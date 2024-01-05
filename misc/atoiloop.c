#include <cs50.h>
#include <stdio.h>
#include <string.h>


int convert(string s);

int main(void)
{
    string number = get_string("Enter a positive integer: ");

    int integer = convert(number);

    if(integer < 1)
    {
        printf("Invalid Input\n");
        return 1;
    }

    printf("%i\n", integer);
    return 0;

}

int convert(string s)
{
    int n = 0;
    for (int i = 0, j = (strlen(s) - 1), k = 1; i < strlen(s); i++, j--, k *= 10)
    {
        n += (s[j] - 48) * k;
    }

    return n;
}