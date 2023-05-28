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
	MOV SI, A
	ADD SI, SI
	MOV AX, array[SI]
	MOV SI, 2h
	ADD SI, SI
	ADD AX, array[SI]
	MOV SI, ah
	ADD SI, SI
	MOV array[SI], AX
	MOV t0, AX
	MOV AX, fh
	DIV AX, 3h
	ADD AX, B
	MOV t2, AX
	MOV AX, C
	SUB AX, ah
	DIV AX, C
	MOV t4, AX
	MOV AX, t2
	MUL AX, t4
	MOV t5, AX
	MOV AX, D
	DIV AX, 2h
	MOV t6, AX
	MOV AX, t5
	ADD AX, t6
	MOV A, AX
	MOV SI, i
	ADD SI, SI
	MOV AX, array[SI]
	MOV BX, ah
	CMP AX, BX
	JB E0
	MOV t8, 1h
	JMP E1
E0:
	MOV t8, 0h
E1:
	MOV BX, t8
	CMP BX, 0h
	JE E2
	MOV A, 0h
	JMP E3
	ADD AX, 1h
E3:
	MOV AX, A
	MOV BX, D
	CMP AX, BX
	JA E4
	MOV A, AX
	MOV AX, D
	MUL AX, A
	MOV A, AX
	JMP E5
E4:
	JMP E6
E2:
	MOV AX, A
	MOV BX, fh
	CMP AX, BX
	JA E7
	MOV t10, 1h
	JMP E8
E7:
	MOV t10, 0h
E8:
	MOV AX, A
	MOV BX, 0h
	CMP AX, BX
	JB E9
	MOV t11, 1h
	JMP E10
E9:
	MOV t11, 0h
E10:
	MOV A, AX
	MOV AX, t10
	MUL AX, t11
	MOV BX, t12
	CMP BX, 0h
	JE E11
	MOV t13, 1h
	JMP E12
E11:
	MOV t13, 0h
E12:
	MOV BX, t13
	CMP BX, 0h
	JE E13
	MOV t12, AX
	MOV AX, A
	SUB AX, 4h
	MOV BX, t14
	CMP BX, 0h
	JNE E14
	MOV t15, 1h
	JMP E15
E14:
	MOV t15, 0h
E15:
	MOV BX, t15
	CMP BX, 0h
	JE E16
	MOV B, 63h
	MOV t14, AX
	MOV AX, B
	SUB AX, D
	SUB AX, fh
	MOV BX, t17
	CMP BX, 0h
	JNE E17
	MOV t18, 1h
	JMP E18
E17:
	MOV t18, 0h
E18:
	MOV BX, t18
	CMP BX, 0h
	JE E19
	MOV B, 0h
	JMP E16
E19:
	MOV A, 0h
E16:
	JMP E2
E13:
	MOV t17, AX
	MOV AX, A
	ADD AX, 1h
	MOV A, AX
E6:
	MOV AH, 4Ch
	INT 21h
	Pgm ENDS
END Pgm