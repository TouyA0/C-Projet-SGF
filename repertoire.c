/**
 * ProgC - Projet Automne 25-26 : Gestion de systèmes de fichiers
 * VERSION 4
 * Fichier : repertoire.c
 * Module de gestion d'un répertoire d'un systèmes de fichiers (simulé)
 **/

#include "repertoire.h"
#include <stdlib.h>
#include <stdio.h>

// Définition d'un répertoire
struct sRepertoire
{
  tEntreesRepertoire *table;
};

// fonction auxiliaire pour calculer le nombre maximum de fichiers qu'on peut mettre dans un répertoire
static int CalculerCapaciteMax(void) {
  return TailleMaxFichier() / sizeof(struct sEntreesRepertoire);
}

/* V4
 * Crée un nouveau répertoire.
 * Entrée : aucune
 * Sortie : le répertoire créé, ou NULL si problème
 */
tRepertoire CreerRepertoire(void)
{
  // allouer de la mémoire pour la structure du répertoire
  tRepertoire rep = (tRepertoire)malloc(sizeof(struct sRepertoire));

  // erreur allocation mémoire struct répertoire
  if(rep == NULL) {
    fprintf(stderr, "CreerRepertoire : probleme création\n");
    return NULL;
  }

  // capacité max d'entrées de répertoire
  int capaciteMax = CalculerCapaciteMax();

  // allouer de la mémoire pour le tableau qui contiendra les entrées
  rep->table = malloc(capaciteMax * sizeof(struct sEntreesRepertoire*));

  // erreur allocation mémoire tableau entrées
  if (rep->table == NULL) {
    fprintf(stderr, "CreerRepertoire : probleme création\n");
    free(rep); // on libère le repertoire
    return NULL;
  }

  // on initialise tous les pointeurs
  for (int i = 0; i < capaciteMax; i++) {
    // on alloue de la mémoire pour l'entrée du rep
    rep->table[i] = malloc(sizeof(struct sEntreesRepertoire));

    // erreur d'allcation mémoire
    if (rep->table[i] == NULL) {
      fprintf(stderr, "CreerRepertoire : erreur aloocation entree %d\n", i);
      
      // on libère les entrées déjà allouées
      for (int j = 0; j < i; j++) {
        free(rep->table[j]);
      }
      free(rep->table); // on libère le tableau de pointeurs
      free(rep); // on libère la struct
      
      return NULL;
    }
    // on initialise l'entrée avec des valeurs pas défaut
    rep->table[i]->nomEntree[0] = '\0';
    rep->table[i]->numeroInode = 0;
  }

  return rep;
}

/* V4
 * Détruit un répertoire et libère la mémoire associée.
 * Entrée : le répertoire à détruire
 * Sortie : aucune
 */
void DetruireRepertoire(tRepertoire *pRep)
{
  // verif si le pointeur vers le repertoire est valide
  if (pRep == NULL || *pRep == NULL) {
    return;
  }

  // on libère en premier le tableau d'entrées du répertoire
  if ((*pRep)->table != NULL) {
    free((*pRep)->table); // libère la mémoire allouée pour le tableau d'entrées
    (*pRep)->table = NULL; // on met le pointeur à NULL
  }

  // on libère la struct du répertoire
  free(*pRep);

  // on met le pointeur à NULL
  *pRep = NULL;
}

/* V4
 * Écrit une entrée dans un répertoire.
 * Si l'entrée existe déjà dans le répertoire, le numéro d'inode associé est mis à jour.
 * Si l'entrée n'existe pas, elle est ajoutée dans le répertoire.
 * Entrées : le répertoire destination, le nom de l'entrée à écrire,
 *           le numéro d'inode associé à l'entrée
 * Retour : 0 si l'entrée est écrite avec succès, -1 en cas d'erreur
 */
