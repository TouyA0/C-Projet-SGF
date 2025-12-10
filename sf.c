/**
 * ProgC - Projet Automne 25-26 : Gestion de systèmes de fichiers
 * VERSION 4
 * Fichier : sf.c
 * Module de gestion d'un systèmes de fichiers (simulé)
 **/

#include "sf.h"
#include "bloc.h"
#include "repertoire.h"
#include <stdlib.h>
#include <stdio.h>

// Taille maximale du nom du SF (ou nom du disque)
#define TAILLE_NOM_DISQUE 24

// Définition du super-bloc
struct sSuperBloc
{
  // Le nom du disque (ou du système de fichiers) (avec le '\0')
  char nomDisque[TAILLE_NOM_DISQUE+1];
  // La date de dernière modification du système de fichiers
  time_t dateDerModif;
};

// Type représentant le super-bloc
typedef struct sSuperBloc *tSuperBloc;

// Définition de la liste chaînée des inodes
struct sListeInodes
{
  // Références vers le premier et le dernier élément de la liste chaînée
  struct sListeInodesElement *premier, *dernier;
  // Nombre d'inodes dans la liste
  int nbInodes;
};

// Définition d'un élement de la liste chaînée des inodes dans le SF
struct sListeInodesElement
{
  // L'inode proprement dit
  tInode inode;
  // L'élément suivant dans la liste chaînée
  struct sListeInodesElement *suivant;
};

// Définition d'un système de fichiers (simplifié)
struct sSF
{
  // Le super-bloc
  tSuperBloc superBloc;
  // La liste chaînée des inodes
  struct sListeInodes listeInodes;
};

/* V2
*  Crée et retourne un super-bloc.
* Fonction non publique (static)
* Entrée : le nom du disque (ou du SF)
* Sortie : le super-bloc, ou NULL en cas de problème
*/
static tSuperBloc CreerSuperBloc(char nomDisque[]) {
  // alloue de la mémoire au nouveau super bloc
  tSuperBloc nouveauSuperBloc = (tSuperBloc)malloc(sizeof(struct sSuperBloc));

  // erreur si allocation mémoire à échouée -> return NULL
  if (nouveauSuperBloc == NULL) {
    fprintf(stderr, "CreerSuperBloc : probleme creation\n");
    return NULL;
  }

  // on copie le nom de dique
  if (nomDisque != NULL) {
    int i;
    // caractère par caractère
    for (i = 0; i < TAILLE_NOM_DISQUE && nomDisque[i] != '\0'; i++) {
      nouveauSuperBloc->nomDisque[i] = nomDisque[i];
    }

    // on met '\0' à la fin du nom du disque
    nouveauSuperBloc->nomDisque[i] = '\0';
  } else { // si pas de nomDisque alors on met juste '\0'
    nouveauSuperBloc->nomDisque[0] = '\0';
  }

  // initialiser la date de dernière modif
  nouveauSuperBloc->dateDerModif = time(NULL);

  // retourne le nouveau super bloc
  return nouveauSuperBloc;
}

/* V2
*  Détruit un super-bloc.
* Fonction non publique (static)
* Entrée : le super-bloc à détruire
* Sortie : aucune
*/
static void DetruireSuperBloc(tSuperBloc *pSuperBloc) {
  // verifie si pSuperBloc est null
  if (pSuperBloc == NULL) {
    return;
  }

  //vérifie si le super bloc lui même existe
  if (*pSuperBloc == NULL) {
    return;
  }

  // on libère la mémoire associée
  free(*pSuperBloc);

  // on positionne le super bloc à NULL
  *pSuperBloc = NULL;
}

/* V2
*  Affiche le contenu d'un super-bloc.
* Fonction non publique (static)
* Entrée : le super-bloc à afficher
* Sortie : aucune
*/
static void AfficherSuperBloc(tSuperBloc superBloc) {
  // si superBloc null alors rien à afficher
  if (superBloc == NULL) {
    printf("Super-bloc : vide\n");
    return;
  }

  // affiche le nom du disque
  printf("SF de nom %s, super bloc :\n", superBloc->nomDisque);

  // affiche la taille du bloc
  printf("taille bloc = %d", TAILLE_BLOC);

  // affiche la date de deernière modification
  printf(", date der modif = %s", ctime(&superBloc->dateDerModif));
}

