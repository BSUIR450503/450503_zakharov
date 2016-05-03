#include "Server.h"
#include "Client.h"

int main(int argc, char* argv[]) {
	switch (argc) {
	case 1: {
		Server server;
		server.createSharedMemory();
		server.start(argv[0]);
	}break;
	default: {
		Client client;
		client.start();
	}break;
	}
	return 0;
}