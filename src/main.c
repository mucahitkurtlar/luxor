#include <libcryptsetup.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#define MAX_PASSPHRASE_SIZE 512

struct args {
    char* device;
    char* wordlist;
};

static void print_usage(const char* prog_name) { fprintf(stderr, "Usage: %s <device> <wordlist>\n", prog_name); }

static void fatal(const char* msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}

void parse_args(int argc, char* argv[], struct args* args) {
    if (argc < 3) {
        print_usage(argv[0]);
        exit(EXIT_FAILURE);
    }

    args->device = argv[1];
    args->wordlist = argv[2];
}

void open_wordlist(const char* path, FILE** file) {
    *file = fopen(path, "r");
    if (*file == NULL) {
        fatal("fopen");
    }
}

void close_wordlist(FILE* file) {
    if (fclose(file) != 0) {
        fatal("fclose");
    }
}

ssize_t read_pass(FILE* file, char** pass) {
    size_t n = 0;
    ssize_t read = getline(pass, &n, file);

    if (read > 0 && (*pass)[read - 1] == '\n') {
        (*pass)[read - 1] = '\0';
    }

    return read;
}

int unlock_device(const char* device, const char* passphrase) {
    struct crypt_device* cd;
    int r;

    if ((r = crypt_init(&cd, device)) != 0) {
        fatal("crypt_init failed");
        return r;
    }

    if ((r = crypt_load(cd, CRYPT_LUKS2, NULL)) != 0) {
        fatal("crypt_load failed");
        crypt_free(cd);
        return r;
    }

    r = crypt_activate_by_passphrase(cd, NULL, CRYPT_ANY_SLOT, passphrase, strlen(passphrase), 0);
    crypt_free(cd);

    return r;
}

int main(int argc, char* argv[]) {
    struct args args;
    parse_args(argc, argv, &args);

    FILE* wordlist;
    open_wordlist(args.wordlist, &wordlist);

    char* pass = (char*)malloc(MAX_PASSPHRASE_SIZE);
    if (pass == NULL) {
        fatal("malloc");
    }

    int success = 0;

    while (1) {
        ssize_t read = read_pass(wordlist, &pass);
        if (read == -1) {
            break;
        }

        printf("Trying passphrase: %s ", pass);
        fflush(stdout);

        int r = unlock_device(args.device, pass);
        if (r == 0) {
            printf("success!\n");
            success = 1;
            break;
        } else if (r == -1) {
            printf("failed\n");
        } else {
            fprintf(stderr, "crypt_activate_by_passphrase failed\n");
        }
    }

    close_wordlist(wordlist);
    free(pass);

    return success ? EXIT_SUCCESS : EXIT_FAILURE;
}
