#include "util.h"

void usage() {
    printf("Usage:\n");
    printf("./elf (integer[1,4])<sampler> [sigma] [mu] (int[1,u64_max])<num of sample>\n");
    exit(EXIT_FAILURE);
}

void parse_arg(int argc, char **argv, int *sampler, double *sigma, double *mu, uint64_t *samples) {
    if(argc < 3)
        usage();
    *sampler = atoi(argv[1]);
    if(*sampler < 1 || *sampler > 4)
        usage();
    if(*sampler == 1) {
        *sigma = 0.75;
        *mu = 0;
        *samples = strtoull(argv[2], NULL, 10);
    }
    else if(*sampler == 2) {
        *sigma = 1024;
        *mu = 0;
        *samples = strtoull(argv[2], NULL, 10);
    }
    else if(*sampler == 3) {
        *sigma = 1.5;
        *mu = strtoull(argv[2], NULL, 10);
        *samples = strtoull(argv[3], NULL, 10);
    }
    else {
        *sigma = strtod(argv[2], NULL);
        *mu = strtod(argv[3], NULL);
        *samples = strtoull(argv[4], NULL, 10);
    }
    if(*sampler == 3 && (*mu < 0 || *mu >= 1)) {
        printf("sigma or mu value err\n");
        exit(EXIT_FAILURE);
    }
    if(*sampler == 4 && (*sigma <= 0.8 || *sigma >= 1.6 || *mu < 0 || *mu >= 1)) {
        printf("sigma or mu value err\n");
        exit(EXIT_FAILURE);
    }
}

int get_phy_mem(const pid_t p) {
    char file[64] = {0};
    FILE *fd;
    char line_buff[256] = {0};
    sprintf(file, "/proc/%d/status", p);
    fd = fopen(file, "r");
    char name[32];
    int vmrss;
    for(int i = 0; i < VMRSS_LINE - 1; ++i)
        fgets(line_buff, sizeof(line_buff), fd);
    fgets(line_buff, sizeof(line_buff), fd);
    sscanf(line_buff, "%s %d", name, &vmrss);
    fclose(fd);
    return vmrss;
}
int get_rmem(pid_t p)
{
    return get_phy_mem(p);
}