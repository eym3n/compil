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
	MOV t0, AX
	MOV AX, t0
	MOV SI, ah
	ADD SI, SI
	MOV array[SI], AX
	MOV AX, fh
	DIV AX, 3h
	MOV t1, AX
	MOV AX, t1
	ADD AX, B
	MOV t2, AX
	MOV AX, C
	SUB AX, ah
	MOV t3, AX
	MOV AX, t3
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
	MOV t7, AX
	MOV AX, t7
	MOV A, AX
	MOV SI, i
	ADD SI, SI
	MOV AX, array[SI]
	MOV BX, ah
	CMP AX, BX
	JB E0
	MOV t8, 1h
	JMP E0
E0:
	MOV t8, 0h
	MOV BX, t8
	CMP BX, 0h
	JE E1
	MOV A, 0h
	JMP E2
E2:
	MOV AX, A
	ADD AX, 1h
	MOV A, AX
	JMP E1
	MOV AX, D
	MUL AX, A
	MOV t9, AX
	MOV AX, t9
	MOV A, AX
	JMP E3
E1:
	JMP E4
	MOV AX, A
	MOV BX, fh
	CMP AX, BX
	JA E5
	MOV t10, 1h
	JMP E5
E5:
	MOV t10, 0h
	MOV AX, A
	MOV BX, 0h
	CMP AX, BX
	JB E6
	MOV t11, 1h
	JMP E6
E6:
	MOV t11, 0h
	MOV AX, t10
	MUL AX, t11
	MOV t12, AX
	MOV BX, t12
	CMP BX, 0h
	JE E7
	MOV t13, 1h
E7:
	JMP E8
E8:
	MOV t13, 0h
	MOV BX, t13
	CMP BX, 0h
	JE E9
	MOV AX, A
	SUB AX, 4h
	MOV t14, AX
	MOV BX, t14
	CMP BX, 0h
	JNE E10
	MOV t15, 1h
	JMP E10
E10:
	MOV t15, 0h
	MOV BX, t15
	CMP BX, 0h
	JE E11
	MOV B, 63h
	MOV AX, B
	SUB AX, D
	MOV t16, AX
	MOV AX, t16
	SUB AX, fh
	MOV t17, AX
	MOV BX, t17
	CMP BX, 0h
	JNE E12
	MOV t18, 1h
	JMP E12
E12:
	MOV t18, 0h
	MOV BX, t18
	CMP BX, 0h
	JE E13
	MOV B, 0h
E13:
	JMP E11
E11:
	MOV A, 0h
E9:
	JMP E1
	MOV AX, A
	MOV A, AX
E4:
	MOV AH, 4Ch
	INT 21h
	Pgm ENDS
END Pgm