#include "servidor.h"



void init() {
	// Inicialização do jogo
	j.nInvasores = 0;
	j.nDefensores = 0;
	j.nBombas = 0;
	j.nTiros = 0;
	j.nPowerups = 0;
	j.nJogadores = 0;

	// Configuração da nave defensora
	adicionarDefensor('D', 25, 45, 600, 700, 1);


	// Configuração das naves invasoras
	adicionarInvasor('B', 30, 30, 20, 5, 1, 1);
	adicionarInvasor('B', 30, 30, 100, 5, 1, 1); //ID: 3
	adicionarInvasor('B', 30, 30, 180, 5, 1, 1); //ID: 3
	adicionarInvasor('B', 30, 30, 260, 5, 1, 1); //ID: 3
	adicionarInvasor('B', 30, 30, 340, 5, 1, 1); //ID: 3
	adicionarInvasor('B', 30, 30, 420, 5, 1, 1); //ID: 3
	adicionarInvasor('B', 30, 30, 500, 5, 1, 1); //ID: 3
	adicionarInvasor('B', 30, 30, 580, 5, 1, 1); //ID: 3
	adicionarInvasor('B', 30, 30, 660, 5, 1, 1); //ID: 3
	adicionarInvasor('B', 30, 30, 740, 5, 1, 1); //ID: 3
	adicionarInvasor('B', 30, 30, 820, 5, 1, 1); //ID: 3

	adicionarInvasor('E', 30, 350, 90, 5, 1, 3);
	adicionarInvasor('E', 30, 80, 200, 5, 1, 3);
	adicionarInvasor('E', 30, 100, 400, 5, 1, 3);

	pushInfoDoJogo();
}

int verificaAlvoPeloTipo(int x, int y, int altura, int largura, char tipo) {
	// Verificar se o ponto (x,y) se encontra dentro de um retangulo pertencente a uma nave do tipo pretendido ('I' ou 'D')
	int encontrouNave = -1;


	// PROCURA POR INVASORES
	if (tipo == 'E' || tipo == 'B') {
		for (int i = 0; i < j.nInvasores; i++) {
			if ((x >= j.invasores[i].posx && x <= j.invasores[i].posx + j.invasores[i].largura) || x + largura >= j.invasores[i].posx && x + largura <= j.invasores[i].posx + j.invasores[i].largura) {
				if ((y >= j.invasores[i].posy && y <= j.invasores[i].posy + j.invasores[i].altura) || y + altura >= j.invasores[i].posy && y + altura <= j.invasores[i].posy + j.invasores[i].altura)
					encontrouNave = j.invasores[i].id;
				break;
			}
		}
	}



	// PROCURA POR DEFENSORES
	if (tipo == 'D') {
		for (int i = 0; i < j.nDefensores; i++) {
			if ((x >= j.defensores[i].posx && x <= j.defensores[i].posx + j.defensores[i].largura) || x + largura >= j.defensores[i].posx && x + largura <= j.defensores[i].posx + j.defensores[i].largura) {
				if ((y >= j.defensores[i].posy && y <= j.defensores[i].posy + j.defensores[i].altura) || y + altura >= j.defensores[i].posy && y + altura <= j.defensores[i].posy + j.defensores[i].altura)
					encontrouNave = j.defensores[i].id;
				break;
			}
		}
	}
	return encontrouNave; // if == -1 <- Não encontrou nave; else -> == nr. do id da nave encontrada
}

int verificaAlvo(int x, int y, int altura, int largura) {
	// Verificar se o ponto (x,y) se encontra dentro de um retangulo pertencente a uma nave do tipo pretendido ('I' ou 'D')
	int encontrouNave = -1;


	// PROCURA POR INVASORES
	for (int i = 0; i < j.nInvasores; i++) {
		if ((x >= j.invasores[i].posx && x <= j.invasores[i].posx + j.invasores[i].largura) || x + largura >= j.invasores[i].posx && x + largura <= j.invasores[i].posx + j.invasores[i].largura) {
			if ((y >= j.invasores[i].posy && y <= j.invasores[i].posy + j.invasores[i].altura) || y + altura >= j.invasores[i].posy && y + altura <= j.invasores[i].posy + j.invasores[i].altura)
				encontrouNave = j.invasores[i].id;
			break;
		}
	}


	// PROCURA POR DEFENSORES
	for (int i = 0; i < j.nDefensores; i++) {
		if ((x >= j.defensores[i].posx && x <= j.defensores[i].posx + j.defensores[i].largura) || x + largura >= j.defensores[i].posx && x + largura <= j.defensores[i].posx + j.defensores[i].largura) {
			if ((y >= j.defensores[i].posy && y <= j.defensores[i].posy + j.defensores[i].altura) || y + altura >= j.defensores[i].posy && y + altura <= j.defensores[i].posy + j.defensores[i].altura)
				encontrouNave = j.defensores[i].id;
			break;
		}
	}

	return encontrouNave; // if == -1 <- Não encontrou nave; else -> == nr. do id da nave encontrada
}

