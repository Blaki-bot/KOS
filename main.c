#include <ps5/kernel.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <ps5/payload_main.h>

int payload_main(struct payload_args *args) {
    kernel_init_rw(args->rwpair[0], args->rwpair[1], args->rwpipe, args->kpipe_addr);

    uint64_t kbase = 0xffffffff80210000; // KBASE de Relapse 13.00
    size_t dump_size = 0x1000000;        // 16MB, cubre .text

    void *buffer = malloc(dump_size);
    if (!buffer) return -1;

    kernel_copyout(kbase, buffer, dump_size);

    int fd = open("/data/kernel_dump.bin", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) { free(buffer); return -1; }

    ssize_t written = write(fd, buffer, dump_size);
    close(fd);
    free(buffer);

    return (written == (ssize_t)dump_size) ? 0 : -1;
}
