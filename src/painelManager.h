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
  std::vector<Botao*> botoes;

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

  }

  void SetVisivel(bool v) {
    visivel = v;
  }

  bool IsVisivel() const {
    return visivel;
  }

  bool Colidiu(int mx, int my)
  {
      if( (mx >= x && mx <= (x + largura)) && (my >= y && my <= (y + altura)) ) {
          return true;
      }
      return false;
  }

  void Render() const {
    if (!visivel) return;

    CV::color(r,g,b);
    CV::rectFill(x, y, largura , altura);
    CV::color(0);
  }
  void AddBotao(Botao* botao) {
    botoes.push_back(botao);
  }
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

public:
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
      if(ativo[i] && paineis[i].Colidiu(mx, my)){
         printf("painel clicado");
        return i;
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


};

#endif
