@ exemple de code ARM qui calcule la représentation minuscule 
@ d'une chaine statique (rodata) de caracteres majuscules  


.syntax unified

.text
	ldr	r3, =.majuscule    @stocke l'@ de majuscule dans r3 
	ldr	r4, =.minuscule    @stocke l'@ de minuscule dans r4 	
	mov r5,#24             @ nombre de caracteres
	
.for:
	cmp r5,#0
	beq .fin
	ldr r7,[r3]            @ copie 4 caracteres dans r7
	add r7,r7,#0x20202020  @ ajout 0x20 pour 4 caracteres d'affilé
	str r7,[r4]            @ sauve r7 dans minuscule
	add r3,#4              @ on decale les pointeurs
	add r4,#4
	sub r5,#4              
	b	.for
	
.fin:
  svc #0                 @ dans le projet, on sort (exit) avec ce code
  
.section	.rodata
  .majuscule:
	 .ascii	"ABCDEFGHIJKLMNOPQRSTUVWX\000"  @chaine tronquée pour être multiple de 4

.bss 
  .minuscule: .space 30  @ espace où sera stockée la chaine des minuscules
