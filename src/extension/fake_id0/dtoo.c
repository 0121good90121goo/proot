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
