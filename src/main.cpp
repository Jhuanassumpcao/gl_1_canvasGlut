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
int screenWidth = 1366, screenHeight = 705; //largura e altura inicial da tela . Alteram com o redimensionamento de tela.
int mouseX, mouseY; //variaveis globais do mouse para poder exibir dentro da render().

void DesenhaSenoide()
{
   float x=0, y;
   CV::color(1);
   CV::translate(20, 200); //desenha o objeto a partir da coordenada (200, 200)
   for(float i=0; i < 68; i+=0.001)
   {
      y = sin(i)*50;
      CV::point(x, y);
      x+=0.01;
   }
   CV::translate(0, 0);
}



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
   CV::color(0);
   CV::text(20,500,"Programa Demo Canvas2D");
   //CV::rect(Ponto(300,300), Ponto(500,500));
   DrawMouseScreenCoords();

   painelManager.Render();
   figuraManager->Render();
   botaoManager.Render();
    //botaoQuadrado->render();


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
     bool bclicado = botaoManager.BotaoClicado(mouse);
      // botaoQuadrado->callback(mouse);

   }
}

void DrawBotoes() {

}

void DrawPainels() {

   Painel TopPainel(0, 580, 1366, 130,0.85,0.85,0.85);
   Painel FormasPainel(68, 592, 300, 100,0.93,0.93,0.93);
   Painel Color1Painel(415, 592, 60, 100,0.93,0.93,0.93);
   Painel ColorPainel(500, 592, 300, 100,0.93,0.93,0.93);

   painelManager.AddPainel(TopPainel);
   painelManager.AddPainel(FormasPainel);
   painelManager.AddPainel(Color1Painel);
   painelManager.AddPainel(ColorPainel);
}

int main(void)
{
   b = new Bola();
   r = new Relogio();
   DrawPainels();
   figuraManager = new FiguraManager();

   CV::color(0);
   Linha linha(Ponto(100,100));
   Circulo circulo(Ponto(400,400));
   Quadrado quadrado(Ponto(100,100), Ponto(200,200));

   Botao* botaoQuadrado = new Botao(new Quadrado(Ponto(72,500), Ponto(80, 560)), [](Ponto p) { return new  Quadrado(Ponto(600,600), Ponto(650, 650)); });
   botaoManager.AddBotao(botaoQuadrado);

   //figuraManager->AddFigura(&linha);
  // figuraManager->AddFigura(&circulo);
  // figuraManager->AddFigura(&quadrado);


   DrawBotoes();


   CV::init(&screenWidth, &screenHeight, "Trabalho 1: Jhuan Assump��o");
   CV::run();
}
