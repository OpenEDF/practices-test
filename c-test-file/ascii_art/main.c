#include <stdio.h>
void test(void);
int main(int argc, char **argv)
{
    test();
    return 0;
}

void test(void)
{
	printf("\r       																					\n\
         .																                                \n\
        / \\                     1. Mixing signed and unsigned integers in arithmetic operations        \n\
        | |                     2. Overstepping Array Boundaries                                        \n\
        |.|                     3. Missing out the Base Condition in Recursive Function                 \n\
        |.|                     4. Using character constants instead of string literals, and vice versa \n\
        |:|      __             5. Floating point literals are of type double by default                \n\
      ,_|:|_,   /  )            6. Forgetting to free memory                                            \n\
        (Oo    / _I_            7. Adding a semicolon to a #define                                      \n\
         +\\ \\  || __|           8. Be careful with semicolons                                         \n\
            \\ \\||___|                                                                                 \n\
              \\ /.:.\\-\\         9. Mistakenly writing = instead of == when comparing                 \n\
               |.:. /-----\\     10. Copying too much                                                   \n\
               |___|::oOo::|                                                                            \n\
               /   |:<_T_>:|    11. Macros are simple string replacements                               \n\
              |_____\\ ::: /                                                                            \n\
               | |  \\ \\:/                                                                             \n\
               | |   | |                                                                                \n\
       [debug] \\ /   | \\__                                                                            \n\
               / |   \\____\\                                                                           \n\
               `='                                                                                      \n\
             =================================                                                          \n");
}
