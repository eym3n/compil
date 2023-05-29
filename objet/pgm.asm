Pile segment stack ;
	dw 100 dup(?)
Pile ends
DATA segment
	A         DW	?
	B         DW	?
	C         DW	?
	D         DW	?
	E         DW	?
	t0        DW	?
	t1        DW	?
	t2        DW	?
	t3        DW	?
	t4        DW	?
	t5        DW	?
	t6        DW	?
	t7        DW	?
	t8        DW	?
	t9        DW	?
	t10       DW	?
	t11       DW	?
	t12       DW	?
	t13       DW	?
	t14       DW	?
	t15       DW	?
	t16       DW	?
	t17       DW	?
	t18       DW	?
	t19       DW	?
DATA ends
CODE SEGMENT
	Pgm:
	ASSUME CS:CODE, DS:DATA
	MOV AX,DATA
	MOV DS, AX
	MOV A, 1h
	MOV AX, 1h
	ADD AX, 1h
	MOV B, AX
	MOV B, AX
	MOV AX, t0
	MOV C, AX
	MOV D, 1h
	MOV C, AX
	MOV AX, 1h
	ADD AX, 2h
	MOV Pgm, AX
	MOV AH, 4Ch
	INT 21h
CODE ENDS
END Pgm