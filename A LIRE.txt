Exemple d'interpréteur

L'exemple constitue un "Exemple". Il est donné à titre d'illustration et ce n'est donc pas parce que "c'est le prof" qui le donne qu'il faut l'utiliser tel quel sans le comprendre (voir notamment le traitement des hexa). Cela dit, il constitue une bonne base pour démarrer le projet. 
 
L'interpréteur utilise libreadline. Sur Linux, il faut donc installer le package de développement qui convient. Il apparaît que l'appel à lreadline n'est pas toujours suffisant pour compiler et nécessite parfois d'ajouter -lcurses selon les versions et l'état de votre OS. Ne pas hésiter à chercher sur le net ou nous consulter si vous avez des problèmes d'installation. 

pour compiler la version debuggable 
$ make debug 

pour générer la documentation
$ make doc

pour nettoyer
$ make clean

pour créer l'archive à envoyer à votre tuteur
$ make archive


pour exécuter

  emul-arm [fichier de scripts] 



les sources se trouvent dans
./src/

les includes dans 
./include/ 

Les fichiers de script de test se trouvent dans 
./tests/

Les rapports à nous envoyer se trouvent dans 
./rapports/