char verificaPosicao(int x, int y, int altura, int largura) { // Verificar se a posição já se encontra ocupada e se sim, porque tipo de objeto
	char objetoEncontrado = 'N';

	// PROCURA POR INVASORES
	for (int i = 0; i < j.nInvasores; i++) {
		if ((x >= j.invasores[i].posx && x <= j.invasores[i].posx + j.invasores[i].largura) || x + largura >= j.invasores[i].posx && x + largura <= j.invasores[i].posx + j.invasores[i].largura) {
			if ((y >= j.invasores[i].posy && y <= j.invasores[i].posy + j.invasores[i].altura) || y + altura >= j.invasores[i].posy && y + altura <= j.invasores[i].posy + j.invasores[i].altura)
				objetoEncontrado = j.invasores[i].tipo;
			break;
		}
	}


	// PROCURA POR DEFENSORES
	for (int i = 0; i < j.nDefensores; i++) {
		if ((x >= j.defensores[i].posx && x <= j.defensores[i].posx + j.defensores[i].largura) || x + largura >= j.defensores[i].posx && x + largura <= j.defensores[i].posx + j.defensores[i].largura) {
			if ((y >= j.defensores[i].posy && y <= j.defensores[i].posy + j.defensores[i].altura) || y + altura >= j.defensores[i].posy && y + altura <= j.defensores[i].posy + j.defensores[i].altura)
				objetoEncontrado = j.defensores[i].tipo;
			break;
		}
	}




	return objetoEncontrado; // 'N' <- Nenhum | 'I' <- Nave Invasora | 'B' ou 'E' <- Nave Defensora
}

void adicionarTiro() {
	int i = j.nTiros;
	j.tiros[i].velocidade = VELOCIDADE_PADRAO_TIRO;
	j.tiros[i].altura = ALTURA_PADRAO_TIRO;
	j.tiros[i].largura = LARGURA_PADRAO_TIRO;
	j.tiros[i].posx = j.defensores[0].posx + j.defensores[0].largura / 4;
	j.tiros[i].posy = j.defensores[0].posy - j.defensores[0].altura;
	j.tiros[i].acertou = 0;
	j.nTiros++;
}

void adicionarDefensor(char tipo, int altura, int largura, int posx, int posy, int velocidade) {
	int i = j.nDefensores;
	j.defensores[i].id = i;
	j.defensores[i].tipo = tipo;
	j.defensores[i].altura = altura;
	j.defensores[i].largura = largura;
	j.defensores[i].posx = posx;
	j.defensores[i].posy = posy;
	j.defensores[i].velocidade = velocidade;

	for (int m = 0; m < MAXPOWERUPS; m++)
		j.defensores[i].idDosPowerups[m] = -1;

	j.nDefensores++;
}

void adicionarInvasor(char tipo, int altura, int largura, int posx, int posy, int velocidade, int resistencia) {
	int i = j.nInvasores;
	j.invasores[i].id = i;
	j.invasores[i].tipo = tipo;
	j.invasores[i].altura = altura;
	j.invasores[i].largura = largura;
	j.invasores[i].posx = posx;
	j.invasores[i].posy = posy;
	j.invasores[i].velocidade = velocidade;
	j.invasores[i].resistencia = resistencia;
	j.invasores[i].direcaoDoMovimento = 1;
	j.invasores[i].movimentos = 0;
	j.nInvasores++;


	//_tprintf(_T("\n[SERVIDOR] Invasor adicionado com o id %d na posição (%d, %d).\n"), i, j.invasores[i].posx, j.invasores[i].posy);
}


void adicionarJogador(TCHAR *nome, TCHAR *pword) {
	static int i;
	_tcscpy_s(j.jogadores[i].nome, sizeof(j.jogadores[i].nome), nome);
	_tcscpy_s(j.jogadores[i].pword, sizeof(j.jogadores[i].pword), pword);
	j.jogadores[i].pontuacao = 0;
	j.nJogadores++;
	i++;
}

void removerTiro(int indice) {
	int n = j.nTiros;


	for (int c = indice; c < n - 1; c++) {
		j.tiros[c] = j.tiros[c + 1];
	}


	j.nTiros--;
}

