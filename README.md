# Simualtion de billard avec OpenGL
##Projet du cours Graphisme et visualisation au MPRI
Année 2021-2022

**Billard en 2D et en 3D (sans gravité)**

### Compiler
Avec CMake. 
####Pour Windows avec Visual Studio
- Changer le répertoire de sortie (rubrique Général) pour qu'il soit dans le dossier racine 
(retirer les deux derniers composants du chemin par défaut).
- Changer dans la rubrique Débogage le répertoire de travail en ```$(TargetDir)```.
- Déboguer avec la configuration RelWithDebInfo.

###Structure du code
La documentation est à la fois en anglais et en français pour le moment.

La branche Refactor est un essai d'utilisation de l'héritage dans nos structures, 
permettant une meilleure modularité. Cependant des problèmes sont survenus
au terme du projet vis-à-vis de cette refactorisation. Elle a donc été 
mise de côté et constitue une évolution souhaitable.
Le code actuel est par conséquent assez peu modulaire (la duplication de certaines 
parties a été nécessaire vu le problème de refactorisation). Pour le moment,
utilisez la macro de préprocesseur #define TROIS_DIMENSION dans le fichier main.cpp en 
recompilant tous les fichiers pour compiler la version 3D du billard.
