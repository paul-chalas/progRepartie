#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <iostream>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>


using namespace std;

void exitErreur(const char * msg) {
	perror(msg);
	exit(EXIT_FAILURE);
}

int main(int argc, char const *argv[]){
	int sock_client = socket(AF_INET,SOCK_STREAM,0);
	struct sockaddr_in sockaddr_serveur;
	sockaddr_serveur.sin_family = AF_INET;
	sockaddr_serveur.sin_port = htons(atoi(argv[2])); //htons(50013);
	//sockaddr_serveur.sin_addr.s_addr = inet_addr("10.203.9.198");
	inet_aton(argv[1],&sockaddr_serveur.sin_addr);

	if(connect(sock_client,(struct sockaddr *) &sockaddr_serveur,sizeof(struct sockaddr_in)) == -1){
		exitErreur("Connection failed");
	}

	char const *requete = "Client connecté\n";
	write(sock_client,requete,strlen(requete));

	char buf [50];
	ssize_t nbOctetsLus = 0;
	string resultat;

	while(true != false){
		if(nbOctetsLus = read(sock_client,buf,sizeof(buf))){
			string bufferLu = string(buf,nbOctetsLus);
			cout << bufferLu << endl;
		}
		const char * cmsg = "Msg\n";

		if (write(sock_client, cmsg, strlen(cmsg)) == -1) exitErreur("Write");
	}

	cout << "client fermé !";
	close(sock_client);
	return 0;

}
