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
	MOV t0, AX
	MOV AX, t0
	MOV SI, ah
	ADD SI, SI
	MOV array[SI], AX
	MOV SI, i
	ADD SI, SI
	MOV AX, array[SI]
	MOV BX, ah
	CMP AX, BX
	JB E0
	MOV t1, 1h
	JMP E1
E0:
	MOV t1, 0h
E1:
	MOV BX, t1
	CMP BX, 0h
	JE E2
	MOV A, 0h
	JMP E3
	MOV AX, A
	ADD AX, 1h
	MOV A, AX
E3:
	MOV AX, A
	MOV BX, D
	CMP AX, BX
	JA E4
	MOV AX, D
	MUL AX, A
	MOV t2, AX
	MOV AX, t2
	MOV A, AX
	JMP E5
E4:
	JMP E6
E2:
	MOV AX, A
	MOV BX, fh
	CMP AX, BX
	JA E7
	MOV t3, 1h
	JMP E8
E7:
	MOV t3, 0h
E8:
	MOV AX, A
	MOV BX, 0h
	CMP AX, BX
	JB E9
	MOV t4, 1h
	JMP E10
E9:
	MOV t4, 0h
E10:
	MOV AX, t3
	MUL AX, t4
	MOV t5, AX
	MOV BX, t5
	CMP BX, 0h
	JE E11
	MOV t6, 1h
	JMP E12
E11:
	MOV t6, 0h
E12:
	MOV BX, t6
	CMP BX, 0h
	JE E13
	MOV AX, A
	SUB AX, 4h
	MOV t7, AX
	MOV BX, t7
	CMP BX, 0h
	JNE E14
	MOV t8, 1h
	JMP E15
E14:
	MOV t8, 0h
E15:
	MOV BX, t8
	CMP BX, 0h
	JE E16
	MOV B, 63h
	MOV AX, B
	SUB AX, D
	MOV t9, AX
	MOV AX, t9
	SUB AX, fh
	MOV t10, AX
	MOV BX, t10
	CMP BX, 0h
	JNE E17
	MOV t11, 1h
	JMP E18
E17:
	MOV t11, 0h
E18:
	MOV BX, t11
	CMP BX, 0h
	JE E19
	MOV B, 0h
	JMP E16
E19:
	MOV A, 0h
E16:
	JMP E2
E13:
	MOV AX, A
	ADD AX, 1h
	MOV t12, AX
	MOV AX, t12
	MOV A, AX
E6:
	MOV AH, 4Ch
	INT 21h
	Pgm ENDS
END Pgm