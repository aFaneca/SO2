#include "servidor.h"





void init(jogo *j) {
	// Inicialização do jogo
	j->nInvasores = 0;
	j->nDefensores = 0;
	j->nBombas = 0;
	j->nTiros = 0;
	j->nPowerups = 0;
	j->nJogadores = 0;

	// Configuração da nave defensora
	adicionarDefensor(j, 'D', 1, 1, 0, 0, 1);


	// Configuração das naves invasoras
	adicionarInvasor(j, 'B', 1, 1, 1, 0, 1, 5);
	adicionarInvasor(j, 'E', 1, 1, 2, 0, 1, 5);
	adicionarInvasor(j, 'B', 2, 2, 5, 6, 1, 5); //ID: 3

	pushInfoDoJogo();
}


int verificaAlvo(jogo *j, int x, int y, char alvo) {
	// Verificar se o ponto (x,y) se encontra dentro de um retangulo pertencente a uma nave do tipo pretendido ('I' ou 'D')
	int encontrouNave = -1;

	// PROCURA POR INVASORES
	for (int i = 0; i < j->nInvasores; i++) {
		if ((x >= j->invasores[i].posx) && x <= (j->invasores[i].posx + j->invasores[i].largura) &&
			y <= j->invasores[i].posy && y >= (j->invasores[i].posy - j->invasores[i].altura)) {
			encontrouNave = j->invasores[i].id;
			break;
		}
	}

	// PROCURA POR DEFENSORES
	for (int i = 0; i < j->nDefensores; i++) {
		if ((x >= j->defensores[i].posx) && x <= (j->defensores[i].posx + j->defensores[i].largura) &&
			y <= j->defensores[i].posy && y >= (j->defensores[i].posy - j->defensores[i].altura)) {
			encontrouNave = j->invasores[i].id;
			break;
		}
	}

	return encontrouNave; // if == -1 <- Não encontrou nave; else -> == nr. do id da nave encontrada
}

char verificaPosicao(jogo *j, int x, int y) { // Verificar se a posição já se encontra ocupada e se sim, porque tipo de objeto
	char objetoEncontrado = 'N';

	// PROCURA POR INVASORES
	for (int i = 0; i < j->nInvasores; i++) {
		if ((x >= j->invasores[i].posx) && x <= (j->invasores[i].posx + j->invasores[i].largura) &&
			y <= j->invasores[i].posy && y >= (j->invasores[i].posy - j->invasores[i].altura)) {
			objetoEncontrado = j->invasores[i].tipo;
			break;
		}
	}

	// PROCURA POR DEFENSORES
	for (int i = 0; i < j->nDefensores; i++) {
		if ((x >= j->defensores[i].posx) && x <= (j->defensores[i].posx + j->defensores[i].largura) &&
			y <= j->defensores[i].posy && y >= (j->defensores[i].posy - j->defensores[i].altura)) {
			objetoEncontrado = j->defensores[i].tipo;
			break;
		}
	}

	return objetoEncontrado; // 'N' <- Nenhum | 'I' <- Nave Invasora | 'B' ou 'E' <- Nave Defensora
}

void adicionarDefensor(jogo *j, char tipo, int altura, int largura, int posx, int posy, int velocidade) {
	static int i = 0;
	j->defensores[i].id = i;
	j->defensores[i].tipo = tipo;
	j->defensores[i].altura = altura;
	j->defensores[i].largura = largura;
	j->defensores[i].posx = posx;
	j->defensores[i].posy = posy;
	j->defensores[i].velocidade = velocidade;
	j->nDefensores++;
	i++;
}