/* V2 & V4
 * Crée un nouveau système de fichiers.
 * Entrée : nom du disque à associer au système de fichiers créé
 * Retour : le système de fichiers créé, ou NULL en cas d'erreur
 */
tSF CreerSF (char nomDisque[]){
  // on alloue de la mémoire pour le système de fichiers
  tSF nouveauSF = (tSF)malloc(sizeof(struct sSF));

  // si allocation mémoire ne marche pas
  if (nouveauSF == NULL) {
    fprintf(stderr, "CreerSF : probleme creation");
    return NULL;
  }

  // création du super bloc avec le nom donné
  nouveauSF->superBloc = CreerSuperBloc(nomDisque);

  // on vérifie que le super bloc est bien crée
  if (nouveauSF->superBloc == NULL) {
    free(nouveauSF);
    return NULL;
  }

  // on initialise la liste des inodes
  nouveauSF->listeInodes.premier = NULL;
  nouveauSF->listeInodes.dernier = NULL;
  nouveauSF->listeInodes.nbInodes = 0;

  // création du répertoire racine (inode 0)
  tInode racine = CreerInode(0, REPERTOIRE);

  // erreur création répertoire racine
  if (racine == NULL) {
    fprintf(stderr, "CreerSF : probleme creation inode racine");
    DetruireSuperBloc(&nouveauSF->superBloc); // on détruit le super bloc
    free(nouveauSF); // on libère le nouveau sf
    return NULL;
  }

  // on ajoute à la liste
  struct sListeInodesElement *nouvelElement = (struct sListeInodesElement*)malloc(sizeof(struct sListeInodesElement));

  // erreur création élément liste
  if (nouvelElement == NULL) {
    fprintf(stderr, "CreerSF : probleme creation element liste\n");
    DetruireInode(&racine); // on détruit l'inode racine
    DetruireSuperBloc(&nouveauSF->superBloc); // on détruit le super bloc
    free(nouveauSF); // on libère le nv sf
    return NULL;
  }

  // associe l'inode racine au nouvel élément de liste
  nouvelElement->inode = racine;
  nouvelElement->suivant = NULL;

  // initialise la liste avec l'élément racine
  nouveauSF->listeInodes.premier = nouvelElement;
  nouveauSF->listeInodes.dernier = nouvelElement;
  nouveauSF->listeInodes.nbInodes = 1;

  return nouveauSF; // retourne le nouveau sf
}

/* V2
 * Détruit un système de fichiers et libère la mémoire associée.
 * Entrée : le SF à détruire
 * Sortie : aucune
 */
void DetruireSF(tSF *pSF) {
  // on vérifie si le pointeur vers le sf existe
  if (pSF == NULL) {
    return;
  }

  // on vérifie si le sf existe
  if (*pSF == NULL) {
    return;
  }

  // on détruit le super bloc du sf
  DetruireSuperBloc(&(*pSF)->superBloc);

  // on détruit tous les inodes de la liste
  struct sListeInodesElement *elementActuel = (*pSF)->listeInodes.premier; // element de la liste qu'on est en train de détruire
  struct sListeInodesElement *elementProchain; // prochain element de la liste qu'on va détruire
  
  while (elementActuel != NULL) {
    // sauvegarde de l'élement suivant
    elementProchain = elementActuel->suivant;

    // détruit l'inode
    DetruireInode(&elementActuel->inode);

    // on libère l'élément de la liste
    free(elementActuel);

    // on passe à l'élément suivant
    elementActuel = elementProchain;
  }

  // on libère la mémoire du sf
  free(*pSF);

  // on positionne le sf à null
  *pSF = NULL;
}

/* V2
 * Affiche les informations relative à un système de fichiers i.e;
 * le contenu du super-bloc et celui des différents inodes du SF
 * Entrée : le SF à afficher
 * Sortie : aucune
 */
void AfficherSF(tSF sf){
  // on vérifie si le sf existe
  if (sf == NULL) {
    printf("Systeme de fichiers : vide\n");
    return;
  }

  // affiche super bloc
  AfficherSuperBloc(sf->superBloc);

  // affiche tous les inodes de la liste
  printf("Inodes :\n");
  struct sListeInodesElement *elementAffiche = sf->listeInodes.premier; // élément de la liste qu'on affiche
  int compteurInodes = 0;

  while (elementAffiche != NULL) {
    // affiche l'inode
    AfficherInode(elementAffiche->inode);

    // element suivant
    elementAffiche = elementAffiche->suivant;
    compteurInodes++;

    // separation entre les inodes
    if (elementAffiche != NULL) {
      printf("\n");
    }
  }

  // si aucun inode
  if (compteurInodes == 0) {
    printf("  (Aucun inode dans le systeme de fichiers)\n");
  }
}

