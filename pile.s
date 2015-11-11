@ programme qui fait appel a une procedure (BL) et utilise la pile
@ se contente de verifier si un decalage à gauche puis à droite donne la valeur initiale

.syntax unified
_start:
.global _start

mov r0,#0             @ mise à jour des valeurs des registres
mov r1,#8
mov r2,r1
ADD r1,r0,r1,LSL #1   @ décalage effectué à travers le barrel shifter
BL  procedure         @ appel explicite à l'adresse de la procedure  
CMP r2,r1
BEQ OK                @ branchement à OK si les deux valeurs sont égales
B KO 						      @ sinon saut à KO


procedure:
 push {r0,lr}         @ on stocke les registres sur lesquels la procedure 
                      @ agit (ici r0 seulement) et on stocke l'adresse de retour
 SUB sp,sp,#8         @ on decremente le pointeur de pile
 
 mov r0,#0
 ADD r1,r0,r1,LSR #1
 
 pop {r0,lr}          @ on recupere les registres sur lesquels la procedure agit
                      @ et on recupere l'adresse de retour
 ADD sp,sp,#8         @on decremente le pointeur de pile
 BX lr

OK:
	mov r0,#0
 SVC #0
KO:
	mov r0,#1
 SVC #0
 
.data 
chaine: .ascii "Youpi"
