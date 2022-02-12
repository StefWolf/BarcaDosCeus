
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

#define MAX_STR 50

FILE *arquivo;
// VARIÁVEIS DE MEDIÇÃO DO TABULEIRO
int height = 0;
int width = 0;
// VARIÁVEIS DE MIRA 
int alvoX;
int alvoY;
// VARIÁVEIS SOLTAS DO BOT
int peso = 0;
int estoque;

typedef struct{ //REGISTRO DO BOT
  int dadosBot;
  int x;
  int y;
  int **campo;
} Bot;

void readData(int h, int w, Bot *bot) {
  char id[MAX_STR];
  int v, n, x, y;
 
  // lê os dados da área de pesca
  for (int i = 0; i < h; i++) {   
    for (int j = 0; j < w; j++) {
      scanf("%i", &v); /*0 - mar, 1 - porto, 10 a 39*/
      bot->campo[i][j] = v; /* recolhendo infos matriz para o bot */
    }
  }
  // lê os dados dos bots
  scanf(" BOTS %i", &n);
  bot->dadosBot = n; /* rescolhendo para variavel bot*/
 
  for (int i = 0; i < n; i++) {
    scanf("%s %i %i", id, &x, &y);
    if(strcmp(id, "mimosa") == 0){ /* Quando for o botA*/
    fprintf(arquivo, "\n============ PARTIDA %s ===========\n", id);
      bot->x = x;
      bot->y = y;
    }
    
  }
  
}


/*-------------------------- O OLHO DE DEUS  ------------------------*/
/* PARA RASTREAR PEIXES*/
void rastrearPeixe(Bot bot){
  fprintf(arquivo, "Rastreando peixe \n");
  int isca = 20; //raio de 20
  int calculo; //calculo dinamico
  for(int i = 0; i < height; i++){ //escaneando toda a matriz...
    for(int j = 0; j < width; j++){

      if((bot.campo[i][j] % 10) > 1){ //peixes
        calculo = abs(bot.x - i) + abs(bot.y - j); //distancia do barco para os peixes

        if(calculo < 0){
          calculo = calculo * (-1);  /*Matar os negativos*/
        }

        if(calculo < isca){
          isca = calculo; //Novo alvo mais próximo 
          alvoX = i; //registrar coordenada x do alvo
          alvoY = j; //registrar coordenada y do alvo
        }
      }
    }
  }
}

/* PARA RASTREAR PORTO */
void rastrearPorto(Bot bot){
  fprintf(arquivo, "Rastreando porto \n");
  int isca = 20; //raio de 20
  int calculo; //calculo dinamico

  for(int i = 0; i < height; i++){
    for(int j = 0; j < width; j++){
      if(bot.campo[i][j] == 1){
        calculo = abs(bot.x - i) + abs(bot.y - j);
      }
      if(calculo < 0){
          calculo = calculo * (-1);  /*Matar os negativos*/
      }
      if(calculo < isca){
          isca = calculo; //Novo alvo mais próximo 
          alvoX = i; //registrar coordenada x do alvo
          alvoY = j; //registrar coordenada y do alvo
        }
    }
  }
  fprintf(arquivo, "Coordenadas do alvo: %d %d || Distancia do alvo: %d \n", isca, alvoX, alvoY);
}

/* --------------- FUNÇÃO PEGAR PEIXE ------------------ */
void pegarPeixe(){
  printf("FISH\n");
  fprintf(arquivo, "Pescando peixe \n");
  peso = peso + 1;
}
/* --------------- FUNÇÃO VENDER PEIXE ------------------ */
void venderPeixe(){
  printf("SELL\n");
  fprintf(arquivo, "Vendendo peixe \n");
  peso = 0;

}
/* --------------- FUNÇÃO INSPECIONAR ESTOQUE ------------------ */
void inspecionarEstoque(){
  fprintf(arquivo, "Estoque: %d kg \n", peso);
  // CARGA MÁXIMA: 10
  if(peso < 10){
    estoque = 1; 
  } else{
    estoque = 0; 
  }
}
/*--------------- FUNÇÃO ESCOLHER PESCAR OU VENDER --------------*/
void escolherAcao(){
  if(estoque == 1){
    pegarPeixe();
  } else{
    venderPeixe();
  }
}
/*--------------- FUNÇÃO DE MOVIMENTOS -----------------*/
// FISH - PESCAR | SELL - VENDER
void movimentar(Bot bot){
  if(bot.y > alvoY){
    fprintf(arquivo, "Direção: Esquerda \n");
    printf("LEFT\n");
  } else if(bot.y < alvoY){
    fprintf(arquivo, "Direção: Direita \n");
      printf("RIGHT\n");
  } else{//Se são iguais
      if(bot.x > alvoX){
        fprintf(arquivo, "Direção: Cima \n");
          printf("UP\n");
      } else if(bot.x < alvoX){
        fprintf(arquivo, "Direção: Baixo \n");
          printf("DOWN\n");
      } else{
          fprintf(arquivo, "Direção: Estamos em cima do alvo!! \n");
          escolherAcao(); //Vai pescar ou vender? isso dependerá de seu estoque
      }
  }
}
/*--------------------------------------------------------------*/
int main() { /*É DAQUI QUE INICIA*/
  char line[MAX_STR];   // dados temporários
  char myId[MAX_STR];   // identificador do bot em questão

  arquivo = fopen("logs/mimosa.txt", "w");
  Bot bot; /* Chamando registro */
  estoque = 1;

  setbuf(stdin, NULL);   // stdin, stdout e stderr não terão buffers
  setbuf(stdout, NULL);  // assim, nada é "guardado temporariamente"
  setbuf(stderr, NULL);

  // === INÍCIO DA PARTIDA ===
  int h, w;
  scanf("AREA %i %i", &h, &w);  // lê a dimensão da área de pesca: altura (h) x largura (w)
  height = h;
  width = w;
  bot.campo = malloc(height * sizeof(int*));
  for(int i = 0; i < height; i++){
    bot.campo[i] = malloc(width * sizeof(int));
  }
  

  scanf(" ID %s", myId);        // ...e o id do bot



  while (1) {

    readData(h, w, &bot); /*Atualiza o registro*/
    inspecionarEstoque();

    fprintf(arquivo, "====================================\n");
    fprintf(arquivo, "Coordenadas do barco: X: %d Y: %d\n", bot.x, bot.y);
    fprintf(arquivo, "Coordenadas do Alvo: X: %d Y: %d\n", alvoX, alvoY);

    switch (estoque){
      case 1:
        rastrearPeixe(bot);
      break;

      case 0:
        rastrearPorto(bot); 
    }
    movimentar(bot);
    scanf("%s", line);
  }

  
  fprintf(arquivo,"Tamanho do mar: %d x %d\n", width, height);

  fclose(arquivo);
  return 0;
}