/* V2
 * Ecrit un fichier d'un seul bloc dans le système de fichiers.
 * Entrées : le système de fichiers, le nom du fichier (sur disque) et son type dans le SF (simulé)
 * Sortie : le nombre d'octets effectivement écrits, -1 en cas d'erreur.
 */
long Ecrire1BlocFichierSF(tSF sf, char nomFichier[], natureFichier type) {
  // verif params d'entrée
  if (sf == NULL || nomFichier == NULL) {
    return -1;
  }

  // on ouvre le fichier en lecture binaire
  FILE *fichier;
  fichier=fopen(nomFichier, "rb");

  // si pas de fichier
  if (fichier == NULL) {
    fprintf(stderr, "Ecrire1BlocFichierSF : impossible d'ouvrir le fichier %s\n", nomFichier);
    return -1;
  }

  // contenu d'un fichier dans un buffer
  unsigned char buffer[TAILLE_BLOC];
  long octetsLus = fread(buffer, 1, TAILLE_BLOC, fichier);

  // on ferme le fichier
  fclose(fichier);

  // on crée un nouvel inode
  unsigned int nouveauNumero = sf->listeInodes.nbInodes; // numéro unique
  tInode nouvelInode = CreerInode(nouveauNumero, type);

  // echec création inode
  if (nouvelInode == NULL) {
    fprintf(stderr, "Ecrire1BlocFichierSF : Impossible de creer l'inode\n");
    return -1;
  }

  // on écrit le contenu du fichier dans l'inode
  long octetsEcris = EcrireDonneesInode1bloc(nouvelInode, buffer, octetsLus);

  // echec ecriture dans l'inode -> destruction de l'inode
  if (octetsEcris <= 0) {
    fprintf(stderr, "Ecrire1BlocFichierSF : impossible d'écrire dans l'inode\n");
    DetruireInode(&nouvelInode);
    return -1;
  }

  // on crée un nouvel élément pour la liste des inodes
  struct sListeInodesElement *nouvelElement = (struct sListeInodesElement*)malloc(sizeof(struct sListeInodesElement));

  // echec allocation mémoire de l'élément
  if (nouvelElement == NULL) {
    fprintf(stderr, "Ecrire1BlocFichierSF : impossible d'ajouter l'inode à la liste");
    DetruireInode(&nouvelInode);
    return -1;
  }

  // initialise le nouvel élément avec l'inode créé
  nouvelElement->inode = nouvelInode; // élément pointe vers le nouvel inode
  nouvelElement->suivant = NULL; // dernier élément de la liste

  // on ajoute l'élement au sf
  if (sf->listeInodes.premier == NULL) { // si la liste est vide
    sf->listeInodes.premier = nouvelElement;
    sf->listeInodes.dernier = nouvelElement;
  } else { // si la liste a déjà des éléments
    sf->listeInodes.dernier->suivant = nouvelElement;
    sf->listeInodes.dernier = nouvelElement;
  }

  // on augmente le nb total d'inodes dans le compteur
  sf->listeInodes.nbInodes++;

  // met à jour la date de modification du super bloc
  sf->superBloc->dateDerModif = time(NULL);

  return octetsEcris;
}

/* V3 & V4
 * Ecrit un fichier (d'un nombre de blocs quelconque) dans le système de fichiers.
 * Si la taille du fichier à écrire dépasse la taille maximale d'un fichier dans le SF(10 x 64 octets),
 * seuls les 640 premiers octets seront écrits dans le système de fichiers.
 * Entrées : le système de fichiers, le nom du fichier (sur disque) et son type dans le SF (simulé)
 * Sortie : le nombre d'octets effectivement écrits, -1 en cas d'erreur.
 */
