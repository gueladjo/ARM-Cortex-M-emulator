# allons au ru
.syntax unified

.text
    MOV r0,#8
    ldr r2,=lunchtime
    ldr r1, [r2]
boucle:
    CMP r0,r1
    ITE NE
    addne r0 , #1
    BLEQ byebye
    B boucle   
      
byebye:
    svc #0 

.bss
tableau: .space 16

.data
debut_cours: .word 8
lunchtime: .word 12
