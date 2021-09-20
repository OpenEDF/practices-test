#include <stdio.h>

int foo(int a)
{
    if (a > 0) return -a;
    return a;    
}        

int main(int argc, char *argv[])
{
    (void)(argc);
    (void)(argv);

    int b  =3;
    
    if (foo(b) > 3)
    {
        printf("Large\n");        
    }    
    else
    {
        printf("Small\n");
    }

    return 0;
}

