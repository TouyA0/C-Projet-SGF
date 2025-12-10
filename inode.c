/**
 * ProgC - Projet Automne 25-26 : Gestion de systèmes de fichiers
 * VERSION 4
 * Fichier : inode.c
 * Module de gestion des inodes.
 **/

#include "inode.h"
#include "bloc.h"
#include <stdlib.h>

// Nombre maximal de blocs dans un inode
#define NB_BLOCS_DIRECTS 10

// Définition d'un inode
struct sInode
{
  // Numéro de l'inode
  unsigned int numero;
  // Le type du fichier : ordinaire, répertoire ou autre
  natureFichier type;
  // La taille en octets du fichier
  long taille;
  // Les adresses directes vers les blocs (NB_BLOCS_DIRECTS au maximum)
  tBloc blocDonnees[NB_BLOCS_DIRECTS];
  // Les dates : dernier accès à l'inode, dernière modification du fichier
  // et de l'inode
  time_t dateDerAcces, dateDerModif, dateDerModifInode;
};


// fonction auxiliaire pour mettre à jour les dates selon l'accès
static void mettreAJourDateAccess(tInode inode) {
  if (inode != NULL) {
    inode->dateDerAcces = time(NULL);
  }
}

// fonction auxiliaire pour mettre à jour les dates selon modification métadonnées de l'inode
static void mettreAJourDateModifInode(tInode inode) {
  if (inode != NULL) {
    inode->dateDerModifInode = time(NULL);
  }
}

// fonction auxiliaire pour mettre à jour les dates selon modification du contenu du fichier
static void mettreAJourDateModifContenu(tInode inode) {
  if (inode != NULL) {
    inode->dateDerModif = time(NULL);
    inode->dateDerModifInode = time(NULL);
  }
}


/* V1
 * Crée et retourne un inode.
 * Entrées : numéro de l'inode et le type de fichier qui y est associé
 * Retour : l'inode créé ou NULL en cas de problème
 */
tInode CreerInode(int numInode, natureFichier type) {
  // creer un nouveau inode avec paramètres
  tInode nouveau = (tInode)malloc(sizeof(struct sInode));

  // probleme d'allocation mémoire
  if (nouveau == NULL) {
    fprintf(stderr, "CreerInode : probleme creation");
    return NULL;
  }

  // initalisation des champts de définition d'un inode
  nouveau->numero = numInode;
  nouveau->type = type;
  nouveau->taille = 0;

  // initialisation des adresses directes vers les blocs à NULL
  for (int i = 0; i < NB_BLOCS_DIRECTS; i++) {
    nouveau->blocDonnees[i] = NULL;
  }

  // initialisation des dates
  time_t maintenant = time(NULL); // obetnir heure actuelle
  nouveau->dateDerAcces = maintenant;
  nouveau->dateDerModif = maintenant;
  nouveau->dateDerModifInode = maintenant;

  return nouveau; // retourne le nouveau inode
}

/* V1
 * Détruit un inode.
 * Entrée : l'inode à détruire
 * Retour : aucun
 */
void DetruireInode(tInode *pInode) {
  if (pInode != NULL && *pInode != NULL) {
    // libère les adresses directes vers les blocs
    for (int i = 0; i < NB_BLOCS_DIRECTS; i++) {
      if ((*pInode)->blocDonnees[i] != NULL) {
        DetruireBloc(&(*pInode)->blocDonnees[i]);
      }
    }

    free(*pInode); // libère la mémoire allouée
    *pInode = NULL; // positionne inode à NULL
  }
}

/* V1
 * Récupère la date de dernier accès à un inode.
 * Entrée : l'inode pour lequel on souhaite connaître la date de dernier accès
 * Retour : la date de dernier accès à l'inode
 */
time_t DateDerAcces(tInode inode) {
  // verifie inode pas null
  if (inode == NULL) {
    return 0;
  }

  return inode->dateDerAcces;
}

