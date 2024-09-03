#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    switch(argc){
        case 1:
            printf("No se ingresaron parametros\n");
        break;
        case 2:
            printf("Se ingreso un parametro\n");
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