
// Cliente.cpp : define o ponto de entrada para o aplicativo do console.
//
#include "cliente.h"

void enviarParaNamedPipe(int i, TCHAR * string1, HANDLE h) {
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

TCHAR * lerDoNamedPipe(HANDLE h) {
	TCHAR mensagemLida[256];
	ZeroMemory(&ov, sizeof(ov));
	ZeroMemory(&n, sizeof(n));
	ZeroMemory(mensagemLida, sizeof(mensagemLida));
	ResetEvent(IOReady);
	ov.hEvent = IOReady;


	ReadFile(h, mensagemLida, sizeof(mensagemLida), &n, &ov);
	mensagemLida[n / sizeof(TCHAR)] = '\0';

	WaitForSingleObject(IOReady, INFINITE);
	GetOverlappedResult(h, &ov, &n, FALSE);

	return mensagemLida;
}

// LER ESTRUTURA DO JOGO PELO NAMED PIPE
void lerEstruturaDeJogoDoNamedPipe(HANDLE h) {

	TCHAR mensagemLida[256];
	ZeroMemory(&ov, sizeof(ov));
	ZeroMemory(&n, sizeof(n));
	ResetEvent(IOReady);
	ov.hEvent = IOReady;

	
	ReadFile(h, &j, sizeof(j), &n, &ov);
	//mensagemLida[n / sizeof(TCHAR)] = '\0';
	//_tprintf(_T("\nNAVE: (%d, %d) TOTAL: %d\n"), j->defensores[0].posx, j->defensores[0].posy, j->nDefensores);
	WaitForSingleObject(IOReady, INFINITE);
	GetOverlappedResult(h, &ov, &n, FALSE);
}


DWORD WINAPI lerJogo(LPVOID param) { // Está constantemente a ler do gateway a versão mais atualizada da estrutura de jogo
	
	HANDLE hPipe = (HANDLE)param;
	
	while (1) {
		lerEstruturaDeJogoDoNamedPipe(hPipe);

		/*if (!n) {
			_tprintf(TEXT("[LEITOR] %d... (ReadFile)\n"), n);
			break;
		}*/
		_tprintf(_T("\nNAVE: (%d, %d) TOTAL: %d\n"), j.invasores[0].posx, j.invasores[0].posy, j.nInvasores);
	}


	return 0;
}


int main()
{
	HANDLE hPipe, hTLerJogo;
#ifdef UNICODE
	_setmode(_fileno(stdin), _O_WTEXT);
	_setmode(_fileno(stdout), _O_WTEXT);
#endif

	// TEMPORÁRIO
	_tprintf(_T(">> Prima qualquer tecla para iniciar o CLIENTE...\n"));
	_gettch();
	//

	_tprintf(TEXT("[CLIENTE] Esperar pelo pipe '%s' (WaitNamedPipe)\n"), PIPE_NAME);
	if (!WaitNamedPipe(PIPE_NAME, NMPWAIT_WAIT_FOREVER)) {
		_tprintf(TEXT("[ERRO] Ligar ao pipe '%s'! (WaitNamedPipe)\n"), PIPE_NAME);
		exit(-1);
	}

	_tprintf(TEXT("[CLIENTE] Ligação ao pipe do escritor... (CreateFile)\n"));
	hPipe = CreateFile(PIPE_NAME, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0 | FILE_FLAG_OVERLAPPED, NULL);
	if (hPipe == NULL) {
		_tprintf(TEXT("[ERRO] Ligar ao pipe '%s'! (CreateFile)\n"), PIPE_NAME);
		exit(-1);
	}
	_tprintf(TEXT("[CLIENTE] Liguei-me...\n"));

	// Mudar modo de funcionamento do pipe para PIPE_READMODE_MESSAGE
	DWORD mode = PIPE_READMODE_MESSAGE;
	SetNamedPipeHandleState(hPipe, &mode, NULL, NULL);

	IOReady = CreateEvent(NULL, TRUE, FALSE, NULL);

	hTLerJogo = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)lerJogo, (LPVOID)hPipe, 0, NULL);
	if (hTLerJogo == NULL) {
		_tprintf(TEXT("[ERR0] Lançamento da Thread de leitura de jogo. %d\n"));
		exit(EXIT_FAILURE);
	}


	
	Sleep(200000);
	CloseHandle(hPipe);
	CloseHandle(hTLerJogo);
	


	return 0;
}
