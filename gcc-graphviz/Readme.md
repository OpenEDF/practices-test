### 1. FOSS
 - GCC: version 9.3.0 (Ubuntu 9.3.0-17ubuntu1~20.04)
 - Graphviz: (for processing .dot file)

### 2. Install Graphviz Graphviz for Ubuntu 20.04
```shell
$ sudo apt install grapjviz
```
### 3. C example source
``` c
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
```
### 4.Gernerate .dot file using gcc
```shell
gcc -fudump-tree-all-graph target.c
```

### 5.Process .dot file using Graphviz
```shell
dot -Tpng target.c.011t.cfg.dot -o target.png
```
![target.png](https://github.com/OpenEDF/practices-test/blob/main/gcc-graphviz/target.png)


