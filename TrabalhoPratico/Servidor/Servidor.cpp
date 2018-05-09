#include "cabecalho.h"



#define BufferSize 100
#define Buffers 10

#define FILA_SIZE           10
#define FILA_END     (FILA_SIZE - 1)




// Comunicação entre o Servidor e o Gateway


#define BLK_SIZE        100
struct item {                   // os registos na fila
	int    block_num;             // o número de sequencia do bloco
	size_t data_len;        // tamanho dos dados no bloco
	char   data[BLK_SIZE + 1];   // a informação no bloco
};
bool atEOF;          // para saber quando o ficheiro chegou ao fim


struct fila {   
	int  topo;//inicio da fila
	int  fim;  // fim da fila
	long cnt; // nr de registos na fila
	struct item registo[FILA_SIZE];
} f;



//



void init(jogo *j) {
	// Inicialização do jogo
	j->nNaves = 0;
	j->nBombas = 0;
	j->nTiros = 0;
	j->nPowerups = 0;

	// Configuração da nave defensora
	adicionarNave(j, 'D', 1, 1, 0, 0, 1, 0);
	
	// Configuração das naves invasoras
	adicionarNave(j, 'I', 1, 1, 1, 0, 1, 5);
	adicionarNave(j, 'I', 1, 1, 2, 0, 1, 5);
	adicionarNave(j, 'I', 2, 2, 5, 6, 1, 5); //ID: 3
}


int verificaAlvo(jogo *j, int x, int y, char alvo) {
	// Verificar se o ponto (x,y) se encontra dentro de um retangulo pertencente a uma nave do tipo pretendido ('I' ou 'D')
	int encontrouNave = -1;

	for (int i = 0; i < j->nNaves; i++) {
		if ((x >= j->naves[i].posx) && x <= (j->naves[i].posx + j->naves[i].largura) &&
			y <= j->naves[i].posy && y >= (j->naves[i].posy - j->naves[i].altura)) {
			if(j->naves[i].tipo == alvo)
				encontrouNave = j->naves[i].id;
			break;
		}
			
	}
	
	return encontrouNave; // if == -1 <- Não encontrou nave; else -> == nr. do id da nave encontrada
}

char verificaPosicao(jogo *j, int x, int y) { // Verificar se a posição já se encontra ocupada e se sim, porque tipo de objeto
	char objetoEncontrado = 'N'; 

	for (int i = 0; i < j->nNaves; i++) {
		if ((x >= j->naves[i].posx) && x <= (j->naves[i].posx + j->naves[i].largura) &&
			y <= j->naves[i].posy && y >= (j->naves[i].posy - j->naves[i].altura)) {
			objetoEncontrado = j->naves[i].tipo;
			break;
		}
	}

	return objetoEncontrado; // 'N' <- Nenhum | 'I' <- Nave Invasora | 'D' <- Nave Defensora
}

void adicionarNave(jogo *j, char tipo, int altura, int largura, int posx, int posy, int velocidade, int resistencia) {
	static int i = 0;
	j->naves[i].id = i;
	j->naves[i].tipo = tipo;
	j->naves[i].altura = altura;
	j->naves[i].largura = largura;
	j->naves[i].posx = posx;
	j->naves[i].posy = posy;
	j->naves[i].velocidade = velocidade;
	j->naves[i].resistencia = resistencia;
	j->nNaves++;
	i++;
}

void removerNave(jogo *j, int indice) {
	int n = j->nNaves;

	/*for (int c = indice - 1; c < n - 1; c++)
		j->naves[c] = j->naves[c + 1];*/
	for (int c = indice; c < n - 1; c++)
		j->naves[c] = j->naves[c + 1];
	
	j->nNaves--;
}


int contaNaves(jogo *j, char tipo) {
	int contagem = 0;

	for (int i = 0; i < j->nNaves; i++) {
		if (j->naves[i].tipo == tipo)
			contagem++;
	}

	return contagem;
}

void mostraInfo(jogo *j) {

	_tprintf(TEXT("\n### JOGO ###\n"));
	_tprintf(TEXT("\nNr. de Naves: %d"), j->nNaves);
	_tprintf(TEXT("\n\t-> Naves Defensoras: %d"), contaNaves(j, 'D'));

	for (int i = 0; i < j->nNaves; i++) {
		if(j->naves[i].tipo == 'D')
			_tprintf(TEXT("\n\t\t=> NAVE %d (%d, %d)"), j->naves[i].id, j->naves[i].posx, j->naves[i].posy);
	}

	_tprintf(TEXT("\n\t-> Naves Invasoras: %d"), contaNaves(j, 'I'));
	for (int i = 0; i < j->nNaves; i++) {
		if (j->naves[i].tipo == 'I')
			_tprintf(TEXT("\n\t\t=> NAVE %d (%d, %d)"), j->naves[i].id, j->naves[i].posx, j->naves[i].posy);
	}
	_tprintf(TEXT("\nNr. de Bombas: %d"), j->nBombas);
	_tprintf(TEXT("\nNr. de Tiros: %d"), j->nTiros);
	_tprintf(TEXT("\nNr. de Powerups: %d\n"), j->nPowerups);
}

void alterarPosicao(jogo *j, int id, int novoX, int novoY) { // Alterar posição de uma nave (identificada pelo ID)
	// NESTA FASE, AS VERIFICAÇÕES DE ACORDO COM A NAVE E COM A NOVA POSIÇÃO A OCUPAR JÁ TÊM QUE TER SIDO FEITAS
	
	for (int i = 0; i < j->nNaves; i++) {
		if (j->naves[i].id == id) {
			j->naves[i].posx = novoX;
			j->naves[i].posy = novoY;
		}
	}
}


int _tmain()
{
#ifdef UNICODE 
	_setmode(_fileno(stdin), _O_WTEXT);
	_setmode(_fileno(stdout), _O_WTEXT);
#endif

	jogo *j = (jogo *)malloc(sizeof(jogo));
	init(j);
	removerNave(j, 3);
	mostraInfo(j);

	int testeX = 6;
	int testeY = 4;
	_tprintf(TEXT("A atacar posição (%d, %d)"), testeX, testeY);
	int resultado = verificaAlvo(j, testeX, testeY, 'I');

	if (resultado == -1) {
		_tprintf(TEXT("\nNão encontrou inimigos nessa posição."));
	}else {
		_tprintf(TEXT("\nEncontrou o inimigo %d\n"), resultado);
	}


	
	return 0;
}