/* V1
 * Récupère la date de dernière modification d'un inode.
 * Entrée : l'inode pour lequel on souhaite connaître la date de dernière modification
 * Retour : la date de dernière modification de l'inode
 */
time_t DateDerModif(tInode inode) {
  if (inode == NULL) {
    return 0;
  }

  return inode->dateDerModif;
}

/* V1
 * Récupère la date de dernière modification d'u fichier associé à un inode.
 * Entrée : l'inode pour lequel on souhaite connaître la date de dernière modification du fichier associé
 * Retour : la date de dernière modification du fichier associé à l'inode
 */
time_t DateDerModifFichier(tInode inode) {
  if (inode == NULL) {
    return 0;
  }

  return inode->dateDerModifInode;
}

/* V1
 * Récupère le numéro d'un inode.
 * Entrée : l'inode pour lequel on souhaite connaître le numéro
 * Retour : le numéro de l'inode
 */
unsigned int Numero(tInode inode) {
  if (inode == NULL) {
    return 0;
  }

  return inode->numero;
}

/* V1
 * Récupère la taille en octets du fichier associé à un inode.
 * Entrée : l'inode pour lequel on souhaite connaître la taille
 * Retour : la taille en octets du fichier associé à l'inode
 */
long Taille(tInode inode) {
  if (inode == NULL) {
    return 0;
  }

  return inode->taille;
}

/* V1
 * Récupère le type du fichier associé à un inode.
 * Entrée : l'inode pour lequel on souhaite connaître le tyep de fichier associé
 * Retour : le type du fichier associé à l'inode
 */
natureFichier Type(tInode inode) {
  if (inode == NULL) {
    return 0;
  }

  return inode->type;
}

/* V1 & V3
 * Affiche les informations d'un inode
 * Entrée : l'inode dont on souhaite afficher les informations
 * Retour : aucun
 */
void AfficherInode(tInode inode) {
  // si inode n'existe pas
  if (inode == NULL) {
    printf("vide\n");
    return;
  }

  //mettre a jour date d'accès
  mettreAJourDateAccess(inode);

  // affichage du contenu de l'inode
  printf("--------Inode----[%u]:\n", inode->numero);
  printf("type : ");
  switch (inode->type) { //selon le type de fichier associé à l'inode
    case ORDINAIRE:
      printf("Ordinaire\n");
      break;
    case REPERTOIRE:
      printf("Repertoire\n");
      break;
    case AUTRE:
      printf("Autre\n");
      break;
    default:
      printf("Inconnu\n");
  }
  printf("  taille : %ld octets\n", Taille(inode)); // affiche la taille du fichier

  // on stocke les valeurs de temps
  time_t dateAcces = DateDerAcces(inode);
  time_t dateModif = DateDerModif(inode);
  time_t dateModifInode = DateDerModifFichier(inode);

  // affiche les valeurs de temps selon variable
  printf("  date dernier accès : %s", ctime(&dateAcces));
  printf("  date dernière modification : %s", ctime(&dateModif));
  printf("  date dernière modification inode : %s", ctime(&dateModifInode));

  // affichage des données si elles existent
  if (Taille(inode)>0) {
    printf("  Données :\n");

    // calculer le nombre de blocs utilisés par le fichier
    int nbBlocsUtilises = (inode->taille + TAILLE_BLOC -1) / TAILLE_BLOC;

    // verif de pas dépasser le nb max de blocs autorisés
    if (nbBlocsUtilises > NB_BLOCS_DIRECTS) {
      nbBlocsUtilises = NB_BLOCS_DIRECTS;
    }

    // parcours chaque bloc utilisé
    for (int i = 0; i < nbBlocsUtilises; i++) {
      // verif que le bloc existe dans l'inode
      if (inode->blocDonnees[i] != NULL) {
        printf("  Bloc %d : ", i);

        // parcours chaque octet du bloc
        for (int j = 0; j < TAILLE_BLOC; j++) {
          // récupère l'octet à la position j dans bloc i
          unsigned char caractereActuel = inode->blocDonnees[i][j];

          // si le caractère est affichable (code ASCII entre 32 et 126)
          if (caractereActuel >= 32 && caractereActuel <= 126) {
            // affiche le caractère normalement
            printf("%c", caractereActuel);
          }
          // si l(octet nul (=0)
          else if (caractereActuel == 0) {
            break; // on sort de la boucle
          }
          // si le caractère n'est pas affichable
          else {
            printf("?"); // affiche un point d'interrogation si caractère indéchiffrable
          }
        }
        printf("\n");
      }
    }

    /* ANCIEN CODE V1
    // creer un espace temporaire en mémoire pour stocker les données (+1 octet pour le caractère de fin de chaîne '\0)
    unsigned char *buffer = (unsigned char*)malloc(Taille(inode)+1);
    // si allocation memoire à réussi
    if (buffer != NULL) {
      // copier les données depuis l'inode vers buffer
      long octetsLus = LireDonneesInode1bloc(inode, buffer, Taille(inode));
      //ajouter caractère '\0' en fin de chaine
      buffer[octetsLus] = '\0';
      // affichage des données
      printf("%s\n", buffer);
      // affiche le nombre d'octets lus
      printf("Nombre d'octets lus : %ld\n", octetsLus);
      // libère la memoire allouer pour buffer
      free(buffer);
    }*/
  }
}

