#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct produto {
	char* chave;
	int valor;
} *Produto;
/* Estrutura Produto. Contem:
 * -Chave do produto
 * -Valor do stock */



static int N_Produtos=0;				/*Variavel global que conta o numero de produtos*/
static Produto *Table;					/* Ponteiro para o primeiro elemento da Hashtable*/
static Produto maximo;					/* Ponteiro para o Produto com maior stock.*/
static int Cap_Hashtable;				/*Capacidade da hashtable*/

#define CAP_INIT 49999

/*-------------------PRODUTO------------------------*/
void ProdutoIncrementaValor (Produto produto, int stock) {
	/* ProdutoIncrementaValor: Produto x int -> {}
	 * Funcao que recebe um ponteiro para uma estrutura struct produto 
	 * e um valor e incrementa o numero do stock pelo valor recebido.
	 * Se o incremento for negativo o stock fica com o valor zero.*/
	if (produto->valor+stock>=0)
		produto->valor+=stock;
	else
		produto->valor=0;
}

char *ProdutoChave (Produto produto) {
	/* ProdutoChave : Produto -> string
	 * Funcao que recebe um ponteiro para uma estrutura struct produto 
	 * e devolve a chave correspondente a esse produto.*/
	return produto->chave;
}

int ProdutoValor (Produto produto) {
	/* ProdutoValor : Produto -> int
	 * Funcao que recebe um ponteiro para uma estrutura struct produto
	 * e devolve um inteiro correspondente ao stock desse produto.*/
	return produto->valor;
}

void ProdutoPrint (Produto produto) {
	/* ProdutoPrint : Produto -> {}
	 * Funcao que recebe um ponteiro para uma estrutura struct produto e 
	 * imprime essa estrutura no standard output, ou seja, retorna a chave e o valor.*/
	if (produto!=NULL)
		printf("%s %d\n", produto->chave, produto->valor);
}

int ProdutoCompare (Produto p1, Produto p2) {
	/* Produtocompare : Produto x Produto -> int
	 * Funcao que recebe dois ponteiros para estruturas struct produto
	 * e compara as chaves. Devolve:
	 * int = 0 se forem iguais.
	 * int > 0 se t1 > t2
	 * int < 0 se t1 < t2*/
	return strcmp(p1->chave,p2->chave);
}

void ProdutoFree(Produto produto) {
	/* ProdutoFree : Produto -> {}
	 * Funcao que recebe um ponteiro para uma estrutura Struct produto e
	 * liberta a memoria dessa estrutura.
	 * Decrementa uma unidade na variavel global que guarda o numero de produtos.*/
	free(produto->chave);
	free(produto);
	N_Produtos--;
}

Produto ProdutoNew(char* chave, int valor) {
	/* ProdutoNew : string x int -> Produto
	 * Funcao que recebe uma chave correspondente a um codigo do produto
	 * e um valor correspondente ao numero de stock desse produto
	 * e devolve um ponteiro para a uma nova estrutura Struct produto.*/
	Produto new=(Produto)malloc(sizeof(struct produto));
	/*strcpy(new->chave,chave);*/
	new->chave=strdup(chave);
	if (valor<0)
		valor=0;
	new->valor=valor;
	return new;
}


/*----------------------HASHTABLE------------------------*/
void HashtableFree() {
	/* HashtableFree : {} -> {}
	 * Funcao que liberta a memoria de todos os elementos de uma
	 * hashtable e da propria table. */
	int i;
	for (i=0;i<Cap_Hashtable;i++)
		if (Table[i]!=NULL)
			ProdutoFree(Table[i]);
	free(Table);
}


void Percorre_Hashtable() {
	/* Percorre_Hashtable : {} -> {}
	 * Funcao que percorre toda a hashtable e encontra o produto com maior stock (maximo).*/
	int i;
	maximo=NULL;
	for (i=0;i<Cap_Hashtable;i++) {
		if (Table[i]!=NULL) {
			if(maximo==NULL || ProdutoValor(Table[i])>ProdutoValor(maximo))
				maximo=Table[i];
			else if (ProdutoValor(Table[i]) == ProdutoValor(maximo) && ProdutoCompare(maximo, Table[i]) > 0)
				maximo=Table[i];
		}
	}
}


void HashtableInit(int size) {
	/* HashtableInit : inteiro -> {}
	 * Funcao que cria uma Hashtable e inicializa-a com NULL.
	 * Inicializa o maximo a NULL*/
	int i;
	Cap_Hashtable = size;
	Table = (Produto*)malloc(Cap_Hashtable*sizeof(Produto));
	
	for (i = 0; i < Cap_Hashtable; i++)
		Table[i] = NULL;
	
	maximo = NULL;
}

unsigned int hash1(char *v, int Cap_Hashtable) { 
	/* hash : string x inteiro -> inteiro
	 * Funcao que calcula a posicao onde colocar a chave na hashtable.
	 * Evita anomalias com chaves altamente regulares.*/ 
  	unsigned int h, a = 31415, b = 27183; 
  	for (h = 0; *v != '\0'; v++, a = a*b % (Cap_Hashtable-1)) 
    	h = (a*h + *v) % Cap_Hashtable; 
  	return h; 
} 


void HashtableExpand();			/*Prototipo funcao HashtableExpand*/

void HashtableInsert(Produto prod) {
	/* HashtableInsert : Produto -> {}
	 * Funcao que recebe um ponteiro para uma estrutura struct produto
	 * e insere essa estrutura na hashtable.*/
	char *s = ProdutoChave(prod);
	unsigned int i = hash1(s,Cap_Hashtable);
	
	while (Table[i] != NULL) 
		i = (i+1) % Cap_Hashtable;
	Table[i] = prod;
	if (N_Produtos > Cap_Hashtable/2) 
		HashtableExpand();
} 


