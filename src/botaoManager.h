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
int screenWidth = 1366, screenHeight = 700;
int InicioXPainel = (screenWidth / 17) + 5;
int InicioYPainel = screenHeight - 30;



class Botao {
public:
    Figura* figura;
    std::function<Figura*(Ponto)> callback;
    float r;
    float g;
    float b;

    bool botaoPressionado = false;
    bool isFill = false;

    Botao(Figura* figura, std::function<Figura*(Ponto)> callback, float r, float g, float b) : figura(figura), callback(callback), r(r),g(g),b(b) {}

    bool BotaoClicado(Ponto p) {
        if (figura->Colidiu(p)) {
            printf("botao clicado");
            Figura* forma = callback(p);
            botaoPressionado = false;
            if(forma != nullptr) {
                //Figura* mini = callback(Ponto(800,675));
                forma->setVisivel(true); // torna a figura visível
                forma->setCor(r,g,b); // define a cor da figura
               // mini->setVisivel(true);
                figuraManager->AddFigura(forma);
                //figuraManager->AddFigura(mini);
            }else{
                printf("callback vazio");
                figura->setFill();
                isFill = true;
                botaoPressionado = true;
;            }

            return true;
        }
        return false;
    }
    //void MudaPosicao(Ponto p) {
    //    figura->moveArrastando(p, figura->pontos[0]);
    //}

    void render() {
        figura->setVisivel(true);
        figura->setCor(r,g,b);
        figura->Render();
        //figura->moveArrastando(Ponto(InicioXPainel, InicioYPainel), figura->getCentro());
        //figuraManager->AddFigura(figura);

    }
    void atualizaCor(float r, float g, float b){
        this->r = r;
        this->r = g;
        this->r = b;
    }
    bool getBotaoPressionado(){
        return this->botaoPressionado;
    }
    bool botaoIsfill(){
        return this->isFill;
    }
    void setBotaoPressionado(bool value){
        this->botaoPressionado = value;
    }

    float getR(){
        return r;
    }
    float getG(){
        return g;
    }
    float getB(){
        botaoPressionado = false;
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
         printf("bateu no botao !!!!!!!!%d!!!!!!!!! ", i);
         printf("%d", botoes[i]->botaoIsfill());


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

 bool botaoCor(int i) {
        return botoes[i]->botaoIsfill();

  }
   bool getPressionado(int i ){

        return botoes[i]->getBotaoPressionado();

       }
   void atualizaCor(int i, float r, float g, float b){
       //figuraManager->ColoreFigura(21, 255,0,0);
        botoes[7]->atualizaCor(255,0,0);
   }

    float getR(int i){
        return botoes[i]->getR();
    }
    float getG(int i){
        return botoes[i]->getG();
    }
    float getB(int i){
        return botoes[i]->getB();
    }


};

#endif
