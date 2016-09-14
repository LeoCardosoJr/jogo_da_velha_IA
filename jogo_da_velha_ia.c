#include <stdio.h> // printf e etc
#include <stdlib.h> // Malloc
#include <locale.h> // Correção dos caracteres
#include <string.h> // memcpy
#include <stdbool.h> // Utilização do tipo de dados bool -> false = 0 e true = 1
#include <conio.h> // getch

int joga_PC(char *campo);
void formar_campo(char *campo); // Função forma o campo
void fazer_jogada(char *campo, int jogada); // Atribui jogada do jogador ao campo
int ver_win(char *campo); // Função verifica o ganhador
int* melhor_jogada(char campo[], bool jogando);
int AI_ver_win(char *campo);

int velha=1, jogador = 1; // Variaveis globais (não se limitam aos escopo da função

int main(void){
	setlocale(LC_ALL, "Portuguese"); // Arrumando a pontuação

	int novo, contra; // Declaração da variavel do menu no próximo jogo

	do{
		int menu = 0;
		do{
			printf("Escolha uma das opções abaixo:\n");
			printf("1 - Jogar contra o computador\n");
			printf("2 - Jogar com um amigo\n");
			menu = (int) getch();
			menu -= 48;
		}while(menu != 1 && menu != 2);



		int i = 1, inv = 0, win = 0, opcao = 1;
		char campo[9], jogada;

		for(i = 1; i <= 9; i++){ // Looping para setar o campo
			campo[i] = i + 48; // numerais na tabela asc, devido ao problema de passar um inteiro para dentro de um ponteiro de char
		}

		do{
		    if(jogador == 1 && menu == 1){
			   joga_PC(campo);
			   win = ver_win(campo);
			}
			if(velha <= 9){ // Ultima jogada
			do{
				if(win != 1){
                    system("cls || clear"); // Limpa tela
					if ((jogada > 0 ) && ( jogada < 10)){
						if(inv == 1){ // Caso a jogada tenha sido inválida, dá mensagem
							printf("JOGADA INVÁLIDA\n\n");
						}
					}
					printf("É a vez do jogador %d.\nEscolha um campo, ainda não escolhido\n\n", jogador);

					formar_campo(campo);

					setbuf(stdin, NULL); // Limpa a memória do teclado
					// Pega um caracter digitado no teclado sem a necessidade de pressionar 'enter' e tem um cast para transformar esse caracter em inteiro
					// O número pego nele é o número correspodente a tabela ASC

					inv = 0;

					jogada = (int) getch();

					jogada -= 48; // Diminuo 48, pois o 48 é zero na tabela ASC, assim tenho o valor do número dele

					if(((jogada < 1 ) || ( jogada > 10)) || (campo[jogada]!=jogada+48)){ // Se a jogada for inválida ou então caso o campo já tenha sido preenchido
			  		    inv = 1; // Seta a variavel que repete a operação caso a jogada seja inválida
					}
				}
			}while(inv == 1); // Enquanto jogada inválida da mensagem de jogada inválida e manda o jogador jogar novamente
			}

             fazer_jogada(campo, jogada); // Faz a jogada do usuário

			 win = ver_win(campo); // Verifica se tem algum ganhador


		}while((win != 1) && (velha <= 9)); // Enquanto não houver ganhador ou empate o jogo continua

		system("cls || clear"); // Limpa tela

		if(win == 1){ // Verifica se existe um ganhador

			if(jogador == 1){ // se for a vez do jogador 1 o ganhador vai ser o jogador 2
				jogador = 2;
			}else if(jogador == 2){
			 	jogador = 1;
			}

			printf("O JOGADOR %d É O VENCEDOR!\n\n", jogador);

		}else if(velha >= 9){ // Caso não tenha nenhum vencedor, deu empate

			printf("O JOGO FOI EMPATADO!\n\n");

		}
		formar_campo(campo); // Atualiza o campo (só perfumaria)


		do{	 // Abre o menu

			printf("Escolha uma das opções abaixo:\n");
			printf("1 - Novo jogo\n");
			printf("2 - Sair\n");

			opcao = (int) getch();
			opcao -= 48;

			if(opcao != 1 && opcao != 2){
				system("cls || clear"); // Limpa tela
				printf("OPÇÃO INVÁLIDA\n\n");
			}else if(opcao == 2){
				novo++;
			}else if(opcao == 1){
	            velha = 1;
				jogador = 1;
				novo = 0;
			}

		}while(opcao != 1 && opcao != 2); // opções inválida?

	}while(novo == 0); // Novo jogo?

	return 0;
}


