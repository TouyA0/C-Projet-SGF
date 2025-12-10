/**
 * ProgC - Projet Automne 25-26 : Gestion de systèmes de fichiers
 * VERSION 4 = VERSION 3
 * Fichier : bloc.c
 * Module de gestion des blocs de données.
 **/

#include "bloc.h"
#include <stdlib.h>
#include <stdio.h>

/* V1
 * Crée et retourne un nouveau bloc de données.
 * Entrée : Aucune
 * Retour : le bloc créé ou NULL en cas de problème
 */
tBloc CreerBloc (void) {
  // alloue mémoire de taille TAILLE_BLOC octets pour un nouveau bloc de type tBloc
  tBloc nouveauBloc = (tBloc)malloc(TAILLE_BLOC);

  // en cas de problème d'allocation mémoire
  if (nouveauBloc == NULL) {
    fprintf(stderr, "CreerBloc : probleme creation\n"); // message erreur sur stderr
    return NULL; // retourne NULL
  }

  // retourne nouveau bloc
  return nouveauBloc;
}

/* V1
 * Détruit un bloc de données.
 * Entrée : le bloc à détruire (libération mémoire allouée)
 * Retour : aucun
 */
void DetruireBloc(tBloc *pBloc) {
  // vérifie que le pointeur et que le bloc pointé existe
  if (pBloc != NULL && *pBloc != NULL) {
    free(*pBloc); // libère la mémoire allouée
    *pBloc = NULL; // positionne le bloc à NULL
  }
}

/* V1
 * Copie dans un bloc les taille octets situés à l’adresse contenu.
 * Si taille est supérieure à la taille d’un bloc, seuls les TAILLE_BLOC premiers octets doivent être copiés.
 * Entrées : le bloc, l'adresse du contenu à copier et sa taille en octets
 * Retour : le nombre d'octets effectivement écrits dans le bloc
 */
long EcrireContenuBloc (tBloc bloc, unsigned char *contenu, long taille) {
  // verifie si bloc ou contenu NULL -> alors impossible d'écrire -> retourne 0 octet écrit
  if (bloc == NULL || contenu == NULL) {
    return 0;
  }

  // détermine le nombre d'octets à copier sans dépasser la taille du bloc (prend la plus petite des deux valeurs)
  long octetsACopier = (taille > TAILLE_BLOC) ? TAILLE_BLOC : taille;

  // copie octet par octet de contenu dans bloc
  for (long i = 0; i < octetsACopier; i++) {
    bloc[i] = contenu[i];
  }

  // retourne le nomre d'octets effectivement écrits dans le bloc
  return octetsACopier;
}

/* V1
 * Copie à l'adresse contenu, les taille octets stockés dans un bloc.
 * Si taille est supérieure à la taille d’un bloc, seuls les TAILLE_BLOC premiers octets doivent être copiés.
 * Entrées : le bloc, l'adresse contenu à laquelle recopier et la taille en octets du bloc
 * Retour : le nombre d'octets effectivement lus dans le bloc
 */
long LireContenuBloc(tBloc bloc, unsigned char *contenu, long taille) {
  if (bloc == NULL || contenu == NULL) {
    return 0;
  }

  long octetsACopier = (taille > TAILLE_BLOC) ? TAILLE_BLOC : taille;

  for (long i = 0; i < octetsACopier; i++) {
    contenu[i] = bloc[i]; // faut juste faire l'inverse car on extrait les données de bloc -> contenu au lieu de contenu -> bloc (EcrireContenuBloc)
  }

  return octetsACopier;
}

/* V3
 * Sauvegarde les données d'un bloc en les écrivant dans un fichier (sur disque).
 * Entrées : le bloc à sauvegarder, sa taille en octets, le nom du fichier cible
 * Retour : 0 en cas de succès, -1 en cas d'erreur
 */
int SauvegarderBloc(tBloc bloc, long taille, FILE *fichier){
  // erreur bloc
  if (bloc == NULL) {
    fprintf(stderr, "SauvegarderBloc : le bloc n'existe pas\n");
    return -1;
  }

  // erreur fichier
  if (fichier == NULL) {
    fprintf(stderr, "SauvegarderBloc : le fichier n'existe pas (ou pas les permissions nécessaires)\n");
    return -1;
  }

  // vérifier si taille pas supérieur à TAILLE_BLOC sinon on se limite au max
  long tailleReelle = taille;
  if (taille > TAILLE_BLOC) {
    tailleReelle = TAILLE_BLOC;
    printf("SauvegarderBloc : taille réduite à %ld octets (car max atteint)\n", tailleReelle);
  }

  // écriture des données du bloc dans le fichier
  size_t elementsEcrits = fwrite(bloc, 1, tailleReelle, fichier);

  // verifie que elementsEcrits = tailleReelle
  if (elementsEcrits != (size_t)tailleReelle) {
    fprintf(stderr, "SauvegarderBloc : erreur écriture (%lu/%ld éléments écrits)\n", elementsEcrits, tailleReelle);
    return -1;
  }

  // forcer écriture immédiate des données
  if (fflush(fichier) != 0) {
    fprintf(stderr, "SauvegarderBloc : erreur lors du fflush\n");
    return -1;
  }

  return 0;
}

/* V3
 * Charge dans un bloc les données lues dans un fichier (sur disque).
 * Entrées : le bloc dans lequel charger, la taille en octets du fichier contenant les données, son nom (sur le disque))
 * Retour : 0 en cas de succès, -1 en cas d'erreur
 */
int ChargerBloc(tBloc bloc, long taille, FILE *fichier){
  // erreur bloc
  if (bloc == NULL) {
    fprintf(stderr, "ChargerBloc : le bloc n'existe pas\n");
    return -1;
  }

  // erreur fichier
  if (fichier == NULL) {
    fprintf(stderr, "ChargerBloc : le fichier n'existe pas (ou pas les permissions nécessaires)\n");
    return -1;
  }

  // vérifier qu'on est pas à la fin du fichier
  if (feof(fichier)) {
    fprintf(stderr, "ChargerBloc : fin de fichier atteinte\n");
    return -1;
  }

  // vérifier si taille pas supérieur à TAILLE_BLOC sinon on se limite au max
  long tailleReelle = taille;
  if (taille > TAILLE_BLOC) {
    tailleReelle = TAILLE_BLOC;
    printf("ChargerBloc : taille réduite à %ld octets (car max atteint)\n", tailleReelle);
  }

  // lecture des données depuis le fichier vers bloc
  size_t elementsLus = fread(bloc, 1, tailleReelle, fichier);

  // verif que les éléments lus = taille réelle
  if (elementsLus != (size_t)tailleReelle) {
    fprintf(stderr, "ChargerBloc : erreur lecture (%lu/%ld éléments lus)\n", elementsLus, tailleReelle);
    return -1;
  }

  return 0;
}
