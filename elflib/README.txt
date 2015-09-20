Chargeur de fichier ELF 

Le chargeur utilise la libelf. Sur Linux, il faut donc installer le pakage : libelf-dev. Sous mac il semble que la version libelf-0.8.6 fonctionne

pour compiler
$ make

pour nettoyer
$ make clean

pour executer

test_read_elf_file fichier.o

des fichiers assembleurs et objets se trouvent sur le site web du projet

