#ifndef __BOTAOMANAGER_H__
#define __BOTAOMANAGER_H__

#include <iostream>
#include <vector>
#include <cmath>
#include "gl_canvas2d.h"
#include "figuraManager.h"
#include "Color.h"
#include <functional>
FiguraManager *figuraManager;

class Botao {
public:
    Figura* figura;
    std::function<Figura*(Ponto)> callback;
    float r;
    float g;
    float b;

    bool botaoPressionado = false;

    Botao(Figura* figura, std::function<Figura*(Ponto)> callback, float r, float g, float b) : figura(figura), callback(callback), r(r),g(g),b(b) {}

    bool BotaoClicado(Ponto p) {
        if (figura->Colidiu(p)) {
            printf("botao clicado");
            Figura* forma = callback(p);
            if(forma != nullptr) {
                forma->setVisivel(true); // torna a figura visível
                forma->setCor(r,g,b); // define a cor da figura
                figuraManager->AddFigura(forma);
            }else{
                printf("callback vazio");
                figura->setFill();
;            }

            return true;
        }
        return false;
    }

    void render() {
        figura->setVisivel(true);
        figura->Render();
    }
    float getR(){
        return r;
    }
    float getG(){
        return g;
    }
    float getB(){
        return b;
    }
};


class BotaoManager {
private:
  std::vector<Botao*> botoes;
  std::vector<bool> ativo;
public:
  void AddBotao(Botao* botao)
  {
    botoes.push_back(botao);
    ativo.push_back(true);
  }

  void RemoveBotao(unsigned int index)
  {
    if(index < botoes.size()){
      botoes.erase(botoes.begin()+index);
      ativo.erase(ativo.begin()+index);
    }
  }

  int BotaoClicado(Ponto mouse)
  {
    for(unsigned int i = 0; i < botoes.size(); i++){
      if(ativo[i] && botoes[i]->BotaoClicado(mouse)){
         printf("bateu no botao %f %f", mouse.x, mouse.y);
         int figuraClicada = figuraManager->FiguraClicada(mouse);
         printf("a figurta clicada foi %d", figuraClicada);
         if(figuraClicada >= 0)
             if(figuraManager->getFill(figuraClicada) == true){
                 figuraManager->ColoreFigura(figuraManager->FiguraClicada(mouse), botoes[i]->getR(), botoes[i]->getR(),botoes[i]->getB());
             }

        return i;
      }
    }
    return -1;
  }

  void Render()
  {
    for(unsigned int i = 0; i < botoes.size(); i++){
      if(ativo[i]){
        botoes[i]->render();
      }
    }
  }


};

#endif
