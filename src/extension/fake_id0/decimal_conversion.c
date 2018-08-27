/** Converts a decimal number to its octal representation. Used to convert
 *  system returned modes to a more common form for humans.
 */

int dtoo(int n) 
{
    int rem, i=1, octal=0;
    while (n!=0)
    {
        rem=n%8;
        n/=8;
        octal+=rem*i;
        i*=10;
    }
    return octal;
}

/** Converts an octal number to its decimal representation. Used to return to a
 *  more machine-usable form of mode from human-readable.
 */

int otod(int n)
{
    int decimal=0, i=0, rem;
    while (n!=0)
    {
        int j;
        int pow = 1;
        for(j = 0; j < i; j++)
            pow = pow * 8;
        rem = n%10;
        n/=10;
        decimal += rem*pow;
        ++i;
    }
    return decimal;
}

