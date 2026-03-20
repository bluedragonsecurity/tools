#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <payload_file>\n", argv[0]);
        return -1;
    }

    int fd = open("/dev/vuln_device", O_RDWR);
    if (fd < 0) {
        perror("[-] Failed to open /dev/vuln_device");
        return -1;
    }

    int p_fd = open(argv[1], O_RDONLY);
    if (p_fd < 0) {
        perror("[-] Failed to open payload");
        close(fd);
        return -1;
    }

    struct stat st;
    fstat(p_fd, &st);
    size_t p_size = st.st_size;

    char *buffer = malloc(p_size);
    if (!buffer) {
        perror("[-] Malloc failed memory");
        return -1;
    }
    read(p_fd, buffer, p_size);

    printf("[+] Sending %zu byte payload at %s ...\n", p_size, argv[1]);

    ssize_t w = write(fd, buffer, p_size);
    
    if (w < 0) {
        printf("[-] Write failed).\n");
    } else {
        printf("[+] Payload Sent !\n");
    }

    free(buffer);
    close(p_fd);
    close(fd);
    return 0;
}