long EcrireFichierSF(tSF sf, char nomFichier[], natureFichier type) {
  // erreur sf
  if (sf == NULL) {
    fprintf(stderr, "EcrireFichierSF : le sf n'existe pas\n");
    return -1;
  }

  // erreur nomFichier
  if (nomFichier == NULL) {
    fprintf(stderr, "EcrireFichierSF : le fichier n'existe pas\n");
    return -1;
  }
  
  // ouvrir le fichier source en lecture binaire
  FILE *fichierSource;
  fichierSource = fopen(nomFichier, "rb");

  // erreur ouverture fichier source
  if (fichierSource == NULL) {
    fprintf(stderr, "EcrireFichierSF : erreur ouverture fichier source\n");
    return -1;
  }

  // obtenir la taille du fichier source
  fseek(fichierSource, 0, SEEK_END); // on se place à la fin du fichier
  long tailleFichier = ftell(fichierSource);// récupérer la taille
  fseek(fichierSource, 0, SEEK_SET);// retourne au début du fichier

  // erreur taille de fichier
  if (tailleFichier < 0) {
    fprintf(stderr, "EcrireFichierSF : impossible de déterminer la taille du fichier\n");
    fclose(fichierSource); // on ferme le fichier
    return -1;
  }

  long tailleMax = 10 * TAILLE_BLOC; // 10 = NB_BLOCS_DIRECTS
  long octetsALire = tailleFichier;

  // verif si le fichier dépasse la capacité maximale
  if (tailleFichier > tailleMax) {
    printf("Fichier '%s' trop volumineux, tronqué à %ld octets\n", nomFichier, tailleMax);
    octetsALire = tailleMax; // on lit que la capacité max
  }

  // buffer temporaire pour stocker le contenu du fichier
  unsigned char *buffer = malloc(octetsALire);

  // erreur allocation mémoire du buffer
  if (buffer == NULL) {
    fprintf(stderr, "EcrireFichierSF : échec allocation mémoire pour le buffer\n");
    fclose(fichierSource); // on ferme le fichier source
    return -1;
  }

  // lecture du contenu du fichier vers le buffer
  long octetsLus = fread(buffer, 1, octetsALire, fichierSource);

  // si lecture incomplète du fichier
  if (octetsLus != octetsALire) {
    fprintf(stderr, "EcrireFichierSF : lecture incomplète (%ld/%ld octets lus)\n", octetsLus, octetsALire);
    free(buffer); // on libère le buffer
    fclose(fichierSource); // on ferme le fichier source
    return -1;
  }

  // on ferme le fichier source
  fclose(fichierSource);

  // on crée un nouvel inode dans le SF
  unsigned int nouveauNumero = sf->listeInodes.nbInodes; // numéro unique
  tInode nouvelInode = CreerInode(nouveauNumero, type);

  // erreur création inode
  if (nouvelInode == NULL) {
    fprintf(stderr, "EcrireFichierSF : échec création inode numéro %u\n", nouveauNumero);
    free(buffer); // on libère le buffer
    return -1;
  }

  // on écrit les données du buffer vers l'inode
  long octetsEcrits = EcrireDonneesInode(nouvelInode, buffer, octetsLus, 0);

  // echec ecriture dans l'inode -> destruction de l'inode
  if (octetsEcrits < 0) {
    printf("EcrireFichierSF : impossible d'écrire dans l'inode\n");
    DetruireInode(&nouvelInode);
    free(buffer); // on libère le buffer
    return -1;
  }

  // on libère le buffer
  free(buffer);

  // trouver l'inode racine
  tInode racine = NULL; // pointeur vers l'inode racine
  struct sListeInodesElement *current = sf->listeInodes.premier; // on commence au premier élément de la liste

  // parcourt toute la liste
  while (current != NULL) {
    // verif si cet élément contient un inode valide et si c'est l'inode numéro 0
    if (current->inode != NULL && Numero(current->inode) == 0) {
      racine = current->inode; // on a trouvé le répertoire racine
      break; // on arrête la recherche
    }
    current = current->suivant; // passe à l'élément suivant
  }
  
  // verif si on a bien trouvé l'inode racine
  if (racine == NULL) {
    fprintf(stderr, "EcrireFichierSF : répertoire racine introuvable\n");
    DetruireInode(&nouvelInode); // on détruit l'inode
    return -1;
  }

  // structure temporaire en mémoire pour manipuler le répertoire
  tRepertoire repertoireRacine = NULL;

  // on lit le contenu du répertoire depuis inode vers struct temp
  if (LireRepertoireDepuisInode(&repertoireRacine, racine) != 0) {
    fprintf(stderr, "EcrireFichierSF : erreur lecture répertoire racine\n");
    DetruireInode(&nouvelInode); // on détruit l'inode
    return -1;
  }
  
  // on ajoute l'entrée au répertoire
  if (EcrireEntreeRepertoire(repertoireRacine, nomFichier, nouveauNumero) != 0) {
    fprintf(stderr, "EcrireFichierSF : erreur ajout entrée dans répertoire\n");
    DetruireRepertoire(&repertoireRacine); // on détruit le répertoire temporaire
    DetruireInode(&nouvelInode); // on détruit l'inode
    return -1;
  }
  
  // répertoire -> inode racine
  if (EcrireRepertoireDansInode(repertoireRacine, racine) != 0) {
    fprintf(stderr, "EcrireFichierSF : erreur écriture répertoire dans inode\n");
    DetruireRepertoire(&repertoireRacine); // on détruit le répertoire temporaire
    DetruireInode(&nouvelInode); // on détruit l'inode
    return -1;
  }
  
  // on détruit le répertoire temporaire
  DetruireRepertoire(&repertoireRacine);
  
  // on crée un nouvel élément pour la liste des inodes
  struct sListeInodesElement *nouvelElement = malloc(sizeof(struct sListeInodesElement));

  // echec allocation mémoire de l'élément -> destruction de l'inode
  if (nouvelElement == NULL) {
    fprintf(stderr, "EcrireFichierSF : échec allocation mémoire pour l'élément de la liste\n");
    DetruireInode(&nouvelInode);
    return -1;
  }

  // initialise le nouvel élément avec l'inode créé
  nouvelElement->inode = nouvelInode; // élément pointe vers le nouvel inode
  nouvelElement->suivant = NULL; // dernier élément de la liste

  // on ajoute l'élement au sf
  if (sf->listeInodes.premier == NULL) { // si la liste est vide
    sf->listeInodes.premier = nouvelElement;
    sf->listeInodes.dernier = nouvelElement;
  } else { // si la liste a déjà des éléments
    sf->listeInodes.dernier->suivant = nouvelElement;
    sf->listeInodes.dernier = nouvelElement;
  }

  // on augmente le nb total d'inodes dans le compteur
  sf->listeInodes.nbInodes++;

  // met à jour la date de modification du super bloc
  sf->superBloc->dateDerModif = time(NULL);

  return octetsEcrits;
}

