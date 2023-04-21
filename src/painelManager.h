#ifndef __PAINELMANAGER_H__
#define __PAINELMANAGER_H__

#include "gl_canvas2d.h"
#include "botaoManager.h"
#include <vector>

class Painel {
private:
  float x, y, largura, altura;
  float r,g,b;
  bool visivel;
  bool isFill;
  float corAtual[3];
  //std::vector<Botao*> botoes;

public:
  Painel(float _x, float _y, float _largura, float _altura, float _r, float _g, float _b, bool _visivel = true)
  {
    x = _x;
    y = _y;
    largura = _largura;
    altura = _altura;
    r = _r;
    g = _g;
    b = _b;
    visivel = _visivel;
    corAtual[0] = r;
    corAtual[1] = g;
    corAtual[2] = b;

  }

  void SetVisivel(bool v) {
    visivel = v;
  }

  bool IsVisivel() const {
    return visivel;
  }

    bool Colidiu(int mx, int my)
    {
        if (mx >= x && mx <= (x + largura) && my >= y && my <= (y + altura))
        {
            return true;
        }
        return true;
    }
   void SetCor(float _r, float _g, float _b) {
    r = _r;
    g = _g;
    b = _b;
  }
  float GetR() {
      return this->r;
  }
  float GetG() {
      return this->g;
  }
  float GetB() {
      return this->b;
  }


  void Render() const {
    if (!visivel) return;

    CV::color(r,g,b);
    CV::rectFill(x, y, largura , altura);
    CV::color(0,0,0);
    CV::rect(x, y, largura , altura);
    CV::color(0);
  }
  //void AddBotao(Botao* botao) {
   // botoes.push_back(botao);
  //}
  void SetPosicao(float _x, float _y, float _largura, float _altura) {
    x = _x;
    y = _y;
    largura = _largura;
    altura = _altura;
}

};

class PainelManager {
private:
  std::vector<Painel> paineis;
  std::vector<bool> ativo;
  unsigned int painel_selecionado;
  bool painel_selecionado_ativo = false;
  float corAtual[3];

public:
    PainelManager() {
    corAtual[0] = 1.0f;
    corAtual[1] = 1.0f;
    corAtual[2] = 1.0f;
  }
  void AddPainel(Painel painel) {
    paineis.push_back(painel);
    ativo.push_back(true);
  }

  void RemovePainel(unsigned int index) {
    if (index >= 0 && index < paineis.size()) {
      paineis.erase(paineis.begin() + index);
      ativo.erase(ativo.begin() + index);
    }
  }

  int PainelClicado(int mx, int my)
  {
    for(unsigned int i = 0; i < paineis.size(); i++){
      if (paineis[i].Colidiu(mx, my)){
          return i;
        }else {
            painel_selecionado_ativo = false;
        }

    }
    return -1;
  }

  void SetVisivel(unsigned int index, bool v) {
    if (index >= 0 && index < paineis.size()) {
      paineis[index].SetVisivel(v);
    }
  }

  bool IsVisivel(unsigned int index)  {
    if (index >= 0 && index < paineis.size()) {
      return paineis[index].IsVisivel();
    }
    return false;
  }

  void Render()  {
    for (unsigned int i = 0; i < paineis.size(); i++) {
      if (ativo[i]) {
        paineis[i].Render();

      }

    }
  }
  void AtualizaPosicao(int index, float x, float y, float largura, float altura) {
      paineis[index].SetPosicao(x, y, largura, altura);
  }
  void SetCorSelecionada(float r, float g, float b) {
    corAtual[0] = r;
    corAtual[1] = g;
    corAtual[2] = b;
  }

  const float* GetCorSelecionada() const {
    return corAtual;
  }
  bool getPainelClicado(){
    return  painel_selecionado_ativo;
  }


};

#endif