void adicionaPowerupAJogador(int indice) {
	static int i = 0;
	j.defensores[0].idDosPowerups[i] = indice;
	i++;

}

void removerPowerupDoJogador(int indice) {

	for (int i = 0; i < sizeof(j.defensores[0].idDosPowerups) / sizeof(int); i++) {
		if (j.defensores[0].idDosPowerups[i] == indice) {
			j.defensores[0].idDosPowerups[i] = -1;
		}
	}
}


void removerPowerup(int indice) {
	int n = j.nPowerups;


	for (int c = indice; c < n - 1; c++) {
		j.powerups[c] = j.powerups[c + 1];
	}


	j.nPowerups--;
}

void removerNave(char tipo, int indice) {

	if (tipo == 'D') {
		int n = j.nDefensores;

		/*for (int c = indice - 1; c < n - 1; c++)
		j->naves[c] = j->naves[c + 1];*/
		for (int c = indice; c < n - 1; c++)
			j.defensores[c] = j.defensores[c + 1];

		j.nDefensores--;
	}
	else {
		int n = j.nInvasores;

		/*for (int c = indice - 1; c < n - 1; c++)
		j->naves[c] = j->naves[c + 1];*/
		for (int c = indice; c < n - 1; c++)
			j.invasores[c] = j.invasores[c + 1];

		j.nInvasores--;
	}

}


int contaNaves(char tipo) {
	int contagem = 0;

	if (tipo == 'D') {
		for (int i = 0; i < j.nDefensores; i++) {
			if (j.defensores[i].tipo == tipo)
				contagem++;
		}
	}
	else {
		for (int i = 0; i < j.nInvasores; i++) {
			if (j.invasores[i].tipo == tipo)
				contagem++;
		}
	}



	return contagem;
}

void mostraInfo() {

	_tprintf(TEXT("\n### JOGO ###\n"));
	_tprintf(TEXT("\nNr. de Naves: %d"), j.nDefensores + j.nInvasores);
	_tprintf(TEXT("\n\t-> Naves Defensoras: %d"), j.nDefensores);

	for (int i = 0; i < j.nDefensores; i++)
		_tprintf(TEXT("\n\t\t=> NAVE %d (%d, %d)"), j.defensores[i].id, j.defensores[i].posx, j.defensores[i].posy);


	_tprintf(TEXT("\n\t-> Naves Invasoras: %d"), j.nInvasores);
	for (int i = 0; i < j.nInvasores; i++)
		_tprintf(TEXT("\n\t\t=> NAVE %d (%d, %d)"), j.invasores[i].id, j.invasores[i].posx, j.invasores[i].posy);

	_tprintf(TEXT("\nNr. de Bombas: %d"), j.nBombas);
	_tprintf(TEXT("\nNr. de Tiros: %d"), j.nTiros);
	_tprintf(TEXT("\nNr. de Powerups: %d\n"), j.nPowerups);
}

void alterarPosicao(char tipo, int id, int novoX, int novoY) { // Alterar posição de uma nave (identificada pelo ID)
															   // NESTA FASE, AS VERIFICAÇÕES DE ACORDO COM A NAVE E COM A NOVA POSIÇÃO A OCUPAR JÁ TÊM QUE TER SIDO FEITAS

	if (tipo == 'D') {
		for (int i = 0; i < j.nDefensores; i++) {
			if (j.defensores[i].id == id) {
				j.defensores[i].posx = novoX;
				j.defensores[i].posy = novoY;
			}
		}
	}
	else {
		for (int i = 0; i < j.nInvasores; i++) {
			if (j.invasores[i].id == id) {
				j.invasores[i].posx = novoX;
				j.invasores[i].posy = novoY;
			}
		}
	}

}

TCHAR ** processaComando(TCHAR *comando, int *tamCMD) {

	TCHAR ** cmd = NULL;
	TCHAR **proximo_token = (TCHAR **)malloc(sizeof(TCHAR));


	TCHAR *  p = _tcstok_s(comando, TEXT(" "), proximo_token);
	int n_espacos = 0;

	while (p) { 	/* divide a string em palavras */
		cmd = (TCHAR **)realloc(cmd, sizeof(TCHAR*) * ++n_espacos);

		if (cmd == NULL)
			exit(-1); /* se alocação de memória falhar */

		cmd[n_espacos - 1] = p;
		p = _tcstok_s(NULL, TEXT(" "), proximo_token);
	}

	*tamCMD = n_espacos;

	return cmd;
}

