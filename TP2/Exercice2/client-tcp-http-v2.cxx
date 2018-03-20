#include <arpa/inet.h>
#include <sys/socket.h>
#include <iostream>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>


using namespace std;

void exitErreur(const char * msg) {
	perror(msg);
	exit(EXIT_FAILURE);
}

void testgethostbyname(const char * name){

struct  hostent  * h= gethostbyname(name);

if(h == NULL) {cout << "Erreur gethostbyname " << endl; 
exit(EXIT_FAILURE);}

cout << "Première adresse IP de ce nom de machine : " << endl;

cout << inet_ntoa(*(struct in_addr *)(h->h_addr)) <<endl ;

char ** p;

cout << "Liste de toutes les adresses IP de ce nom de machine : " << endl;

for(p = h->h_addr_list; *p != NULL ; p++) {
cout << inet_ntoa(*(struct in_addr *) (*p)) << endl;
}

cout << "Liste de tous les alias de ce nom de machine : " << endl;

for(p = h->h_aliases; *p != NULL ; p++) {
cout << *p << endl;
}
}

int getadresseIP(const char * name, struct in_addr * adr){

struct  hostent  * h= gethostbyname(name);

if(h == NULL) return 0 ;

* adr = *(struct in_addr *)(h->h_addr) ;
return 1;

}

int main(int argc, char const *argv[]){
	int sock_client = socket(AF_INET,SOCK_STREAM,0);
	struct sockaddr_in sockaddr_serveur;
	sockaddr_serveur.sin_family = AF_INET;
	sockaddr_serveur.sin_port = htons(atoi(argv[2]));

	//inet_aton(argv[1],&sockaddr_serveur.sin_addr);
	getadresseIP(argv[1], &sockaddr_serveur.sin_addr);

	if(connect(sock_client,(struct sockaddr *) &sockaddr_serveur,sizeof(struct sockaddr_in)) == -1){
		exitErreur("Connection failed");
	}
	char const *requete = "GET /index.html\r\n";
	write(sock_client,requete,strlen(requete));

	char buf [1];
	ssize_t nbOctetsLus = 0;
	string resultat;
	while(nbOctetsLus = read(sock_client,buf,sizeof(buf)) > 0){
		resultat += string(buf,nbOctetsLus);
	}
	cout << resultat <<endl;
	close(sock_client);
	return 0;
}
