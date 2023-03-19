#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct{
    char tipo[5];
    int largura, altura;
    int brilhoMax;
    int **mat;
} IMG;

IMG lerArquivo(char *dict){
    IMG foto;
    int i, j; 
    int pixel;

    //abrindo arquivo
    FILE* in;
    in = fopen(dict, "rt");
    if(in == NULL){
        printf("ERRO FATAL\n");
    } else{
        printf("SHOW!\n");
    }

    //lendo 3 primeiras linhas do arquivo
    fscanf(in, "%[^\n]\n", foto.tipo);
    fscanf(in, "%d %d\n", &foto.largura, &foto.altura);
    fscanf(in, "%d\n", &foto.brilhoMax);

    //gera matriz
    foto.mat = (int **) malloc(foto.altura * sizeof(int *));
    for(i = 0; i < foto.altura; i++){
        foto.mat[i] = (int *) malloc(foto.largura * sizeof(int));
        for(j = 0; j < foto.largura; j++){
            fscanf(in, "%d", &pixel);
            foto.mat[i][j] = pixel;
        }
    }

    fclose(in);
    return foto;
}

void salvarArquivo(IMG foto, char* dict){
    
    int i, j;

    //abre arquivo
    FILE * out;
    out = fopen(dict, "wt");
    
    //escreve 3 linhas iniciais
    fprintf(out, "%s\n", foto.tipo);
    fprintf(out, "%d %d\n", foto.largura, foto.altura);
    fprintf(out, "%d\n", foto.brilhoMax);

    //escreve matriz
    for(i = 0; i < foto.altura; i++){
        for(j = 0; j < foto.largura; j++){
            fprintf(out, "%d ", foto.mat[i][j]);
        }
        fprintf(out, "\n");
    }

    fclose(out);
}

IMG negativo(IMG foto){
    
    int i, j;

    for(i = 0; i < foto.altura; i++){
        for(j = 0; j < foto.largura; j++){
            foto.mat[i][j] = 255 - foto.mat[i][j];
        }  
    }
    return foto;
}

IMG espelhamento(IMG foto, IMG fotoED){
    
    int i, j;
    int jed;

    for (i = 0; i < foto.altura; i++){     
        jed = 0;
        for(j = foto.largura; j > 0; j--){;
            fotoED.mat[i][jed] = foto.mat[i][j];
            jed++;
        }
    }
    
    return fotoED;
}

int main(void){
    
    int i;
    char dict[] = "ifes.pgm";
    char dictOut[] = "ifes-out.pgm";

    IMG foto = lerArquivo(dict);
    IMG fotoED = lerArquivo(dict);
    
    //foto = negativo(foto);

    foto = espelhamento(foto, fotoED);

    salvarArquivo(foto, dictOut);


    //libera a memória alocada para a matriz
    for (i = 0; i < foto.altura; i++) {
    free(foto.mat[i]);
    }
    free(foto.mat);
    
    return 0;
}