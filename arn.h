/***************************************************
Nome: Bárbara Beatriz Bueno de Oliveira
RGA: 2019.0743.014-8
Implementacao 4
Disciplina: Estruturas de Dados e Programacao I
Professor: Ronaldo Fiorilo
***************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 100
#define RED 1
#define BLACK 0

/*Struct da lista para armazenar os sinonimos*/
typedef struct lista_sinonimo{
	char sinonimo[MAX];
	struct lista_sinonimo *prox_sin; 
}lista_sinonimo;

/*Struct da arvore para armazenar as palavras*/
typedef struct arvore{
	char palavra[MAX];
	char idioma[3];
	int cor;
	int existe; /*existe 0 = No existe, existe 1 = No removido*/
	struct lista_sinonimo *primeiro_conceito;
	struct arvore *esq; 
	struct arvore *dir;
	struct arvore *pai;
}arvore;

/*Escopo de todas as funções utilizadas na implementação*/
lista_sinonimo *aloca_sinonimo(char *s);
arvore *aloca(arvore *pPai, char *str, char *id, char *s);

void insereArvore(arvore **tree, arvore *pai, arvore **raiz, char *str, char *id, char *sin);

arvore *nTio(arvore *tree);

void rotacaoDireita(arvore **tree);
void rotacaoEsquerda(arvore **tree);

void conserta(arvore **tree);

void removeArvore(arvore **tree, char *str);
void remove_sinonimo(arvore **tree, char *str1, char *str2);

void busca(arvore *tree, char *str);

void lista_idioma(arvore *tree, char *id);
void lista_idioma_emIntervalo(arvore *tree, char *id, char *i, char *f);

void libera(arvore *tree);