void lerComandos() {
	TCHAR **cmd;
	TCHAR *comando = NULL;
	comando = (TCHAR *)malloc(sizeof(TCHAR) * 20);
	TCHAR *comandoCopia = NULL;
	comandoCopia = (TCHAR *)malloc(sizeof(TCHAR) * 20);
	int tamCMD = 0;

	while (0 == 0) {

		_tprintf(_T("\n> "));
		_fgetts(comandoCopia, 25, stdin);
		int i = 0;

		for (i = 0; i < _tcslen(comandoCopia) - 1; i++) {
			comando[i] = comandoCopia[i];
		}

		comando[_tcslen(comandoCopia) - 1] = '\0';

		cmd = processaComando(comando, &tamCMD);

		if (tamCMD != 0) {
			if (_tcscmp(cmd[0], TEXT("info")) == 0) {
				if (tamCMD == 1) {
					mostraInfo();
				}
				else {
					_tprintf(_T("Erro de Sintaxe. <info>\n"));
				}
			}
			if (_tcscmp(cmd[0], TEXT("iniciar")) == 0) {
				if (tamCMD == 1) {
					iniciarJogo();
				}
				else {
					_tprintf(_T("Erro de Sintaxe. <info>\n"));
				}
			}
			else if (_tcscmp(cmd[0], _T("sair")) == 0) {
				break;
			}
			else {

				_tprintf(TEXT("Erro de Sintaxe.\n"));
			}
		}


		comando[0] = '\0';
		comandoCopia[0] = '\0';
	}
}

void iniciarJogo() {


	// CRIAR THREAD PARA LER INSTRUÇÕES RECEBIDAS DO GATEWAY
	hTLerOrdens = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)LerOrdensDoGateway, (LPVOID)NULL, 0, &thrLerOrdensId);
	if (hTLerOrdens == NULL) {
		_tprintf(TEXT("\n[ERRO] Erro a lançar a thread para ler ordens do Gateway - %d"), GetLastError());
		_gettch();
		exit(EXIT_FAILURE);
	}

	// CRIAR THREAD PARA ENVIAR DE X EM X TEMPO A ESTRUTURA DE JOGO PARA O GATEWAY
	hTEnviarJogo = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)enviarJogo, (LPVOID)NULL, 0, &thrEnviarJogo);
	if (hTEnviarJogo == NULL) {
		_tprintf(TEXT("\n[ERRO] Erro a lançar a thread para enviar a estrutura de Jogo ao Gateway - %d"), GetLastError());
		_gettch();
		exit(EXIT_FAILURE);
	}



	// CRIAR THREAD PARA CADA TIPO DE INVASOR
	//-> Básicos

	hTBasicos = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)GerirBasicos, (LPVOID)NULL, 0, &thrBasicosId);
	if (hTBasicos != NULL) {
		//_tprintf(TEXT("Thread de gestão de Invasores Básicos lançada com o id %d\n"), thrBasicosId);
		//Sleep(10000);
	}
	else {
		_tprintf(TEXT("\n[ERRO] Erro a lançar a thread para gerir Invasores Básicos - %d"), GetLastError());
		_gettch();
		exit(EXIT_FAILURE);
	}
	//-> Esquivos

	hTEsquivos = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)GerirEsquivos, (LPVOID)NULL, 0, &thrEsquivosId);
	if (hTEsquivos != NULL) {
		//_tprintf(TEXT("Thread de gestão de Invasores Esquivos lançada com o id %d\n"), thrEsquivosId);
		//Sleep(10000);
	}
	else {
		_tprintf(TEXT("\n[ERRO] Erro a lançar a thread para gerir Invasores Esquivos - %d"), GetLastError());
		_gettch();
		exit(EXIT_FAILURE);
	}
	// CRIAR THREAD PARA GERIR OS TIROS DOS DEFENSORES, AS BOMBAS DOS INVASORES E A QUEDA DOS POWERUPS

	hTTirosBombasPowerups = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)GerirTirosBombasPowerups, (LPVOID)NULL, 0, &thrTirosBombasPowerupsId);
	if (hTTirosBombasPowerups != NULL) {
		//_tprintf(TEXT("Thread de gestão de Tiros, Bombas e Powerups lançada com o id %d\n"), thrTirosBombasPowerupsId);
		//Sleep(10000);
	}
	else {
		_tprintf(TEXT("\n[ERRO] Erro a lançar a thread para gerir Invasores Esquivos - %d"), GetLastError());
		_gettch();
		exit(EXIT_FAILURE);
	}
	// CRIAR THREAD PARA GERIR A TEMPORIZAÇÃO E OS EFEITOS DOS POWERUPS QUE FORAM APANHADOS

	hTTempEfeitos = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)GerirTempEfeitos, (LPVOID)NULL, 0, &thrTempEfeitosId);
	if (hTTempEfeitos != NULL) {
		//_tprintf(TEXT("Thread de gestão de Temporização e Efeitos lançada com o id %d\n"), thrTempEfeitosId);
		//Sleep(10000);
	}
	else {
		_tprintf(TEXT("\n[ERRO] Erro a lançar a thread para gerir Temporização e Efeitos - %d"), GetLastError());
		_gettch();
		exit(EXIT_FAILURE);
	}
	// CRIAR (PELO MENOS UMA) THREAD PARA GERIR OS JOGADORES

	hTJogadores = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)GerirJogadores, (LPVOID)NULL, 0, &thrJogadoresId);
	if (hTJogadores != NULL) {
		//_tprintf(TEXT("Thread de gestão dos jogadores lançada com o id %d\n"), thrJogadoresId);
		//Sleep(10000);

	}
	else {
		_tprintf(TEXT("\n[ERRO] Erro a lançar a thread para gerir Jogadores - %d"), GetLastError());
		_gettch();
		exit(EXIT_FAILURE);
	}

}

