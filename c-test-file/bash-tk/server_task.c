typedef struct {
    int argc;
    char **argv;
    int (*main)(int argc, char **argv);
    int ret;
}sub_main_t;

/* analysis process */
void server(void)
{
    sub_main_t sub_main;
    sub_main.argc = 0;
    sub_main.argv = NULL;
    sub_main.main = NULL;
    sub_main.ret  = 0;

    while (1) {
       int ana;
       ana =  read_new_main(&sub_main);

       /* run new main */
       sub_main.ret = sub_main.main(sub_main.argc, sub_main.argv);
       if (sub_main.ret != 0)
       {
            print(xxxx);
       }

       /* wait new main run */
       ....
    }
}
int read_new_main(sub_main_t *test)
{
    test->argc = read_main_argc;
    test->argv = read_main_argv;
    test->main = read_main;
}
