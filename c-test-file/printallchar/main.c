#include <stdio.h>

int main()
{
    int i;
    i=0;
    do
    {
        printf("%d %c \t",i,i);
        i++;
		if ((i % 8 ) == 0)
			printf("\n");
    }
    while(i<=255);
    return 0;
}
