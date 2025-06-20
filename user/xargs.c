#include "kernel/types.h"
#include "kernel/param.h"
#include "user/user.h"

#define BUFSIZE 1024

int main(int argc, char **argv) {
    char *xargv[MAXARG];
    int xargc = 0;
    for(int i = 1; i < argc; i++) {
        xargv[xargc] = argv[i];
        xargc++;
    }

    char buf[BUFSIZE];
    int n = read(0, buf, BUFSIZE);

    char *p = buf;
    for(int i = 0; i < n; i++) {
        if(buf[i] == '\n') {
            int pid = fork();
            if(pid != 0) {
                p = &buf[i + 1];
                wait(0);
            } else {
                buf[i] = 0;
                xargv[xargc] = p;
                xargc++;
                xargv[xargc] = 0;
                xargc++;
                exec(xargv[0], xargv);
                exit(0);
            }
        }
    }

    return 0;
}