int getPontuacao(TCHAR *nomeDoJogador) {
	int pont = -1;
	for (int i = 0; i < j.nJogadores; i++) {
		if (_tcscmp(j.jogadores[i].nome, nomeDoJogador) == 0) {
			pont = j.jogadores[i].pontuacao;
			break;
		}
	}

	return pont;
}

int recuperarPontuacao(TCHAR *nomeDoJogador) {
	// OBTER DO REGISTRY A PONTUAÇÃO DO JOGADOR, ASSOCIADA AO SEU NOME
	HKEY chave;
	DWORD resultado;
	int pontuacao;


	//Criar/abrir uma chave em HKEY_CURRENT_USER\Software\TrabalhoPratico
	if (RegCreateKeyEx(HKEY_CURRENT_USER, TEXT("Software\\TrabalhoPratico"), 0, NULL,
		REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &chave, &resultado) != ERROR_SUCCESS) {
		_tprintf(TEXT("Erro ao criar/abrir chave (%d)\n"), GetLastError());
		return -1;
	}
	else {
		if (resultado == REG_OPENED_EXISTING_KEY) {

			DWORD tamanho = sizeof(tamanho);
			if (RegQueryValueEx(chave, nomeDoJogador, NULL, NULL, (LPBYTE)&pontuacao, &tamanho) != ERROR_SUCCESS) {
				_tprintf(TEXT("Erro a adquirir valor da chave - %d\n"), GetLastError());
				return -1;
			}
		}
	}

	return pontuacao;
}

void guardarPontuacao(TCHAR *nomeDoJogador) {
	// GUARDAR NO REGISTRY A PONTUAÇÃO DO JOGADOR, ASSOCIADA AO SEU NOME
	HKEY chave;
	DWORD resultado;

	int pontuacao = getPontuacao(nomeDoJogador);

	//Criar/abrir uma chave em HKEY_CURRENT_USER\Software\TrabalhoPratico
	if (RegCreateKeyEx(HKEY_CURRENT_USER, TEXT("Software\\TrabalhoPratico"), 0, NULL,
		REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &chave, &resultado) != ERROR_SUCCESS) {
		_tprintf(TEXT("Erro ao criar/abrir chave (%d)\n"), GetLastError());
		return;
	}
	else {
		// APÓS A CHAVE TER SIDO CRIADA/ABERTA, INSERIR/ATUALIZAR A PONTUAÇÃO DO JOGADOR
		if (RegSetValueEx(chave, nomeDoJogador, 0, REG_DWORD, (LPBYTE)&pontuacao,
			sizeof(DWORD)) != ERROR_SUCCESS) {
			_tprintf(TEXT("Erro a adicionar valores ao registo . (%d)\n"), GetLastError());
		}

		RegCloseKey(chave);

	}
}

void processaOrdem(TCHAR * ordem) { // Processa as ordens recebidas do GateWay (enviadas para o Gateway pelo Cliente)
	if (_tcscmp(ordem, _T("direita")) == 0) {
		if (j.defensores[0].posx < DIM_X)
			j.defensores[0].posx = j.defensores[0].posx + 10;
	}
	else if (_tcscmp(ordem, _T("esquerda")) == 0) {
		if (j.defensores[0].posx > 0)
			j.defensores[0].posx = j.defensores[0].posx - 10;
	}
	else if (_tcscmp(ordem, _T("cima")) == 0) {
		if (j.defensores[0].posy >= DIM_Y * 0.8)
			j.defensores[0].posy = j.defensores[0].posy - 10;
	}
	else if (_tcscmp(ordem, _T("baixo")) == 0) {
		if (j.defensores[0].posy < DIM_Y)
			j.defensores[0].posy = j.defensores[0].posy + 10;
	}
	else if (_tcscmp(ordem, _T("bala")) == 0) {
		adicionarTiro();
	}
	else if (_tcscmp(ordem, _T("sair")) == 0) {
		//exit(EXIT_SUCCESS);
	}
}

