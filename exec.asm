sseg SEGMENT STACK ;início seg. pilha
byte 4000h DUP(?) ;dimensiona pilha
sseg ENDS ;fim seg. pilha

dseg SEGMENT PUBLIC ;início seg. dados
byte 4000h DUP(?) ;temporários
byte 10 DUP(?)
dseg ENDS ;fim seg. dados

cseg SEGMENT PUBLIC ;início seg. código
ASSUME CS:cseg, DS:dseg
strt:
mov ax, dseg
mov ds, ax
mov AX, 0
mov DS:[0], AX 
mov AX, 1
mov DS:[2], AX 
mov AX, 'b'
mov DS:[4], AX 
dseg SEGMENT PUBLIC
byte "ab$"
dseg ENDS
