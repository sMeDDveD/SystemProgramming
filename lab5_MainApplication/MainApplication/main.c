#include <Windows.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <tchar.h>

#include "MainMenu.h"
#include "AsciiArtPrinter.h"

#pragma comment (lib, "..\\Plugins\\AsciiArtPrinter")
#pragma comment (lib, "..\\Plugins\\StaticMenu")

#define GET_AUTHOR "GetAuthor"
#define GET_DESCRIPTION "GetDescription"
#define EXECUTE "Execute"

#define NUMBER_OF_PLUGINS 3
#define BUFFER_SIZE 256

typedef struct {
	LPTSTR name;
	BOOLEAN isPreloaded;
	HMODULE handle;
} PluginStruct;

PluginStruct plugins[] = {
	{"AsciiArtPrinter.dll", TRUE, NULL},
	{"ProcessorInfo.dll", FALSE, NULL},
	{"MiniGame.dll", FALSE, NULL},
};

VOID uploadPlugins() {
	for (INT i = 0; i < NUMBER_OF_PLUGINS; i++) {
		PluginStruct* currentPlugin = &plugins[i];

		if (!currentPlugin->isPreloaded) {
			currentPlugin->handle = LoadLibrary(currentPlugin->name);
		}

		if (currentPlugin->isPreloaded || currentPlugin->handle) {
			_tprintf_s(TEXT("Module %s - was loaded!\n"), currentPlugin->name);
		}
		else {
			_tprintf_s(TEXT("Module %s - was not loaded!\n"), currentPlugin->name);
		}
	}
}

VOID unloadPlugins() {
	for (INT i = 0; i < NUMBER_OF_PLUGINS; i++) {
		PluginStruct* currentPlugin = &plugins[i];

		if (!currentPlugin->isPreloaded) {
			if (FreeLibrary(currentPlugin->handle)) {
				_tprintf_s(TEXT("Module %s - was succesfully unloaded!\n"), currentPlugin->name);
			}
			else {
				_tprintf_s(TEXT("Module %s - was not unloaded!\n"), currentPlugin->name);
			}
		}
		else {
			_tprintf_s(TEXT("Module %s - preloaded library!\n"), currentPlugin->name);
		}
	}
}

VOID showPlugins() {
	_tprintf_s(TEXT("Number of plugins: %d\n"), NUMBER_OF_PLUGINS);
	for (INT i = 0; i < NUMBER_OF_PLUGINS; i++) {
		_tprintf_s("#%d - %s\n", i + 1, plugins[i].name);
	}
}

VOID showInformation() {
	PTCHAR authorStr = malloc(BUFFER_SIZE);
	PTCHAR desriptionsStr = malloc(BUFFER_SIZE);
	DWORD dwBytes = 0; 

	for (INT i = 0; i < NUMBER_OF_PLUGINS; i++) {
		PluginStruct* currentPlugin = &plugins[i];

		_tprintf_s(TEXT("Module %s:\n"), currentPlugin->name);
		if (currentPlugin->isPreloaded) {
			GetAuthor(authorStr, BUFFER_SIZE, &dwBytes);
			GetDescription(desriptionsStr, BUFFER_SIZE, &dwBytes);

			_tprintf_s(TEXT("Author - %s\n"), authorStr);
			_tprintf_s(TEXT("Description - %s\n"), desriptionsStr);
		} 
		else {
			if (currentPlugin->handle) {
				FARPROC author = GetProcAddress(plugins[i].handle, GET_AUTHOR);
				FARPROC descripton = GetProcAddress(plugins[i].handle, GET_DESCRIPTION);
				author(authorStr, 100, &dwBytes);
				descripton(desriptionsStr, 100, &dwBytes);

				_tprintf_s(TEXT("Author - %s\n"), authorStr);
				_tprintf_s(TEXT("Description - %s\n"), desriptionsStr);
			}
			else {
				_tprintf_s(TEXT("Module %s - is not loaded!\n"), currentPlugin->name);
			}
		}
		_tprintf_s(TEXT("\n"));
	}

	free(authorStr);
	free(desriptionsStr);
}

VOID executePlugin() {
	_tprintf_s(TEXT("Choose plugin to execute:\n"));
	showPlugins();

	INT chosen;
	scanf_s("%d", &chosen);
	chosen--;
	getc(stdin);

	if (chosen < NUMBER_OF_PLUGINS) {
		PluginStruct* currentPlugin = &plugins[chosen];
		if (currentPlugin->isPreloaded) {
			Execute();
		}
		else {
			if (currentPlugin->handle) {
				FARPROC execute = GetProcAddress(currentPlugin->handle, EXECUTE);
				execute();
			}
			else {
				_tprintf_s(TEXT("Module %s - is not loaded!\n"), currentPlugin->name);
			}
		}
	}
	else {
		_tprintf_s(TEXT("Wrong number"));
	}
	_tprintf_s(TEXT("\n"));
}

INT main(VOID) {
	SetDllDirectory(TEXT("..\\Plugins"));

	BOOL bCycle = TRUE;
	while (bCycle) {
		showMenu();
		TCHAR choice;
		choice = getc(stdin);
		getc(stdin);
		switch (choice) {
		case '0':
			bCycle = FALSE;
			break;
		case '1':
			uploadPlugins();
			break;
		case '2':
			unloadPlugins();
			break;
		case '3':
			showPlugins();
			break;
		case '4':
			showInformation();
			break;
		case '5':
			executePlugin();
			break;
		default:
			_tprintf_s(TEXT("Something went wrong..\n"));
			break;
		}
	}
	return 0;
}