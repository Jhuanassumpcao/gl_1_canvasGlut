/*********************************************************************
// Canvas para desenho, criada sobre a API OpenGL. Nao eh necessario conhecimentos de OpenGL para usar.
//  Autor: Cesar Tadeu Pozzer
//         05/2020
//
//  Pode ser utilizada para fazer desenhos, animacoes, e jogos simples.
//  Tem tratamento de mouse e teclado
//  Estude o OpenGL antes de tentar compreender o arquivo gl_canvas.cpp
//
//  Versao 2.0
//
//  Instru��es:
//	  Para alterar a animacao, digite numeros entre 1 e 3
// *********************************************************************/

#include <GL/glut.h>
#include <GL/freeglut_ext.h> //callback da wheel do mouse.

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>

#include "gl_canvas2d.h"

#include "Bola.h"
#include "Relogio.h"
#include "botaoManager.h"
#include "painelmanager.h"
#include "figuraManager.h"

Bola    *b = NULL;
Relogio *r = NULL;
BotaoManager botaoManager;
PainelManager painelManager;

//variavel global para selecao do que sera exibido na canvas.
int opcao  = 50;
int screenWidth = 1366, screenHeight = 700; //largura e altura inicial da tela . Alteram com o redimensionamento de tela.
int mouseX, mouseY; //variaveis globais do mouse para poder exibir dentro da render().

Painel TopPainel(0, screenHeight, screenWidth, screenHeight - 120,0.93,0.93,0.93);
Painel FormasPainel(screenWidth / 20, screenHeight -  8, (screenWidth / 4) - 30, screenHeight - 110,0.85,0.85,0.85);
Painel ColorPainel((screenWidth / 4)- 3, screenHeight -  8, (screenWidth / 3) - 30, screenHeight - 110,0.93,0.93,0.93);
Painel Color1Painel((screenWidth / 3)- 3, screenHeight -  8, (screenWidth / 2) , screenHeight - 110,0.93,0.93,0.93);

int InicioXPainel = (screenWidth / 17) - 2;
int InicioYPainel = screenHeight - 110;

static int figuraPressionada = -1;

void DrawMouseScreenCoords()
{
    char str[100];
    sprintf(str, "Mouse: (%d,%d)", mouseX, mouseY);
    CV::text(10,300, str);
    sprintf(str, "Screen: (%d,%d)", screenWidth, screenHeight);
    CV::text(10,320, str);
}

//funcao chamada continuamente. Deve-se controlar o que desenhar por meio de variaveis globais
//Todos os comandos para desenho na canvas devem ser chamados dentro da render().
//Deve-se manter essa fun��o com poucas linhas de codigo.
void render()
{
   CV::text(20,500,"Programa Demo Canvas2D");
   DrawMouseScreenCoords();
   InicioYPainel = screenHeight -  100;

   figuraManager->Render();

   painelManager.AtualizaPosicao(0, 0, screenHeight, screenWidth, screenHeight - 120);
   painelManager.AtualizaPosicao(1,screenWidth / 20, screenHeight -  8, (screenWidth / 4) - 30, screenHeight - 110);
   painelManager.AtualizaPosicao(2,(screenWidth / 4)- 3, screenHeight -  8, (screenWidth / 3) - 30, screenHeight - 110);
   painelManager.AtualizaPosicao(3,(screenWidth / 3)- 3, screenHeight -  8, (screenWidth / 2) , screenHeight - 110);





   painelManager.Render();
   botaoManager.Render();


}

//funcao chamada toda vez que uma tecla for pressionada.
void keyboard(int key)
{
   printf("\nTecla: %d" , key);
   if( key < 200 )
   {
      opcao = key;
   }

   switch(key)
   {
      case 27:
	     exit(0);
	  break;

	  //seta para a esquerda
      case 200:
         if(figuraPressionada >= 0){
            figuraManager->aumentaTamanho(figuraPressionada);
         }

	  break;

	  case 202:
	      if(figuraPressionada >= 0){
            figuraManager->dimunuiTamanho(figuraPressionada);
	      }
	  break;

	  //seta para a direita
	  case 127:
          figuraManager->RemoveFigura(figuraPressionada);
	  break;
   }
}

//funcao chamada toda vez que uma tecla for liberada
void keyboardUp(int key)
{
   printf("\nLiberou: %d" , key);
}
//funcao para tratamento de mouse: cliques, movimentos e arrastos

void mouse(int button, int state, int wheel, int direction, int x, int y)
{
    mouseX = x;
    mouseY = y;

    Ponto mouse;
    mouse.x = mouseX;
    mouse.y = mouseY;

    printf("\nmouse %d %d %d %d %d %d", button, state, wheel, direction,  x, y);

    static bool mousePressed = false;
    static bool mouseEsqPressed = false;


    if (button == 0)
    {
        if (state == 0) // botão pressionado
        {
            figuraPressionada = figuraManager->FiguraClicada(mouse);
            botaoManager.BotaoClicado(mouse);
            mousePressed = true;
        }
        else if (state == 1) // botão liberado
        {
            mousePressed = false;
        }
    }else if(button == 2){
        if(state == 0){
            figuraPressionada = figuraManager->FiguraClicada(mouse);
             mouseEsqPressed = true;
        }else if (state == 1) {
            mouseEsqPressed = false;
        }
    }

    if (mousePressed && figuraPressionada >= 0)
    {
        figuraManager->moveFigura(figuraPressionada, mouse, state);
    }
    if (mouseEsqPressed && figuraPressionada >= 0)
    {
        figuraManager->RotacionarFigura(figuraPressionada);
    }



}


void DrawBotoes() {

    const int num_botoes = 7;
    Botao* botoes[num_botoes];
    int lados = 3;

    for (int i = 0; i < num_botoes; i++) {
        botoes[i] = new Botao(new Poligono(Ponto(InicioXPainel, InicioYPainel), 15, lados),
                      [lados](Ponto p) { return new Poligono(Ponto(576, 247), 60, lados); });
        InicioXPainel += 34;
        lados++;
    }

    // Adicionar os botões ao gerenciador de botões usando um loop for
    for (Botao* botao : botoes) {
        botaoManager.AddBotao(botao);
    }


   Botao* botaoCircular = new Botao(new Poligono(Ponto(117,640), 15, 100), [](Ponto p) { return new  Poligono(Ponto(700,247), 60, 100); });
   Botao* botaoLinha = new Botao(new Linha(Ponto(75,625)), [](Ponto p) { return new  Linha(Ponto(696,247)); });

   botaoManager.AddBotao(botaoLinha);
   botaoManager.AddBotao(botaoCircular);

}

void DrawPainels() {
   //Painel FormasPainel(68, 592, 300, 100,0.93,0.93,0.93);


     painelManager.AddPainel(FormasPainel);
     painelManager.AddPainel(TopPainel);
     painelManager.AddPainel(ColorPainel);
     painelManager.AddPainel(Color1Painel);


}

int main(void)
{
   b = new Bola();
   r = new Relogio();

   figuraManager = new FiguraManager();

   DrawPainels();
   DrawBotoes();

   CV::init(&screenWidth, &screenHeight, "Trabalho 1: Jhuan Assumpcao");
   CV::run();
}
