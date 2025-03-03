
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include "../essentials.h"

int siginterrupt(int sig, int flag) {
        struct sigaction sa; 
        
        if (sigaction(sig, NULL, &sa) == -1)
                return -1;
        
        if (flag == 0)
                sa.sa_flags |= SA_RESTART;
        else if (flag == 1)
                sa.sa_flags &= ~SA_RESTART;

        if (sigaction(sig, &sa, NULL) == -1)
                return -1;

        return 0;
}

void handler(int signum) {
    printf("Получен сигнал %d\n", signum);
}

int main() {
    char buf[100];
    struct sigaction sa;

    // Настройка обработчика сигнала
    sa.sa_handler = handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;

    if (sigaction(SIGINT, &sa, NULL) == -1) {
        perror("sigaction");
        return 1;
    }

    // Устанавливаем поведение для SIGINT: прерывать системные вызовы
    if (siginterrupt(SIGINT, 0) == -1) {
        perror("siginterrupt");
        return 1;
    }

    printf("Введите текст (Ctrl+C для прерывания):\n");

    // Чтение из стандартного ввода
    ssize_t n = read(STDIN_FILENO, buf, sizeof(buf));
    if (n == -1) {
        if (errno == EINTR) {
            printf("Системный вызов read был прерван сигналом\n");
        } else {
            perror("read");
        }
    } else {
        printf("Вы ввели: %.*s", (int)n, buf);
    }

    return 0;
}