/* V1
 * Copie à l'adresse contenu les taille octets stockés dans un inode.
 * Si taille est supérieure à la taille d’un bloc, seuls les TAILLE_BLOC premiers octets doivent être copiés.
 * Entrées : l'inode, l'adresse de la zone où recopier et la taille en octets de l'inode
 * Retour : le nombre d'octets effectivement lus dans l'inode ou -1 en cas d'erreur
 */
long LireDonneesInode1bloc(tInode inode, unsigned char *contenu, long taille) {
  // vérifie si inode et contenu pointent vers des zones de mémoires valides
  if (inode == NULL || contenu == NULL) {
    return -1; // return -1 en cas d'erreur
  }

  // verifie que l'inode contient des données
  if (Taille(inode) == 0) {
    return 0; // si aucune donnée alors return 0
  }

  // on prend le plus petit entre taille demandé par l'utilisation et TAILLE_BLOC (64)
  if (taille > TAILLE_BLOC) {
    taille = TAILLE_BLOC;
  }
  // vérifie que ça dépasse pas la taille max des données qui existent
  if (taille > inode->taille) {
    taille = inode->taille;
  }

  // on cherche le premier bloc de l'inode qui contient des données
  tBloc blocSource = NULL;
  for (int i = 0; i < 10; i++) {
    if (inode->blocDonnees[i] != NULL) {
      blocSource = inode->blocDonnees[i];
      break;
    }
  }

  // on utilise la fonction LireContenuBloc pour faire la copie
  long octetsLus = LireContenuBloc(blocSource, contenu, taille);

  // on met à jour la date d'accès
  mettreAJourDateAccess(inode);

  return octetsLus;
}

/* V1
 * Copie dans un inode les taille octets situés à l’adresse contenu.
 * Si taille est supérieure à la taille d’un bloc, seuls les TAILLE_BLOC premiers octets doivent être copiés.
 * Entrées : l'inode, l'adresse de la zone à recopier et sa taille en octets
 * Retour : le nombre d'octets effectivement écrits dans l'inode ou -1 en cas d'erreur
 */
