

#include "main.h"


/**
 * _putchar - writes the character c to stdout
 * OCI The character to print
 * Return: on sucess 1.
 * On error, -1 is returned, and error is set apropriately
 */

int _putcher(char c)

{
    return(write(1, &c, 1));

}