/* V3
 * Sauvegarde un système de fichiers dans un fichier (sur disque).
 * Entrées : le système de fichiers, le nom du fichier sauvegarde (sur disque)
 * Sortie : 0 en cas de succèe, -1 en cas d'erreur
 */
int SauvegarderSF(tSF sf, char nomFichier[]) {
  // erreur sf
  if (sf == NULL) {
    fprintf(stderr, "SauvegarderSF : le SF n'existe pas\n");
    return -1;
  }

  // erreur nomFichier
  if (nomFichier == NULL) {
    fprintf(stderr, "SauvegarderSF : le nom de fichier est invalide\n");
    return -1;
  }

  // on ouvre le fichier de sauvegarde en écriture binaire
  FILE *fichierSauvegarde;
  fichierSauvegarde = fopen(nomFichier, "wb");

  // erreur ouverture fichier sauvegarde
  if (fichierSauvegarde == NULL) {
    fprintf(stderr, "SauvegarderSF : erreur ouverture fichier sauvegarde");
    return -1;
  }

  // sauvegarde métadonnées du super-bloc dans le fichier
  fwrite(sf->superBloc->nomDisque, sizeof(char), TAILLE_NOM_DISQUE+1, fichierSauvegarde);
  fwrite(&sf->superBloc->dateDerModif, sizeof(time_t), 1, fichierSauvegarde);

  // écriture nombre total d'inodes
  int nbInodes = sf->listeInodes.nbInodes;

  // sauvegarde le nb d'inodes
  size_t elementsNbInodes = fwrite(&nbInodes, sizeof(int), 1, fichierSauvegarde);

  // erreur sauvegarde nombre d'inodes 
  if (elementsNbInodes != 1) {
    fprintf(stderr, "SauvegarderSF : erreur sauvegarde nombre d'inodes\n");
    fclose(fichierSauvegarde); // on ferme le fichier sauvegarde
    return -1;
  }

  // parcours tous les inodes de la liste pour les sauvegarder
  struct sListeInodesElement *elementCourant = sf->listeInodes.premier; // on commence au premier élément
  int compteurInodes = 0; // verif qu'on sauvegarde bien tous les inodes

  // chaque inode un par un
  while (elementCourant != NULL) {
    // sauvegarde l'inode actuel dans le fichier
    int resultatSauvegarde = SauvegarderInode(elementCourant->inode, fichierSauvegarde);

    // verif sauvegarde de l'inode
    if (resultatSauvegarde != 0) {
      fprintf(stderr, "SauvegarderSF : échec sauvegarde de l'inode numéro %d\n", compteurInodes);
      fclose(fichierSauvegarde); // on ferme le fichier sauvegarde
      return -1;
    }

    // on passe à l'inode suivant
    elementCourant = elementCourant->suivant;

    compteurInodes++;
  }
  // verif nb d'inodes sauvegardés
  if (compteurInodes != nbInodes) {
    fprintf(stderr, "SauvegarderSF : erreur sauvegarde d'inodes (%d inodes sauvegardés au lieu de %d)\n", compteurInodes, nbInodes);
    fclose(fichierSauvegarde); // on ferme le fichier sauvegarde
    return -1;
  }

  // forcer écriture des données
  if (fflush(fichierSauvegarde) != 0) {
    fprintf(stderr, "SauvegarderSF : erreur lors du fflush\n");
    fclose(fichierSauvegarde); // on ferme le fichier
    return -1;
  }

  // on ferme le fichier
  fclose(fichierSauvegarde);

  return 0;
}