long EcrireDonneesInode1bloc(tInode inode, unsigned char *contenu, long taille) {
  // vérifie si inode et contenu pointent vers des zones de mémoires valides
  if (inode == NULL || contenu == NULL) {
    return -1; // return -1 en cas d'erreur
  }

  // on prend le plus petit entre taille demandé par l'utilisation et TAILLE_BLOC (64)
  if (taille > TAILLE_BLOC) {
    taille = TAILLE_BLOC;
  }

  // on cherche un bloc déjà existant dans l'inode
  int indexBloc = -1;
  for (int i = 0; i < NB_BLOCS_DIRECTS; i++) {
    if (inode->blocDonnees[i] != NULL) {
      indexBloc = i; // on trouve un bloc dans l'inode déjà alloué
      break;
    }
  }

  // on crée un nouveau bloc s'il n'en existe pas
  if (indexBloc == -1) {
    for (int i = 0; i < NB_BLOCS_DIRECTS; i++) {
      if (inode->blocDonnees[i] == NULL) {
        indexBloc = i;
        break;
      }
    }

    // vérifie que l'emplacement trouvé est bien libre (si indexBloc reste à -1 alors tous les blocs sont déjà occupés
    if (indexBloc == -1) {
      return -1;
    }

    // cree un nouveau bloc
    inode->blocDonnees[indexBloc] = CreerBloc();
    if (inode->blocDonnees[indexBloc] == NULL) {
      return -1; // echec de la creation du bloc
    }
  }

  // on utilise la fonction EcrireContenuBloc pour faire la copie depuis contenu vers bloc
  long octetsEcris = EcrireContenuBloc(inode->blocDonnees[indexBloc], contenu, taille);

  // mettre a jour la taille de l'inode avec le nombre d'octets effectivement écrits
  inode->taille = octetsEcris;

  // mettre à jour les dates de modification
  mettreAJourDateModifContenu(inode);
  mettreAJourDateModifInode(inode);

  return octetsEcris;
}

/* V3
 * Lit les données d'un inode avec décalage, et les stocke à une adresse donnée
 * Entrées : l'inode d'où les données sont lues, la zone où recopier ces données, la taille en octets
 * des données à lire et le décalage à appliquer (voir énoncé)
 * Sortie : le nombre d'octets effectivement lus, 0 si le décalage est au-delà de la taille
 */
long LireDonneesInode(tInode inode, unsigned char *contenu, long taille, long decalage) {
  // erreur inode
  if (inode == NULL) {
    fprintf(stderr, "LireDonneesInode : l'inode n'existe pas\n");
    return -1;
  }

  // erreur contenu
  if (contenu == NULL) {
    fprintf(stderr, "LireDonneesInode : pas de zone où recopier les données\n");
    return -1;
  }

  // verif si le décalage dépasse la taille du fichier
  if (decalage >= inode->taille) {
    return 0;
  }

  long tailleDemandee = taille; // ce que l'utilisateur a demandé à lire
  long tailleALire = tailleDemandee; // ce qu'on va réellement lire

  // si decalage + taille demandée est supérieur à la taille du fichier
  if (decalage + tailleDemandee > inode->taille) {
    tailleALire = inode->taille - decalage;
  }

  // si rien à lire
  if (tailleALire <= 0) {
    return 0;
  }

  long totalOctetsLus = 0; // nombre total d'octets lus jusqu'à présent
  long positionActuelle = decalage; // position courante dans le fichier

  // lecture bloc par bloc
  while (totalOctetsLus < tailleALire) {
    // calcul du bloc courant et décalage dans ce bloc
    int numeroBloc = positionActuelle / TAILLE_BLOC; // numéro du bloc actuel (de 0 à 9)
    int decalageDansBloc = positionActuelle % TAILLE_BLOC; // position dans le bloc actuel (de 0 à 63)

    // verif que le bloc existe
    if (numeroBloc >= NB_BLOCS_DIRECTS || inode->blocDonnees[numeroBloc] == NULL) {
      break; // on arrete si bloc inexistant
    }

    long octetsRestants = tailleALire - totalOctetsLus; // ce qu'il reste à lire
    long espaceDansBloc = TAILLE_BLOC - decalageDansBloc; // espace disponible dans le bloc actuel
    long octetsALireDansBloc = (octetsRestants < espaceDansBloc) ? octetsRestants : espaceDansBloc; // le minimum entre les deux

    // lecture avec LireContenuBloc
    long octetsLusDansBloc = LireContenuBloc(
      inode->blocDonnees[numeroBloc], // bloc source où lire les données
      contenu + totalOctetsLus, // destination
      octetsALireDansBloc // nb d'octets à lire
    );

    // si aucun octet lus dans le bloc
    if (octetsLusDansBloc <= 0) {
      break;
    }

    totalOctetsLus += octetsLusDansBloc; // ajoute les octets lus au total
    positionActuelle += octetsLusDansBloc; // avance la position dans le fichier
  }

  // met à jour date d'accès
  mettreAJourDateAccess(inode);

  return totalOctetsLus;
}

