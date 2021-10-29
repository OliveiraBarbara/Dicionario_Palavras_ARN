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
#include "arn.h"

lista_sinonimo *aloca_sinonimo(char *s){
	lista_sinonimo *nova;
	
	nova = (lista_sinonimo *) malloc (sizeof (lista_sinonimo)); /*aloco um espaco na memoria*/
	
	if(nova != NULL){
		strcpy(nova->sinonimo, s); /*na posição da lista na variavel do sinonimo eu atribuo o sinonimo correto*/
		nova->prox_sin = NULL;
	}else{ /*se a alocacao nao deu certo, informo na tela*/
		printf("Erro ao alocar memória\n");
	}
	return nova;
}

arvore *aloca(arvore *pPai, char *str, char *id, char *s){
	arvore *nova;
	
	nova = (arvore *) malloc (sizeof (arvore)); /*aloco um espaço na memória*/
	
	if(nova != NULL){ /*se consegui alocar o espaço faço o novo nó receber o conteudo (palavra, sinonimo e idioma)*/
		strcpy(nova->palavra, str);
		strcpy(nova->idioma, id);
		nova->cor = RED;
		nova->primeiro_conceito = aloca_sinonimo(s);
		nova->esq = NULL;
		nova->dir = NULL;
		nova->pai = pPai;
	}else{ /*se a alocacao nao deu certo, informo na tela*/
		printf("Erro ao alocar memória\n");
	}
	return nova;
}

void insereArvore(arvore **tree, arvore *pai, arvore **raiz, char *str, char *id, char *sin){
	lista_sinonimo *nova, *aux;

	if(*tree == NULL){ /*se a arvore estiver vazia ou o local que quero alocar estiver vazio, aloco um novo no na arvore*/
		*tree = aloca(pai, str, id, sin);
	}else if(strcmp(str, (*tree)->palavra) < 0){ /*se a nova palavra for menor lexicograficamente, chamo a função e faço o no ser inserido na subarvore esquerda*/
		insereArvore((&(*tree)->esq), *tree, raiz, str, id, sin);
		conserta((&(*tree)->esq));
	}else if(strcmp(str, (*tree)->palavra) > 0){ /*se a nova palavra for maior lexicograficamente, chamo a função e faço o no ser inserido na subarvore direita*/
		insereArvore((&(*tree)->dir), *tree, raiz, str, id, sin);
		conserta((&(*tree)->dir));
	}else if(strcmp(str, (*tree)->palavra) == 0){ /*se a palavra já existir, aloco somento o seu novo sinonimo*/
		if(strcmp(sin, (*tree)->primeiro_conceito->sinonimo) < 0){ /*se o primeiro sinonimo for menor lexicograficamente menor que o sinonimo ja inserido, insiro o novo na primeira posicao*/
			nova = aloca_sinonimo(sin);
			nova->prox_sin = (*tree)->primeiro_conceito;
			(*tree)->primeiro_conceito = nova;
		}else{ /*se nao procuro o lugar que ele deve ser inserido*/
			aux = (*tree)->primeiro_conceito;
			
			while(aux->prox_sin != NULL && (strcmp(sin, (*tree)->primeiro_conceito->sinonimo) > 0))
				aux = aux->prox_sin;
			
			nova = aloca_sinonimo(sin);
			nova->prox_sin = aux->prox_sin;
			aux->prox_sin = nova;
		}
	}
	
	if(*tree == *raiz)
		(*tree)->cor = BLACK;
}

arvore *nTio(arvore *tree){
	arvore *avo = (tree->pai)->pai;
	if (tree->pai == avo->esq)
		return avo->dir;
	else
		return avo->esq;
}

void rotacaoDireita(arvore **tree){
	arvore *u, *p;
	p = (*tree);
	
	u = p->esq;
	p->esq = u->dir;
	u->dir = p;
	u->pai = p->pai;
	if(p->dir != NULL)
		(p->dir)->pai = p;
	p->pai = u;
	u->cor = p->cor;
	p->cor = RED;
	*tree = u;
}

void rotacaoEsquerda(arvore **tree){
	arvore *u, *p;
	p = (*tree);
	
	u = p->dir;
	p->dir = u->esq;
	u->esq = p;
	u->pai = p->pai;
	if(p->esq != NULL)
		(p->esq)->pai = p;
	p->pai = u;
	u->cor = p->cor;
	p->cor = RED;
	*tree = u;
}

