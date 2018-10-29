#include "listadeprodutos.h"

PLISTA criarLista(){
  PLISTA res = (PLISTA) malloc(sizeof(LISTADEPRODUTOS));
  int x;
  for (x=0;x<NUMTIPOS;x++){
    res->LISTADELISTAS[x]=(PONT) malloc(sizeof(REGISTRO));
    res->LISTADELISTAS[x]->id=-1;
    res->LISTADELISTAS[x]->quantidade=0;
    res->LISTADELISTAS[x]->valorUnitario=0;
    res->LISTADELISTAS[x]->proxProd=NULL;
  }
  return res;
}


int tamanho(PLISTA l){
  int tam = 0;
  int x;
  PONT atual;
  for (x=0;x<NUMTIPOS;x++){
    atual = l->LISTADELISTAS[x]->proxProd;
    while (atual) {
      atual = atual->proxProd;
      tam++;
    }
  }
  return tam;
}

PONT buscarID(PLISTA l, int id){
  int x;
  PONT atual;
  for (x=0;x<NUMTIPOS;x++){
    atual = l->LISTADELISTAS[x]->proxProd;
    while (atual) {
      if (atual->id == id) return atual;
      atual = atual->proxProd;
    }
  }
  return NULL;
}

PONT buscarIDTipo(PLISTA l, int id, int tipo){
  if (tipo<0 || tipo>=NUMTIPOS) return NULL;
  PONT atual = l->LISTADELISTAS[tipo]->proxProd;
  while (atual) {
    if (atual->id == id) return atual;
    atual = atual->proxProd;
  }
  return NULL;
}

void exibirLog(PLISTA f){
  int numElementos = tamanho(f);
  printf("Log lista [elementos: %i]\n", numElementos);
  int x;
  PONT atual;
  for (x=0;x<NUMTIPOS;x++){
    printf("  #TIPO: %i -> ", x);
    atual = f->LISTADELISTAS[x]->proxProd;
    while (atual){
      printf(" [%i;%i;%i;$%i]", atual->id, atual->quantidade, atual->valorUnitario, atual->quantidade*atual->valorUnitario);
      atual = atual->proxProd;
    }
    printf("\n");
  }
  printf("\n");
}

int consultarValorUnitario(PLISTA l, int id){
  int x;
  PONT atual;
  for (x=0;x<NUMTIPOS;x++){
    atual = l->LISTADELISTAS[x]->proxProd;
    while (atual) {
      if (atual->id == id) return atual->valorUnitario;
      atual = atual->proxProd;
    }
  }
  return 0;
}

bool existeIdProduto(PLISTA l, int id){
  int x;
  PONT atual;
  for (x=0;x<NUMTIPOS;x++){
    atual = l->LISTADELISTAS[x]->proxProd;
    while (atual) {
      if (atual->id == id) return true;
      atual = atual->proxProd;
    }
  }

  return false;
}

int pegaIndexListaProduto(PLISTA l, int id){
  int x;
  PONT atual;
  for (x=0;x<NUMTIPOS;x++){
    atual = l->LISTADELISTAS[x]->proxProd;
    while (atual) {
      if (atual->id == id) return x;
      atual = atual->proxProd;
    }
  }

  return -1;
}

PONT buscaSeqExc(PLISTA l, int tipo, int id, int valorTotal, PONT* ant){
  *ant = NULL;
  PONT atual = l->LISTADELISTAS[tipo]->proxProd; //PRIMEIRA VEZ SEMPRE É NULL
  while (atual != NULL && (atual->quantidade*atual->valorUnitario < valorTotal)) {
    *ant = atual;
    atual = atual->proxProd;
  }
  if ((atual != NULL) && (atual->id == id)) return atual;
  return NULL;
}

PONT buscaSeqExc2(PLISTA l, int tipo, int id, PONT* ant){
  *ant = NULL;
  PONT atual = l->LISTADELISTAS[tipo]->proxProd; 
  while (atual != NULL && atual->id != id){
    *ant = atual;
    atual = atual->proxProd;
  }
  if ((atual != NULL) && (atual->id == id)) return atual;
  return NULL;
}

bool inserirNovoProduto(PLISTA l, int id, int tipo, int quantidade, int valor)
{
  if (id <= 0 || quantidade <= 0 || valor <= 0 || tipo > NUMTIPOS-1 || tipo <= 0 || existeIdProduto(l, id)) return false;
 
  PONT atual = l->LISTADELISTAS[tipo]->proxProd;
  PONT ant, i;
 
  i = buscaSeqExc(l, tipo, id, quantidade*valor, &ant);
  if (i != NULL)	return false;

  PONT novo = (PONT) malloc(sizeof(REGISTRO));
  novo->id = id;
  novo->quantidade = quantidade;
  novo->valorUnitario = valor;

  if (ant == NULL) {
    l->LISTADELISTAS[tipo]->proxProd = novo;
    novo->proxProd = atual;
  } else {
    novo->proxProd = ant->proxProd;
    ant->proxProd = novo;
  }

  return true;
}

bool removerItensDeUmProduto(PLISTA l, int id, int quantidade)
{
  if (quantidade <= 0 || id <= 0 || !existeIdProduto(l, id)) return false;

  int tipo = pegaIndexListaProduto(l, id);
  PONT ant, i;
  printf("%d", tipo);
  i = buscaSeqExc2(l, tipo, id, &ant);
  if (i == NULL)	return false; //if i==null nao existe o elemento na lista
  if (i->quantidade < quantidade)	return false;
  if (ant == NULL) l->LISTADELISTAS[tipo]->proxProd = i->proxProd;
  else ant->proxProd = i->proxProd;
  inserirNovoProduto(l, id, tipo, i->quantidade-quantidade, i->valorUnitario);
  free(i);

  return true;
}


bool atualizarValorDoProduto(PLISTA l, int id, int valor)
{
  if (valor <= 0 || id <= 0 || !existeIdProduto(l, id)) return false;

  int tipo = pegaIndexListaProduto(l, id);
  PONT ant, i;
  i = buscaSeqExc2(l, tipo, id, &ant);
  if (i == NULL)	return false; //if i==null nao existe o elemento na lista
  if (ant == NULL) l->LISTADELISTAS[tipo]->proxProd = i->proxProd;
  else ant->proxProd = i->proxProd;
  inserirNovoProduto(l, id, tipo, i->quantidade, valor);
  free(i);

  return true;
}