/* V3
 * Ecrit dans un inode, avec décalage, ls données stockées à une adresse donnée
 * Entrées : l'inode où écrire le contenu, l'adesse de la zone depuis laquelle lire les données, la taille en octets
 * de ces données et le décalage à appliquer (voir énoncé)
 * Sortie : le nombre d'octets effectivement écrits, ou -1 en cas d'erreur
 */
long EcrireDonneesInode(tInode inode, unsigned char *contenu, long taille, long decalage) {
  // erreur inode
  if (inode == NULL) {
    fprintf(stderr, "EcrireDonneesInode : l'inode n'existe pas\n");
    return -1;
  }

  // erreur contenu
  if (contenu == NULL) {
    fprintf(stderr, "EcrireDonneesInode : pas de données à écrire\n");
    return -1;
  }

  // verif si le décalage dépasse la taille maximale du fichier
  if (decalage >= NB_BLOCS_DIRECTS * TAILLE_BLOC) {
    fprintf(stderr, "EcrireDonneesInode : décalage supérieur à la taille maximale\n");
    return -1;
  }

  long tailleDemandee = taille; // ce que l'utilisateur a demandé à écrire
  long tailleAEcrire = tailleDemandee; // ce qu'on va réellement écrire

  // si decalage + taille demandée est supérieur à la taille maximale du fichier
  if (decalage + tailleDemandee > NB_BLOCS_DIRECTS * TAILLE_BLOC) {
    tailleAEcrire = NB_BLOCS_DIRECTS * TAILLE_BLOC - decalage;
  }

  // si rien à écrire
  if (tailleAEcrire <= 0) {
    return 0;
  }

  long totalOctetsEcrits = 0; // nombre total d'octets écris jusqu'à présent
  long positionActuelle = decalage; // position courante dans le fichier

  // écriture bloc par bloc
  while (totalOctetsEcrits < tailleAEcrire) {
    // calcul du bloc courant et décalage dans ce bloc
    int numeroBloc = positionActuelle / TAILLE_BLOC; // numéro du bloc actuel (de 0 à 9)
    int decalageDansBloc = positionActuelle % TAILLE_BLOC; // position dans le bloc actuel (de 0 à 63)

    // verif que le numéro de bloc est valide
    if (numeroBloc >= NB_BLOCS_DIRECTS) {
      break; // on arrete si bloc inexistant
    }

    // si le bloc existe pas encore, on le crée
    if (inode->blocDonnees[numeroBloc] == NULL) {
      inode->blocDonnees[numeroBloc] = CreerBloc();
      // si erreur creation bloc
      if (inode->blocDonnees[numeroBloc] == NULL) {
        fprintf(stderr, "EcrireDonneesInode : impossible de créer le bloc %d\n", numeroBloc);
        break;
      }
    }

    long octetsRestants = tailleAEcrire - totalOctetsEcrits; // ce qu'il reste à écrire
    long espaceDansBloc = TAILLE_BLOC - decalageDansBloc; // espace disponible dans le bloc actuel
    long octetsAEcrireDansBloc = (octetsRestants < espaceDansBloc) ? octetsRestants : espaceDansBloc; // le minimum entre les deux

    // écriture avec EcrireContenuBloc
    long octetsEcritsDansBloc = EcrireContenuBloc(
      inode->blocDonnees[numeroBloc], // destination
      contenu + totalOctetsEcrits, // source
      octetsAEcrireDansBloc // nb d'octets à écrire
    );

    // si aucun octet écrit dans le bloc
    if (octetsEcritsDansBloc <= 0) {
      break;
    }

    totalOctetsEcrits += octetsEcritsDansBloc; // ajoute les octets écrits au total
    positionActuelle += octetsEcritsDansBloc; // avance la position dans le fichier
  }

  // met à juor taille du fichier
  if (decalage + totalOctetsEcrits > inode->taille) {
    inode->taille = decalage + totalOctetsEcrits;
  }

  // met à jour les dates
  mettreAJourDateModifContenu(inode);
  mettreAJourDateModifInode(inode);

  return totalOctetsEcrits;
}