int nrAleatorio(int min, int max) //range : [min, max)
{
	static bool primeiro = true;
	if (primeiro)
	{
		srand(time(NULL)); //seeding for the first time only!
		primeiro = false;
	}
	return (int)min + rand() % ((max + 1) - min);
}

void lancaBomba(int pX, int pY) {
	int i = j.nBombas;
	j.bombas[i].posx = pX;
	j.bombas[i].posy = pY;
	j.bombas[i].altura = 20;
	j.bombas[i].largura = 5;
	j.bombas[i].velocidade = VELOCIDADE_PADRAO_TIRO;
	j.nBombas++;

}

void lancaPowerup() {
	int i = j.nPowerups;
	j.powerups[i].posx = nrAleatorio(0, DIM_X);
	j.powerups[i].posy = 0;
	j.powerups[i].acertou = 0;
	j.powerups[i].altura = 20;
	j.powerups[i].largura = 20;

	switch (nrAleatorio(1, 6)) {
	case 1: _tcscpy_s(j.powerups[i].nome, sizeof(j.powerups[i].nome), _T("Escudo"));
		break;
	case 2: _tcscpy_s(j.powerups[i].nome, sizeof(j.powerups[i].nome), _T("Gelo"));
		break;
	case 3: _tcscpy_s(j.powerups[i].nome, sizeof(j.powerups[i].nome), _T("Bateria"));
		break;
	case 4: _tcscpy_s(j.powerups[i].nome, sizeof(j.powerups[i].nome), _T("Mais"));
		break;
	case 5: _tcscpy_s(j.powerups[i].nome, sizeof(j.powerups[i].nome), _T("Vida"));
		break;
	case 6: _tcscpy_s(j.powerups[i].nome, sizeof(j.powerups[i].nome), _T("Alcool"));
		break;
	}

	j.nPowerups++;
}

DWORD WINAPI LerOrdensDoGateway(LPVOID param) {

	while (1) {
		//Sleep(200);
		processaOrdem(ler());
	}

	return 0;
}

DWORD WINAPI GerirBasicos(LPVOID param) {
	HANDLE hMutex;

	char tipo = 'B';
	int novaPosX = 0, novaPosY = 0, continuar;

	/*
	Deslocam-se horizontalmente. Quando atingem o limite, desce o equivalente à sua altura e desloca-se horizontalmente no sentido oposto.
	Quando atinge o limite inferior, reaparece no topo.
	*/
	continuar = 1;
	// DESLOCAÇÃO DAS UNIDADES
	while (continuar == 1) {
		Sleep(INTERVALO_MOVIMENTACAO);

		for (int i = 0; i < j.nInvasores; i++) {

			if (j.invasores[i].tipo == tipo) {
				if (((j.invasores[i].movimentos++) % TAXA_DISPARO_BASICA) == 0) {
					if (j.invasores[i].movimentos - 1 != 0) {
						// Lança Bomba
						lancaBomba(j.invasores[i].posx, j.invasores[i].posy + j.invasores[i].altura);
					}

				}
				novaPosX = j.invasores[i].posx + (j.invasores[i].direcaoDoMovimento * VELOCIDADE_BASE_INVASORES);
				novaPosY = j.invasores[i].posy;
				if (novaPosX >= DIM_X || novaPosX <= 0) { // se tiver atingido o limite no eixo XX
														  //DESCE NO EIXO YY = ALTURA DA NAVE (+y)
					novaPosY += j.invasores[i].altura;
					//INVERTE DIREÇÃO DO MOVIMENTO
					j.invasores[i].direcaoDoMovimento *= -1;
					novaPosX = j.invasores[i].posx; // a posição no eixo do x mantém-se a mesma
				}

				if (novaPosY >= DIM_Y || novaPosY <= 0) { // se tiver atingido o limite no eixo yy
														  //Reaparece no topo
					novaPosY = 0;
				}

				// ESTABELECE A NOVA POSIÇÃO DA NAVE
				alterarPosicao(tipo, j.invasores[i].id, novaPosX, novaPosY);
				//_tprintf(_T("\nNAVE 2: (%d, %d) TOTAL: %d\n"), j.invasores[0].posx, j.invasores[0].posy, j.nInvasores);
				/*j.invasores[i].posx = novaPosX;
				j.invasores[i].posy = novaPosY;*/


			}
		}
	}


	return 0;
}