int ver_win(char *campo){

	int i,a;

	for(a = 1; a <= 7; a += 3){ // verifica horizontal
		if((campo[a] == campo[a + 1]) && (campo[a + 1] == campo[a + 2])){
			return 1;
		}
	}


	for(a = 1; a <= 3; a++){ // verifica vertical
		if((campo[a] == campo[a + 3]) && (campo[a + 3] == campo[a + 6])){
			return 1;
		}
	}

	for(a = 2, i = 3; a <= 4 && i > 0; a += 2, i -= 2){ // verifica diagonal
		if((campo[i] == campo[i + a]) && (campo[i + a] == campo[i + a + a])){
			return 1;
		}
	}

	return 0;

}


void formar_campo(char *campo){
	printf("_%c_|_%c_|_%c_\n", campo[1], campo[2],campo[3]);
	printf("_%c_|_%c_|_%c_\n", campo[4], campo[5],campo[6]);
	printf(" %c | %c | %c \n\n", campo[7], campo[8],campo[9]);
}


void fazer_jogada(char *campo, int jogada){
    if(jogador == 1){
        campo[jogada] = 'x'; // aspas ' faz um char, aspas " faz uma string
        jogador += 1;
  		velha++;
    }else if(jogador == 2){
        campo[jogada] = 'o';
        jogador = jogador - 1;
  		velha++;
    }

}


// AI --- Código da Inteligencia artificial abaixo
int joga_PC(char *campo){
	int *a, i, x;

	for(i=1;i<10;i++) {
		if(campo[i]==i+48) {
		   campo[i]='x';
		   x=AI_ver_win(campo); // Função que verifica se ganha com essa jogada
		   if(x==1) { // Se pode ganhar, então faz a jogada
		       fazer_jogada(campo, i);
		       return 0;
		   }
		   campo[i]=i + 48; // Se não pode jogar, retira a jogada que simulou
		}
	}


	a=melhor_jogada(campo , true); // Verifica qual a melhor jogada possível
    fazer_jogada(campo, a[0]);
}


int AI_ver_win(char *campo){

	int i,a;

	for(a = 1; a <= 7; a += 3){ // verifica horizontal
		if((campo[a] == campo[a + 1]) && (campo[a + 1] == campo[a + 2])){
			return campo[a] == 'x'? 2 : -2;
		}
	}


	for(a = 1; a <= 3; a++){ // verifica vertical
		if((campo[a] == campo[a + 3]) && (campo[a + 3] == campo[a + 6])){
			return campo[a] == 'x'? 2 : -2;
		}
	}

	// _1_|_2_|_3_
    // _4_|_5_|_6_
	//  7 | 8 | 9

	for(a = 2, i = 3; a <= 4 && i > 0; a += 2, i -= 2){ // verifica diagonal
		if((campo[i] == campo[i + a]) && (campo[i + a] == campo[i + a + a])){
			return campo[i] == 'x'? 2 : -2;
		}
	}

	if(velha > 10){
		return 0;
	}


	return -1;

}


// O algoritimo é minmax, ele verifica todas as possibilidades de jogadas e escolhe a melhor,
// atribuindo uma pontuação mais alta para aonde ele pode ganhar e mais baixa para aonde ele pode perder
int* melhor_jogada(char campo[], bool jogando) {

    int *a;
    a = (int *) malloc(2 * sizeof(int)); // sizeof da o tamanho do parâmetro, no caso é o tamanho do tipo de dados int
    int x=AI_ver_win(campo);
    int i,lol;

    if(x!=-1) { // se não continua (chegou ao final da árvore e resultou em empate ou na vitória de algum dos participantes)
        a[0]=-1;
        a[1]=x; // retorna a pontuação para determinado resultado do final da árvore
        return a;
    }

    if(jogando==true) { // Se o jogador for o computador
        int max=-20;    // máximo de pontos para o computador

        for(i=1;i<10;i++) { // varre todas as possibilidades horizontalmente
            if(campo[i]==i + 48) { // Se o campo não foi preenchido ainda
                campo[i]='x'; // simula a jogada
                velha++; // adiciona +1 a variavel velha, para que caso de empate na árvore o programa saiba quando deu empate e pare a árvore
                a=melhor_jogada(campo , !jogando); // mais profundidade na árvore
                if(max < a[1]) {
                    max=a[1];
                    lol=i;
                }
                campo[i]= i + 48;
                velha--;
            }
        }
        a[0]=lol;
        a[1]=max;
        return a;
    }else { /// se não é o adversário

        int min=+20;   // Minimo de pontos do adversário

        for(i=1;i<10;i++) {
			if(campo[i]==i+48) {
			campo[i]='o';
			velha++;
			a=melhor_jogada(campo , !jogando);
			if(min > a[1]) {
	            min=a[1];
	            lol=i;
	        }

	        campo[i]=i+48;
	        velha--;
	        }
        }
        a[0]=lol;
        a[1]=min;
        return a;
    }
    free(a);
}