/* V3
 * Sauvegarde toutes les informations contenues dans un inode dans un fichier (sur disque,
 * et préalablement ouvert en écriture et en mode binaire)
 * Entrées : l'inode concerné, l'identificateur du fichier
 * Sortie : 0 en cas de succès, -1 en cas d'erreur
 */
int SauvegarderInode(tInode inode, FILE *fichier) {
  // erreur inode
  if (inode == NULL) {
    fprintf(stderr, "SauvegarderInode : l'inode n'existe pas\n");
    return -1;
  }

  // erreur fichier
  if (fichier == NULL) {
    fprintf(stderr, "SauvegarderInode : le fichier n'existe pas\n");
    return -1;
  }

  // sauvegarde des métadonnées de l'inode champ par champ
  fwrite(&inode->numero, sizeof(unsigned int), 1, fichier);
  fwrite(&inode->type, sizeof(natureFichier), 1, fichier);
  fwrite(&inode->taille, sizeof(long), 1, fichier);
  fwrite(&inode->dateDerAcces, sizeof(time_t), 1, fichier);
  fwrite(&inode->dateDerModif, sizeof(time_t), 1, fichier);
  fwrite(&inode->dateDerModifInode, sizeof(time_t), 1, fichier);  

  // sauvegarde du contenu de chaque bloc utilisé
  int nombreBlocsUtilises = (inode->taille + TAILLE_BLOC - 1) / TAILLE_BLOC; // calcul le nombre de blocs utilisés
  if (nombreBlocsUtilises > NB_BLOCS_DIRECTS) {
    nombreBlocsUtilises = NB_BLOCS_DIRECTS; // petite vérification
  }

  // parcours tous les blocs utilisés
  for (int i = 0; i < nombreBlocsUtilises; i++) {
    // le bloc existe et contient des données
    if (inode->blocDonnees[i] != NULL) {
      // sauvegarde du bloc
      int resultatSauvegarde = SauvegarderBloc(inode->blocDonnees[i], TAILLE_BLOC, fichier);

      // erreur de sauvegarde
      if (resultatSauvegarde != 0) {
        fprintf(stderr, "SauvegarderInode : erreur sauvegarde bloc %d\n", i);
        return -1;
      }
    } else { // le bloc n'existe pas
      // cn crée un bloc temporaire vide
      tBloc blocVide = CreerBloc();

      // erreur création bloc
      if (blocVide == NULL) {
        fprintf(stderr, "SauvegarderInode : erreur création bloc vide %d", i);
        return -1;
      }

      // sauvegarde du bloc vide
      int resultatSauvegarde = SauvegarderBloc(blocVide, TAILLE_BLOC, fichier);

      // on détruit le bloc temporaire
      DetruireBloc(&blocVide);

      // erreur sauvegarde du bloc vide
      if (resultatSauvegarde != 0) {
        fprintf(stderr, "SauvegarderInode : erreur savegarde bloc vide %d", i);
        return -1;
      }
    }
  }
  // forcer écriture des données
  if (fflush(fichier) != 0) {
    fprintf(stderr, "SauvergarderInode : erreur lors du fflush\n");
    return -1;
  }

  return 0;
}