void conserta(arvore **tree){
	arvore *avo = NULL;
	arvore *tio = NULL;

	if((*tree)->pai != NULL){
		if((*tree)->pai->cor == RED && (*tree)->cor == RED){
			tio = nTio(*tree); 
			if(tio != NULL && tio->cor == RED){ /*Caso no eh rubro e o tio do no tambem eh rubro*/
				avo = ((*tree)->pai)->pai;
				(*tree)->pai->cor = BLACK;
				tio->cor = BLACK;
				if(avo->pai != NULL) 
					avo->cor = RED;
			}else if(tio == NULL || tio->cor == BLACK){ /*tio ser preto ou nulo e o pai e o no serem vermelhos*/
				if(avo != NULL){
					if(avo->dir == tio){ /*tio eh filho direito do avo*/
						if((avo->esq)->esq == *tree){ /*no eh filho esquerdo esquerdo do avo*/
							rotacaoDireita(&(avo)); /*faco rotacao a direita no avo*/
						}else if((avo->esq)->dir == *tree){ /*no eh filho esquerdo direito do avo*/
							rotacaoEsquerda(&((*tree)->pai)); /*faco rotacao a esquerda no pai*/
							rotacaoDireita(&(avo)); /*faco rotacao a direita no avo*/
						}
					}else{ /*tio eh filho esquerdo do avo*/
						if((avo->dir)->dir == *tree){ /*no eh filho direito direito do avo*/
							rotacaoEsquerda(&(avo)); /*faco rotacao a esquerda no avo*/
						}else if((avo->dir)->esq == *tree){ /*no eh filho direito esquerdo do avo*/
							rotacaoDireita(&((*tree)->pai)); /*faco rotacao a direita no pai*/
							rotacaoEsquerda(&((avo))); /*faco rotacao a esquerda no avo*/
						}
					}
				}	
			}
		}
	}
}

void busca(arvore *tree, char *str){
	lista_sinonimo *q;

	if(tree == NULL) /*se a palavra não existir na arvore ou a arvore estiver vazia eu retorno a mensagem*/
		printf("hein?\n");
	else{
		if(strcmp(str, tree->palavra) == 0){ /*se a palavra existir, imprimo seus respectivos sinonimos*/
			for(q = tree->primeiro_conceito; q != NULL; q = q->prox_sin)
				printf("%s\n", q->sinonimo);
		}else if(strcmp(str, tree->palavra) < 0) /*se a palavra for menor lexicograficamento eu busco na subarvore esquerda*/
			busca(tree->esq, str);
		else if(strcmp(str, tree->palavra) > 0)/*se a palavra for maior lexicograficamento eu busco na subarvore direita*/
			busca(tree->dir, str);
	}
}

