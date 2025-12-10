/**
 * @file gerer_sf_v4.c
 * @brief Programme principal pour la version 4 du système de fichiers
 * @details Gestion complète du système de fichiers avec répertoire racine
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

#include "bloc.h"
#include "inode.h"
#include "sf.h"
#include "repertoire.h"

void creerFichiersTest(void) {
    printf("Creation des fichiers de test...\n");
    
    // Fichier 1 : petit texte
    FILE* f = fopen("test1.txt", "w");
    if (f) {
        fprintf(f, "Ceci est le contenu du fichier test1.txt\n");
        fclose(f);
        printf("  - test1.txt cree\n");
    }
    
    // Fichier 2 : un peu plus long
    f = fopen("test2.txt", "w");
    if (f) {
        fprintf(f, "Contenu du fichier test2.txt\n");
        fprintf(f, "Deuxieme ligne de texte\n");
        fprintf(f, "Troisieme ligne pour remplir un peu\n");
        fclose(f);
        printf("  - test2.txt cree\n");
    }
    
    // Fichier 3 : fichier vide
    f = fopen("document.pdf", "w");
    if (f) {
        // Fichier vide pour tester
        fclose(f);
        printf("  - document.pdf cree (vide)\n");
    }
    
    // Fichier 4 : avec du contenu binaire simulé
    f = fopen("image.jpg", "wb");
    if (f) {
        // Simuler des données binaires
        unsigned char data[] = {0xFF, 0xD8, 0xFF, 0xE0, 0x00, 0x10, 0x4A, 0x46};
        fwrite(data, sizeof(unsigned char), sizeof(data), f);
        fclose(f);
        printf("  - image.jpg cree (donnees binaires)\n");
    }
    
    // Fichier 5 : code source
    f = fopen("code.c", "w");
    if (f) {
        fprintf(f, "#include <stdio.h>\n\n");
        fprintf(f, "int main() {\n");
        fprintf(f, "    printf(\"Hello, World!\\n\");\n");
        fprintf(f, "    return 0;\n");
        fprintf(f, "}\n");
        fclose(f);
        printf("  - code.c cree\n");
    }
}

void nettoyerFichiersTest(void) {
    remove("test1.txt");
    remove("test2.txt");
    remove("document.pdf");
    remove("image.jpg");
    remove("code.c");
    remove("sauvegarde_v4.sf");
    printf("Fichiers de test nettoyes\n");
}


int main(void) {
     
    // Création des fichiers de test
    creerFichiersTest();
    printf("\n");
    
    // === PHASE 1 : Création du système de fichiers ===
    printf("--- PHASE 1 : Creation du systeme de fichiers ---\n");
    tSF sf = CreerSF("disque_virtuel_v4");
    if (sf == NULL) {
        fprintf(stderr, "ERREUR: Impossible de creer le systeme de fichiers\n");
        nettoyerFichiersTest();
        return 1;
    }
    printf("Systeme de fichiers cree avec succes\n");
    
    // === PHASE 2 : Ajout de fichiers dans le SF ===
    printf("\n--- PHASE 2 : Ajout de fichiers dans le systeme de fichiers ---\n");
    
    const char* fichiers[] = {
        "test1.txt", "test2.txt", "document.pdf", "image.jpg", "code.c"
    };
    const natureFichier types[] = {
        ORDINAIRE, ORDINAIRE, ORDINAIRE, ORDINAIRE, ORDINAIRE
    };
    
    int nbFichiers = sizeof(fichiers) / sizeof(fichiers[0]);
    int fichiersReussis = 0;
    
    for (int i = 0; i < nbFichiers; i++) {
        printf("Ecriture du fichier: %s... ", fichiers[i]);
        long octetsEcrits = EcrireFichierSF(sf, (char*)fichiers[i], types[i]);
        if (octetsEcrits >= 0) {
            printf("OK (%ld octets)\n", octetsEcrits);
            fichiersReussis++;
        } else {
            printf("ECHEC\n");
        }
    }
    
    if (fichiersReussis == 0) {
        printf("ATTENTION: Aucun fichier n'a pu etre ecrit dans le SF\n");
        printf("Cela peut etre normal si la fonction EcrireFichierSF n'est pas implementee\n");
    }
    
    // === PHASE 3 : Affichage du système de fichiers ===
    printf("\n--- PHASE 3 : Affichage du systeme de fichiers ---\n");
    AfficherSF(sf);
    
    // === PHASE 4 : Liste du répertoire racine (sans détails) ===
    printf("\n--- PHASE 4 : Liste du repertoire racine (sans details) ---\n");
    if (Ls(sf, false) != 0) {
        fprintf(stderr, "ERREUR lors du listage du repertoire\n");
    }
    
    // === PHASE 5 : Liste du répertoire racine (avec détails) ===
    printf("\n--- PHASE 5 : Liste du repertoire racine (avec details) ---\n");
    if (Ls(sf, true) != 0) {
        fprintf(stderr, "ERREUR lors du listage detaille du repertoire\n");
    }
    
    // === PHASE 6 : Sauvegarde du système de fichiers ===
    printf("\n--- PHASE 6 : Sauvegarde du systeme de fichiers ---\n");
    if (SauvegarderSF(sf, "sauvegarde_v4.sf") == 0) {
        printf("Sauvegarde reussie dans 'sauvegarde_v4.sf'\n");
    } else {
        fprintf(stderr, "ERREUR lors de la sauvegarde\n");
    }
    
    // === PHASE 7 : Destruction du système de fichiers original ===
    printf("\n--- PHASE 7 : Destruction du systeme de fichiers original ---\n");
    DetruireSF(&sf);
    if (sf == NULL) {
        printf("Systeme de fichiers original detruit avec succes\n");
    } else {
        fprintf(stderr, "ERREUR: Le systeme de fichiers n'a pas ete correctement detruit\n");
    }
    
    // === PHASE 8 : Restauration du système de fichiers ===
    printf("\n--- PHASE 8 : Restauration du systeme de fichiers ---\n");
    tSF sf_restaure = NULL;
    if (ChargerSF(&sf_restaure, "sauvegarde_v4.sf") == 0) {
        printf("Restauration reussie depuis 'sauvegarde_v4.sf'\n");
    } else {
        fprintf(stderr, "ERREUR lors de la restauration\n");
        nettoyerFichiersTest();
        return 1;
    }
    
    // === PHASE 9 : Affichage du système de fichiers restauré ===
    printf("\n--- PHASE 9 : Affichage du systeme de fichiers restaure ---\n");
    AfficherSF(sf_restaure);
    
    // === PHASE 10 : Liste du répertoire racine restauré (sans détails) ===
    printf("\n--- PHASE 10 : Liste du repertoire racine restaure (sans details) ---\n");
    if (Ls(sf_restaure, false) != 0) {
        fprintf(stderr, "ERREUR lors du listage du repertoire restaure\n");
    }
    
    // === PHASE 11 : Liste du répertoire racine restauré (avec détails) ===
    printf("\n--- PHASE 11 : Liste du repertoire racine restaure (avec details) ---\n");
    if (Ls(sf_restaure, true) != 0) {
        fprintf(stderr, "ERREUR lors du listage detaille du repertoire restaure\n");
    }
    
    // === PHASE 12 : Destruction finale ===
    printf("\n--- PHASE 12 : Destruction finale ---\n");
    DetruireSF(&sf_restaure);
    if (sf_restaure == NULL) {
        printf("Systeme de fichiers restaure detruit avec succes\n");
    } else {
        fprintf(stderr, "ERREUR: Le systeme de fichiers restaure n'a pas ete correctement detruit\n");
    }
    
    // Nettoyage
    printf("\n");
    nettoyerFichiersTest();
    
    printf("\n=== FIN TEST NIVEAU 4 - TOUTES LES OPERATIONS TERMINEES ===\n");
    
    return 0;
}