DWORD WINAPI GerirEsquivos(LPVOID param) {

	char tipo = 'E';
	int novaPosX, novaPosY, continuar, valida;
	srand(time(NULL)); // seed para a geração dos números aleatórios
					   /*
					   Deslocam-se aleatoriamente. É 10% mais rápida que a nave básica
					   */
	continuar = 1;

	// DESLOCAÇÃO DAS UNIDADES
	while (continuar == 1) {
		Sleep(INTERVALO_MOVIMENTACAO * 1.10); // É 10% mais rápida que a velocidade da nave básica

		for (int i = 0; i < j.nInvasores; i++) {

			if (j.invasores[i].tipo == tipo) {
				if (((j.invasores[i].movimentos++) % TAXA_DISPARO_BASICA) == 0) {
					if (j.invasores[i].movimentos - 1 != 0) {
						// Lança Bomba
						lancaBomba(j.invasores[i].posx, j.invasores[i].posy + j.invasores[i].altura);
					}

				}

				valida = 0;
				// Gerar posições aleatórias até que encontre uma posição livre, dentro dos limites
				while (valida != 1) {
					novaPosX = nrAleatorio(0, DIM_X); // Gera um número aleatório entre 0 e DIM_X
					novaPosY = nrAleatorio(0, DIM_Y); //... entre 0 e DIM_Y

													  //Testar validade da nova posição - está ocupada?
					char validacao = verificaPosicao(novaPosX, novaPosY, j.invasores[i].altura, j.invasores[i].largura);
					if (validacao == 'N') { // a posição está livre
						valida = 1;
					}
					else if (validacao == 'D') {
						//remove ambas as naves
						removerNave('D', verificaAlvoPeloTipo(novaPosX, novaPosY, j.invasores[i].altura, j.invasores[i].largura, 'D'));
						removerNave('E', verificaAlvoPeloTipo(novaPosX, novaPosY, j.invasores[i].altura, j.invasores[i].largura, 'E'));
					}
				}
				// ESTABELECE A NOVA POSIÇÃO DA NAVE
				alterarPosicao(tipo, j.invasores[i].id, novaPosY, novaPosX);
				/*j->invasores[i].posx = novaPosX;
				j->invasores[i].posy = novaPosY;*/
			}
		}
	}

	return 0;
}

DWORD WINAPI GerirTirosBombasPowerups(LPVOID param) {
	char resposta;
	int idDaNave;

	while (1) {
		Sleep(VELOCIDADE_PADRAO_TIRO); // DEFAULT: 1s

									   //TIROS
		for (int i = 0; i < j.nTiros; i++) {
			j.tiros[i].posy -= 10;

			// Verificar se tiro acertou em alguma nave invasora
			resposta = verificaPosicao(j.tiros[i].posx, j.tiros[i].posy, j.tiros[i].altura, j.tiros[i].largura);
			if (resposta == 'B' || resposta == 'E') {
				j.tiros[i].acertou = 1;

				idDaNave = verificaAlvo(j.tiros[i].posx, j.tiros[i].posy, j.tiros[i].altura, j.tiros[i].largura);

				if (j.invasores[idDaNave].resistencia-- < 2)
					removerNave(resposta, idDaNave);

				removerTiro(i);
				i--;
			}
		}

		//BOMBAS
		for (int i = 0; i < j.nBombas; i++) {
			j.bombas[i].posy += 10;

			if (j.bombas[i].posy > DIM_Y) j.bombas[i].acertou = 1;

			// Verificar se bomba acertou em alguma nave defensora
			resposta = verificaPosicao(j.bombas[i].posx, j.bombas[i].posy, j.bombas[i].altura, j.bombas[i].largura);
			if (resposta == 'D') {
				j.bombas[i].acertou = 1;
				removerNave(resposta, verificaAlvo(j.tiros[i].posx, j.tiros[i].posy, j.tiros[i].altura, j.tiros[i].largura));
				// FIM DO JOGO
			}
		}


		// POWERUPS
		for (int i = 0; i < j.nPowerups; i++) {
			j.powerups[i].posy += 10;

			if (j.powerups[i].posy > DIM_Y) j.powerups[i].acertou = 1; // SE CHEGAR AO FUNDO DO DISPLAY

																	   // Verificar se bomba acertou em alguma nave defensora
			resposta = verificaPosicao(j.powerups[i].posx, j.powerups[i].posy, j.powerups[i].altura, j.powerups[i].largura);
			if (resposta == 'D') {
				j.powerups[i].acertou = 1;
				adicionaPowerupAJogador(i);
				removerPowerup(i);
				i--;
			}
		}


		// LANÇAMENTO DE NOVOS POWERUPS
		if (nrAleatorio(0, 100) < PROBABILIDADE_POWERUPS) {
			lancaPowerup();
		}

	}

	return 0;
}

