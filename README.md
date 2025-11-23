📚 Projet C – Analyse de graphe de Markov

Bienvenue dans notre projet réalisé dans le cadre du cours de programmation en C à l'Efrei Paris.
Le but de ce projet était de manipuler un graphe orienté pondéré, de vérifier s’il s’agit bien d’un graphe de Markov, de l’exporter visuellement, d’identifier certains types d’états, et de simuler un comportement à travers une marche aléatoire.

🧠 Ce qu’on a fait

Tout le projet repose sur un graphe représenté sous forme de listes d’adjacence. On a tout d’abord commencé par lire ce graphe depuis un fichier texte, puis on l’a affiché pour vérifier visuellement sa structure.
Ensuite, on a vérifié si ce graphe respecte bien les propriétés d’un graphe de Markov (par exemple que la somme des probabilités pour chaque sommet vaut 1). Si ce n’était pas le cas, on l’indiquait clairement à l’utilisateur.
Une fois que c’était validé, on a exporté le graphe en format Mermaid, ce qui permet d’avoir un aperçu graphique du graphe dans un outil compatible.
Ensuite, on a recherché les états absorbants (des sommets sans issue), puis on a aussi affiché les états accessibles à partir d’un sommet donné. C’est pratique pour mieux comprendre comment le graphe est structuré.
Et enfin, on a simulé une marche aléatoire dans ce graphe, en suivant les probabilités de transition. Ça donne un aperçu du comportement dynamique d’un tel système.

🛠️ Comment exécuter le projet

Cloner le projet depuis GitHub.

Ouvrir le dossier avec CLion.

Lancer l’exécution depuis le fichier main.c.

Un fichier texte (comme exemple1.txt) est déjà fourni dans le dossier data/, mais vous pouvez aussi créer le vôtre.


📂 Structure du projet

main.c → le point d’entrée du programme

graph.c / graph.h → toutes les fonctions utiles pour gérer le graphe

data/ → contient les fichiers textes de graphes à tester

README.md → ce fichier que vous êtes en train de lire :)



👥 Réalisé par :  
- Nada Lemdjadi  
- Hamada Sbai
