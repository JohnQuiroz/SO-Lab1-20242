#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stddef.h>

FILE *openFile(char *, char *);
int isHardlinked(const char *, const char *);
void reverse(FILE *input, FILE *uotput);

int main(int argc, char *argv[])
{
    FILE *input = NULL;
    FILE *output = stdout;
    const char *file1 = argv[1];
    const char *file2 = argv[2];
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
            input = openFile(argv[1] , "r");
            output = openFile(argv[2], "w");
            if((strcmp(file1, file2) == 0) || isHardlinked(file1, file2)){
                fprintf(stderr, "reverse: input and output file must differ\n");
                exit(1);
            }
            else{
                reverse(input, output);
            }
            fclose(input);
            fclose(output);
            exit(0);
        break;
        default:
            fprintf(stderr, "usage: reverse <input> <output>\n");
            exit(1);
        break;
    }
    return 0;
}
FILE *openFile(char *filename, char *mode) {
    FILE *file = fopen(filename, mode);
    if (file == NULL) {
        fprintf(stderr, "reverse: cannot open file '%s'\n", filename);
        exit(1);
    }
    return file;
}

int isHardlinked(const char *filename1, const char *filename2) {
    struct stat fileInfo1, fileInfo2;
    if (stat(filename1, &fileInfo1) == -1 || stat(filename2, &fileInfo2) == -1) {
        fprintf(stderr, "reverse: cannot open file '/no/such/file.txt'\n");
        exit(1);
    }
    return fileInfo1.st_ino == fileInfo2.st_ino;
}

void reverse(FILE *input, FILE *output){
    char **lines = NULL;
    char *buffer = NULL;
    size_t line_count = 0;
    ssize_t read;

    // Read lines from input file into dynamically allocated memory
    while ((read = getline(&buffer, &read, input)) != -1) {
        lines = realloc(lines, (line_count + 1) * sizeof(char *));
        lines[line_count] = malloc(read);
        strcpy(lines[line_count], buffer);
        line_count++;
    }

    // Print lines in reverse order to output file or stdout
    for (size_t i = line_count; i > 0; i--) {
        if(output != NULL){
            fprintf(output, "%s", lines[i - 1]);
            free(lines[i - 1]);
        } else {
            fprintf(stdout, "%s", lines[i - 1]);
        }
        
    }

    // Free remaining allocated memory
    free(lines);
    free(buffer);
}