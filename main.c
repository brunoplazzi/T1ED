#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct{
    char tipo[5];
    int largura, altura;
    int brilhoMax;
    int **mat;
} IMG;

//retorna o maior valor de um vetor v, com n elementos
int maior(int n, int* v){

  int i;
  int maior = v[0];

  for(i = 0; i < n; i++){
    if(v[i] >= maior){
      maior = v[i];
    }
  }
  return maior;
}


//le arquivo da imagem e retorna o tipo IMG
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

//recebe o tipo IMG e o caminho e salva a foto no arquivo
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

IMG blur(IMG foto, IMG fotoED){
    int i, j;
    
    for (i = 0; i < foto.altura; i++){
        for(j = 0; j < foto.largura ; j++){
            //primeira linha
            if (i == 0){
                //primeira coluna
                if (j == 0){
                    fotoED.mat[i][j] = (foto.mat[i][j] + foto.mat[i+1][j] + foto.mat[i][j+1] + foto.mat[i+1][j+1])/4;
                }
                //ultima coluna
                else if (j == foto.largura - 1){
                    fotoED.mat[i][j] = (foto.mat[i][j] + foto.mat[i+1][j] + foto.mat[i+1][j-1] + foto.mat[i][j-1])/4;
                }
                //colunas do meio
                else{
                    fotoED.mat[i][j] = (foto.mat[i][j] + foto.mat[i][j-1] + foto.mat[i+1][j-1] + foto.mat[i+1][j] + foto.mat[i+1][j+1] + foto.mat[i][j+1])/6;
                }
            }
            //ultima linha
            if (i == foto.altura - 1){
                //primeira coluna
                if (j == 0){
                    fotoED.mat[i][j] = (foto.mat[i][j] + foto.mat[i-1][j] + foto.mat[i-1][j+1] + foto.mat[i][j+1])/4;
                }
                //ultima coluna
                else if (j == foto.largura - 1){
                    fotoED.mat[i][j] = (foto.mat[i][j] + foto.mat[i][j-1] + foto.mat[i-1][j-1] + foto.mat[i-1][j])/4;
                }
                //colunas do meio
                else{
                    fotoED.mat[i][j] = (foto.mat[i][j] + foto.mat[i][j-1] + foto.mat[i-1][j-1] + foto.mat[i-1][j] + foto.mat[i-1][j+1] + foto.mat[i][j+1])/6;
                }
            }
            //primeira coluna, exceto primeira e ultima linha
            if((j == 0) && (i > 0) && (i < foto.altura - 1)){
                fotoED.mat[i][j] = (foto.mat[i][j] + foto.mat[i-1][j] + foto.mat[i-1][j+1] + foto.mat[i][j+1] + foto.mat[i+1][j+1] + foto.mat[i+1][j])/6; 
            }

            //ultima coluna, exceto primeira e ultima linha
            if((j == foto.largura - 1) && (i > 0) && (i < foto.altura - 1)){  
                fotoED.mat[i][j] = (foto.mat[i][j] + foto.mat[i-1][j] + foto.mat[i-1][j-1] + foto.mat[i][j-1] + foto.mat[i+1][j-1] + foto.mat[i+1][j])/6;   
            }

            //linhas e colunas do meio
            if((i > 0) && (i < foto.altura - 1) && (j > 0) && (j < foto.largura - 1)){
                fotoED.mat[i][j] = (foto.mat[i][j] + foto.mat[i-1][j] + foto.mat[i-1][j-1] + foto.mat[i][j-1] + foto.mat[i+1][j-1] + foto.mat[i+1][j] + foto.mat[i+1][j+1] + foto.mat[i][j+1] + foto.mat[i-1][j+1])/9;
            }
        }
    }

    return fotoED;

}

IMG bright(IMG foto, IMG fotoED){
    
    int i, j;

    for (i = 0; i < foto.altura; i++){
        for(j = 0; j < foto.largura ; j++){
            //primeira linha
            if (i == 0){
                //primeira coluna
                if (j == 0){
                    int v[4] = {foto.mat[i][j], foto.mat[i+1][j], foto.mat[i][j+1], foto.mat[i+1][j+1]};
                    fotoED.mat[i][j] = maior(4, v);
                } 
                //ultima coluna
                else if (j == foto.largura - 1){
                    int v[4] = {foto.mat[i][j], foto.mat[i+1][j], foto.mat[i+1][j-1], foto.mat[i][j-1]};
                    fotoED.mat[i][j] = maior(4, v);
                }
                //colunas do meio
                else{
                    int v[6] = {foto.mat[i][j], foto.mat[i][j-1], foto.mat[i+1][j-1], foto.mat[i+1][j], foto.mat[i+1][j+1], foto.mat[i][j+1]};
                    fotoED.mat[i][j] = maior(6, v);
                }
            }
            //ultima linha
            if (i == foto.altura - 1){
                //primeira coluna
                if (j == 0){
                    int v[4] = {foto.mat[i][j], foto.mat[i-1][j], foto.mat[i-1][j+1], foto.mat[i][j+1]};
                    fotoED.mat[i][j] = maior(4, v);
                }
                //ultima coluna
                else if (j == foto.largura - 1){
                    int v[4] = {foto.mat[i][j], foto.mat[i][j-1], foto.mat[i-1][j-1], foto.mat[i-1][j]};
                    fotoED.mat[i][j] = maior(4, v);
                }
                //colunas do meio
                else{
                    int v[6] = {foto.mat[i][j], foto.mat[i][j-1], foto.mat[i-1][j-1], foto.mat[i-1][j], foto.mat[i-1][j+1], foto.mat[i][j+1]};
                    fotoED.mat[i][j] = maior(6, v);
                }
            }
            //primeira coluna, exceto primeira e ultima linha
            if((j == 0) && (i > 0) && (i < foto.altura - 1)){
                int v[6] = {foto.mat[i][j], foto.mat[i-1][j], foto.mat[i-1][j+1], foto.mat[i][j+1], foto.mat[i+1][j+1], foto.mat[i+1][j]};
                fotoED.mat[i][j] = maior(6, v);
            }

            //ultima coluna, exceto primeira e ultima linha
            if((j == foto.largura - 1) && (i > 0) && (i < foto.altura - 1)){ 
                int v[6] = {foto.mat[i][j], foto.mat[i-1][j], foto.mat[i-1][j-1], foto.mat[i][j-1], foto.mat[i+1][j-1], foto.mat[i+1][j]}; 
                fotoED.mat[i][j] = maior(6, v);
            }

            //linhas e colunas do meio
            if((i > 0) && (i < foto.altura - 1) && (j > 0) && (j < foto.largura - 1)){
                int v[9] = {foto.mat[i][j], foto.mat[i-1][j], foto.mat[i-1][j-1], foto.mat[i][j-1], foto.mat[i+1][j-1], foto.mat[i+1][j], foto.mat[i+1][j+1], foto.mat[i][j+1], foto.mat[i-1][j+1]};
                fotoED.mat[i][j] = maior(9, v);
            }
        }
    }
    
    return fotoED;
}

