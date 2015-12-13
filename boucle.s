@ programme qui fait une boucle en incrementant un registre

.syntax unified
_start:
.global _start

mov r1,#12   @ set r1 a 12
mov r2,#0    @ set r2 à zéro
inc:
add r2,r2,#1 @ incremente
cmp r1,r2    @ test entre r1 et r2 sont different 
bne inc      @ boucle sur l'increment tant que r1 et r2 sont differents

.data 
X: .word 4
