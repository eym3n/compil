Pile segment stack ;
	dw 100 dup(?)
Pile ends
DATA segment
	A         DW	?
	array     DW	54	DUP(?)
	B         DW	?
	C         DW	?
	D         DW	?
	E         fff2h
	F         c180h
	i         DW	?
	Z         DW	?
	t0        DW	?
	t1        DW	?
	t2        DW	?
DATA ends
CODE SEGMENT
	Pgm:
	ASSUME CS:CODE, DS:DATA
	MOV AX,DATA
	MOV DS, AX
	MOV SI, A
	ADD SI, SI
	MOV AX, array[SI]
	MOV SI, 2h
	ADD SI, SI
	ADD AX, array[SI]
	MOV SI, ah
	ADD SI, SI
	MOV array[SI], AX
	MOV A, 1h
	MOV t0, AX
	MOV AX, A
	ADD AX, 1h
	MOV B, AX
	MOV B, AX
	MOV AX, t1
	MOV t2, AX
	MOV Z, AX
	MOV AH, 4Ch
	INT 21h
CODE ENDS
END Pgm