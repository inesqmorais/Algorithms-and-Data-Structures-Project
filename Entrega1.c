#include <stdio.h>
#include <string.h>


#define MAX_MSN 10000 
#define MAX_USERS 1000
#define MAX_FRASE 140

/* Estrutura da mensagem: Uma  mensagem e composta por um identificador de utilizador
(um inteiro) e uma frase (uma sequência de caracteres) e o comprimento da frase (um inteiro)*/
typedef struct {
    int user;
    char frase[MAX_FRASE+1];
    int len;
} Forum;


/*---------------------------------------------*/
/*                                             */
/*              VARIAVEIS GLOBAIS              */
/*                                             */
/*---------------------------------------------*/

Forum mensagem[MAX_MSN];				/*vetor de estruturas que contem as mensagens do forum*/
int msn_por_user[MAX_USERS]={0};
int nMsn=0, max_msn=0, max_len=0;		/*nMsn: contador de mensagens ,max_len: guarda o comprimento maximo de cada frase*/ 
										/*max_msn: guarda o numero maximo de mensagens escrito por um so user*/


/*---------------------------------------------*/
/*                                             */
/*             FUNCOES AUXILIARES              */
/*                                             */
/*---------------------------------------------*/

int le_linha(char frase[]) {
  /* Funcao que le do standard input e escreve num vetor.
  Output --> Nao tem qualuqer output*/
  int  i, c;
  for (i=0;(c = getchar())!= '\n' && c != EOF;i++)
      frase[i]=c;
  frase[i] = '\0';

  return i;
}

int espacos_brancos(char s) {
  /* Funcao responsavel por devolver um valor verdadeiro se o caracter lido for um espaco branco.
  Output --> retorna 1 (valor verdadeiro) ou retorna 0 (valor falso)*/
  int r;
  if (s==';'|| s== ',' || s==' ' || s=='\t' || s=='\n' || s=='.' || s=='\0')
    r=1;
  else
    r=0;
  return r;
}


/*---------------------------------------------*/
/*                                             */
/*             FUNCOES PRINCIPAIS              */
/*                                             */
/*---------------------------------------------*/

void comando_A() {
  /* Funcao que adiciona uma mensagem ao forum de mensagens com os valores:
    - User
    - Frase
  Output --> Nao tem qualquer output*/
  scanf("%d", &mensagem[nMsn].user);
  getchar();												/*le o espaco que esta entre o user e a frase*/
  mensagem[nMsn].len = le_linha(mensagem[nMsn].frase);		/*o le_linha devolve o comprimento da frase, esse valor eh guardado no len, para alem disso escreve em mensagem[nMsn].frase a frase lida do standard input*/
  msn_por_user[mensagem[nMsn].user]++;						/*contador do numero de mensagens escritas por cada user*/
  if (msn_por_user[mensagem[nMsn].user]>max_msn)
   	max_msn=msn_por_user[mensagem[nMsn].user];				/*max_msn guarda o numero maximo de mensagens escrito por um so user*/
  if (mensagem[nMsn].len > max_len)
   	max_len=mensagem[nMsn].len;								/*max_len guarda o comprimento maximo de uma mensagem*/
  nMsn++;													/*incrementa o contador das mensagens*/
}


void comando_L() {
  /* Funcao que lista todas as mensagens do forum pela ordem em que foram introduzidas no sistema.
  Output --> *TOTAL MESSAGES:<total_mensagens>
  			  <user>:<frase>*/
  int i=0;
  printf("*TOTAL MESSAGES:%d\n", nMsn);
  for (i=0;i<nMsn;i++)										/*ciclo que percorre todas as mensagens e lista a frase e o user guardados em cada mensagem*/
    printf("%d:%s\n", mensagem[i].user, mensagem[i].frase);
}


void comando_U() {
  /* Funcao que listas todas as mensagens de um dado user do forum pela ordem em que foram introduzidas no sistema, comecando por ler o user pretendido.
  Output --> *MESSAGES FROM USER:<user>
  			  <frase>*/
  int i=0, user;
  scanf("%d", &user);
  printf("*MESSAGES FROM USER:%d\n", user);
  for (i=0;i<nMsn;i++)
    if (mensagem[i].user == user)				/*percorre o vetor de mensagens e imprime as frases cujo o user corresponde ao procurado*/
      printf("%s\n", mensagem[i].frase);
}


void comando_O() {
  /* Funcao que lista a frase mais longa. 
  	 Se existir mais do que uma frase com esse comprimento, devera listar todas as frases com igual comprimento pela ordem que foram introduzidas no sistema.
  Output --> *LONGEST SENTENCE:<user>:<frase>*/
  int j;
  for (j=0;j<nMsn;j++)						/*percorre todas as mensagens*/
    if (max_len == mensagem[j].len)			/*ao encontrar uma frase com o comprimento maximo existente (calculado no comando A) imprime-a*/
      printf("*LONGEST SENTENCE:%d:%s\n", mensagem[j].user, mensagem[j].frase);
}


