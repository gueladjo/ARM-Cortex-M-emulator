simpleUnitTest.sh : script de test "unitaire" "boite noire" (ou "externe") minimal.

########################################
# Creation Aout 2012
# Update September 2015
########################################
# No restriction on usage nor dissemination
########################################
# Problèmes, commentaires : nicolas.castagne@phelma.grenoble-inp.fr
########################################

########################################
# Lancement rapide des tests de l'incrément 1 du projet EMULATEUR
########################################
1/ se placer dans le figure l'exécutable a tester, par exemple le répertoire interpreteur
2/ entrer, par exemple
	../testing/simpleUnitTest.sh -e ./emul-arm test/exit/*.emu

Ou : 
	../testing/simpleUnitTest.sh est le chemin vers script de test
	./emul-arm est le chemin vers l'executable a tester (l'émulateur arm ici) 
	test/exit/*.emu  designe l'ensemble des fichiers de la commande 'exit' de l'émulateur à tester

Pour lancer en mode batch : option -b :
	../testing/simpleUnitTest.sh -e ./emulMips -b test/exit/*.emu


Voir la suite pour des détails.


########################################
# Introduction
########################################
Le script lance un <executable> sur une liste de fichiers de <test>, l'ensemble étant passé en argument du script.


Chaque fichier de <test>.emu doit contenir :
1/ des commandes de l'émulateur (une par ligne)
2/ éventuellement des commentaires (tout ce qui est après #)
3/ en première ligne, la variable #TEST_RETURN_CODE=X ou X est dans {FAIL,PASS}           
4/ éventuellement en deuxième ligne #TEST_COMMENT="Teste fonctionnement standard" qui s'affichera lors des tests


TEST_RETURN_CODE, which values should be
	PASS if the test should pass without an error code (zero)
	FAIL if the test should generate an error code (non zero)

TEST_COMMENT (optional)
	a comment string describing the test


Example: here is a valid header in a test file :
#-------
TEST_RETURN_CODE=PASS		# the test should pass w/o error
TEST_COMMENT="Test empty file"	# this comment will be displayed before performing the test
#-------


Pour chacun des fichiers de <test> passé en argument, le script : 
	- lance l'<executable> en lui passant <test> en argument
	- détecte si l'<executable> a "planté" (segmentation fault, etc.)
	- détecte si le code de sortie renvoyé par l'<exécutable> correspond au code erreur attendu (conformement à 

Enfin, le script génère un rapport de test avec les résultats de tous les tests.

Pour les options et l'aide taper : 
  ../testing/simpleUnitTest.sh -h