void adicionarInvasor(jogo *j, char tipo, int altura, int largura, int posx, int posy, int velocidade, int resistencia) {
	static int i = 0;
	j->invasores[i].id = i;
	j->invasores[i].tipo = tipo;
	j->invasores[i].altura = altura;
	j->invasores[i].largura = largura;
	j->invasores[i].posx = posx;
	j->invasores[i].posy = posy;
	j->invasores[i].velocidade = velocidade;
	j->invasores[i].resistencia = resistencia;
	j->nInvasores++;
	

	_tprintf(_T("\n[SERVIDOR] Invasor adicionado com o id %d na posição (%d, %d).\n"), i, j->invasores[i].posx, j->invasores[i].posy);

	i++;
}


void adicionarJogador(jogo *j, TCHAR *nome, TCHAR *pword) {
	static int i;
	_tcscpy_s(j->jogadores[i].nome, nome);
	_tcscpy_s(j->jogadores[i].pword, pword);
	j->jogadores[i].pontuacao = 0;
	j->nJogadores++;
	i++;
}

void removerNave(jogo *j, char tipo, int indice) {

	if (tipo == 'D') {
		int n = j->nDefensores;

		/*for (int c = indice - 1; c < n - 1; c++)
		j->naves[c] = j->naves[c + 1];*/
		for (int c = indice; c < n - 1; c++)
			j->defensores[c] = j->defensores[c + 1];

		j->nDefensores--;
	}
	else {
		int n = j->nInvasores;

		/*for (int c = indice - 1; c < n - 1; c++)
		j->naves[c] = j->naves[c + 1];*/
		for (int c = indice; c < n - 1; c++)
			j->invasores[c] = j->invasores[c + 1];

		j->nInvasores--;
	}

}


int contaNaves(jogo *j, char tipo) {
	int contagem = 0;

	if (tipo == 'D') {
		for (int i = 0; i < j->nDefensores; i++) {
			if (j->defensores[i].tipo == tipo)
				contagem++;
		}
	}
	else {
		for (int i = 0; i < j->nInvasores; i++) {
			if (j->invasores[i].tipo == tipo)
				contagem++;
		}
	}



	return contagem;
}

void mostraInfo(jogo *j) {

	_tprintf(TEXT("\n### JOGO ###\n"));
	_tprintf(TEXT("\nNr. de Naves: %d"), j->nDefensores + j->nInvasores);
	_tprintf(TEXT("\n\t-> Naves Defensoras: %d"), j->nDefensores);

	for (int i = 0; i < j->nDefensores; i++)
		_tprintf(TEXT("\n\t\t=> NAVE %d (%d, %d)"), j->defensores[i].id, j->defensores[i].posx, j->defensores[i].posy);


	_tprintf(TEXT("\n\t-> Naves Invasoras: %d"), j->nInvasores);
	for (int i = 0; i < j->nInvasores; i++)
		_tprintf(TEXT("\n\t\t=> NAVE %d (%d, %d)"), j->invasores[i].id, j->invasores[i].posx, j->invasores[i].posy);

	_tprintf(TEXT("\nNr. de Bombas: %d"), j->nBombas);
	_tprintf(TEXT("\nNr. de Tiros: %d"), j->nTiros);
	_tprintf(TEXT("\nNr. de Powerups: %d\n"), j->nPowerups);
}

void alterarPosicao(jogo *j, char tipo, int id, int novoX, int novoY) { // Alterar posição de uma nave (identificada pelo ID)
																		// NESTA FASE, AS VERIFICAÇÕES DE ACORDO COM A NAVE E COM A NOVA POSIÇÃO A OCUPAR JÁ TÊM QUE TER SIDO FEITAS

	if (tipo == 'D') {
		for (int i = 0; i < j->nDefensores; i++) {
			if (j->defensores[i].id == id) {
				j->defensores[i].posx = novoX;
				j->defensores[i].posy = novoY;
			}
		}
	}
	else {
		for (int i = 0; i < j->nInvasores; i++) {
			if (j->invasores[i].id == id) {
				j->invasores[i].posx = novoX;
				j->invasores[i].posy = novoY;
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

void lerComandos(jogo *j) {
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
					mostraInfo(j);
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

	hTEsquivos = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)GerirEsquivos, (LPVOID)j, 0, &thrEsquivosId);
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

	hTTirosBombasPowerups = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)GerirTirosBombasPowerups, (LPVOID)j, 0, &thrTirosBombasPowerupsId);
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

	hTTempEfeitos = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)GerirTempEfeitos, (LPVOID)j, 0, &thrTempEfeitosId);
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

	hTJogadores = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)GerirJogadores, (LPVOID)j, 0, &thrJogadoresId);
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

