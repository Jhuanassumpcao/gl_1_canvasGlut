#ifndef __FIGURAMANAGER_H__
#define __FIGURAMANAGER_H__

#include <iostream>
#include <vector>
#include <cmath>
#include "gl_canvas2d.h"
#include "Vector2.h"
#include <fstream>
#include "Ponto.h"
#include <memory>

static int figuraPressionada = -1;

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
    int num_lados;
    Ponto centro;

    Figura(){

    }


    void moveArrastando(Ponto ponto, Ponto mouse){
        //printf("%d %d", ponto.x, ponto.y);
        int x = ponto.x - mouse.x;
        int y = ponto.y - mouse.y;
        for (auto& ponto : pontos) {
            ponto.x -= x;
            ponto.y -= y;
        }
        //this->centro = ponto;

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
    int getNum_lados(){
        return num_lados;
    }
    int getRaio() {
        return raio;
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
    Ponto pegacentro(){
        return this->centro;
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
    void setCentro(Ponto centro){
        this->centro = centro;
    }
    std::vector<Ponto> getPontos() const {
        return pontos;
    }
   virtual void escalar(float escala, Ponto centro) {
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
    virtual int getTipo()= 0;


};


class Linha : public Figura {
public:

    Linha(Ponto ponto, Ponto ponto2, bool isFill){
        pontos.push_back(ponto);
        pontos.push_back(ponto2);

        if(isFill) {
            this->isFill = true;
        }
    }


    void Render() override
    {

        CV::color(r, g, b);
        CV::polygon(pontos);

    }


    bool Colidiu(Ponto mouse) override
    {
        Ponto p = pontos[0];
        Ponto q = pontos[1];
        float numerador = (p.y - q.y) * mouse.x - (p.x - q.x) * mouse.y + p.x * q.y - p.y * q.x;
        float denominador = (p.y - q.y) * (p.y - q.y) + (p.x - q.x) * (p.x - q.x);
        return abs(numerador) / sqrt(denominador) < 2;
    }
    void escalar(float escala, Ponto centro) {
        Ponto p1 = pontos[0];
        float novaEscala;
        if((float)escala == (float)-0.1){
            novaEscala = 0.4;
        }else {
            novaEscala = 1.3;
        }

        Ponto p2 = pontos[1];
        float dx = p2.x - p1.x;
        float dy = p2.y - p1.y;
        float nova_distancia = sqrt(dx*dx + dy*dy) * novaEscala;
        float proporcao = nova_distancia / sqrt(dx*dx + dy*dy);
        pontos[0] = Ponto(centro.x + dx * proporcao / 2, centro.y + dy * proporcao / 2);
        pontos[1] = Ponto(centro.x - dx * proporcao / 2, centro.y - dy * proporcao / 2);
    }
    void Rotacionar(float angulo, Ponto centro) {
        Ponto p1 = pontos[0];
        Ponto p2 = pontos[1];
        float nova_x1 = centro.x + (p1.x - centro.x) * cos(angulo) - (p1.y - centro.y) * sin(angulo);
        float nova_y1 = centro.y + (p1.x - centro.x) * sin(angulo) + (p1.y - centro.y) * cos(angulo);
        float nova_x2 = centro.x + (p2.x - centro.x) * cos(angulo) - (p2.y - centro.y) * sin(angulo);
        float nova_y2 = centro.y + (p2.x - centro.x) * sin(angulo) + (p2.y - centro.y) * cos(angulo);
        pontos[0] = Ponto(nova_x1, nova_y1);
        pontos[1] = Ponto(nova_x2, nova_y2);
    }
    int getTipo(){
        return 0;
    }

};

class Poligono : public Figura {
public:


    Poligono(Ponto centro, float raio, int num_lados, bool isFill) {
        // calcular o ângulo entre cada vértice
        float angulo = (2.0 * PI) / num_lados;
        this->raio = raio;
        this->num_lados = num_lados;
        this->centro = centro;
        // gerar os vértices em torno do ponto central
        for (int i = 0; i < num_lados; i++) {
            float x = centro.x + raio * cos(i * angulo + 0.79);
            float y = centro.y + raio * sin(i * angulo + 0.79);
            pontos.push_back(Ponto(x, y));
        }

        //this->isFill = isFill;
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
    void escalar(float escala, Ponto centro) {
            for (auto& ponto : pontos) {
                ponto.x = centro.x + (ponto.x - centro.x) * (1 + escala);
                ponto.y = centro.y + (ponto.y - centro.y) * (1 + escala);
            }
            raio *= (1 + escala);

    }
    int getTipo(){
        return 1;
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
  int getTamanho(){
    return figuras.size();
  }

  int FiguraClicada(Ponto mouse)
  {
    for(unsigned int i = 0; i < figuras.size(); i++){
      if(ativo[i] && figuras[i]->Colidiu(mouse)){
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
         Ponto centro = figuras[i]->getCentro();
         figuras[i]->setCentro(centro);
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
        figura->escalar(0.1, figuras[i]->getCentro());
    }

    void dimunuiTamanho(int i) {
        Figura* figura = figuras[i];
        figura->escalar(-0.1, figuras[i]->getCentro());
    }


    void RotacionarFigura(int i, float angulo) {
        Figura* figura = figuras[i];
        //Poligono* poligono = dynamic_cast<Poligono*>(figura);
        if (figura != nullptr) {
            figura->Rotacionar(angulo, figuras[i]->getCentro());
        }
    }

    void ColoreFigura(int i, float r, float g, float b) {
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


    void escreverDados(const std::string& nomeArquivo, int criptografia) {
        std::ofstream arquivo(nomeArquivo);
        if (!arquivo) {
            std::cerr << "Erro ao abrir arquivo" << std::endl;
            return;
        }
        for (auto& figura : figuras) {
            arquivo << figura->getTipo() << " ";

            Ponto centro = figura->pegacentro();
            arquivo << figura->getR() + criptografia << " " << figura->getG() + criptografia << " " << figura->getB() + criptografia << " " << figura->getFill()<< " " << figura->getRaio() + criptografia << " " << centro.x  + criptografia << " " << centro.y  + criptografia << " " << figura->getNum_lados() + criptografia<< std::endl;
        }
    }
    Figura&  getFigura(int indice) {
        return *figuras[indice];
    }

    void lerDados(const std::string& nomeArquivo, int criptografia) {
        std::ifstream arquivo(nomeArquivo);
        if (!arquivo) {
            std::cerr << "Erro ao abrir arquivo" << std::endl;
            return;
        }

        int tipo;
        while (arquivo >> tipo) {
            float x, y;

            float r, g, b, raio;
            bool isFill;
            int centroX, centroY, num_lados;
            arquivo >> r >> g >> b >> isFill >> raio >> centroX >> centroY >> num_lados;
            Ponto centro = Ponto(centroX - criptografia, centroY - criptografia);
            if (tipo == 0) { // Linha
                //Figura* figura = new Linha(ponto, ponto2, isFill);
                //AddFigura(figura);
               // figura->setVisivel(true);
               // figura->setCor(r,g,b);
            } else if (tipo == 1) { // Poligono
                Figura* figura = new Poligono(centro, raio - criptografia, num_lados - criptografia, isFill);
                AddFigura(figura);
                figura->setVisivel(true);
                figura->setCor(r - criptografia,g - criptografia,b - criptografia);

            } else {
                std::cerr << "Tipo de figura inválido: " << tipo << std::endl;
            }
        }
    }

};



#endif


