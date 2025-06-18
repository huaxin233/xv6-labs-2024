#include "kernel/types.h"
#include "user/user.h"

int main() {
    int p1[2], p2[2];
    pipe(p1);
    pipe(p2);
    int pid;
    
    pid = fork();
    if(pid == 0) {
        close(p1[1]);
        close(p2[0]);
        int buf;
        read(p1[0], &buf, sizeof(buf));
        printf("%d: received ping\n", getpid());
        write(p2[1], &buf, sizeof(buf));
    } else {
        close(p1[0]);
        close(p2[1]);
        write(p1[1], "a", sizeof("a"));
        int buf;
        read(p2[0], &buf, sizeof(buf));
        printf("%d: received pong\n", getpid());
        wait(0);
    }
    exit(0);
}