void comando_T() {
  /* Funcao lista o participante mais ativo.
	 Se existir mais que um participante a ser o mais ativo, lista os seus users por ordem crescente seguido do numero de mensagens inseridas por esse user.
  Output --> *MOST ACTIVE USER:<user>:<N_frases>*/
  int j;
  for (j=0;j<MAX_USERS;j++)					/*percorre todos os utilizadores*/
    if (max_msn == msn_por_user[j])			/*quando o numero de mensagens escritas por um dado utilizador eh igual ao max_msn (calculado no comando A) imprime-o*/
  		printf("*MOST ACTIVE USER:%d:%d\n", j, max_msn);
}


void comando_C() {
  /* Funcao que imprime no ecra o numero de ocorrencias de uma certa palavra em todas as mensagens do forum.
  Output --> *WORD <palavra>:<N_ocorrencias>*/
  int x, i, k, j, count=0, len;
  char word[MAX_FRASE];
  scanf("%s", word);								/*le a palavra que vamos procurar na frase*/
  len=strlen(word);
  for (x=0;x<nMsn;x++) 								/*ciclo que percorre todas mensagens*/
    for (i=0;mensagem[x].frase[i] != '\0';i++) {	/*ciclo que percorre os caracteres da frase de cada mensagem*/
      k=i;j=0;
      while (word[j]!='\0' && mensagem[x].frase[k] != '\0' && mensagem[x].frase[k]== word[j]) {		/*ciclo que compara os caracteres da palavra com os caracteres da frase*/
        k++;
        j++;
      } 
      if (word[j]=='\0' && k-len==0 && espacos_brancos(mensagem[x].frase[k]))		/*se a palavra chegar ao fim e a posicao da frase menos o comprimento for 0 (significando que a palavra esta no inicio da frase)*/
        count ++;																	/*e se a posicao a seguir a palavra na frase for um espaco branco incrementa o contador*/
      else if (word[j]=='\0' && espacos_brancos(mensagem[x].frase[k-len-1]) && espacos_brancos(mensagem[x].frase[k]))
        count++;																	/*se houver um espaco branco antes e depois da palavra na frase incrementa o contador*/
    }
    printf("*WORD %s:%d\n",word,count);
}


void comando_S() {
  /* Funcao lista as mensagens enviadas, ordenadas por ordem alfabetica.
	 Caso existam frases iguais devera ordenar pelo identificador do utilizador, por ordem crescente.
  Output --> *SORTED MESSAGES:<total_mensagens>
  			  <user>:<frase>*/
  int ordem[nMsn], i,j;
  printf("*SORTED MESSAGES:%d\n", nMsn);
  for (i=0;i<nMsn;i++)
    ordem[i]=i;

  for (i=1;i<nMsn;i++) {    /*INSERTION SORT para ordenar as frases por ordem alfabetica*/
    int k=ordem[i];
    j=i-1;
    while (j>=0 && (strcmp(mensagem[k].frase, mensagem[ordem[j]].frase) <0 )) {		/*strcmp eh menor que zero se a primeira string for alfabeticamente inferior a segunda*/
      ordem[j+1] = ordem[j];
      j--;
    }
    ordem[j+1]=k;
  }

  for (i=1;i<nMsn;i++) {    /*INSERTION SORT para ordenar as mensagens pelo identificador do utilizador, por ordem crescente, caso as frases sejam iguais*/
  	int k=ordem[i];
  	j=i-1;
  	while (j>=0 && strcmp(mensagem[k].frase, mensagem[ordem[j]].frase)==0 && mensagem[k].user < mensagem[ordem[j]].user) {		/*strcmp eh igual a zero se a primeira string for alfabeticamente igual a segunda*/
  		ordem[j+1]=ordem[j];
  		j--;
  	}
  	ordem[j+1]=k;
  }

  for (i=0;i<nMsn;i++)
    printf("%d:%s\n", mensagem[ordem[i]].user, mensagem[ordem[i]].frase);
}

void comando_X() {
  /*Funcao que termina o programa retornando o numero total de mensagens
  Output --> <total_mensagens>*/
  printf("%d\n", nMsn);
}


/*---------------------------------------------*/
/*                                             */
/*             PORGRAMA PRINCIPAL              */
/*                                             */
/*---------------------------------------------*/

int main()
{
  char comando;

  while (1) {
    comando=getchar();
    switch (comando) {
      case 'A':
        comando_A(); /*Adiciona uma mensagem*/
        break;
      case 'L':
        comando_L(); /*Lista todas as mensagens*/
        getchar();
        break;
      case 'U':
        comando_U(); /*Lista todas as mensagens introduzidas por um dado utilizador*/
        getchar();
        break;
      case 'O':
        comando_O(); /*Lista as mensagens mais longas*/
        getchar();
        break;
      case 'T':
        comando_T(); /*Escreve o utilizador mais activo na escrita de mensagens*/
        getchar();
        break;
      case 'C':
        comando_C(); /*Escreve o numero de ocorrencias de uma palavra*/
        getchar();
        break;
      case 'S':
        comando_S(); /*Lista todas as mensagens por ordem alfabetica*/
        getchar();
        break;
      case 'X':
        comando_X(); /*Termina o programa*/
        return 0;
      default:
        return -1;
    }
  }
}