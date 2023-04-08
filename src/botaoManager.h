#ifndef __BOTAOMANAGER_H__
#define __BOTAOMANAGER_H__

#include <iostream>
#include <vector>
#include <cmath>
#include "gl_canvas2d.h"
#include "figuraManager.h"
#include <functional>
FiguraManager *figuraManager;

class Botao {
public:
    Figura* figura;
    std::function<Figura*(Ponto)> callback;
    Botao(Figura* figura, std::function<Figura*(Ponto)> callback) : figura(figura), callback(callback) {}

    bool BotaoClicado(Ponto p) {
        if (figura->Colidiu(p)) {
            printf("botao clicado");
            Figura* forma = callback(Ponto(500,500));
            forma->setVisivel(true); // torna a figura visível
            figuraManager->AddFigura(forma);
            return true;
        }
        return false;
    }

    void render() {
        figura->setVisivel(true);
        figura->Render();
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
