// dllmain.cpp : Define o ponto de entrada para da aplicação DLL.

#include <windows.h>
#include <tchar.h>

#define BufferSize 100
#define Buffers 1

TCHAR NomeMemoria[] = TEXT("ComunicacaoServidorGateway");
TCHAR(*PtrMemoria)[Buffers][BufferSize];
TCHAR NomeSemaforoPodeEscrever[] = TEXT("Semáforo Pode Escrever");
TCHAR NomeSemaforoPodeLer[] = TEXT("Semáforo Pode Ler");
HANDLE PodeEscrever;
HANDLE PodeLer;
HANDLE hMemoria;


BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	PodeEscrever = CreateSemaphore(NULL, Buffers, Buffers, NomeSemaforoPodeEscrever);
	PodeLer = CreateSemaphore(NULL, 0, Buffers, NomeSemaforoPodeLer);
	hMemoria = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0,
		sizeof(TCHAR[Buffers][BufferSize]), NomeMemoria);

	if (PodeEscrever == NULL || PodeLer == NULL || hMemoria == NULL) {
		_tprintf(TEXT("[ERRO] Criação de objetos do Windows(%d)\n"), GetLastError());
		return -1;
	}
	PtrMemoria = (TCHAR(*)[Buffers][BufferSize])MapViewOfFile(
		hMemoria, FILE_MAP_WRITE, 0, 0, sizeof(TCHAR[Buffers][BufferSize]));
	if (PtrMemoria == NULL) {
		_tprintf(TEXT("[Erro]Mapeamento da memória partilhada(%d)\n"), GetLastError());
		return -1;
	}


	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}





#define DLL_EXPORTAR __declspec(dllexport)
extern "C" {

	DLL_EXPORTAR void escrever(TCHAR * mensagem) {
		//_tprintf(TEXT("\nA ENVIAR: %s\n"), mensagem);
		int i = 0;
		WaitForSingleObject(PodeEscrever, INFINITE);
		//_tprintf(TEXT("Escrever para buffer\n"));
		_stprintf_s((*PtrMemoria)[0 % Buffers], BufferSize, TEXT("%s"), mensagem);
		//Sleep(1000);
		ReleaseSemaphore(PodeLer, 1, NULL);
	}

	DLL_EXPORTAR TCHAR * recebe() {
		WaitForSingleObject(PodeLer, INFINITE);
		TCHAR * mensagem = (*PtrMemoria)[0 % Buffers];
		//_tprintf(TEXT("Lido: %s\n"), mensagem);
												/*if (_tcscmp(mensagem, TEXT("terminar")) == 0)
												exit(1);*/
		ReleaseSemaphore(PodeEscrever, 1, NULL);

		return mensagem;
	}
}