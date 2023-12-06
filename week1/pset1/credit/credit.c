#include <cs50.h>
#include <stdio.h>


int main(void)
{
    long cc, x;
    int sum, lengh, d, l, fd, sd, i;
    //input
    do
    {
        cc = get_long("write the code of the card\n");
    }
    while (cc < 0);
    //first sum
    sum = 0;
    x = cc;
    while (x != 0)
    {
        d = x % 10;
        sum = sum + d;
        x = x / 100;
    }
    //second sum
    x = cc;
    x = x / 10;
    while (x != 0)
    {
        d = x % 10;
        d = d * 2;
        if (d < 10)
        {
            sum = sum + d;
        }
        else
        {
            sum = sum + d % 10 + d / 10 ;
        }
        x = x / 100;
    }
    // lengh of the cc
    x = cc;
    l = 0;
    while (x != 0)
    {
        x = x / 10;
        l = l + 1;
    }
    //getting first digit and second digit
    x = cc;
    for (i = 0 ; i < l - 2 ; i++)
    {
        x = x / 10;
    }
    sd = x % 10;
    fd = (x / 10) % 10;
    //checinkg

    if (sum % 10 == 0)
    {
        if ((l == 13 || l == 16) && (fd == 4))
        {
            printf("VISA\n");
        }
        else if (l == 15 && fd == 3 && (sd == 4 || sd == 7))
        {
            printf("AMEX\n");
        }
        else if (l == 16 && fd == 5 && (sd > 0 && sd < 6))
        {
            printf("MASTERCARD\n");
        }
        else
        {
            printf("INVALID\n");
        }
    }
    else
    {
        printf("INVALID\n");
    }




}