void removeArvore(arvore **tree, char *str){
	arvore *q, *irmao;
	
	if((*tree) != NULL){/*se tiver elementos na arvore ou ainda nao cheguei ainda não cheguei ao fim da arvore*/
		if(strcmp(str, (*tree)->palavra) == 0){ /*verifico se a palavra que quero remover eh a palavra que está na arvore*/
			q = (*tree); /*faco q receber o elemento que quero remover*/
			if(((*tree)->esq == NULL) && ((*tree)->dir == NULL)){
				if((*tree)->pai == NULL){ /*remocao do no raiz*/
					free(*tree); /*se for uma folha, removo direto o elemento*/
				     (*tree) = NULL; 
				     printf("raiz\n");
				}else{
					if((*tree)->cor == RED){ /*se for um no folha e for vermelho, apenas removo e faco o pai apontar pra nulo*/
						if((*tree)->pai->esq == *tree){
							free(*tree);
							(*tree)->pai->esq = NULL;
							printf("filho vermelho esq\n");
						}else{
							free(*tree);
							(*tree)->pai->dir = NULL;
							printf("filho vermelho dir\n");
						}
					}else{
						if((*tree)->pai->dir == (*tree)){
							irmao = (*tree)->pai->esq;
							if(irmao->cor == RED){ /*caso 1: irmao eh vermelho*/
								irmaoRED(&(irmao), &((*tree)->pai));
								free(*tree);
								printf("caso 1 esquerdo\n");
							}else if(irmao->cor == BLACK && irmao->dir->cor == BLACK && irmao->esq->cor == BLACK){/*caso2:irmao preto e os 2 filhos pretos*/
								irmaoBlackfilhosBlack(&(irmao), &((*tree)->pai));
								free(*tree);
								printf("caso 2 esquerdo\n");
							}else if(irmao->cor == BLACK && irmao->dir->cor == RED && irmao->esq->cor == BLACK){/*caso3:irmao preto e filho mais perto vermelho*/
								irmaoBlackfilhosRedBlack(&(irmao), &((irmao)->dir), &((*tree)->pai));
								free(*tree);
								printf("caso 3 esquerdo\n");
							}else if(irmao->cor == BLACK && irmao->esq->cor == RED){ /*caso 4: irmao preto e filho mais distante do no vermelho*/
								irmaoBlackfilhoRed(&(irmao), &((irmao)->esq), &((*tree)->pai));
								free(*tree);
								printf("caso 4 esquerdo\n");
							}
						}else{
							irmao = (*tree)->pai->dir;
							if(irmao->cor == RED){
								irmaoRED(&(irmao), &((*tree)->pai));
								free(*tree);
								printf("caso 1 direito\n");
							}else if(irmao->cor == BLACK && irmao->dir->cor == BLACK && irmao->esq->cor == BLACK){
								irmaoBlackfilhosBlack(&(irmao), &((*tree)->pai));
								free(*tree);
								printf("caso 2 direito\n");
							}else if(irmao->cor == BLACK && irmao->esq->cor == RED && irmao->dir->cor == BLACK){
								irmaoBlackfilhosRedBlack(&(irmao), &((irmao)->esq), &((*tree)->pai));
								free(*tree);
								printf("caso 3 direito\n");
							}else if(irmao->cor == BLACK && irmao->dir->cor == RED){
								irmaoBlackfilhoRed(&(irmao), &((irmao)->dir), &((*tree)->pai));
								free(*tree);
								printf("caso 4 direito\n");
							}
						}
					}
				}
			}else if((*tree)->esq == NULL){ /*remocao para no com um filho a direita*/
				(*tree) = (*tree)->dir; /*faço o no receber seu filho*/
				q->dir = NULL; /*faco o filho do q ser nulo*/
				free(q); /*removo o q*/
				printf("1filho esquerdo\n");
			}else if((*tree)->dir == NULL){ /*remocao para no com um filho a esquerda*/
				(*tree) = (*tree)->esq; /*faço o no receber seu filho*/
				q->esq = NULL; /*faco o filho do q ser nulo*/
				free(q);
				printf("1filho direito\n");
			}else{ /*remocao para no com 2 filhos*/
				q = (*tree)->esq; /*o q recebe o filho esquerdo do no que vou remover*/
				while(q->dir != NULL) /*procuro o maior elemento mais a esquerda do no*/
					q = q->dir;
				strcpy((*tree)->palavra, q->palavra); /*passo as informacoes do no que eu encontrei para o no que vou remover*/
				strcpy((*tree)->idioma, q->idioma);
				(*tree)->primeiro_conceito = q->primeiro_conceito;
				printf("2 filhos\n");
				removeArvore((&(*tree)->esq), (*tree)->palavra); /*chamo a funcao pra remover o no que troquei as informacoes*/
			}	
		}else if(strcmp(str, (*tree)->palavra) < 0) /*se a palavra for menor lexicograficamento eu busco na subarvore esquerda o elemento que vou remover*/
			removeArvore((&(*tree)->esq), str);
		else if(strcmp(str, (*tree)->palavra) > 0)/*se a palavra for maior lexicograficamento eu busco na subarvore direita o elemento que vou remover*/
			removeArvore((&(*tree)->dir), str);
	}
}

void irmaoRED(arvore **irmao, arvore **pai){ /*caso 1*/
	(*irmao)->cor = BLACK;
	(*pai)->cor = RED;
	
	if((*pai)->dir == (*irmao))
		rotacaoDireita(pai);
	else
		rotacaoEsquerda(pai);
	
	if((*irmao)->cor == BLACK && (*irmao)->dir->cor == RED && (*irmao)->esq->cor == BLACK)
		irmaoBlackfilhosRedBlack(irmao, &(*irmao)->dir, pai);
	if((*irmao)->cor == BLACK && (*irmao)->esq->cor == RED && (*irmao)->dir->cor == BLACK)
		irmaoBlackfilhosRedBlack(irmao, &(*irmao)->esq, pai);
	if((*irmao)->cor == BLACK && (*irmao)->esq->cor == RED)
		irmaoBlackfilhoRed(irmao, &(*irmao)->esq, pai);
	if((*irmao)->cor == BLACK && (*irmao)->dir->cor == RED)
		irmaoBlackfilhoRed(irmao, &(*irmao)->dir, pai);
}

void irmaoBlackfilhosBlack(arvore **irmao, arvore **pai){ /*caso 2*/
	(*irmao)->cor = RED;
	(*pai)->cor = BLACK;
}

void irmaoBlackfilhosRedBlack(arvore **irmao, arvore **fIrmao, arvore **pai){ /*caso 3*/
	(*irmao)->cor = RED;
	(*fIrmao)->cor = BLACK;
	if((*irmao)->dir == (*fIrmao))
		rotacaoDireita(pai);
	else
		rotacaoEsquerda(pai);
		
	if((*irmao)->cor == BLACK && (*irmao)->esq->cor == RED)
		irmaoBlackfilhoRed(irmao, &(*irmao)->esq, pai);
	if((*irmao)->cor == BLACK && (*irmao)->dir->cor == RED)
		irmaoBlackfilhoRed(irmao, &(*irmao)->dir, pai);
}

