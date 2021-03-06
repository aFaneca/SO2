// Gateway.cpp : define o ponto de entrada para o aplicativo do console.
//

#include "gateway.h"

void enviarPorNamedPipeParaClientes(TCHAR * string1, HANDLE h) {
	ZeroMemory(&ov, sizeof(ov));
	ResetEvent(IOReady);
	ov.hEvent = IOReady;

	_tprintf(TEXT("Escrever no pipe %d"), h);
	if (!WriteFile(h, string1, TAM * sizeof(TCHAR), &n, &ov)) {
		_tprintf(TEXT("[ERRO] Escrever no pipe %d! (WriteFile)\n"));
		exit(-1);
	}

	GetOverlappedResult(h, &ov, &n, FALSE);

}

// ENVIAR A ESTRUTURA DO JOGO PELO NAMED PIPE
void enviarPorNamedPipeParaClientes(HANDLE h) {
	ZeroMemory(&ov, sizeof(ov));
	ResetEvent(IOReady);
	ov.hEvent = IOReady;
	jogo copia = *j;
	//_tprintf(TEXT("\n[GATEWAY] Informação do jogo enviada com sucesso para o cliente %d.\n"), h);
	if (!WriteFile(h, (LPVOID)&copia, sizeof(copia), &n, &ov)) {
		_tprintf(TEXT("[ERRO] Escrever no pipe %d! (WriteFile)\n"));
		exit(-1);
	}

	GetOverlappedResult(h, &ov, &n, FALSE);

}


TCHAR * lerDoNamedPipe(HANDLE h) {
	TCHAR mensagemLida[256];
	ZeroMemory(&ov, sizeof(ov));

	ResetEvent(IOReady);
	ov.hEvent = IOReady;


	ReadFile(h, mensagemLida, sizeof(mensagemLida), &n, &ov);
	//mensagemLida[_tcslen(mensagemLida) - 1] = '\0';
	mensagemLida[n / sizeof(TCHAR)] = '\0';
	WaitForSingleObject(IOReady, INFINITE);
	GetOverlappedResult(h, &ov, &n, FALSE);

	return mensagemLida;
}



int pullInfoDoJogo() {
	HANDLE hMutex, hNovaInfo;
	LPCTSTR pBuf;
	// Abre Mutex
	hMutex = OpenMutex(
		MUTEX_ALL_ACCESS,
		FALSE,
		nomeDoMutex); // NAMED MUTEX

	if (!hMutex) {
		_tprintf(TEXT("Erro a abrir o mutex."));
		getwchar();
		exit(EXIT_FAILURE);
	}

	// Abre Evento
	hNovaInfo = OpenEvent(EVENT_ALL_ACCESS,  // Atributos de Segurança
		TRUE,  // TRUE=manual, FALSE=auto
		nomeDoEventoComunicacao   // nome do Evento
	);
	if (hNovaInfo == NULL) {
		_tprintf(_T("Erro a abrir evento de comunicação com servidor - %d"), GetLastError());
		_gettch();
		exit(EXIT_FAILURE);
	}
	// Espera pela sinalização do evento
	DWORD estado = WaitForSingleObject(
		hNovaInfo,   // handle
		INFINITE // timeout
	);

	if (estado == WAIT_OBJECT_0)
	{
		// evento foi sinalizado
		hMapFile = OpenFileMapping(
			FILE_MAP_ALL_ACCESS,    // acesso de escrita/leitura
			FALSE,                    // não herdar o nome
			nomeDaMemoria);                 // nome do objeto de mapeamento

		if (hMapFile == NULL) {
			_tprintf(TEXT("Erro a abrir mapeamento do ficheiro (%d).\n"), GetLastError());
			return 1;
		}
		j = (jogo *)MapViewOfFile(hMapFile,   // handle
			FILE_MAP_READ, // só dar permissões de leitura, de acordo com o enunciado
			0,
			0,
			sizeof(jogo));

		if (j == NULL)
		{
			_tprintf(TEXT("Não foi possível mapear a view do ficheiro (%d).\n"),
				GetLastError());

			CloseHandle(hMapFile);

			return 1;
		}

		//_tprintf(_T("Estrutura atualizada do jogo recebida com sucesso.\n"));


		ResetEvent(hNovaInfo); // Volta a dessinalizar o evento
		ReleaseMutex(hMutex);

		
	}

	return 0;
}

int terminar = 0;
HANDLE hT, hT2;


DWORD WINAPI atendeCliente(LPVOID param) {


	HANDLE hPipe = (HANDLE)param;

	OVERLAPPED ov;
	HANDLE IOReady;

	IOReady = CreateEvent(NULL, TRUE, FALSE, NULL);
	do {
		
		_tcscpy_s(buf, lerDoNamedPipe(hPipe));
		if (!n) {
			_tprintf(TEXT("[LEITOR] %d... (ReadFile)\n"), n);
			break;
		}
		
		escrever(buf); // Envia a ordem para o servidor
		//_tprintf(_T("\n[GATEWAY] Recebi pedido de cliente de %d bytes: %s\n"), n, buf);

	} while (1); // (n > 0) -> Enquanto houverem dados para ler

	return 0;
}


