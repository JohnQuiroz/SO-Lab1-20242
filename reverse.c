#include <stdio.h>
#include <stdlib.h>

FILE *openFile(char *filename, char *mode) {
    FILE *file = fopen(filename, mode);
    if (file == NULL) {
        fprintf(stderr, "reverse: cannot open file '%s'\n", filename);
        exit(1);
    }
    return file;
}

int main(int argc, char *argv[])
{
    FILE *input = NULL;
    switch(argc){
        case 1:
            printf("No se ingresaron parametros\n");
        break;
        case 2:
            input = openFile(argv[1], "r");
            fclose(input);
            exit(0);
        break;
        case 3:
            printf("Se ingresaron dos parametros\n");
        break;
        default:
            fprintf(stderr, "usage: reverse <input> <output>\n");
            exit(1);
        break;
    }
    return 0;
}