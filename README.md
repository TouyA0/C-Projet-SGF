# 🗂️ Simulateur de Système de Fichiers - Projet C
*Une implémentation en C d'un système de fichiers de type Unix pour l'apprentissage*

## 📋 Table des matières
- [🚀 Aperçu du projet](#-aperçu-du-projet)
- [📁 Structure du projet](#-structure-du-projet)
- [🛠️ Fonctionnalités par niveau](#️-fonctionnalités-par-niveau)
- [🏗️ Architecture technique](#️-architecture-technique)
- [📦 Compilation et exécution](#-compilation-et-exécution)
- [📄 Licence et crédits](#-licence-et-crédits)

## 🚀 Aperçu du projet

Un simulateur complet de système de fichiers écrit en C qui implémente les concepts fondamentaux d'un système de fichiers Unix :  
- Stockage de fichiers basé sur les inodes  
- Gestion d'allocation de blocs   
- Structure de répertoires  
- Opérations de sauvegarde/restauration  
- Architecture modulaire avec encapsulation  

## 📁 Structure du projet

```
projet/  
├── bloc.c           # Implémentation du module "bloc" : création, destruction, lecture/écriture  
├── bloc.h           # Interface publique du module "bloc" : types et prototypes  
├── inode.c          # Implémentation du module "inode" : gestion des nœuds d’index  
├── inode.h          # Interface publique du module "inode" : structures et fonctions  
├── sf.c             # Implémentation du module "système de fichiers" : SF simplifié  
├── sf.h             # Interface publique du module "SF" : superbloc et liste d’inodes  
├── repertoire.c     # Implémentation du module "répertoire" : table d’entrées (niveau 4)  
├── repertoire.h     # Interface publique du module "répertoire"  
│  
├── gerer_sf_v1.c    # Programme principal – niveau 1 : test des modules bloc et inode  
├── gerer_sf_v2.c    # Programme principal – niveau 2 : SF avec fichiers ≤ 1 bloc  
├── gerer_sf_v3.c    # Programme principal – niveau 3 : fichiers > 1 bloc + sauvegarde  
├── gerer_sf_v4.c    # Programme principal – niveau 4 : gestion du répertoire racine  
│  
├── Makefile1        # Génère l’exécutable pour le niveau 1  
├── Makefile2        # Génère l’exécutable pour le niveau 2  
├── Makefile3        # Génère l’exécutable pour le niveau 3  
├── Makefile4        # Génère l’exécutable pour le niveau 4  
│  
├── niveau_1.txt     # Explications/choix de conception pour le niveau 1  
├── niveau_2.txt     # Explications/choix de conception pour le niveau 2  
├── niveau_3.txt     # Explications/choix de conception pour le niveau 3  
└── niveau_4.txt     # Explications/choix de conception pour le niveau 4  
```

## 🛠️ Fonctionnalités principales par niveau

### 📊 Niveau 1 : Gestion basique des inodes
| Fonctionnalité | Description | Statut |  
|----------------|------------------|-------------|  
| `CreerBloc()`/`DetruireBloc()` | Création et destruction de blocs | ✅ |  
| `CreerInode()`/`DetruireInode()` | Gestion du cycle de vie des inodes | ✅ |  
| `EcrireContenuBloc()`/`LireContenuBloc()` | Lecture/écriture dans les blocs | ✅ |  
| `AfficherInode()` | Affichage formaté des métadonnées | ✅ |  

### 🗂️ Niveau 2 : Système de fichiers complet
| Fonctionnalité | Description | Statut |
|----------------|------------------|-------------|
| `CreerSF()`/`DetruireSF()` | Initialisation du système de fichiers | ✅ |
| Super-bloc | Stockage des métadonnées système | ✅ |
| Liste chaînée d'inodes | Organisation des fichiers | ✅ |
| `Ecrire1BlocFichierSF()` | Ajout de fichiers ≤ 64 octets | ✅ |

### 💾 Niveau 3 : Fichiers avancés + sauvegarde
| Fonctionnalité | Description | Statut |
|---------------|------------------|-------------|
| Fichiers multi-blocs | Support jusqu'à 640 octets (10 blocs) | ✅ |
| `EcrireDonneesInode()` avec décalage | Écriture à position spécifique | ✅ |
| `SauvegarderSF()`/`ChargerSF()` | Persistance sur disque | ✅ |
| `SauvegarderInode()`/`ChargerInode()` | Sauvegarde individuelle | ✅ |

### 📂 Niveau 4 : Gestion des répertoires
| Fonctionnalité | Description | Statut |
|----------------|------------------|-------------|
| Répertoire racine `/` | Premier inode du système | ✅ |
| `CreerRepertoire()`/`DetruireRepertoire()` | Gestion des répertoires | ✅ |
| `ls()` avec/sans détails | Listing formaté | ✅ |
| Table d'entrées | Association nom ↔ numéro d'inode | ✅ |
| Intégration complète | Coordination inode ↔ répertoire | ✅ |

## 🔧 Spécifications techniques
- **Taille des blocs** : 64 octets (configurable via `TAILLE_BLOC`)
- **Nombre max de blocs directs** : 10 (configurable via `NB_BLOCS_DIRECTS`)
- **Taille max d'un fichier** : 640 octets (10 × 64)
- **Taille max d'un nom de fichier** : 24 caractères
- **Nombre max d'entrées de répertoire** : 22 (calculé dynamiquement)
- **Encodage** : UTF-8
- **Compatibilité** : C99 standard

## 📦 Compilation et exécution

### 🛠️ Commande de compilation
Pour compiler un niveau spécifique :

```bash
# Niveau 1
make -f Makefile1

# Niveau 2
make -f Makefile2

# Niveau 3
make -f Makefile3

# Niveau 4
make -f Makefile4
```

### ▶️ Exécution
```bash
# Après compilation
./gerer_sf_v1    # Niveau 1
./gerer_sf_v2    # Niveau 2
./gerer_sf_v3    # Niveau 3
./gerer_sf_v4    # Niveau 4
```

### 🧹 Nettoyage
Pour nettoyer un niveau spécifique :

```bash
# Niveau 1
make -f Makefile1 clean

# Niveau 2
make -f Makefile2 clean

# Niveau 3
make -f Makefile3 clean

# Niveau 4
make -f Makefile4 clean
```
