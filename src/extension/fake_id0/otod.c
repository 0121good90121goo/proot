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
