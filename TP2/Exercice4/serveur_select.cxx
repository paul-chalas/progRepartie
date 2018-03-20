#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <iostream>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#define NUM_PORT 40013
#define BACKLOG 50
#define TAILLE_BUFFER 1024
#define NB_CLIENTS 100

using namespace std;

void exitErreur(const char * msg) {
	perror(msg);
	exit(EXIT_FAILURE);
}

int main(int argc, char * argv[]){
	int port;

	if (argc == 2)
		port = atoi(argv[1]);
	else
		port = NUM_PORT;

	struct sockaddr_in sockaddr_serveur;

	sockaddr_serveur.sin_family = AF_INET;
	sockaddr_serveur.sin_port=htons(port);
	sockaddr_serveur.sin_addr.s_addr = htonl(INADDR_ANY);

	int yes = 1;

	//Créer une socket UDP et la lier à l'adresse Wildcard et au port "port"
	int sockudp = socket(AF_INET, SOCK_DGRAM, 0);

	//inet_aton("10.203.9.209",&sockaddr_serveur.sin_addr);

	if (setsockopt(sockudp, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int))
			== -1)
		exitErreur("setsockopt");

	if (bind(sockudp, (struct sockaddr *) &sockaddr_serveur,
			sizeof(sockaddr_in)) == -1)
		exitErreur("bind1");

	//Créer une socket TCP et la lier à l'adresse Wildcard et au port "port"
	int socktcp = socket(AF_INET, SOCK_STREAM, 0);

	if (setsockopt(socktcp, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int))
			== -1)
		exitErreur("setsockopt");

	if (bind(socktcp, (struct sockaddr *) &sockaddr_serveur,
			sizeof(sockaddr_in)) == -1)
		exitErreur("bind2");


	if (listen(socktcp, BACKLOG) == -1)
		exitErreur("listen");

	sockaddr_in sockaddr_client;
	socklen_t size = sizeof(sockaddr_client);

	char buf[TAILLE_BUFFER];

	char * msg;
	time_t date;

	cout << "Serveur DayTime lancé sur le port " << port << endl;

	fd_set readfds, writefds, exceptfds;

	int max;

	for (int i = 1; i <= NB_CLIENTS; i++) {
		//Mettre à jour les listes
		FD_ZERO (&readfds);
		FD_SET(sockudp, &readfds);
		FD_CLR(socktcp, &readfds);
		FD_ZERO(&writefds);
		FD_ZERO(&exceptfds);

		if(sockudp > socktcp)
			max = sockudp;
		else
			max = socktcp;

		// scruter pour les deux sockets TCP et UDP
		select(max +1, &readfds, NULL, NULL, NULL);

		if (FD_ISSET(sockudp, &readfds)) {
			//on a un mesasge pour la socket UDP, on sort alors du select()
			cout << "client UDP" << endl;
			recvfrom(sockudp, buf, sizeof(buf), 0, (struct sockaddr *) & sockaddr_client, &size);

			date = time(NULL); msg = ctime(&date);
			sendto(sockudp, msg, strlen(msg), 0, (struct sockaddr *) & sockaddr_client, size);
			size = sizeof(sockaddr_client);
		}

		else if (FD_ISSET(socktcp, &readfds)) {
			// On a une demande de connexion pour la socket TCP, on sort du select ()
			cout << "client TCP" << endl;
			int sock_client = accept(socktcp, NULL, NULL);

			date = time(NULL); msg = ctime(&date);
			if (write(sock_client, msg, strlen(msg)) == -1)
				perror("write");
		close (sock_client);
		}

	}

	close (sockudp);
	close (socktcp);
	
	return 0;
}
