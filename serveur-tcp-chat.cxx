#include <iostream>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <iostream>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

using namespace std;

#define NUM_PORT 50013
#define BACKLOG 50
#define NB_CLIENTS 100

void exitErreur(const char * msg) {
	perror(msg);
	exit( EXIT_FAILURE);
}

int main() {

	int sock_serveur = socket(AF_INET, SOCK_STREAM, 0);

	struct sockaddr_in sockaddr_serveur;

	sockaddr_serveur.sin_family = AF_INET;
	sockaddr_serveur.sin_port = htons(NUM_PORT);
	sockaddr_serveur.sin_addr.s_addr = htonl(INADDR_ANY);

	int yes = 1;
	if (setsockopt(sock_serveur, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int))
			== -1)
		exitErreur("setsockopt");

	if (bind(sock_serveur, (struct sockaddr *) &sockaddr_serveur,
			sizeof(sockaddr_in)) == -1)
		exitErreur("bind");

	if (listen(sock_serveur, BACKLOG) == -1)
		exitErreur("listen");

	int sock_client;


	time_t date;

	cout << "Serveur chat lancé  sur le port " << NUM_PORT << endl;

	for (int i = 1; i <= NB_CLIENTS; i++) {

		sock_client = accept(sock_serveur, NULL, NULL);
		if (sock_client == -1)
			exitErreur("accept");
      int lu(0);
      char buffer[250];
      string msgRecu = "";
      while (lu = read(sock_client, buffer, sizeof(buffer))) {
        bool endCom = false;
        string bufferLu = string(buffer,lu);
        for(char c : bufferLu){
          if (c == '\n'){
            endCom = true;
          }
        }
        if(endCom){
          cout << bufferLu;
          string msg;
          getline(cin,msg);
          const char * cmsg = msg.c_str();
          if (write(sock_client, cmsg, strlen(cmsg)) == -1) exitErreur("write");
          msgRecu = "";
        } else {
          msgRecu += bufferLu;
          if (msgRecu == "bye") return 0;
        }
      }
      close(sock_serveur);
	}
  close(sock_serveur);
	return 0;
}