//funcao do menu para selecionar a imagem
int caminho(char * dict) {

  int escolha = -1;

  while (escolha != 0) {
    printf("Selecionar imagem:\n\n");

    printf("Caminho atual = %s\n\n", dict);

    printf("Digite o numero da opcao desejada:\n");
    printf("[1] Confirmar caminho\n");
    printf("[2] Editar caminho\n");
    printf("[0] Voltar\n");

    scanf("%d", &escolha);

    if (escolha == 1) {

        printf("Caminho confirmado!\n\n");
        return 0;
    } else if (escolha == 2) {

        printf("Digite o novo caminho: \n");
        scanf("%s", &*dict);
    } else if (escolha == 0) {

        return 0;
    }
  }
  return 0;
}

//funcao do menu para aplicar o filtro
IMG filtro(IMG foto, IMG fotoED) {

    int escolha = -1;

  while (escolha != 0) {
    printf("Selecione o numero do filtro que deseja aplicar:\n");
    printf("[1] Negativo\n");
    printf("[2] Espelhamento\n");
    printf("[3] Borramento\n");
    printf("[4] Brightening\n");
    printf("[0] Sair\n");

    scanf("%d", &escolha);

    if (escolha == 1) {
      fotoED = negativo(foto);
      printf("Filtro negativo aplicado!\n\n");
      escolha = 0;
    } 
    
    else if (escolha == 2) {
      fotoED = espelhamento(foto, fotoED);
      printf("Filtro espelhamento aplicado!\n\n");
      escolha = 0;
    } 
    
    else if (escolha == 3) {
      fotoED = blur(foto, fotoED);
      printf("Filtro borramento aplicado!\n\n");
      escolha = 0;
    }
    
    else if (escolha == 4) {
      fotoED = bright(foto, fotoED);
      printf("Filtro brightening aplicado!\n\n");
      escolha = 0;
    }
    
    else if (escolha == 0) {
      escolha = 0;
    }
  }
  return fotoED;
}

//funcao do menu para salvar a imagem
int salvar(IMG fotoED, char *dictOut) {

  int escolha = -1;

  while (escolha != 0) {
    printf("Selecionar caminho para salvar imagem:\n\n");

    printf("Caminho atual = %s\n\n", dictOut);

    printf("Digite o numero da opcao desejada:\n");
    printf("[1] Confirmar e salvar\n");
    printf("[2] Editar caminho\n");
    printf("[0] Voltar\n");

    scanf("%d", &escolha);

    if (escolha == 0) {
      return 0;
    } else if (escolha == 1) {
      salvarArquivo(fotoED, dictOut);
      printf("Imagem salva!\n\n");
      return 0;
    } else if (escolha == 2) {
      
      printf("Digite o novo caminho: \n");
      scanf("%s", &*dictOut);
    }
  }
  return 0;
}

//MENU PRINCIPAL
int menu_main(char* dict, char* dictOut, IMG foto, IMG fotoED) {
  
    int action;
    action = -1;

    while (action != 0) {

        printf("Selecione o numero da opcao desejada:\n");
        printf("[1] Selecionar imagem\n");
        printf("[2] Aplicar filtro\n");
        printf("[3] Salvar resultado\n");
        printf("[0] SAIR\n");

        scanf("%d", &action);

        if (action == 1) {
            caminho(dict);
        } else if (action == 2) {
            foto = filtro(foto, fotoED);
        } else if (action == 3) {
            salvar(foto, dictOut);
        } else if (action == 0) {
            printf("FIM\n\n");
            return 0;
        }
    }
    return 0;
}

int main(void){
    
    //int i;
    
    //diretorio padrao
    char dict[1024] = "ifes.pgm";
    char dictOut[1024] = "ifes-out.pgm";
    
    //carregar as imagens
    IMG foto = lerArquivo(dict);
    IMG fotoED = lerArquivo(dict);
    
    menu_main(dict, dictOut, foto, fotoED);
    

    // //libera a memória alocada para a matriz
    // for (i = 0; i < foto.altura; i++) {
    // free(foto.mat[i]);
    // }
    // free(foto.mat);
    
    return 0;
}