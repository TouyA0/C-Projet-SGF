#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

#include "bloc.h"
#include "inode.h"
#include "sf.h"
#include "repertoire.h"

int main(void) {
    tSF sf;
    
    printf("** Systeme de fichiers cree\n");
    sf = CreerSF("Mon_Disque");
    AfficherSF(sf);

    FILE *f = fopen("test1.txt", "w");
    if (f != NULL) {
        fprintf(f, "Contenu du premier fichier test pour le SF.");
        fclose(f);
    }
    
    f = fopen("test2.txt", "w");
    if (f != NULL) {
        fprintf(f, "Second fichier.");
        fclose(f);
    }

    printf("\n** Ajout fichiers au SF\n");
    long res1 = Ecrire1BlocFichierSF(sf, "test1.txt", ORDINAIRE);
    printf("Fichier 1 : %ld octets ecrits\n", res1);
    
    long res2 = Ecrire1BlocFichierSF(sf, "test2.txt", ORDINAIRE);
    printf("Fichier 2 : %ld octets ecrits\n", res2);

    printf("\n** SF avec fichiers\n");
    AfficherSF(sf);

    printf("\n** Test erreur (fichier inexistant)\n");
    long resErr = Ecrire1BlocFichierSF(sf, "inexistant.txt", ORDINAIRE);
    printf("Resultat erreur : %ld\n", resErr);

    DetruireSF(&sf);
    printf("\n** SF detruit : tests V2 OK!\n");

    remove("test1.txt");
    remove("test2.txt");
    
    return 0;
}