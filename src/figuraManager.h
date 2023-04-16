#ifndef __FIGURAMANAGER_H__
#define __FIGURAMANAGER_H__

#include <iostream>
#include <vector>
#include <cmath>
#include "gl_canvas2d.h"
#include "Vector2.h"
#include "Ponto.h"

class Figura {
public:

    std::vector<Ponto> pontos;
    bool isFill = false;
    bool visivel = false;
    float r = 0;
    float g = 0;
    float b = 0;
    unsigned int num_lados = 0;
    int raio;


    void moveArrastando(Ponto ponto, Ponto mouse){
        int x = ponto.x - mouse.x;
        int y = ponto.y - mouse.y;
        for (auto& ponto : pontos) {
            ponto.x -= x;
            ponto.y -= y;
        }

    }

    void aumentaTamanho(float fator) {
        for (unsigned int i = 0; i < pontos.size(); i++) {
            pontos[i].x *= fator;
            pontos[i].y *= fator;
        }
    }


    unsigned int getLados(){
        return num_lados;
    }

    virtual void add_point(Ponto ponto){
        pontos.push_back(ponto);
    }

    void set_mouse(Ponto mouse){
        pontos.back() = mouse;
    }
    void setVisivel(bool visivel) {
        this->visivel = visivel;
    }
    std::vector<Ponto> getPontos() const {
        return pontos;
    }
    void setRaio(int _raio){
        raio = _raio;
    }

    virtual void Render() = 0;
    virtual bool Colidiu(Ponto mouse) = 0;


};

class Retangulo : public Figura {
public:

    Retangulo(Ponto ponto, Ponto ponto2){
        pontos.push_back(ponto);
        pontos.push_back(ponto2);
    }

    void Render() override
    {
        CV::color(r,g,b);
        if(visivel){
            if (isFill) {
            CV::rectFill(pontos[0], pontos[1]);
        } else {
            CV::rect(pontos[0], pontos[1]);
            }
        }
    }

    bool Colidiu(Ponto mouse) override
    {
        Ponto p = pontos[0];
        Ponto q = pontos[1];
        return mouse.x > p.x && mouse.y > p.y && mouse.x < q.x && mouse.y < q.y;
    }
};

class Circulo : public Figura {
public:

    Circulo(Ponto ponto, Ponto ponto2){
        pontos.push_back(ponto);
        pontos.push_back(ponto2);
    }

    void Render() override
    {
        CV::color(r, g, b);
        if(visivel){
            if (isFill) {
                CV::circleFill(pontos[0], radius(), 100);
        } else {
            CV::circle(pontos[0], radius(), 100);
            }
        }

    }

    bool Colidiu(Ponto mouse) override
    {
        Ponto p = pontos[0];
        Ponto q = pontos[1];
        int distancia = sqrt(pow(mouse.x-p.x,2) + pow(mouse.y-p.y,2));
          if (distancia <= radius()){
                return true;
            }
          return false;
    }

    float radius() {
        return pontos[0].distancia(pontos[1]);
    }
};

class Linha : public Figura {
public:

    Linha(Ponto ponto){
        pontos.push_back(ponto);
        Ponto novoPonto;
        novoPonto.x = ponto.x + 25;
        novoPonto.y = ponto.y + 25;
        pontos.push_back(novoPonto);
    }


    void Render() override
    {
        if(visivel){
            CV::color(r, g, b);
            CV::polygon(pontos);
        }

    }

    bool Colidiu(Ponto mouse) override
    {
        Ponto p = pontos[0];
        Ponto q = pontos[1];
        float numerador = (p.y - q.y) * mouse.x - (p.x - q.x) * mouse.y + p.x * q.y - p.y * q.x;
        float denominador = (p.y - q.y) * (p.y - q.y) + (p.x - q.x) * (p.x - q.x);
        return abs(numerador) / sqrt(denominador) < 2;
    }

};

class Poligono : public Figura {
public:

    Poligono(Ponto centro, float raio, int num_lados) {
        // calcular o �ngulo entre cada v�rtice
        float angulo = (2.0 * PI) / num_lados;

        // gerar os v�rtices em torno do ponto central
        for (int i = 0; i < num_lados; i++) {
            float x = centro.x + raio * cos(i * angulo);
            float y = centro.y + raio * sin(i * angulo);
            pontos.push_back(Ponto(x, y));
        }
    }

    void Render() override {
        if(visivel) {
            if (isFill) {
                CV::polygonFill(pontos);
            } else {
                CV::polygon(pontos);
            }
        }
    }

    bool Colidiu(Ponto mouse)
    {
        int n = pontos.size();
        int i, j;
        bool colidiu = false;
        for (i = 0, j = n - 1; i < n; j = i++) {
            if (((pontos[i].y > mouse.y) != (pontos[j].y > mouse.y)) &&
                (mouse.x < (pontos[j].x - pontos[i].x) * (mouse.y - pontos[i].y) / (pontos[j].y - pontos[i].y) + pontos[i].x))
                colidiu = !colidiu;
        }
        return colidiu;
    }


};


class FiguraManager {
private:
  std::vector<Figura*> figuras;
  Figura* figuraSelecionada = nullptr;
  std::vector<bool> ativo;
public:
  void AddFigura(Figura* figura)
  {
    figuras.push_back(figura);
    ativo.push_back(true);
  }

  void RemoveFigura(unsigned int index)
  {
    if(index < figuras.size()){
      figuras.erase(figuras.begin()+index);
      ativo.erase(ativo.begin()+index);
    }
  }

  int FiguraClicada(Ponto mouse)
  {
    for(unsigned int i = 0; i < figuras.size(); i++){
      if(ativo[i] && figuras[i]->Colidiu(mouse)){
         printf("bateu na figura %f %f", mouse.x, mouse.y);
        return i;
      }
    }
    return -1;
  }

  void Render()
  {
    for(unsigned int i = 0; i < figuras.size(); i++){
      if(ativo[i]){
        figuras[i]->Render();
      }
    }

  }
     void moveFigura(int i, Ponto mouse, int state){
        if(ativo[i] && figuras[i]->Colidiu(mouse)){
            // Calcular o centro do pol�gono
            Ponto centro;
            for(unsigned int j = 0; j < figuras[i]->getPontos().size(); j++){
                centro.x += figuras[i]->getPontos()[j].x;
                centro.y += figuras[i]->getPontos()[j].y;
            }
            centro.x /= figuras[i]->getPontos().size();
            centro.y /= figuras[i]->getPontos().size();

            // Usar o centro como ponto de refer�ncia na fun��o moveArrastando()
            figuras[i]->moveArrastando(centro, mouse);
        }
    }


    void diminuiTamanho(int i, float fator) {
        for(unsigned int j = 0; j < figuras[i]->getPontos().size(); j++){
            figuras[i]->getPontos()[j].x /= fator;
            figuras[i]->getPontos()[j].y /= fator;
        }
    }




};



#endif