/* V3
 * Restaure le contenu d'un système de fichiers depuis un fichier sauvegarde (sur disque).
 * Entrées : le système de fichiers où restaurer, le nom du fichier sauvegarde (sur disque)
 * Sortie : 0 en cas de succèe, -1 en cas d'erreur
 */
int ChargerSF(tSF *pSF, char nomFichier[]) {
  // erreur pSF
  if (pSF == NULL) {
    fprintf(stderr, "ChargerSF : le pointeur vers le SF est invalide\n");
    return -1;
  }

  // erreur nomFichier
  if (nomFichier == NULL) {
    fprintf(stderr, "ChargerSF : le nom de fichier est invalide\n");
    return -1;
  }

  // on ouvre le fichier de sauvegarde en lecture binaire
  FILE *fichierSauvegarde;
  fichierSauvegarde = fopen(nomFichier, "rb");

  // erreur ouverture fichier sauvegarde
  if (fichierSauvegarde == NULL) {
    fprintf(stderr, "ChargerSF : erreur d'ouverture du fichier de sauvegarde\n");
    return -1;
  }

  // on crée un SF sans inode racine
  tSF nouveauSF = (tSF)malloc(sizeof(struct sSF));

  // erreur création sf
  if (nouveauSF == NULL) {
    fprintf(stderr, "ChargerSF : erreur création nouveau SF\n");
    fclose(fichierSauvegarde); // on ferme le fichier
    return -1;
  }

  // on initialise la liste vide
  nouveauSF->listeInodes.premier = NULL;
  nouveauSF->listeInodes.dernier = NULL;
  nouveauSF->listeInodes.nbInodes = 0;

  // on cree le super bloc temporaire
  nouveauSF->superBloc = CreerSuperBloc("temp");

  // erreur création super bloc
  if (nouveauSF->superBloc == NULL) {
    free(nouveauSF); // on libère le nouveau sf
    fclose(fichierSauvegarde); // on ferme le fichier 
    return -1;
  }

  // chargement du super-bloc
  fread(nouveauSF->superBloc->nomDisque, sizeof(char), TAILLE_NOM_DISQUE+1, fichierSauvegarde);
  fread(&nouveauSF->superBloc->dateDerModif, sizeof(time_t), 1, fichierSauvegarde);

  // lecture du nb total d'inodes sauvegardés
  int nbInodes;
  size_t elementsNbInodes = fread(&nbInodes, sizeof(int), 1, fichierSauvegarde);

  // erreur lecture nb inodes
  if (elementsNbInodes != 1) {
    fprintf(stderr, "ChargerSF : erreur lecture nombre d'inodes\n");
    DetruireSF(&nouveauSF); // on détruit le SF
    fclose(fichierSauvegarde); // on ferme le fichier
    return -1;
  }

  // verif nbInodes invalide
  if (nbInodes < 0) {
    fprintf(stderr, "ChargerSF : nombre d'inodes invalide (%d)\n", nbInodes);
    DetruireSF(&nouveauSF); // on détruit le SF
    fclose(fichierSauvegarde); // on ferme le fichier
    return -1;
  }

  // chargement des inodes un par un
  for (int i = 0; i < nbInodes; i++) {
    // recevoir l'inode chargé
    tInode inodeCharge;

    // chargement de l'inode depuis le fichier
    int resultatChargement = ChargerInode(&inodeCharge, fichierSauvegarde);

    // erreur chargement inode
    if (resultatChargement != 0) {
      fprintf(stderr, "ChargerSF : erreur chargement inode %d", i);
      DetruireSF(&nouveauSF); // on détruit le SF
      fclose(fichierSauvegarde); // on ferme le fichier
      return -1;
    }

    // création d'un nouvel élément de liste pour cet inode
    struct sListeInodesElement *nouvelElement = malloc(sizeof(struct sListeInodesElement));

    // erreur allocation mémoire élément
    if (nouvelElement == NULL) {
      fprintf(stderr, "ChargerSF : erreur allocation mémoire pour nouvel élément de liste pour inode %d\n", i);
      DetruireInode(&inodeCharge); // on détruit l'inode
      DetruireSF(&nouveauSF); // on détruit le SF
      fclose(fichierSauvegarde); // on ferme le fichier
      return -1;
    }

    // initialisation du nouvel élément
    nouvelElement->inode = inodeCharge; // élément pointe vers le nouvel inode
    nouvelElement->suivant = NULL; // dernier élément de la liste

    // on ajoute l'élement au sf
    if (nouveauSF->listeInodes.premier == NULL) { // si la liste est vide
      nouveauSF->listeInodes.premier = nouvelElement;
      nouveauSF->listeInodes.dernier = nouvelElement;
    } else { // si la liste a déjà des éléments
      nouveauSF->listeInodes.dernier->suivant = nouvelElement;
      nouveauSF->listeInodes.dernier = nouvelElement;
    }

    // on augmente le nb total d'inodes dans le compteur
    nouveauSF->listeInodes.nbInodes++;
  }

  // on ferme le fichier de sauvegarde
  fclose(fichierSauvegarde);

  // on met à jour la date de modif
  nouveauSF->superBloc->dateDerModif = time(NULL);

  // destruction de l'ancien sf s'il existait
  if (*pSF != NULL) {
    DetruireSF(pSF);
  }

  // on attribut le nouveau SF
  *pSF = nouveauSF;

  return 0;
}