int getPontuacao(jogo *j, TCHAR *nomeDoJogador) {
	int pont = -1;
	for (int i = 0; i < j->nJogadores; i++) {
		if (_tcscmp(j->jogadores[i].nome, nomeDoJogador) == 0) {
			pont = j->jogadores[i].pontuacao;
			break;
		}
	}

	return pont;
}

int recuperarPontuacao(jogo *j, TCHAR *nomeDoJogador) {
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

void guardarPontuacao(jogo *j, TCHAR *nomeDoJogador) {
	// GUARDAR NO REGISTRY A PONTUAÇÃO DO JOGADOR, ASSOCIADA AO SEU NOME
	HKEY chave;
	DWORD resultado;

	int pontuacao = getPontuacao(j, nomeDoJogador);

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


DWORD WINAPI GerirBasicos(LPVOID param) {
	HANDLE hMutex;
	//jogo *j = (jogo *)param;
	char tipo = 'B';
	int novaPosX, novaPosY, continuar;

	while (1) {

		// Abre Mutex
		hMutex = OpenMutex(
			MUTEX_ALL_ACCESS,
			FALSE,
			nomeDoMutex); // NAMED MUTEX

		if (!hMutex) {
			_tprintf(TEXT("Erro a abrir o mutex - GerirBásicos()."));
			getwchar();
			exit(EXIT_FAILURE);
		}

		j->invasores[0].posx--;

		ReleaseMutex(hMutex);
	}

	/*
	Deslocam-se horizontalmente. Quando atingem o limite, desce o equivalente à sua altura e desloca-se horizontalmente no sentido oposto.
	Quando atinge o limite inferior, reaparece no topo.
	*/
	//continuar = 1;
	//// DESLOCAÇÃO DAS UNIDADES
	//while (continuar == 1) {
	//	Sleep(INTERVALO_MOVIMENTACAO);

	//	for (int i = 0; i < j->nInvasores; i++) {
	//		if (j->invasores[i].tipo == tipo) {
	//			novaPosX = j->invasores[i].posx + (j->invasores[i].direcaoDoMovimento * VELOCIDADE_BASE_INVASORES);
	//			novaPosY = j->invasores[i].posx + (j->invasores[i].direcaoDoMovimento * VELOCIDADE_BASE_INVASORES);
	//			if (novaPosX >= DIM_X || novaPosX <= 0) { // se tiver atingido o limite no eixo XX
	//													  // DESCE NO EIXO YY = ALTURA DA NAVE
	//				novaPosY -= j->invasores[i].altura;
	//				// INVERTE DIREÇÃO DO MOVIMENTO
	//				j->invasores[i].direcaoDoMovimento *= -1;
	//				novaPosX = j->invasores[i].posx + (j->invasores[i].direcaoDoMovimento * VELOCIDADE_BASE_INVASORES);
	//			}

	//			if (novaPosY >= DIM_Y || novaPosY <= 0) { // se tiver atingido o limite no eixo yy
	//													  // Reaparece no topo
	//				novaPosY = 0;
	//			}

	//			// ESTABELECE A NOVA POSIÇÃO DA NAVE
	//			alterarPosicao(j, tipo, j->invasores[i].id, novaPosY, novaPosX);
	//			/*j->invasores[i].posx = novaPosX;
	//			j->invasores[i].posy = novaPosY;*/
	//		}
	//	}
	//}


	return 0;
}

DWORD WINAPI GerirEsquivos(LPVOID param) {

	jogo *j = (jogo *)param;

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

		for (int i = 0; i < j->nInvasores; i++) {
			if (j->invasores[i].tipo == tipo) {
				valida = 0;
				// Gerar posições aleatórias até que encontre uma posição livre, dentro dos limites
				while (valida != 1) {
					novaPosX = rand() % DIM_X; // Gera um número aleatório entre 0 e DIM_X
					novaPosY = rand() % DIM_Y; //... entre 0 e DIM_Y

											   //Testar validade da nova posição - está ocupada?
					char validacao = verificaPosicao(j, novaPosX, novaPosY);
					if (validacao == 'N') { // a posição está livre
						valida = 1;
					}
					else if (validacao == 'D') {
						//remove ambas as naves
					}
				}
				// ESTABELECE A NOVA POSIÇÃO DA NAVE
				alterarPosicao(j, tipo, j->invasores[i].id, novaPosY, novaPosX);
				/*j->invasores[i].posx = novaPosX;
				j->invasores[i].posy = novaPosY;*/
			}
		}
	}

	return 0;
}

DWORD WINAPI GerirTirosBombasPowerups(LPVOID param) {

	jogo *j = (jogo *)param;

	//_tprintf(TEXT("\nThread de Gestão de Tiros, bombas e powerups em execução."));

	/*while (1 == 1) {
	Sleep(2000);
	_tprintf(TEXT("Naves: %d"), j->nDefensores);
	}*/

	return 0;
}

DWORD WINAPI GerirJogadores(LPVOID param) {

	jogo *j = (jogo *)param;

	//_tprintf(TEXT("\nThread de Gestão de Jogadores em execução."));

	/*while (1 == 1) {
	Sleep(2000);
	_tprintf(TEXT("Naves: %d"), j->nDefensores);
	}*/

	return 0;
}

DWORD WINAPI GerirTempEfeitos(LPVOID param) {

	jogo *j = (jogo *)param;

	//_tprintf(TEXT("\nThread de Gestão de Temporização e Efeitos em execução."));

	/*while (1 == 1) {
	Sleep(2000);
	_tprintf(TEXT("Naves: %d"), j->nDefensores);
	}*/

	return 0;
}


DWORD WINAPI enviarJogo(LPVOID param) {

	while (1) {
		Sleep(1000); //1 segundo
		pushInfoDoJogo();
		_tprintf(_T("\nNAVE: (%d, %d) TOTAL: %d\n"), j->invasores[0].posx, j->invasores[0].posy, j->nInvasores);
		//_gettch();
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

	CopyMemory((PVOID)pBuf, j, (sizeof(jogo)));

	//s_tprintf(_T("\n A estrutura do jogo foi enviada com sucesso."));
	//_gettch();

	//Set do Evento que significa que o servidor tem nova informação para o gateway
	SetEvent(hNovaInfo);
	//Sleep(2000);
	//_gettch();
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


	init(j);

	


	// FUNÇÕES DE TESTE
	adicionarJogador(j, (TCHAR *)TEXT("Joao"), (TCHAR *)TEXT("12345"));
	guardarPontuacao(j, (TCHAR *)TEXT("Joao"));

	/*removerNave(j, 'B', 1);
	mostraInfo(j);
	int testeX = 6;
	int testeY = 4;
	_tprintf(TEXT("A atacar posição (%d, %d)"), testeX, testeY);
	int resultado = verificaAlvo(j, testeX, testeY, 'I');
	if (resultado == -1) {
	_tprintf(TEXT("\nNão encontrou inimigos nessa posição."));
	}else {
	_tprintf(TEXT("\nEncontrou o inimigo %d\n"), resultado);
	}*/

	_tprintf(TEXT("PONTUACAO DO JOAO: %d\n"), recuperarPontuacao(j, (TCHAR *)TEXT("Joao")));
	//
	lerComandos(j);


	_gettch();
	UnmapViewOfFile(pBuf);

	CloseHandle(hMapFile);

	return 0;
}