DWORD WINAPI aceitaClientes(LPVOID param) {

	int desligado = 0;
	while (!terminar) {
		_tprintf(TEXT("[GATEWAY] Criar uma cópia do pipe '%s' ... (CreateNamedPipe)\n"), PIPE_NAME);
		hPipe = CreateNamedPipe(PIPE_NAME, PIPE_ACCESS_DUPLEX | FILE_FLAG_OVERLAPPED, PIPE_WAIT | PIPE_TYPE_MESSAGE
			| PIPE_READMODE_MESSAGE, PIPE_UNLIMITED_INSTANCES, sizeof(buf), sizeof(buf), 1000, NULL);
		if (hPipe == INVALID_HANDLE_VALUE) {
			_tprintf(TEXT("[ERRO] Criar Named Pipe! (CreateNamedPipe)"));
			exit(-1);
		}


		_tprintf(TEXT("[GATEWAY] Esperar ligação de um leitor... (ConnectNamedPipe)\n"));
		if (!ConnectNamedPipe(hPipe, NULL)) {
			_tprintf(TEXT("[ERRO] Ligação ao leitor! (ConnectNamedPipe\n"));
			CloseHandle(hPipe);
			exit(-1);
		}
		//Guardar Handles
		int i;
		for (i = 0; i < NCLIENTES; i++) {
			if (hClientes[i] == INVALID_HANDLE_VALUE) { // Se estiver disponível

				desligado = 1;

				break;
			}

		}
		if (i < NCLIENTES)
			hClientes[i] = hPipe;
		// Senao, disconnectNamedPepe()
		if (desligado == 0) {
			DisconnectNamedPipe(hPipe);
			CloseHandle(hPipe);
			// Voltar  a cima
		}


		// Criar uma Thread que lê pedidos do cliente - AtendeCliente(hPipe)
		hT2 = CreateThread(NULL, 0, atendeCliente, (LPVOID)hPipe, 0, NULL);
		if (!hT2) {
			_tprintf(_T("Erro a criar thread hT2."));
			exit(-1);
		}
	}

	DisconnectNamedPipe(hPipe);
	//CloseHandle(hT2);
	CloseHandle(hT);
	return 0;

}



int _tmain()
{

#ifdef UNICODE
	_setmode(_fileno(stdin), _O_WTEXT);
	_setmode(_fileno(stdout), _O_WTEXT);
#endif

	// TEMPORÁRIO
	_tprintf(_T(">> Prima qualquer tecla para iniciar o Gateway...\n"));
	_gettch();
	//

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

	
	for (int i = 0; i < NCLIENTES; i++) { // Inicialmente são todos inválidos, porque nenhum está a ser utilizado
		hClientes[i] = INVALID_HANDLE_VALUE;
	}

	// Lançar Thread "aceitaClientes()"
	hT = CreateThread(NULL, 0, aceitaClientes, NULL, 0, NULL);
	if (hT != NULL) {
		//_tprintf(TEXT("Thread de gestão de Invasores Básicos lançada com o id %d\n"), thrBasicosId);
		//Sleep(10000);
	}

	IOReady = CreateEvent(NULL, TRUE, FALSE, NULL);

	do {

		pullInfoDoJogo();
		
		//_tprintf(TEXT("[ESCRITOR] Frase: "));
		//_fgetts(buf, 256, stdin);
		//buf[_tcslen(buf) - 1] = '\0';
		//buf[n / sizeof(TCHAR)] = '\0';
		

		//Guardar Handles
		int i;
		for (i = 0; i < NCLIENTES; i++) {
			if (hClientes[i] != INVALID_HANDLE_VALUE) {
				HANDLE h = hClientes[i];

				enviarPorNamedPipeParaClientes(h);
				WaitForSingleObject(IOReady, INFINITE);
	
				//_tprintf(TEXT("[GATEWAY] Estrutura do Jogo (%d bytes) enviada com sucesso para o cliente '%d'.\n"), n, i);
			}
		}

	} while (_tcscmp(buf, TEXT("fim")));

	DisconnectNamedPipe(hPipe);
	CloseHandle(hPipe);
	// informar a thread para terminar
	terminar = 1;

	HANDLE hPipe = CreateFile(PIPE_NAME, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, NULL); // Para terminar a thread

																															//esperar pelo fim da thread 
	WaitForSingleObject(hT, INFINITE);

	_tprintf(TEXT("[ESCRITOR] Desligar o pipe (DisconnectNamedPipe)\n"));
	/*if (!DisconnectNamedPipe(hPipe)) {
	_tprintf(TEXT("[ERRO] Desligar o pipe! (DisconnectNamedPipe)"));
	exit(-1);
	}*/


	//Sleep(2000);
	//CloseHandle(hPipe);



	int i;
	for (i = 0; i < 5; i++) {
		DisconnectNamedPipe(hClientes[i]);
		CloseHandle(hClientes[i]);
	}
	exit(0);

	_gettch();

	UnmapViewOfFile(j);

	CloseHandle(hMapFile);

	return 0;
}
