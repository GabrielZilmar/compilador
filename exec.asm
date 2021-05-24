sseg SEGMENT STACK ;início seg. pilha
byte 4000h DUP(?) ;dimensiona pilha
sseg ENDS ;fim seg. pilha

dseg SEGMENT PUBLIC ;início seg. dados
byte 4000h DUP(?) ;temporários
byte ?
sword ?
sword ?
sword -3
sword ?
byte 21 DUP(?)
sword 7
dseg ENDS ;fim seg. dados

cseg SEGMENT PUBLIC ;início seg. código
ASSUME CS:cseg, DS:dseg
strt:
mov ax, dseg
mov ds, ax
dseg SEGMENT PUBLIC
byte "ola$"
dseg ENDS
mov DI, 0
mov SI, 3
R1:
mov DX, DS:[DI]
cmp DX, 024h
je R4
jmp R2
R3:
add DI, 1
add SI, 1
jmp R1
R2:
mov DX, DS:[SI]
mov DS:[DI], DX
jmp R3
R4:
mov AX, 'a'
mov DS:[0], AX 
mov AX, DS:[0]
mov DS:[0], AX
mov AX, 30
mov DS:[0], AX 
mov AX, DS:[0]
mov DS:[0], AX
mov AX, 0
mov DS:[0], AX 
mov AX, DS:[0]
mov DS:[16387], AX
R5:
mov AX, 10
mov DS:[0], AX 
mov AX, DS:[0]
mov BX, DS:[0]
mov AH, 0
mov BH, 0
cmp AX, BX
jle R9
mov AX, 0
jmp R10
R9:
mov AX, 1
R10:
mov DS:[2], AX
mov AX, DS:[2]
cmp AX, 1
je R7
jmp R6
R8:
mov AX, 1
mov DS:[4], AX 
mov AX, DS:[16387]
mov BX, DS:[4]
add AX, BX
mov DS:[6], AX
mov AX, DS:[0]
mov DS:[16387], AX
jmp R5
R7:
mov AX, 4
mov DS:[0], AX 
mov AX, DS:[16389]
mov BX, DS:[0]
add AX, BX
mov DS:[2], AX
mov AX, DS:[0]
mov DS:[16389], AX
jmp R8
R6:
mov ah, 4Ch
int 21h
cseg ENDS ;fim seg. código
END strt ;fim programa
