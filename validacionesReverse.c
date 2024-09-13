#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <ctype.h>  // Para verificar caracteres imprimibles

FILE *openFile(char *, char *);
int isHardlinked(const char *, const char *);
void reverseContent(FILE *input, FILE *output);
void checkNonPrintableCharacters(const char *line);

int main(int argc, char *argv[])
{
    FILE *input = NULL;
    FILE *output = stdout;
    const char *file1 = argv[1];
    const char *file2 = argv[2];
    
    switch(argc) {
        case 1:
            printf("No se ingresaron parámetros\n");
        break;
        case 2:
            input = openFile(argv[1], "r");
            fclose(input);
            exit(0);
        break;
        case 3:
            input = openFile(argv[1], "r");
            output = openFile(argv[2], "w");

            // Verificar si los archivos son el mismo archivo o están enlazados
            if ((strcmp(file1, file2) == 0) || isHardlinked(file1, file2)) {
                fprintf(stderr, "reverse: input and output file must differ\n");
                fclose(input);
                fclose(output);
                exit(1);
            }

            // Invertir el contenido del archivo de entrada y escribir en el archivo de salida
            reverseContent(input, output);

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

// Función para abrir archivos con manejo de errores
FILE *openFile(char *filename, char *mode) {
    FILE *file = fopen(filename, mode);
    if (file == NULL) {
        fprintf(stderr, "reverse: cannot open file '%s'\n", filename);
        exit(1);
    }
    return file;
}

// Función para verificar si dos archivos son enlaces duros
int isHardlinked(const char *filename1, const char *filename2) {
    struct stat fileInfo1, fileInfo2;
    if (stat(filename1, &fileInfo1) == -1 || stat(filename2, &fileInfo2) == -1) {
        fprintf(stderr, "reverse: cannot access file(s)\n");
        exit(1);
    }
    return fileInfo1.st_ino == fileInfo2.st_ino;
}

// Función para verificar si hay caracteres no imprimibles en una línea
void checkNonPrintableCharacters(const char *line) {
    for (int i = 0; i < strlen(line); i++) {
        // Verificar si el carácter no es imprimible
        if (!isprint(line[i]) && line[i] != '\n' && line[i] != '\t') {
            fprintf(stderr, "reverse: found non-printable character in line\n");
            exit(1);
        }
    }
}

// Función para invertir el contenido del archivo de entrada y escribirlo en el archivo de salida
void reverseContent(FILE *input, FILE *output) {
    char **lines = NULL;
    char buffer[1024];
    size_t count = 0;
    
    // Leer el archivo línea por línea y almacenar en un array de cadenas
    while (fgets(buffer, sizeof(buffer), input)) {
        // Verificar si la línea leída contiene caracteres no imprimibles
        checkNonPrintableCharacters(buffer);

        // Alocar memoria para almacenar la línea actual
        lines = realloc(lines, (count + 1) * sizeof(char *));
        if (lines == NULL) {
            fprintf(stderr, "reverse: failed to allocate memory\n");
            exit(1);
        }
        
        lines[count] = strdup(buffer);
        if (lines[count] == NULL) {
            fprintf(stderr, "reverse: failed to allocate memory for line\n");
            exit(1);
        }

        count++;
    }

    // Verificar si las líneas fueron almacenadas correctamente
    if (count == 0) {
        fprintf(stderr, "reverse: no lines to reverse\n");
        return;
    }

    // DEBUG: Mostrar las líneas que se almacenaron antes de invertirlas
    printf("Lines read from input file:\n");
    for (size_t i = 0; i < count; i++) {
        printf("%s", lines[i]);
    }

    // Escribir las líneas en orden inverso en el archivo de salida
    printf("\nWriting lines in reverse order:\n");
    for (ssize_t i = count - 1; i >= 0; i--) {
        // DEBUG: Mostrar las líneas que se están escribiendo en orden inverso
        printf("%s", lines[i]);
        
        fputs(lines[i], output);
        free(lines[i]);  // Liberar la memoria de cada línea después de escribirla
    }

    free(lines);  // Liberar el array de líneas
}