void HashtableExpand() {
	/* HashtableExpand : {} -> {}
	 * Funcao que e chamada quando a hashtable chega a metade da capacidade.
	 * Cria uma nova hashtable com uma capacidade superior e copia 
	 * os elementos da hashtable antiga para a nova. No final, liberta
	 * a memoria da hashtable antiga.*/
	int i;
	Produto *t = Table;
	HashtableInit(2*Cap_Hashtable);

	for (i = 0; i < Cap_Hashtable/2; i++)
		if (t[i] != NULL)
			HashtableInsert(t[i]);
	free(t);
}


Produto HashtableSearch(char *s) {
	/* HashtableSearch : string -> Produto
	 * Funcao que recebe uma string correspondente ao nome de um produto,
	 * procura esse produto na hashtable e devolve um ponteiro para a
	 * estrutura struct produto caso exista. Caso o produto nao exista,
	 * devole NULL.*/
	unsigned int i = hash1(s, Cap_Hashtable);
	while (Table[i] != NULL)
		if (!strcmp(s, ProdutoChave(Table[i])))
			return Table[i];
		else 
			i = (i+1) % Cap_Hashtable;
	return NULL;
}


void HashtableIncrementa(Produto prod, int valor) {
	/* HashtableIncrementa : Produto x inteiro -> {}
	 * Funcao que recebe um ponteiro para uma estrutura struct produto
	 * e incrementa o valor do stock desse produto na hashtable.*/
	ProdutoIncrementaValor(prod, valor);
}

/*---------------------IMPLEMENTACAO DOS COMANDOS--------------------------*/

void comando_a() {
	/* comando_a: {} -> {}
	 * Funcao que cria um novo produto com a chave e o codigo dados e adiciona-o na hashtable.
	 * Caso o produto ja exista incrementa o valor do stock.
	 * Verifica se o novo produto eh um maximo.*/
	char codigo[9];
	int stock;
	scanf("%s", codigo);
	scanf("%d", &stock);
	Produto prod;
	if ((prod = HashtableSearch(codigo)) == NULL) {
		prod = ProdutoNew(codigo,stock);
		HashtableInsert(prod);
		N_Produtos++;
	}
	else
		HashtableIncrementa(prod,stock);

	if (maximo!= NULL) {
		if (ProdutoCompare(prod,maximo)==0 && stock<0)
			maximo=NULL;
		else if (ProdutoValor(prod) > ProdutoValor(maximo))
			maximo=prod;
		else if (ProdutoValor(prod) == ProdutoValor(maximo) && ProdutoCompare(maximo,prod) > 0)
			maximo=prod;
	}	
}


void comando_m() {
	/* comando_m: {} -> {}
	 * Funcao que chama a funcao percorre_hashtable caso o maximo seja nulo.
	 * Escreve o maximo.*/
	if (maximo==NULL)
		Percorre_Hashtable();
	ProdutoPrint(maximo);

}



void comando_r(char* s) {
	/* comando_r: {} -> {}
	 * Funcao que remove um elemento da hashtable.
	 * Se o elemento nao existir a hashtable mantem-se igual.
	 * Se for removido algum elemento a hashtable move os elementos que estavam depois uma posicao para cima.
	 * Se o maximo for o produto a eliminar, maximo passa a ser NULL.*/
	int j;
	unsigned int i = hash1(s, Cap_Hashtable);
	if (maximo!=NULL)
		if (strcmp(s,ProdutoChave(maximo))==0)
			maximo=NULL;

	Produto v;

	while (Table[i]!=NULL) {
		if (strcmp(Table[i]->chave,s)==0) 
			break;
		else
			i=(i+1)%Cap_Hashtable;
	}

	if (Table[i]==NULL) return;
	
	ProdutoFree(Table[i]);
	
	Table[i]=NULL;
	
	for (j = (i+1)%Cap_Hashtable ; Table[j]!=NULL ; j = (j+1)%Cap_Hashtable) {
		v=Table[j];
		Table[j]=NULL;
		HashtableInsert(v);
	}
}


int cmpfunc(const void *a, const void *b) {
	/*Funcao auxiliar ao comando_l.
	* Eh responavel por comparar dois elementos conforme a ordem alfabetica.*/
	return ProdutoCompare(*(Produto*)a , *(Produto*)b);
}



void comando_l() {
	/* comando_l: {} -> {}
	 * Funcao copia os elementos da hashtable nao nulos para um vetor auxiliar.
	 * Lista todos elementos.*/
	Produto *prod=(Produto*)malloc(N_Produtos*(sizeof(struct produto)));
	int i, j;
	for (i = 0, j = 0 ; i < Cap_Hashtable ; i++)
		if (Table[i] != NULL) {
			prod[j] = Table[i];
			j++;
		}
	
	qsort(prod, N_Produtos, sizeof(Produto), cmpfunc);

	for (i=0;i<N_Produtos;i++)
		ProdutoPrint(prod[i]);
	free(prod);
}


/*--------------------------------------------------*/
/*                LISTA DE COMANDOS                 */
/*--------------------------------------------------*/
int main() {
  char comando, codigo[9];
  HashtableInit(CAP_INIT);
  while (1) {
    comando=getchar();
    switch (comando) {
      case 'a':
        comando_a();
      	getchar();
        break;
      case 'l':
        comando_l();
        getchar();
        break;
      case 'm':
      	comando_m();
        getchar();
        break;
      case 'r':
        scanf("%s", codigo);
        comando_r(codigo);
        getchar();
        break;
      case 'x':
        printf("%d\n",N_Produtos);
        HashtableFree();
    	  return 0;
      default:
        return -1;
    }
  }
}

