#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#define BUF_SIZE 4096

int cp_main(int argc, char* argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: cp <source> <destination>\n");
        return 1;
    }

    const char* src = argv[1];
    const char* dest = argv[2];

    int src_fd = open(src, O_RDONLY);
    if (src_fd < 0) {
        fprintf(stderr, "cp: cannot open source file '%s': %s\n", src, strerror(errno));
        return 1;
    }

    int dest_fd = open(dest, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (dest_fd < 0) {
        fprintf(stderr, "cp: cannot open/create destination file '%s': %s\n", dest, strerror(errno));
        close(src_fd);
        return 1;
    }

    char buffer[BUF_SIZE];
    ssize_t bytes_read, bytes_written;

    while ((bytes_read = read(src_fd, buffer, BUF_SIZE)) > 0) {
        ssize_t total_written = 0;
        while (total_written < bytes_read) {
            bytes_written = write(dest_fd, buffer + total_written, bytes_read - total_written);
            if (bytes_written < 0) {
                fprintf(stderr, "cp: write error: %s\n", strerror(errno));
                close(src_fd);
                close(dest_fd);
                return 1;
            }
            total_written += bytes_written;
        }
    }

    if (bytes_read < 0) {
        fprintf(stderr, "cp: read error: %s\n", strerror(errno));
        close(src_fd);
        close(dest_fd);
        return 1;
    }

    close(src_fd);
    close(dest_fd);
    return 0;
}