int EcrireEntreeRepertoire(tRepertoire rep, char nomEntree[], unsigned int numeroInode)
{
  // verif si le repertoire existe
  if (rep == NULL) {
    fprintf(stderr, "EcrireEntreeRepertoire : repertoire inexistant\n");
    return -1;
  }
  
  // verif si le nombre d'entrées est valide
  if (nomEntree == NULL || nomEntree[0] == '\0') {
    fprintf(stderr, "EcrireEntreeRepertoire : nom d'entrée invalide\n");
    return -1;
  }
  
  // verif si le tableau d'entrées existe
  if (rep->table == NULL) {
    fprintf(stderr, "EcrireEntreeRepertoire : tableau d'entrees inexistant\n");
    return -1;
  }
  
  int capaciteMax = CalculerCapaciteMax(); // capacité max d'entrées de répertoire
  int indexExistant = -1; // mémorise l'index de l'entrée trouvée

  // chaque entrée du répertoire un par un
  for (int i = 0; i < capaciteMax; i++) {
    // verif si l'entrée est utilisée
    if (rep->table[i]->nomEntree[0] != '\0') {
      int j = 0; // index pour parcourir les caractères
      int identique = 1; // verif si identique (1=vrai, 0=faux)
      
      // compare caract par caract
      while (nomEntree[j] != '\0' && rep->table[i]->nomEntree[j] != '\0') {
        // verif si caract différent
        if (nomEntree[j] != rep->table[i]->nomEntree[j]) {
          identique = 0; // marque comme différent
          break; // on sort de la boucle
        }
        j++; // passe au caract suivant
      }
      // verif identique (identique, se terminent au même endroit)
      if (identique == 1 && nomEntree[j] == '\0' && rep->table[i]->nomEntree[j] == '\0') {
        indexExistant = i; // mémorise l'index de l'entrée
        break;
      }
    }
  }
  // si l'entrée existe déjà
  if (indexExistant != -1) {
    // on met à jour le numéro d'inode associé à l'entrée
    rep->table[indexExistant]->numeroInode = numeroInode;
  } else { // l'entrée n'existe pas
    int indexLibre = -1; //  première position libre dans le tableau
    for (int i = 0; i < capaciteMax; i++) {
      // verif si nom est vide
      if (rep->table[i]->nomEntree[0] == '\0') {
        indexLibre = i;
        break; // on prend la première position libre trouvée
      }
    }
    // erreur pas de position libre trouvée
    if (indexLibre == -1) {
      fprintf(stderr, "EcrireEntreeRepertoire : repertoire plein\n");
      return -1;
    }

    int k = 0; // index pour parcourir les caractères du nom
    
    // boucle de copie
    while (nomEntree[k] != '\0' && k < TAILLE_NOM_FICHIER) {
      // copie le carac dans le nom de l'entrée du répertoire
      rep->table[indexLibre]->nomEntree[k] = nomEntree[k];
      k++; // carac suivant
    }

    // termine la chaine par '\0'
    rep->table[indexLibre]->nomEntree[k] = '\0';

    // on associe le numéro d'inodeà l'entrée du répertoire
    rep->table[indexLibre]->numeroInode = numeroInode;
  }
  return 0;
}

/* V4
 * Lit le contenu d'un répertoire depuis un inode.
 * Entrées : le répertoire mis à jour avec le contenu lu,
 *           l'inode source.
 * Retour : 0 si le répertoire est lu avec succès, -1 en cas d'erreur
 */