DWORD WINAPI GerirJogadores(LPVOID param) {


	return 0;
}

DWORD WINAPI GerirTempEfeitos(LPVOID param) {


	return 0;
}


DWORD WINAPI enviarJogo(LPVOID param) {

	while (1) {
		Sleep(100); // 1 segundo
		pushInfoDoJogo();
	}

	return 0;
}



int pushInfoDoJogo() {
	HANDLE hMutex;
	HANDLE hNovaInfo;

	// Criar Mutex
	hMutex = CreateMutex(
		NULL,
		FALSE,
		nomeDoMutex); // NAMED MUTEX

	if (!hMutex) {
		_tprintf(TEXT("Erro a criar o mutex."));
		getwchar();
		exit(EXIT_FAILURE);
	}

	// Criar evento
	hNovaInfo = CreateEvent(NULL,  // Atributos de Segurança
		TRUE,  // TRUE=manual, FALSE=auto
		FALSE,  // estado da sinalização
		nomeDoEventoComunicacao   // nome do Eventoo
	);
	if (hNovaInfo == NULL) {
		_tprintf(_T("Erro a criar evento de comunicação com gateway - %d"), GetLastError());
		_gettch();
		exit(EXIT_FAILURE);
	}
	// Criar MMF - Memory Mapped Files
	hMapFile = CreateFileMapping(
		INVALID_HANDLE_VALUE,
		NULL,                    // segurança padrão
		PAGE_READWRITE,          // acesso para escrita e leitura necessários
		0,
		sizeof(jogo),
		nomeDaMemoria);                 // nome do mapeamento

	if (hMapFile == NULL) {
		_tprintf(TEXT("Erro a criar mapeamento do ficheiro (%d).\n"), GetLastError());
		return 1;
	}
	pBuf = (LPTSTR)MapViewOfFile(hMapFile,   // handle
		FILE_MAP_ALL_ACCESS, // permissões
		0,
		0,
		sizeof(jogo));

	if (pBuf == NULL) {
		_tprintf(TEXT("Não foi possível mapear a view do ficheiro (%d).\n"),
			GetLastError());

		CloseHandle(hMapFile);

		return 1;
	}



	// Escreve para a memória partilhada

	CopyMemory((PVOID)pBuf, &j, (sizeof(jogo)));


	//Set do Evento que significa que o servidor tem nova informação para o gateway
	SetEvent(hNovaInfo);

	//Release do Mutex
	ReleaseMutex(hMutex);

}


int _tmain()
{
	hDLL = LoadLibraryEx(TEXT("mensagens.dll"), NULL, 0);
	if (hDLL == NULL) {
		_tprintf(_T("Problema a abrir DLL.\n"));
		return true;
	}

	// MÉTODO PARA LER DADOS DA MEMÓRIA PARTILHADA (A PARTIR DA DLL)
	ler = (void(*)()) GetProcAddress(hDLL, "recebe");
	if (ler == NULL) {
		_tprintf(_T("Problema a encontrar função.\n"));

		return true;
	}

	// MÉTODO PARA ENVIAR DADOS PARA A MEMÓRIA PARTILHADA (A PARTIR DA DLL)
	escrever = (void(*) (TCHAR*)) GetProcAddress(hDLL, "escrever");
	if (escrever == NULL) {
		_tprintf(_T("Problema a encontrar função.\n"));

		return true;
	}


#ifdef UNICODE 
	_setmode(_fileno(stdin), _O_WTEXT);
	_setmode(_fileno(stdout), _O_WTEXT);
#endif


	init();


	// FUNÇÕES DE TESTE
	adicionarJogador((TCHAR *)TEXT("Joao"), (TCHAR *)TEXT("12345"));
	guardarPontuacao((TCHAR *)TEXT("Joao"));

	//_tprintf(TEXT("PONTUACAO DO JOAO: %d\n"), recuperarPontuacao((TCHAR *)TEXT("Joao")));

	lerComandos();


	_gettch();
	UnmapViewOfFile(pBuf);

	CloseHandle(hMapFile);

	return 0;
}