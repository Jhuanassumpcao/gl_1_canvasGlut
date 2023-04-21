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
int mouseX, mouseY; //variaveis globais do mouse para poder exibir dentro da render().


int tam_cores = 14;


static bool isLeftButtonDown = false;
static bool isRightButtonDown = false;
typedef void (*ButtonAction)(const Ponto&);

static int botaoPressionado = -1;

//funcao chamada continuamente. Deve-se controlar o que desenhar por meio de variaveis globais
//Todos os comandos para desenho na canvas devem ser chamados dentro da render().
//Deve-se manter essa fun��o com poucas linhas de codigo.
void render()
{
   figuraManager->Render();

   painelManager.AtualizaPosicao(0, 0, screenHeight, screenWidth, screenHeight - 120);
   painelManager.AtualizaPosicao(1,screenWidth / 20, screenHeight -  8, (screenWidth / 4) - 30, screenHeight - 110);
   painelManager.AtualizaPosicao(2,(screenWidth / 4)- 3, screenHeight -  8, (screenWidth / 3) - 30, screenHeight - 110);
   painelManager.AtualizaPosicao(3,(screenWidth / 3)- 3, screenHeight -  8, (screenWidth / 2) , screenHeight - 110);
   painelManager.AtualizaPosicao(4,(screenWidth) - 200, screenHeight -  8, (screenWidth / 2) + 100, screenHeight - 110);

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
	  case 19:
        figuraManager->escreverDados("figuras.txt");
      break;

	  //seta para a esquerda
      case 200:
         if(figuraPressionada >= 0){
            figuraManager->dimunuiTamanho(figuraPressionada);

         }

	  break;
	  case 201:
         if(figuraPressionada >= 0){
            figuraManager->RotacionarFigura(figuraPressionada, -5.0f);

         }

	  break;
	  case 203:
         if(figuraPressionada >= 0){
            figuraManager->RotacionarFigura(figuraPressionada, 5.0f);

         }

	  break;


	  case 202:
	      if(figuraPressionada >= 0){
            figuraManager->aumentaTamanho(figuraPressionada);
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

void LeftButtonDown(const Ponto& mousePos) {
    figuraPressionada = figuraManager->FiguraClicada(mousePos);
    botaoManager.BotaoClicado(mousePos);

    isLeftButtonDown = true;
}

void LeftButtonUp(const Ponto& mousePos) {
    isLeftButtonDown = false;
}

void RightButtonDown(const Ponto& mousePos) {
    figuraPressionada = figuraManager->FiguraClicada(mousePos);
    isRightButtonDown = true;
}

void RightButtonUp(const Ponto& mousePos) {
    isRightButtonDown = false;
}

ButtonAction buttonActions[3][2] = {
    {LeftButtonDown, LeftButtonUp},  // Botão esquerdo
    {nullptr, nullptr},             // Botão do meio
    {RightButtonDown, RightButtonUp} // Botão direito
};

void mouse(int button, int state, int wheel, int direction, int x, int y)
{
    mouseX = x;
    mouseY = y;

    Ponto mousePos;
    mousePos.x = mouseX;
    mousePos.y = mouseY;

    printf("\nmouse %d %d %d %d %d %d", button, state, wheel, direction,  x, y);

    if (button >= 0 && button < 3) {
        ButtonAction buttonAction = buttonActions[button][state];
        if (buttonAction) {
            buttonAction(mousePos);
        }
    }


    if (isLeftButtonDown && figuraPressionada >= 0)
    {
        figuraManager->moveFigura(figuraPressionada, mousePos, state);
        for(int i = 7; i <= tam_cores + 7; i++) {
            if(botaoManager.getPressionado(i)) {
                botaoManager.atualizaCor(21,botaoManager.getR(i), botaoManager.getG(i), botaoManager.getB(i));
                figuraManager->ColoreFigura(figuraPressionada, botaoManager.getR(i), botaoManager.getG(i), botaoManager.getB(i));
                break; // interrompe o loop após encontrar o primeiro botão pressionado
            }
        }


    }
    if (isRightButtonDown && figuraPressionada >= 0)
    {
        figuraManager->RotacionarFigura(figuraPressionada , -5.0f);
    }
}



void DrawBotoes() {

    const int num_botoes = 7;
    Botao* botoes[num_botoes];
    int lados = 3;

    for (int i = 0; i < num_botoes; i++) {
        botoes[i] = new Botao(new Poligono(Ponto(InicioXPainel, InicioYPainel), 15, lados, false),
                      [lados](Ponto p) { return new Poligono(Ponto(576, 247), 60, lados, false); }, 0,0,0);
        InicioXPainel += 34;
        lados++;
    }

    // Adicionar os botões ao gerenciador de botões usando um loop for
    for (Botao* botao : botoes) {
        botaoManager.AddBotao(botao);
    }
  Botao* botoesCor[tam_cores];
    const int margem = 13;
    const int x_inicial = 470;
    int x = x_inicial;
    const int tam_botao = 20;
    int y = 640;
    Color cores[] = {
        Color(255, 165, 0), // Laranja
        Color(255, 255, 0), // Amarelo
        Color(0, 255, 255), // Ciano
        Color(0, 0, 255),   // Azul
        Color(75, 0, 130),  // Índigo
        Color(238, 130, 238), // Violeta
        Color(255, 0, 0),   // Vermelho
        Color(165, 42, 0),  // Marrom
        Color(128, 255, 0), // Verde-limão
        Color(0, 255, 0),   // Verde
        Color(128, 0, 128), // Roxo
        Color(0, 128, 0),   // Verde-escuro
        Color(75, 0, 130),  // Índigo
        Color(238, 130, 238) // Violeta
    };
    Botao* botaoCor = new Botao(new Poligono(Ponto(380, 650), 40, 4, true), [](Ponto p) { return nullptr; }, 0, 0, 0);
       botaoManager.AddBotao(botaoCor);
    for (int i = 0; i < 14; i++) {
        botoesCor[i] = new Botao(new Poligono(Ponto(x, y), tam_botao, 4, true), [](Ponto p) { return nullptr; }, cores[i].r, cores[i].g, cores[i].b);
        x += tam_botao + margem;
        if (x > x_inicial + 6 * (tam_botao + margem)) {
            x = x_inicial;
            y += tam_botao + margem;
        }
    }

    for (Botao* botao : botoesCor) {
        botaoManager.AddBotao(botao);
    }

   Botao* botaoCircular = new Botao(new Poligono(Ponto(117,640), 15, 20, false), [](Ponto p) { return new  Poligono(Ponto(700,247), 60, 30, false); },0,0,0);
   botaoManager.AddBotao(botaoCircular);

}

void DrawPainels() {

   Painel TopPainel(0, screenHeight, screenWidth, screenHeight - 120,0.93,0.93,0.93);
   Painel FormasPainel(screenWidth / 20, screenHeight -  8, (screenWidth / 4) - 30, screenHeight - 110,0.85,0.85,0.85);
   Painel ColorPainel((screenWidth / 4)- 3, screenHeight -  8, (screenWidth / 3) - 30, screenHeight - 110,0.93,0.93,0.93);
   Painel Color1Painel((screenWidth / 3)- 3, screenHeight -  8, (screenWidth / 2) , screenHeight - 110,0.93,0.93,0.93);
    Painel FigsPainel((screenWidth) - 200, screenHeight -  8, (screenWidth / 2) + 100, screenHeight - 110,0.93,0.93,0.93);



     painelManager.AddPainel(FormasPainel);
     painelManager.AddPainel(TopPainel);
     painelManager.AddPainel(ColorPainel);
     painelManager.AddPainel(Color1Painel);
     painelManager.AddPainel(FigsPainel);


}

int main(void)
{

   figuraManager = new FiguraManager();
   figuraManager->lerDados("figuras.txt");

   DrawPainels();
   DrawBotoes();



   CV::init(&screenWidth, &screenHeight, "Trabalho 1: Jhuan Assumpcao");
   CV::run();
}