void irmaoBlackfilhoRed(arvore **irmao, arvore **fIrmao, arvore **pai){ /*caso 4*/
	(*irmao)->cor = RED;
	(*fIrmao)->cor = BLACK;
	(*pai)->cor = BLACK;
	if((*irmao)->dir == (*fIrmao))
		rotacaoDireita(pai);
	else
		rotacaoEsquerda(pai);
}

void remove_sinonimo(arvore **tree, char *str1, char *str2){
	arvore *q;
	lista_sinonimo *p, *s;
	s = NULL;
	
	if(*tree != NULL){ /*se tiver elementos na arvore ou ainda nao cheguei ainda não cheguei ao fim da arvore*/
		if(strcmp(str1, (*tree)->palavra) == 0){ /*verifico se a palavra que quero remover eh a palavra que está na arvore*/
			p = (*tree)->primeiro_conceito;
			q = (*tree);
			while (p != NULL && (strcmp(p->sinonimo, str2) != 0)){ /*procuro o sinonimo na lista de sinonimo*/
				s = p;
				p = p->prox_sin;
			}
			if (p != NULL){ /*se ele estiver na lista eu verifico se ele é o primeiro sinonimo, se for o primeiro eu faco a primeira posicao da lista de sinonimo receber o sinonimo seguinte do que eu vou remover, e se ele nao for o primeiro, faco o sinonimo anterior receber o sinonimo seguinte do que vou remover*/
				if(s != NULL)
					s->prox_sin = p->prox_sin;
				else
					(*tree)->primeiro_conceito = p->prox_sin;
				free(p); /*removo o sinonimo*/
			}
			if(q != NULL && q->primeiro_conceito == NULL) /*verifico se a palavra ficou sem sinonimo, se ficou eu chamo a funcao para remover ela*/
				removeArvore((&(*tree)), str1);
		}else if(strcmp(str1, (*tree)->palavra) < 0) /*se a palavra for menor lexicograficamento eu busco na subarvore esquerda o elemento que vou remover*/
			remove_sinonimo((&(*tree)->esq), str1, str2);
		else if(strcmp(str1, (*tree)->palavra) > 0) /*se a palavra for maior lexicograficamento eu busco na subarvore direita o elemento que vou remover*/
			remove_sinonimo((&(*tree)->dir), str1, str2);
	}
}

void lista_idioma(arvore *tree, char *id){ 
	lista_sinonimo *q;
	
	if (tree != NULL){ /*verifico se a arvore nao eh nula*/
		lista_idioma(tree->esq, id); /*vou ate a folha da subarvore esquerda*/
		if(strcmp(id, tree->idioma) == 0){ /*verifico se o idioma eh o que quero*/
			printf("%s(%d) : %s", tree->palavra, tree->cor, tree->primeiro_conceito->sinonimo); /*imprimo a palavra com seus sinonimos*/
			for(q = tree->primeiro_conceito->prox_sin; q != NULL; q = q->prox_sin)
				printf(", %s", q->sinonimo);
			printf("\n");
		}
		lista_idioma(tree->dir, id); /*vou ate a folha da subarvore direita*/
	} 
}

void lista_idioma_emIntervalo(arvore *tree, char *id, char *i, char *f){
	lista_sinonimo *q;
	
	if (tree != NULL){ /*verifico se a arvore nao eh nula*/
		lista_idioma_emIntervalo(tree->esq, id, i, f); /*vou ate a folha da subarvore esquerda*/
		if((strcmp(id, tree->idioma) == 0)){ /*verifico se o idioma eh o que quero*/
			if((tree->palavra[0] >= i[0]) && (tree->palavra[0] <= f[0])){ /*verifico se a palavra esta entre as iniciais informadas*/
				printf("%s : %s", tree->palavra, tree->primeiro_conceito->sinonimo); /*imprimo a palavra com seus sinonimos*/
				for(q = tree->primeiro_conceito->prox_sin; q != NULL; q = q->prox_sin)
					printf(", %s", q->sinonimo);
				printf("\n");
			}
		}
		lista_idioma_emIntervalo(tree->dir, id, i, f); /*vou ate a folha da subarvore direita*/
	} 
}

/*Funcao libera: Libera o espaco que foi armazenado em uma arvore*/
void libera(arvore *tree){/*realizo a liberacao do espaço armazenado da arvore em pos-ordem para começar dos nos folhas e por ultimo a raiz*/
	
	if(tree != NULL){ 
		libera(tree->esq);
		libera(tree->dir);
		free(tree);
	}
}
