int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "usage: reverse <filename>\n");
        exit(1);
    }
    
    FILE *file = fopen(argv[1], "r");
    if (file == NULL) {
        perror("fopen");
        exit(1);
    }