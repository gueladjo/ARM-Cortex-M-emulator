# programme contenant toutes les relocations étudiées lors du projet
.syntax unified

.text
    ldr r2,=lunchtime
    ldr r1, [r2]
    add r1,r1,r1   
    mov r4, #0
    sub r4,#4    
    ldr r3, =resultat  
    str r1, [r3,r4]    @ on ecrit à l'adresse de resultat-4 soit octet1 qui doit contenir 0x18
    svc #0 

.data
debut_cours: .word 8
lunchtime: .word 12
octet1: .byte lunchtime
octet2: .byte 0xFF, lunchtime,debut_cours
resultat: .word debut_cours
