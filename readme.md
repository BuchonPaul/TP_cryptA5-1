# Titre du Compte Rendu

**Auteurs** : BUCHON Paul / EPPELE Ferdinand  
**Date** : 03-04-2025  
---
## Execution du projet:

Avant de Run le projet pour la première fois:
- ```make clean; make libs```

Pour run le projet:
- ```make clean; make; ./build/main```

## 1. Implémentation de A5/1  
### 1.1 Initialisation de LSFR
La première étape du TP a été le développement du protocole de chiffrement A5/1. \
Pour implémenter ce protocole nous avons choisi de stocker les 3 LSFR sur 3 entiers.

La fonction ```initialize_registers``` initialise trois registres à décalage (L1, L2 et L3) en fonction de la clé de session et du compteur de trame. Cette initialisation se fait en trois étapes :

- Chargement de la clé de session (64 bits) dans les registres.

- Chargement du compteur de trame (22 bits).

- Exécution de 100 cycles d'initialisation avec le critère de majorité pour synchroniser les registres.

### 1.2 Chiffrement du message
La fonction ```encrypt_decrypt_message``` effectue le chiffrement et le déchiffrement d'un message en :

- Générant un octet dans un buffer à partir de 8 bit donné par le LSFR.

- Effectuant un XOR entre chaque octet du message et le buffer de chiffrement.

- Réinitialisant les registres pour chaque caractère avec une valeur de compteur de trame incrémentée

- Cette approche permet un chiffrement et un déchiffrement symétriques en utilisant le même algorithme.


## 2. Génération des clefs publiques et privées

### 1. Génération des clefs publiques et privées
La première partie du programme consiste à :
- Générer un grand nombre premier p_512 de 512 bits en utilisant ```generate_prime```.
- Réduire p_512 à une version 64 bits pour servir de clef à A5/1.
- Fixer un générateur g = 3.
- Définir les clefs privées d'Alice (a = 6) et de Bob (b = 15).
- Calculer leurs clefs publiques respectives A = g^a mod p et B = g^b mod p.

### 2. Calcul du secret partagé

Alice et Bob utilisent les valeurs publiques échangées pour dériver un secret partagé :
- Alice calcule secret_Alice = B^a mod p
- Bob calcule secret_Bob = A^b mod p
- Ces deux valeurs sont identiques et servent de clé de session pour chiffrer les messages.

### 3. Chiffrement et transmission du message

Alice chiffre son message avec ```encrypt_decrypt_message``` en utilisant la clé de session.

- Le message chiffré est affiché sous forme binaire et en texte brut.

- Un attaquant hypothétique (Eve) intercepte le message mais ne peut pas le déchiffrer sans la clé.

### 4. Déchiffrement du message par Bob

- Bob utilise ```encrypt_decrypt_message``` avec la même clé de session pour récupérer le message original.


## 3. Choix d'implémentation et problèmes rencontrés

### Choix Technique:
Pour ce projet, nous avons choisi le C notamment car ce .

### A5/1

Initialement, nous avions une implémentation très redondante avec beaucoup de lignes de code similaires. Nous avons ensuitre pu simplifié le code en factorisant les parties qui se répétaient, ce qui a amélioré la lisibilité et l'efficacité.

Un bug possible dans le protocole, c'est que le caractère XORé avec le buffer de bits généré, donne 00000000. Dans ce cas là string est tronqué car on à le char \O qui a été généré. On pourrait géré ce cas mais ça complixifierai le code.

### Diffie-Hellman

Nous avons d'abord réalisé une version fonctionnelle de l'algorithme en 64 bits en utilisant uint64_t. Une fois cette version fonctionnelle, nous avons intégré la bibliothèque GMP pour permettre un fonctionnement sur 512 bits. Pour cela, nous avons traduit ligne par ligne nps algorithmes en 64 bits en utilisant les fonctions GMP (à la place des opérations sur les entiers) afin d'assurer la compatibilité et la gestion des grands nombres.

La traduction du code en gmp a pris beaucoup de temps car toutes les opérations les plus élémentaires demandaient de trouver et d'implémenter les fonction de gmp.

### Améliorations possibles

Les améliorations possibles seraient de rendre interactif le choix des clés pour Alice et de lui permettre de saisir son propre message.

De plus, pour le générateur, nous avons choisi arbitrairement g=3g=3. En effet, nous n'avons pas eu le temps d'implémenter un algorithme de génération en C.


