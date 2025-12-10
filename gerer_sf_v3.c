#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sf.h"

int main(void) {
    tSF sf;
    
    printf("** 1. Systeme de fichiers cree\n");
    sf = CreerSF("Mon_Disque_V3");
    AfficherSF(sf);

    // Création de fichiers tests
    printf("\n** 2. Creation des fichiers tests\n");
    
    // Fichier petit (1 bloc)
    FILE *f = fopen("petit.txt", "w");
    if (f != NULL) {
        fprintf(f, "Petit fichier test.");
        fclose(f);
    }
    
    // Fichier moyen (2 blocs)
    f = fopen("moyen.txt", "w");
    if (f != NULL) {
        fprintf(f, "Fichier moyen qui depasse un bloc. ");
        fprintf(f, "Ce texte devrait occuper plus de 64 octets.");
        fprintf(f, " Voila qui fait plus d'un bloc!");
        fclose(f);
    }
    
    // Fichier gros (plusieurs blocs)
    f = fopen("gros.txt", "w");
    if (f != NULL) {
        for (int i = 0; i < 200; i++) {
            fprintf(f, "Ligne %03d: Ceci est un gros fichier de test pour la V3.\n", i);
        }
        fclose(f);
    }

    printf("\n** 3. Ajout fichiers avec EcrireFichierSF (multi-blocs)\n");
    long res1 = EcrireFichierSF(sf, "petit.txt", ORDINAIRE);
    printf("Fichier petit.txt : %ld octets ecrits\n", res1);
    
    long res2 = EcrireFichierSF(sf, "moyen.txt", ORDINAIRE);
    printf("Fichier moyen.txt : %ld octets ecrits\n", res2);
    
    long res3 = EcrireFichierSF(sf, "gros.txt", ORDINAIRE);
    printf("Fichier gros.txt : %ld octets ecrits (tronque si > 640 octets)\n", res3);

    printf("\n** 4. SF avec fichiers multi-blocs\n");
    AfficherSF(sf);

    printf("\n** 5. Test de sauvegarde du SF\n");
    int save_result = SauvegarderSF(sf, "sauvegarde_v3.bin");
    if (save_result == 0) {
        printf("Sauvegarde reussie dans 'sauvegarde_v3.bin'\n");
    } else {
        printf("ERREUR lors de la sauvegarde!\n");
    }

    printf("\n** 6. Test de restauration du SF\n");
    tSF sf_restaure;
    int load_result = ChargerSF(&sf_restaure, "sauvegarde_v3.bin");
    if (load_result == 0) {
        printf("Restauration reussie depuis 'sauvegarde_v3.bin'\n");
        printf("\n** 7. SF restaure\n");
        AfficherSF(sf_restaure);
    } else {
        printf("ERREUR lors de la restauration!\n");
    }

    printf("\n** 8. Test erreur (fichier inexistant)\n");
    long resErr = EcrireFichierSF(sf, "inexistant.txt", ORDINAIRE);
    printf("Resultat erreur : %ld\n", resErr);

    printf("\n** 9. Nettoyage\n");
    DetruireSF(&sf);
    if (load_result == 0) {
        DetruireSF(&sf_restaure);
    }
    
    // Nettoyage des fichiers temporaires
    remove("petit.txt");
    remove("moyen.txt");
    remove("gros.txt");
    remove("sauvegarde_v3.bin");
    
    printf("\n** Tests V3 termines avec succes!\\n");
    
    return 0;
}