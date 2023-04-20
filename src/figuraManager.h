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
    bool figuraSelecionada = false;
    float r = 0;
    float g = 0;
    float b = 0;
    int raio;


    void moveArrastando(Ponto ponto, Ponto mouse){
        //printf("%d %d", ponto.x, ponto.y);
        int x = ponto.x - mouse.x;
        int y = ponto.y - mouse.y;
        for (auto& ponto : pontos) {
            ponto.x -= x;
            ponto.y -= y;
        }

    }

    void moveFig(Ponto ponto, Ponto mouse){
        int x = ponto.x - mouse.x;
        int y = ponto.y - mouse.y;
        for (auto& ponto : pontos) {
            ponto.x -= x;
            ponto.y -= y;
        }

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
    void setCor(float r, float g, float b) {
        //printf("cores %f %f %f", r,g,b);
        this->r = r;
        this->g = g;
        this->b = b;
    }
    void setFill(){
        this->isFill = true;
    }
    bool getFill() {
        return this->isFill;
    }
    bool getSelecionada() {
        return this->figuraSelecionada;
    }
    void setSelecionada() {
        this->figuraSelecionada = true;
    }
    std::vector<Ponto> getPontos() const {
        return pontos;
    }
   virtual void AumentarRaio(float aumento, Ponto centro) {
        // faz nada por padrão
    }
    virtual void Rotacionar(float aumento, Ponto centro) {
        // faz nada por padrão
    }
    void DiminuirRaio(float reducao, Ponto centro) {

    }
    void MudaPosicao(int x, int y, Ponto centro) {

    }
     Ponto getCentro(){
          Ponto centro;
        for(unsigned int j = 0; j < getPontos().size(); j++){
            centro.x += getPontos()[j].x;
            centro.y += getPontos()[j].y;
        }
        centro.x /= getPontos().size();
        centro.y /= getPontos().size();
        return centro;
  }



    virtual void Render() = 0;
    virtual bool Colidiu(Ponto mouse) = 0;


};


class Linha : public Figura {
public:
    Linha(){
    }

    Linha(Ponto ponto, Ponto ponto2, bool isFill){
        pontos.push_back(ponto);
        pontos.push_back(ponto2);

        if(isFill) {
            this->isFill = true;
        }
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

    Poligono(Ponto centro, float raio, int num_lados, bool isFill) {
        // calcular o ângulo entre cada vértice
        float angulo = (2.0 * PI) / num_lados;

        // gerar os vértices em torno do ponto central
        for (int i = 0; i < num_lados; i++) {
            float x = centro.x + raio * cos(i * angulo + 0.79);
            float y = centro.y + raio * sin(i * angulo + 0.79);
            pontos.push_back(Ponto(x, y));
        }
        if(isFill) {
            this->isFill = true;
        }
    }

    void Render() override {
        CV::color(r, g, b);
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
    void AumentarRaio(float aumento, Ponto centro) {
            for (auto& ponto : pontos) {
                ponto.x = centro.x + (ponto.x - centro.x) * (1 + aumento);
                ponto.y = centro.y + (ponto.y - centro.y) * (1 + aumento);
            }
            raio *= (1 + aumento);

    }
    void DiminuirRaio(float reducao, Ponto centro) {
         for (auto& ponto : pontos) {
                ponto.x = centro.x + (ponto.x - centro.x) * (1 - reducao);
                ponto.y = centro.y + (ponto.y - centro.y) * (1 - reducao);
            }
        raio *= (1 - reducao);
    }


    void Rotacionar(float angulo, Ponto centro) {
            // Calcular a matriz de rotação
        float rad = angulo * PI / 180.0;
        float cos_ang = cos(rad);
        float sin_ang = sin(rad);
        float ox = centro.x * (1 - cos_ang) + centro.y * sin_ang;
        float oy = centro.y * (1 - cos_ang) - centro.x * sin_ang;
        float m[3][3] = {
            { cos_ang, -sin_ang, ox },
            { sin_ang,  cos_ang, oy },
            { 0,        0,       1  }
        };

        // Rotacionar cada ponto do polígono
        for (auto& ponto : pontos) {
            float x = ponto.x;
            float y = ponto.y;
            ponto.x = m[0][0]*x + m[0][1]*y + m[0][2];
            ponto.y = m[1][0]*x + m[1][1]*y + m[1][2];
        }
    }
    void MudaPosicao(int x, int y, Ponto centro) {
        for (auto& ponto : pontos) {
            ponto.x = ponto.x + (x - centro.x);
            ponto.y = ponto.y + (y - centro.y);
        }
        centro.x = x;
        centro.y = y;
    }




};


class FiguraManager {
private:
  std::vector<Figura*> figuras;
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
         printf("bateu na figura");
         figuras.push_back(figuras[i]);
         figuras.erase(figuras.begin() + i);

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
            // Calcular o centro do polígono
            Ponto centro = figuras[i]->getCentro();

            // Usar o centro como ponto de referência na função moveArrastando()
            figuras[i]->moveArrastando(centro, mouse);
        }
    }


    void aumentaTamanho(int i) {
        Figura* figura = figuras[i];
        Poligono* poligono = dynamic_cast<Poligono*>(figura);
        if (poligono != nullptr) {
            poligono->AumentarRaio(0.1, figuras[i]->getCentro());
        }
    }

    void dimunuiTamanho(int i) {
        Figura* figura = figuras[i];
        Poligono* poligono = dynamic_cast<Poligono*>(figura);
        if (poligono != nullptr) {
            poligono->DiminuirRaio(0.1, figuras[i]->getCentro());
        }
    }

    void RotacionarFigura(int i) {
        Figura* figura = figuras[i];
        Poligono* poligono = dynamic_cast<Poligono*>(figura);
        if (poligono != nullptr) {
            poligono->Rotacionar(5.0, figuras[i]->getCentro());
            printf("girandooo");
        }
    }

    void ColoreFigura(int i, float r, float g, float b) {
        printf("colorindo");
        figuras[i]->setFill();
        figuras[i]->setCor(r,g,b);
    }
    bool getFill(int i) {
        return figuras[i]->getFill();
    }
    bool getSelecionada(int i) {
        return figuras[i]->getSelecionada();
    }
    void setSelecionada(int i) {
        return figuras[i]->setSelecionada();
    }
    void AtualizaPosicao(int i, Ponto ponto) {
        //for(unsigned int i = 0; i < 3; i++ ){
            figuras[i]->moveFig(ponto, figuras[i]->getCentro());
        //}
    }





};



#endif


