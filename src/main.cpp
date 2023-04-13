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

   figuraManager->Render();
   CV::color(0.85,0.85,0.85);
   CV::rectFill(0, screenHeight,screenWidth,  screenHeight - 120);
   //int screenWidth = 1366, screenHeight = 700;
   //Painel TopPainel(0, screenHeight - 142, screenWidth, screenHeight / 3,0.85,0.85,0.85);
   CV::color(0.93,0.93,0.93);
   CV::rectFill(screenWidth/20 , screenHeight - 8,(screenWidth/ 3) - 87,  screenHeight - 110);


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
         b->move(-10);
	  break;

	  //seta para a direita
	  case 202:
         b->move(10);
	  break;
   }
}

//funcao chamada toda vez que uma tecla for liberada
void keyboardUp(int key)
{
   printf("\nLiberou: %d" , key);
}
//funcao para tratamento de mouse: cliques, movimentos e arrastos
/*
void mouse(int button, int state, int wheel, int direction, int x, int y)
{
   mouseX = x; //guarda as coordenadas do mouse para exibir dentro da render()
   mouseY = y;

   Ponto mouse;
   mouse.x = mouseX;
   mouse.y = mouseY;

   printf("\nmouse %d %d %d %d %d %d", button, state, wheel, direction,  x, y);

   if( state == 0 ) //clicou
   {
     int fclicada = figuraManager->FiguraClicada(mouse);
     int pclicado = painelManager.PainelClicado(mouseX, mouseY);
     int bclicado = botaoManager.BotaoClicado(mouse);
     if(fclicada>=0 && state == 0){
        figuraManager->moveFigura(fclicada, mouse, state);
     }
      // botaoQuadrado->callback(mouse);

   }
}
*/
void mouse(int button, int state, int wheel, int direction, int x, int y)
{
    mouseX = x;
    mouseY = y;

    Ponto mouse;
    mouse.x = mouseX;
    mouse.y = mouseY;

    printf("\nmouse %d %d %d %d %d %d", button, state, wheel, direction,  x, y);

    static bool mousePressed = false;
    static int figuraPressionada = -1;

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
    }

    if (mousePressed && figuraPressionada >= 0)
    {
        figuraManager->moveFigura(figuraPressionada, mouse, state);
    }

}


void DrawBotoes() {

   Botao* botaoRetangular = new Botao(new Retangulo(Ponto(73,662), Ponto(96, 687)), [](Ponto p) { return new  Retangulo(Ponto(576,247), Ponto(754, 381)); });
   Botao* botaoCircular = new Botao(new Circulo(Ponto(118,675), Ponto(126, 687)), [](Ponto p) { return new  Circulo(Ponto(696,247), Ponto(754, 381)); });
   Botao* botaoLinha = new Botao(new Linha(Ponto(139,662)), [](Ponto p) { return new  Linha(Ponto(696,247)); });
   Botao* botaoPoligono = new Botao(new Poligono({Ponto(167,662), Ponto(180, 687), Ponto(190, 662)}), [](Ponto p) { return new  Poligono({Ponto(576,247), Ponto(660, 381), Ponto(754, 247)}); });



   botaoManager.AddBotao(botaoPoligono);
   botaoManager.AddBotao(botaoLinha);
   botaoManager.AddBotao(botaoRetangular);
   botaoManager.AddBotao(botaoCircular);

}

void DrawPainels() {
   //Painel FormasPainel(68, 592, 300, 100,0.93,0.93,0.93);
   Painel Color1Painel(415, 592, 60, 100,0.93,0.93,0.93);
   Painel ColorPainel(500, 592, 300, 100,0.93,0.93,0.93);

   //painelManager.AddPainel(TopPainel);
 //  painelManager.AddPainel(FormasPainel);
   painelManager.AddPainel(Color1Painel);
   painelManager.AddPainel(ColorPainel);

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
