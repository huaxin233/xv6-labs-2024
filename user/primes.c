#include "kernel/types.h"
#include "user/user.h"

int get_prime(int pleft[2]) {
    int n;
    read(pleft[0], &n, sizeof(n));

    if(n == -1) exit(0);
    printf("prime %d\n", n);

    int pright[2];
    pipe(pright);

    if(fork() == 0) {
        close(pleft[0]);
        close(pright[1]);
        get_prime(pright);
    } else {
        close(pright[0]);
        int buf;
        while(read(pleft[0], &buf, sizeof(buf)) && buf != -1) {
            if(buf % n != 0) {
                write(pright[1], &buf, sizeof(buf));
            }
        }
        buf = -1;
        write(pright[1], &buf, sizeof(buf));
        close(pright[1]);
        close(pleft[0]);
        wait(0);
        return 0;
    }
    return 0;
}

int main() {
    int input_pipe[2];
    pipe(input_pipe);
    int pid;

    pid = fork();
    if(pid == 0) {
        close(input_pipe[1]);
        get_prime(input_pipe);
    } else {
        close(input_pipe[0]);
        int i;
        for(i = 2; i <= 280; i++) {
            write(input_pipe[1], &i, sizeof(i));
        }
        i = -1;
        write(input_pipe[1], &i, sizeof(i));
    }
    wait(0);
    exit(0);
}