/* V3
 * Charge toutes les informations d'un inode à partir d'un fichier (sur disque,
 * et préalablement ouvert en lecture et en mode binaire)
 * Entrées : l'inode concerné, l'identificateur du fichier
 * Sortie : 0 en cas de succès, -1 en cas d'erreur
 */
int ChargerInode(tInode *pInode, FILE *fichier) {
  // erreur pointeur inode
  if (pInode == NULL) {
    fprintf(stderr, "ChargerInode : le pointeur d'inode n'existe pas\n");
    return -1;
  }

  // erreur fichier
  if (fichier == NULL) {
    fprintf(stderr, "ChargerInode : le fichier n'existe pas\n");
    return -1;
  }

  // on crée un inode temporaire
  tInode inodeTemporaire = (tInode)malloc(sizeof(struct sInode));

  // erreur allouer inode temporaire
  if (inodeTemporaire == NULL) {
    fprintf(stderr, "ChargerInode : impossible d'allouer l'inode");
    return -1;
  }

  // chargement des métadonnées de l'inode depuis le fichier champ par champ
  fread(&inodeTemporaire->numero, sizeof(unsigned int), 1, fichier);
  fread(&inodeTemporaire->type, sizeof(natureFichier), 1, fichier);
  fread(&inodeTemporaire->taille, sizeof(long), 1, fichier);
  fread(&inodeTemporaire->dateDerAcces, sizeof(time_t), 1, fichier);
  fread(&inodeTemporaire->dateDerModif, sizeof(time_t), 1, fichier);
  fread(&inodeTemporaire->dateDerModifInode, sizeof(time_t), 1, fichier);  

  // initialisation des pointeurs de blocs
  for (int i = 0; i < NB_BLOCS_DIRECTS; i++) {
    inodeTemporaire->blocDonnees[i] = NULL;
  }

  // chargement du contenu de chaque bloc utilisé
  int nombreBlocsACharger = (inodeTemporaire->taille + TAILLE_BLOC - 1) / TAILLE_BLOC; // calcul le nombre de blocs à charger
  if (nombreBlocsACharger > NB_BLOCS_DIRECTS) {
    nombreBlocsACharger = NB_BLOCS_DIRECTS; // petite vérification
  }

  // parcours tous les blocs
  for (int i = 0; i < nombreBlocsACharger; i++) {
    // création d'un nouveau bloc
    inodeTemporaire->blocDonnees[i] = CreerBloc();

    // erreur création bloc
    if (inodeTemporaire->blocDonnees[i] == NULL) {
      fprintf(stderr, "ChargerInode : erreur création bloc %d\n", i);
      DetruireInode(&inodeTemporaire); // on détruit l'inode
      return -1;
    }

    // charger données bloc vers fichier
    int resultatChargement = ChargerBloc(inodeTemporaire->blocDonnees[i], TAILLE_BLOC, fichier);

    // erreur chargement bloc
    if (resultatChargement != 0) {
      fprintf(stderr, "ChargerInode : erreur chargement bloc %d\n", i);
      DetruireInode(&inodeTemporaire); // on détruit l'inode
      return -1;
    }
  }
  
  // attribue l'inode au pointeur
  *pInode = inodeTemporaire;

  return 0;
}

/* V4
 * Retourne la taille maximale d'un fichier contenu dans un inode.
 * Entrée : l'inode concerné,
 * Sortie : la taille du plus grand fichier de cet inode
 */
long TailleMaxFichier(void) {
  return NB_BLOCS_DIRECTS * TAILLE_BLOC;
}