int LireRepertoireDepuisInode(tRepertoire *pRep, tInode inode)
{
  // verif si le pointeur vers le répertoire est valide
  if (pRep == NULL) {
    fprintf(stderr, "LireRepertoireDepuisInode : pointeur repertoire invalide\n");
    return -1;
  }
  
  // verif si l'inode existe
  if (inode == NULL) {
    fprintf(stderr, "LireRepertoireDepuisInode : inode inexistant\n");
    return -1;
  }
  
  // verif que l'inode est de type repertoire
  if (Type(inode) != REPERTOIRE) {
    fprintf(stderr, "LireRepertoireDepuisInode : l'inode n'est pas un repertoire\n");
    return -1;
  }
  
  // detruire l'ancien repertoire s'il existe
  if (*pRep != NULL) {
    DetruireRepertoire(pRep);
  }
  
  // on crée un nouveau rep vide
  *pRep = CreerRepertoire();
  
  // erreur création répertoire
  if (*pRep == NULL) {
    fprintf(stderr, "LireRepertoireDepuisInode : erreur création répertoire\n");
    return -1;
  }
  
  long tailleReelle = Taille(inode); // taille réelle de l'inode

  // verif si inode est vide
  if (tailleReelle <= 0) {
    return 0;
  }

  unsigned char *donneesBrutes = (unsigned char *)malloc(tailleReelle); // alloue de la mémoire pour recevoir les données du répertoire contenue dans l'inode
  
  // erreur allocation mémoire
  if (donneesBrutes == NULL) {
    fprintf(stderr, "LireRepertoireDepuisInode : erreur allocation memoire\n");
    DetruireRepertoire(pRep); // détruit le rep crée
    return -1;
  }
  
  // lire les données du répertoire depuis l'inode
  long octetsLus = LireDonneesInode(inode, donneesBrutes, tailleReelle, 0);
  
  // si aucune donnée lue
  if (octetsLus <= 0) {
    free(donneesBrutes); // on libère la mémoire temporaire des données brutes
    return 0;
  }

  int tailleEntree = sizeof(struct sEntreesRepertoire); // taille en mémoire d'une entrée de répertoire
  int nbEntreesPossibles = octetsLus / tailleEntree; // nombre d'entrees completes qu'on peut extraire des octets lus

  // parcourir toutes les entrees possibles dans les données lus
  for (int i = 0; i < nbEntreesPossibles; i++) {
    // on prend l'entrée i
    struct sEntreesRepertoire *entree = (struct sEntreesRepertoire *)(donneesBrutes + (i * tailleEntree));

    // verif si l'entree est utilisé
    if (entree->nomEntree[0] != '\0') {
      // on ajoute cette entrée dans le répertoire
      int resultat = EcrireEntreeRepertoire(*pRep, entree->nomEntree, entree->numeroInode);

      // erreur ajout entrée
      if (resultat == -1) {
        fprintf(stderr, "LireRepertoireDepuisInode : erreur ajout entree\n");
        free(donneesBrutes); // on libère la mémoire temporaire des données brutes
        DetruireRepertoire(pRep); // on détruit le répertoire
        return -1;
      }
    }
  }
  // on libère la mémoire temporaire des données brutes
  free(donneesBrutes);

  return 0;
}

/* V4
 * Écrit un répertoire dans un inode.
 * Entrées : le répertoire source et l'inode destination
 * Sortie : 0 si le répertoire est écrit avec succès, -1 en cas d'erreur
 */
int EcrireRepertoireDansInode(tRepertoire rep, tInode inode)
{
  // verif si repertoire source existe
  if (rep == NULL) {
    fprintf(stderr, "EcrireRepertoireDansInode : repertoire source inexistant\n");
    return -1;
  }

  // verif si l'inode destination existe
  if (inode == NULL) {
    fprintf(stderr, "EcrireRepertoireDansInode : inode destination inexistante\n");
    return -1;
  }

  // verif si le tableau d'entrées du répertoire existe
  if (rep->table == NULL) {
    fprintf(stderr, "EcrireRepertoireDansInode : tableau d'entrees inexistant\n");
    return -1;
  }

  long tailleMax = TailleMaxFichier(); // taille maximale d'un fichier
  unsigned char *donneesAEcrire = (unsigned char *)malloc(tailleMax); // alloue de la mémoire pour écrire les données du répertoire contenue dans l'inode

  // erreur allocation mémoire
  if (donneesAEcrire == NULL) {
    fprintf(stderr, "EcrireRepertoireDansInode : erreur allocation mémoire\n");
    return -1;
  }

  // initialise tout à 0
  for (int i = 0; i < tailleMax; i++) {
    donneesAEcrire[i] = 0;
  }

  int indexCourant = 0; // position actuelle dans le tableau des données
  int capaciteMax = CalculerCapaciteMax(); // nb max d'entrées dans le répertoire

  // on parcourt toutes les entrées du répertoire
  for (int i = 0; i < capaciteMax; i++) {
    // verif si l'entrée est utilisé
    if (rep->table[i]->nomEntree[0] != '\0') {
      // calcul où écrire l'entrée dans le tableau
      struct sEntreesRepertoire *destination = (struct sEntreesRepertoire *)(donneesAEcrire + indexCourant);

      int j = 0; // index caractères du nom de l'entrée source vers la destination

      // carac par carac 
      while (rep->table[i]->nomEntree[j] != '\0' && j < TAILLE_NOM_FICHIER) {
        // copie du carac du nom source vers destination
        destination->nomEntree[j] = rep->table[i]->nomEntree[j];
        
        j++; // carac suivant
      }
      // termine la chaine par '\0'
      destination->nomEntree[j] = '\0';

      // on enregistre le numéro d'inode qui correspond à ce fichier
      destination->numeroInode = rep->table[i]->numeroInode;

      // on avance à la prochaine entrée
      indexCourant += sizeof(struct sEntreesRepertoire);

      // verif si on dépasse pas la taille max
      if (indexCourant > tailleMax) {
        fprintf(stderr, "EcrireRepertoireDansInode : erreur dépassement taille max\n");
        free(donneesAEcrire); // on libère la mémoire temporaire
        return -1;
      }
    }
  }
  // on écrit toutes les données dans l'inode
  long octetsEcrits = EcrireDonneesInode(inode, donneesAEcrire, indexCourant, 0);

  // on libère la mémoire temporaire
  free(donneesAEcrire);

  // verif si l'écriture à marché
  if (octetsEcrits != indexCourant) {
    fprintf(stderr, "EcrireRepertoireDansInode : erreur ecriture dans l'inode\n");
    return -1;
  }

  return 0;
}