/* V4
 * Affiche le détail du répertoire racine d'un système de fichiers.
 * Entrées : le système de fichiers,
 * et un booléen indiquant si l'affichage doit être détaillé (true) ou non (false),
 * voir l'énoncé pour les consignes d'affichage à suivre
 * Sortie : 0 en cas de succès, -1 en cas d'erreur
 */
int Ls(tSF sf, bool detail)  {
  // verif si le sf existe
  if (sf == NULL) {
    fprintf(stderr, "Ls : erreur sf inexistant");
    return -1;
  }

  // on crée un rep temporaire pour lire les données de l'inode racine
  tRepertoire rep = CreerRepertoire();

  // erreur créer répertoire temp
  if (rep == NULL) {
    fprintf(stderr, "Ls : erreur création du répertoire temporaire\n");
    return -1;
  }

  // on cherche l'inode racine
  tInode inodeRacine = NULL;
  struct sListeInodesElement *current = sf->listeInodes.premier; // on commence au début

  // on parcourt chaque élément pour trouver l'inode 0
  while (current != NULL) {
    // // verif si numéro d'inode est 0
    if (Numero(current->inode) == 0) {
      inodeRacine = current->inode; // on l'a trouvé
      break; // on sort de la boucle
    }
    current = current->suivant; // élément suivant
  }

  // verif inode racine
  if (inodeRacine == NULL) {
    fprintf(stderr, "Ls : l'inode racine n'a pas été trouvé dans le sf\n");
    DetruireRepertoire(&rep); // on détruit le rep temp
    return -1;
  }

  // on lit le contenu de l'inode racine dans le répertoire temporaire
  if (LireRepertoireDepuisInode(&rep, inodeRacine) != 0) {
    fprintf(stderr, "Ls : impossible de lire les données du rep depuis l'inode racine\n");
    DetruireRepertoire(&rep); // on détruit le rep temp
    return -1;
  }

  // nb d'entrées du rep
  int nbEntrees = NbEntreesRepertoire(rep);

  // si param detail sur true
  if (detail) {
    printf("Nombre de fichiers dans le répertoire racine : %d\n", nbEntrees);

    // si rep pas vide
    if (nbEntrees > 0) {
      // on alloue de la mémoire pour un tab pour stocker les entrées du rep
      struct sEntreesRepertoire *tabEntrees = malloc(nbEntrees * sizeof(struct sEntreesRepertoire));

      // erreur allocation mémoire
      if (tabEntrees == NULL) {
        fprintf(stderr, "Ls : erreur d'allocation mémoire pour le tableau des entrées\n");
        DetruireRepertoire(&rep); // on détruit le rep temp
        return -1;
      }

      // on remplit le tab avec toutes les entrées du rep
      int nb = EntreesContenuesDansRepertoire(rep, tabEntrees);

      // entrée après entrée
      for (int i = 0; i < nb; i++) {
        // on récupère les infos de l'entrée actuelle
        unsigned int numInode = tabEntrees[i].numeroInode; // numéro d'inode du fichier
        char *nomFichier = tabEntrees[i].nomEntree; // nom du fichier

        // on cherche l'inode correspondant
        tInode inodeFichier = NULL;
        current = sf->listeInodes.premier; // on commence du début

        // parcours de la liste pour trouver l'inode avec le bon num
        while (current != NULL) {
          // si même num
          if (Numero(current->inode) == numInode) {
            inodeFichier = current->inode; // on a trouvé
            break; // on sort de la boucle
          }
          current = current->suivant; // on passe au prochain
        }

        // si inode trouvé, on affiche les infos détaillés
        if (inodeFichier != NULL) {
          // recup les attributs de l'inode
          natureFichier type = Type(inodeFichier); // type
          long taille = Taille(inodeFichier); // taille
          time_t dateModif = DateDerModifFichier(inodeFichier); // date der modif

          // conversion type en char
          char *typeStr;
          switch(type) {
            case ORDINAIRE:
              typeStr = "ORDINAIRE";
              break;
            case REPERTOIRE:
              typeStr = "REPERTOIRE";
              break;
            case AUTRE:
              typeStr = "AUTRE";
              break;
            default:
              typeStr = "INCONNU";
          }

          // correction problème saut de ligne avec ctime
          char* dateStr = ctime(&dateModif);
          for (int i = 0; dateStr[i] != '\0'; i++) { // parcours la chaine jusqu'au \n
            if (dateStr[i] == '\n') {
              dateStr[i] = '\0'; // remplace \n par \0
              break;
            }
          }

          // on affiche les infos
          printf("%-3u %-12s %6ld %s %s\n", numInode, typeStr, taille, dateStr, nomFichier);
        } else { // inode n'existe pas
          fprintf(stderr, "Ls : inode %u référencée mais non trouvée pour le fichier '%s'\n", numInode, nomFichier);
        }
      }
      // on libère le tableau allouée
      free(tabEntrees);
    }
  } else { // affichage simple : detail sur false
    printf("Nombre de fichiers dans le répertoire racine : %d\n", nbEntrees);

    // si le rep contient des fichiers
    if (nbEntrees > 0) {
      // on alloue de la mémoire pour stocker les entrées
      struct sEntreesRepertoire *tabEntrees = malloc(nbEntrees * sizeof(struct sEntreesRepertoire));

      // erreur allocation mémoire
      if (tabEntrees == NULL) {
        fprintf(stderr, "Ls : échec de l'allocation mémoire pour le tableau des entrées\n");
        DetruireRepertoire(&rep); // on détruit le rep temp
        return -1;
      }

      // on recup les entrées dans le tableau
      int nb = EntreesContenuesDansRepertoire(rep, tabEntrees);

      // on affiche les infos
      for (int i = 0; i < nb; i++) {
        printf("%s\n", tabEntrees[i].nomEntree);
      }

      // on libère le tableau
      free(tabEntrees);
    }
  }

  // on détruit le répertoire temp
  DetruireRepertoire(&rep);

  return 0;
}