/* V4
 * Récupère les entrées contenues dans un répertoire.
 * Entrées : le répertoire source, un tableau récupérant les numéros d'inodes des entrées du rpertoire
 * Retour : le nombre d'entrées dans le répertoire
 */
int EntreesContenuesDansRepertoire(tRepertoire rep, struct sEntreesRepertoire tabNumInodes[])
{
  // verif si le rep existe
  if (rep == NULL) {
    return 0;
  }

  // verif si le tableau d'entrées existe
  if (rep->table == NULL) {
    return 0;
  }

  int compteur = 0; // compte le nb d'entrées valides
  int capaciteMax = CalculerCapaciteMax(); // capacité max du répertoire

  // on parcourt toutes les entrées possibles du répertoire
  for (int i = 0; i < capaciteMax; i++) {
    // verif si l'entrée est utilisée
    if (rep->table[i]->nomEntree[0] != '\0') {
      // on copie le nom de l'entrée
      int j = 0; // position dans le nom

      // carac par carac
      while (rep->table[i]->nomEntree[j] != '\0' && j < TAILLE_NOM_FICHIER) {
        // recopie la lettre du nom original vers le tab
        tabNumInodes[compteur].nomEntree[j] = rep->table[i]->nomEntree[j];

        j++; // lettre suivante
      }

      // on termine la chaine par '\0'
      tabNumInodes[compteur].nomEntree[j] = '\0';

      // on copie le numéro d'inode
      tabNumInodes[compteur].numeroInode = rep->table[i]->numeroInode;

      compteur++; // on passe à l'entrée suivante dans la tableau
    }
  }
  return compteur;
}

/* V4
 * Compte le nombre d'entrées d'un répertoire.
 * Entrée : le répertoire source
 * Retour : le nombre d'entrées du répertoire
 */
int NbEntreesRepertoire(tRepertoire rep)
{
  // verif si le repertoire existe
  if (rep == NULL) {
    return 0;
  }

  // verif si le tableau d'entrées existe
  if (rep->table == NULL) {
    return 0;
  }

  int compteur = 0; // on compte les entrées utilisées
  int capaciteMax = CalculerCapaciteMax(); // capacite max du repertoire

  // on parcourt toutes les entrées du répertoire
  for (int i = 0; i < capaciteMax; i++) {
    // vérifie si l'entrée est utilisée
    if (rep->table[i]->nomEntree[0] != '\0') {
      compteur++; 
    }
  }
  return compteur;
}

/* V4
 * Affiche le contenu d'un répertoire.
 * Entrée : le répertoire à afficher
 * Retour : aucun
 */
void AfficherRepertoire(tRepertoire rep)
{
  // verif si le rep existe
  if (rep == NULL) {
    printf("Repertoire vide\n");
    return;
  }

  int nbEntrees = NbEntreesRepertoire(rep); // nb d'entrées dans le répertoire
  printf("Contenu du repertoire (%d entrées) :\n", nbEntrees);

  // on alloue de la mémoire pour un tableau temporaire pour stocker toutes les entrées du rep
  struct sEntreesRepertoire *tabEntrees = malloc(nbEntrees * sizeof(struct sEntreesRepertoire));

  // erreur allocation mémoire
  if (tabEntrees == NULL) {
    fprintf(stderr, "AfficherRepertoire : erreur d'allocation mémoire\n");
    return;
  }

  // remplir le tableau avec toutes les entrées du rep
  int nb = EntreesContenuesDansRepertoire(rep, tabEntrees);

  // parcours chaque entrée du rep
  for (int i = 0; i < nb; i ++) {
    printf(" %s (inode %u)\n", tabEntrees[i].nomEntree, tabEntrees[i].numeroInode);
  }

  // on libère la mémoire allouée du tableau temporaire
  free(tabEntrees);
}