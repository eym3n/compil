#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include "ts.h"


typedef struct{
    char oper[100];
    char op1[100];
    char op2[100];
    char res[100];
    char etq[20];
} qdr;




#define N 1000

int top1 = -1;
int top2 = -1;
int top3 = -1;

int stack1[N];
int stack2[N];
int stack3[N];



void push(int c, int x){

    int *stack, top;
    if(c==1){
        stack = stack1;
        top = top1;
    } 
    else if (c==2){
        stack = stack2;
        top = top2;
    } 
    else{
        stack = stack3;
        top = top3;
    }

    if(top == N-1)
        printf("Overflow State: can't add more elements into the stack\n");
    else{
        top+=1;
        stack[top] = x;
    }

    if(c==1) top1 = top; 
    else if(c==2) top2 = top;
    else top3 = top;
}

int pop(int c){
    int *stack, top;
    if(c==1){
        stack = stack1;
        top = top1;
    } 
    else{
        stack = stack2;
        top = top2;
    } 


    int x = -1;

    if(top == -1)
        printf("Underflow State: Stack already empty, can't remove any element\n");
    else{
        x = stack[top];
        top-=1;
    }


    if(c==1){
        top1--; 
    } 
    else if(c==2) {
        top2--; 
    }
    else top3 = top;

    return x;
}


void printStacks(){
    printf("Stack 1:\n");
    printf("TOP 1: %d\n", top1);
    int i = top1;
    while(i>=0){
        printf("%d ", stack1[i]);
        i--;
    }

    printf("\nStack 2:\n");
    printf("TOP 2: %d\n", top2);
    i = top2;
    while(i>=0){
        printf("%d ", stack2[i]);
        i--;
    }

    printf("\n");

}



qdr quad[1000];
extern int qc;

void quadr(char opr[], char op1[], char op2[], char res[]){
    strcpy(quad[qc].oper, opr);
    strcpy(quad[qc].op1, op1);
    strcpy(quad[qc].op2, op2);
    strcpy(quad[qc].res, res);
    strcpy(quad[qc].etq, "");

    qc++;
}

void ajouter_quad(int num_quad, int colon_quad, char val[]){
    if(colon_quad == 0) strcpy(quad[num_quad].oper, val);
    else if(colon_quad == 1) strcpy(quad[num_quad].op1, val);
    else if(colon_quad == 2) strcpy(quad[num_quad].op2, val);
    else if(colon_quad == 3) strcpy(quad[num_quad].res, val);
}



void afficher_qdr(){
    printf("\n\n\n\t\tLesQuadruplets\n\n");
    int i; 
    for(i=0;i<qc;i++){
        printf("\n %d - ( %s , %s , %s , %s )",i,quad[i].oper,quad[i].op1,quad[i].op2,quad[i].res); 
        printf("\n---------------------------------------------------\n"); 
    }
}




char* trim(char* val){
    if(strlen(val)>4){
        int s = strlen(val);
        return val + (s - 4);
    }
    return val;
}


int is_direct(char* op){
    if((op[0] >= 'A' && op[0] <= 'Z') || (op[0] >= 'a' && op[0] <= 'z')) return 0;
    return 1;
}


int is_array(char* op){
    return strstr(op, "[") != NULL;
}

void array_elements(char* op, char* arr, char* ind){
    int i = 0;
    while(op[i] != '['){
        arr[i] = op[i];
        i++;
    }
    arr[i] = '\0';

    i++;
    int j = 0;
    while(op[i+j] != ']'){
        ind[j] = op[i+j];
        j++;
    }
    ind[j] = '\0';
}


void arr_to_qsm(FILE* fptr, char* op){
    char arr[50], offset[50];
    array_elements(op, arr, offset);

    if(is_direct(offset)){
        char temp[50];
        sprintf(temp, "%x", atoi(offset));
        sprintf(offset, "%sh", temp); 
    }

    fprintf(fptr, "\tMOV SI, %s\n", offset);
    fprintf(fptr, "\tADD SI, SI\n");
    sprintf(op, "%s[SI]", arr);
}




int e = 0;
char* ax;

int in_acc(char* var, FILE* fptr){
    
    if(ax == NULL) return 0;

    if(strcmp(var, ax)==0){
        return 1;
    }
    else{
        fprintf(fptr, "\tMOV %s, AX\n", ax);
        return 0;
    }
}


void qdr_to_asm(FILE* fptr, int i){

    if(ax != NULL && (strcmp(quad[i].oper, "+") == 0 || strcmp(quad[i].oper, "*") == 0) && strcmp(quad[i].op2, ax) == 0){
        char* temp = strdup(quad[i].op2);
        sprintf(quad[i].op2, "%s", quad[i].op1);
        sprintf(quad[i].op1, "%s", temp);
    }

    // if(ax != NULL && ((strcmp(quad[0].op1, ax) == 0 || strcmp(quad[0].op2, ax) == 0) || quad[i].oper[0] == 'B')){
    //     fprintf(fptr, "\tMOV %s, AX\n", ax);
    //     ax = NULL;
    // }

    char* op1 = strdup(quad[i].op1);
    char* op2 = strdup(quad[i].op2);
    char* res = strdup(quad[i].res);
    char* oper = strdup(quad[i].oper);
    char* etq = strdup(quad[i].etq);

    if(strcmp(etq, "") != 0){
        fprintf(fptr, "%s:\n", etq);
    }


    if(strcmp(oper, ":=") == 0){
        if(is_direct(op1)){
            char temp[50];
            sprintf(temp, "%x", atoi(op1));
            fprintf(fptr, "\tMOV %s, %sh\n", res, trim(temp));  
        }
        else{
            if(is_array(op1)){
                arr_to_qsm(fptr, op1);
                fprintf(fptr, "\tMOV AX, %s\n", op1);
            }
            else{
                if(!in_acc(op1, fptr)){
                    fprintf(fptr, "\tMOV AX, %s\n", op1);
                }
                
            }

            if(is_array(res)){
                arr_to_qsm(fptr, res);
                fprintf(fptr, "\tMOV %s, AX\n", res);
            }
            else{
                fprintf(fptr, "\tMOV %s, AX\n", res);
                ax = strdup(res);
            }
        }
    }
    else if(strcmp(oper, "BR") == 0){
        int d = atoi(op1);
        char temp[20];
        if(strcmp(quad[d].etq, "") == 0){
            sprintf(temp, "E%d", e);
            e++;
            
            sprintf(quad[d].etq, "%s", temp);
            fprintf(fptr, "\tJMP %s\n", temp);
        }
        else{
            fprintf(fptr, "\tJMP %s\n", quad[d].etq);
        }
    }
    else if(strcmp(oper, "BNZ") == 0){
        int d = atoi(op1);
        char temp[20];
        if(strcmp(quad[d].etq, "") == 0){
            sprintf(temp, "E%d", e);
            e++;

            if(is_direct(op2)){
                char temp[50];
                sprintf(temp, "%x", atoi(op2));
                sprintf(op2, "%sh", trim(temp));  
            }
            
            sprintf(quad[d].etq, "%s", temp);

            if(is_array(op2)){
                arr_to_qsm(fptr, op2);
            }

            if(in_acc(op2, fptr)){
                op2 = strdup("AX");
            }

            fprintf(fptr, "\tMOV BX, %s\n", op2);
            fprintf(fptr, "\tCMP BX, 0h\n");
            fprintf(fptr, "\tJNE %s\n", temp);
        }
        else{
            fprintf(fptr, "\tJMP %s\n", quad[d].etq);
        }
    }
    else if(strcmp(oper, "BZ") == 0){
        int d = atoi(op1);
        char temp[20];
        if(strcmp(quad[d].etq, "") == 0){
            sprintf(temp, "E%d", e);
            e++;


            if(is_direct(op2)){
                char temp[50];
                sprintf(temp, "%x", atoi(op2));
                sprintf(op2, "%sh", trim(temp));  
            }
            
            sprintf(quad[d].etq, "%s", temp);

            if(is_array(op2)){
                arr_to_qsm(fptr, op2);
            }

            if(in_acc(op2, fptr)){
                op2 = strdup("AX");
            }
            
            fprintf(fptr, "\tMOV BX, %s\n", op2);
            fprintf(fptr, "\tCMP BX, 0h\n");
            fprintf(fptr, "\tJE %s\n", temp);
        }
        else{
            fprintf(fptr, "\tJMP %s\n", quad[d].etq);
        }
    }
    else if(strcmp(oper, "BGE") == 0){
        int d = atoi(op1);
        char temp[20];
        if(strcmp(quad[d].etq, "") == 0){
            sprintf(temp, "E%d", e);
            e++;
            
            sprintf(quad[d].etq, "%s", temp);

            if(is_direct(op2)){
                char temp[50];
                sprintf(temp, "%x", atoi(op2));
                sprintf(op2, "%sh", trim(temp));  
            }
            if(is_direct(res)){
                char temp[50];
                sprintf(temp, "%x", atoi(res));
                sprintf(res, "%sh", trim(temp));  
            }

            if(is_array(op2)){
                arr_to_qsm(fptr, op2);
            }
            
            fprintf(fptr, "\tMOV AX, %s\n", op2);

            if(is_array(res)){
                arr_to_qsm(fptr, res);
            }

            fprintf(fptr, "\tMOV BX, %s\n", res);
            fprintf(fptr, "\tCMP AX, BX\n");
            fprintf(fptr, "\tJAE %s\n", temp);
        }
        else{
            fprintf(fptr, "\tJMP %s\n", quad[d].etq);
        }
    }
    else if(strcmp(oper, "BG") == 0){
        int d = atoi(op1);
        char temp[20];
        if(strcmp(quad[d].etq, "") == 0){
            sprintf(temp, "E%d", e);
            e++;
            
            sprintf(quad[d].etq, "%s", temp);

            if(is_direct(op2)){
                char temp[50];
                sprintf(temp, "%x", atoi(op2));
                sprintf(op2, "%sh", trim(temp));  
            }
            if(is_direct(res)){
                char temp[50];
                sprintf(temp, "%x", atoi(res));
                sprintf(res, "%sh", trim(temp));  
            }

            if(is_array(op2)){
                arr_to_qsm(fptr, op2);
            }


            fprintf(fptr, "\tMOV AX, %s\n", op2);

            if(is_array(res)){
                arr_to_qsm(fptr, res);
            }

            fprintf(fptr, "\tMOV BX, %s\n", res);
            fprintf(fptr, "\tCMP AX, BX\n");
            fprintf(fptr, "\tJA %s\n", temp);
        }
        else{
            fprintf(fptr, "\tJMP %s\n", quad[d].etq);
        }
    }
    else if(strcmp(oper, "BLE") == 0){
        int d = atoi(op1);
        char temp[20];
        if(strcmp(quad[d].etq, "") == 0){
            sprintf(temp, "E%d", e);
            e++;
            
            sprintf(quad[d].etq, "%s", temp);


            if(is_direct(op2)){
                char temp[50];
                sprintf(temp, "%x", atoi(op2));
                sprintf(op2, "%sh", trim(temp));  
            }
            if(is_direct(res)){
                char temp[50];
                sprintf(temp, "%x", atoi(res));
                sprintf(res, "%sh", trim(temp));  
            }

            if(is_array(op2)){
                arr_to_qsm(fptr, op2);
            }

            fprintf(fptr, "\tMOV AX, %s\n", op2);

            if(is_array(res)){
                arr_to_qsm(fptr, res);
            }

            fprintf(fptr, "\tMOV BX, %s\n", res);
            fprintf(fptr, "\tCMP AX, BX\n");
            fprintf(fptr, "\tJBE %s\n", temp);
        }
        else{
            fprintf(fptr, "\tJMP %s\n", quad[d].etq);
        }
    }
    else if(strcmp(oper, "BL") == 0){
        int d = atoi(op1);
        char temp[20];
        if(strcmp(quad[d].etq, "") == 0){
            sprintf(temp, "E%d", e);
            e++;
            
            sprintf(quad[d].etq, "%s", temp);

            if(is_direct(op2)){
                char temp[50];
                sprintf(temp, "%x", atoi(op2));
                sprintf(op2, "%sh", trim(temp));  
            }
            if(is_direct(res)){
                char temp[50];
                sprintf(temp, "%x", atoi(res));
                sprintf(res, "%sh", trim(temp));  
            }

            if(is_array(op2)){
                arr_to_qsm(fptr, op2);
            }

            fprintf(fptr, "\tMOV AX, %s\n", op2);

            if(is_array(res)){
                arr_to_qsm(fptr, res);
            }

            fprintf(fptr, "\tMOV BX, %s\n", res);
            fprintf(fptr, "\tCMP AX, BX\n");
            fprintf(fptr, "\tJB %s\n", temp);
        }
        else{
            fprintf(fptr, "\tJMP %s\n", quad[d].etq);
        }
    }
    else if(strcmp(oper, "+") == 0 && strcmp(op2, "1") == 0 && strcmp(res, op1) == 0 && !is_array(res)){
        
        fprintf(fptr, "\tINC %s\n", res);
        ax = strdup(res);

    }
    else if(strcmp(oper, "+") == 0){

        if(is_direct(op1)) {
            char temp[50];
            sprintf(temp, "%xh", atoi(op1));
            op1 = strdup(temp);
        }
        if(is_direct(op2)) {
            char temp[50];
            sprintf(temp, "%xh", atoi(op2));
            op2 = strdup(temp);
        }

        if(is_array(op1)){
            arr_to_qsm(fptr, op1);
            fprintf(fptr, "\tMOV AX, %s\n", op1);
        }
        else{
            if(!in_acc(op1, fptr)){
                fprintf(fptr, "\tMOV AX, %s\n", op1);
            }
        }

        

        if(is_array(op2)){
            arr_to_qsm(fptr, op2);
        }
        
        fprintf(fptr, "\tADD AX, %s\n", op2);

        if(is_array(res)){
            arr_to_qsm(fptr, res);
            fprintf(fptr, "\tMOV %s, AX\n", res);
        }
        else{
            ax = strdup(res);
        }

        
    }
    else if(strcmp(oper, "-") == 0){

        if(is_direct(op1)) {
            char temp[50];
            sprintf(temp, "%xh", atoi(op1));
            op1 = strdup(temp);
        }
        if(is_direct(op2)) {
            char temp[50];
            sprintf(temp, "%xh", atoi(op2));
            op2 = strdup(temp);
        }

        if(is_array(op1)){
            arr_to_qsm(fptr, op1);
            fprintf(fptr, "\tMOV AX, %s\n", op1);
        }
        else{
            if(!in_acc(op1, fptr)){
                fprintf(fptr, "\tMOV AX, %s\n", op1);
            }
        }

        if(is_array(op2)){
            arr_to_qsm(fptr, op2);
        }
        
        fprintf(fptr, "\tSUB AX, %s\n", op2);

        if(is_array(res)){
            arr_to_qsm(fptr, res);
            fprintf(fptr, "\tMOV %s, AX\n", res);
        }
        else{
            ax = strdup(res);
        }
    }
    else if(strcmp(oper, "*") == 0){

        if(is_direct(op1)) {
            char temp[50];
            sprintf(temp, "%xh", atoi(op1));
            op1 = strdup(temp);
        }
        if(is_direct(op2)) {
            char temp[50];
            sprintf(temp, "%xh", atoi(op2));
            op2 = strdup(temp);
        }

        if(is_array(op1)){
            arr_to_qsm(fptr, op1);
            fprintf(fptr, "\tMOV AX, %s\n", op1);
        }
        else{
            if(!in_acc(op1, fptr)){
                fprintf(fptr, "\tMOV AX, %s\n", op1);
            }
        }

        if(is_array(op2)){
            arr_to_qsm(fptr, op2);
        }
        
        fprintf(fptr, "\tMUL AX, %s\n", op2);

        if(is_array(res)){
            arr_to_qsm(fptr, res);
            fprintf(fptr, "\tMOV %s, AX\n", res);
        }
        else{
            ax = strdup(res);
        }
    }
    else if(strcmp(oper, "/") == 0){

        if(is_direct(op1)) {
            char temp[50];
            sprintf(temp, "%xh", atoi(op1));
            op1 = strdup(temp);
        }
        if(is_direct(op2)) {
            char temp[50];
            sprintf(temp, "%xh", atoi(op2));
            op2 = strdup(temp);
        }

        if(is_array(op1)){
            arr_to_qsm(fptr, op1);
            fprintf(fptr, "\tMOV AX, %s\n", op1);
        }
        else{
            if(!in_acc(op1, fptr)){
                fprintf(fptr, "\tMOV AX, %s\n", op1);
            }
        }

        if(is_array(op2)){
            arr_to_qsm(fptr, op2);
        }
        
        fprintf(fptr, "\tDIV AX, %s\n", op2);

        if(is_array(res)){
            arr_to_qsm(fptr, res);
            fprintf(fptr, "\tMOV %s, AX\n", res);
        }
        else{
            ax = strdup(res);
        }
    }
}






void generer_asm(int tc){

    char *pgm_id;
    int x = tc;

   FILE *fptr = fopen("objet/pgm.asm","w");

   fprintf(fptr,"Pile segment stack ;\n\tdw 100 dup(?)\nPile ends\nDATA segment\n");

   for(int i=0; i<TABLE_SIZE; i++){
        if(tab[i] != NULL){
            node* t = tab[i];
            while(t != NULL){
                if(strcmp(t->elt.type, "") != 0){
                    if(strcmp(t->elt.code, "IDF") == 0){
                        if(strcmp(t->elt.type, "PROGRAM ID") == 0){
                            strcpy(pgm_id, t->elt.name);
                        }
                        else if(strcmp(t->elt.type, "STRUCT") == 0);
                        else if(t->elt.size > 0){
                            fprintf(fptr, "\t%-8s  DW\t%d\tDUP(?)\n", t->elt.name, t->elt.size);
                        }
                        else{
                            fprintf(fptr, "\t%-8s  DW\t?\n", t->elt.name);
                        }
                    }
                    else if(strcmp(t->elt.code, "CST") == 0 && strcmp(t->elt.val, "/NULL/") != 0){
                        char temp[50];
                        sprintf(temp, "%x", atoi(t->elt.val));
                        fprintf(fptr, "\t%-8s  %sh\n", t->elt.name, trim(temp));   
                    }
                }
                t = t->nxt;
            }
        }
    }

    

    for(int i=0; i<x; i++){
        char temp[50];
        sprintf(temp, "t%d", i);
        fprintf(fptr, "\t%-8s  DW\t?\n", temp);
    }



   fprintf(fptr, "DATA ends\nCODE SEGMENT\n\t%s:\n\tASSUME CS:CODE, DS:DATA\n\tMOV AX,DATA\n\tMOV DS, AX\n", pgm_id);


    for(int i=0; i<qc; i++){
        qdr_to_asm(fptr, i);
    }


   fprintf(fptr, "\tMOV AH, 4Ch\n\tINT 21h\nCODE ENDS\nEND %s", pgm_id);
   fclose(fptr);
}

void removeQuad(int pos) {
  for (int i = pos; i < qc - 1; i++) {
    strcpy(quad[i].oper, quad[i+1].oper);
    strcpy(quad[i].op1, quad[i+1].op1);
    strcpy(quad[i].op2, quad[i+1].op2);
    strcpy(quad[i].res, quad[i+1].res);
  }
  qc--;
}

void Propagation_common_expressions() 
{
    char OPER[200];
    char OP1[200];
    char OP2[200];
    char RES[200];
    char RES2[200];

    int changed;

    printf("\t\t< === Propagation of common expressions : === >\n");
    for(int i=0; i<qc; i++) {
        if(strcmp(quad[i].oper ,":=") != 0 && strcmp(quad[i].oper ,"BZ") != 0 && strcmp(quad[i].oper ,"BR") != 0) {
            for(int j=i+1; j<qc; j++) {
                changed = 0;
                
                if(strcmp(quad[i].op1 ,quad[j].op1)== 0 && strcmp(quad[i].op2 ,quad[j].op2) == 0 && strcmp(quad[i].oper ,quad[j].oper) == 0) {
                    for(int k=i+1; k<j; k++) {
                        if(strcmp(quad[k].res, quad[i].op1) == 0 || strcmp(quad[k].res, quad[i].op2) == 0) changed = 1;
                    }
                    if(!changed) {
                        strcpy(quad[j].oper,":=");
                        strcpy(quad[j].op1,quad[i].res);
                        strcpy(quad[j].op2,"");

                        printf(" Redundancy : %s := %s \n",quad[j].res,quad[i].res);
                    }
                }
            }
        }
    }
}

void Propagation_copie() 
{
    int changed;

    printf("\t\t< === Propagation of copies : === >\n");
    for(int i=0; i<qc; i++) {
        if(strcmp(quad[i].oper ,":=") == 0) {
            for(int j=i+1; j<qc; j++) {
                changed = 0;
                
                if(strcmp(quad[i].res ,quad[j].op1)== 0 || strcmp(quad[i].res ,quad[j].op2) == 0) {
                    for(int k=i+1; k<j; k++) {
                        if(strcmp(quad[k].res, quad[i].res) == 0) changed = 1;
                    }
                    if(!changed) {
                        if(strcmp(quad[i].res ,quad[j].op1)== 0)  strcpy(quad[j].op1,quad[i].op1);
                        if(strcmp(quad[i].res ,quad[j].op2)== 0)  strcpy(quad[j].op2,quad[i].op1);
                        
                        printf(" Copie : %s := %s \n",quad[i].res,quad[i].op1);
                    }
                }
            }
        }
    }
}

void Elimination_useless_code() {
    int used;

    printf("\t\t< === Elimination of useless code : === >\n");
    int i = 0;
    while(i<qc) {
        printf("%d %s - ",!exists(quad[i].res),quad[i].res);
        if(!exists(quad[i].res)) {
           used = 0;
           for(int j=i+1; j<qc; j++) {
               if(strcmp(quad[i].res,quad[j].op1)== 0 || strcmp(quad[i].res,quad[j].op2) == 0) used = 1;
           } 
           if(!used) {
               printf(" Useless : %s := %s \n",quad[i].res,quad[i].op1);
               removeQuad(i);
           }
           else {
               i++;
           }
        }
        else i++;
    }
}
// =======================================================================================================
// =======================================================================================================
// =======================================================================================================
// =======================================================================================================
// Optimisation (By Ayoub)
void shift(int pos) {
  for (int i = pos; i < qc - 1; i++) {
    strcpy(quad[i].oper, quad[i+1].oper);
    strcpy(quad[i].op1, quad[i+1].op1);
    strcpy(quad[i].op2, quad[i+1].op2);
    strcpy(quad[i].res, quad[i+1].res);
  }
  qc--;
}

void Propagation_de_expression() 
{
  printf("-optimization propogation de expression common \n-");
  /*the concept in which optimization of expression common is based on*/
  qdr *p;
  qdr *ps;
  qdr *pf;
  qdr *p1=quad;
  qdr *p2=quad;
  qdr *p3=quad;
  //VARIABLE TO SAVE OP1 AND OP2 AND OPR AND RESULTS
  char OPER[200];
  char OP1[200];
  char OP2[200];
  char RES[200];
  char RES2[200];
  int mod=0;
  int nb_shifted=0;
  for(int i=0;i<qc;i++)
  {
    p = &p1[i];
    printf("|p[%d]=(%s,%s,%s,%s)\n",i,p->oper,p->op1,p->op2,p->res);
    //save p attributes
    //check if the quad isnt Branchement quad or an AFF(=) quadruple
    if(strcmp(p->oper ,":=")!=0 && strcmp(p->oper ,"BZ")!=0 && strcmp(p->oper ,"BR")!=0 )
    {
      for(int j=i+1;j<qc;j++)
      {
        ps=&p2[j];
          strcpy(OPER, p->oper) ;
    	    strcpy(OP1 , p->op1) ;
    	    strcpy(OP2 , p->op2) ;
    	    strcpy(RES , p->res) ;
          if((strcmp(ps->res ,OP1)!=0 && strcmp(ps->res ,OP2)!=0))
          {
            if((strcmp(ps->op1 ,OP1)==0 && strcmp(ps->op2 ,OP2)==0 && strcmp(ps->oper ,OPER)==0) || (strcmp(ps->op1 ,OP2)==0 && strcmp(ps->op2 ,OP1)==0 && strcmp(ps->oper ,OPER)==0))
    			{
            int k=j+1;
            strcpy(RES2,ps->res);
            for(k;k<qc;k++)
            {
              pf=&p3[k];
              printf("\t\t\t\t\t\t|OLD:p[%d]=(%s,%s,%s,%s)\n",k,pf->oper,pf->op1,pf->op2,pf->res);
              if(strcmp(pf->op1 ,RES2)==0 )
    					{
    							strcpy(pf->op1,RES) ;
    							mod =1;
    					}
    					if(strcmp(pf->op2 ,RES2)==0)
    					{
    							strcpy(pf->op2,RES) ;
    							mod =1;
    					}
              printf("\t\t\t\t\t\t|NEW:p[%d]=(%s,%s,%s,%s)\n",k,pf->oper,pf->op1,pf->op2,pf->res);
            }
            if(mod==1)
            {
        	    printf("\t \t \t \t  \tAfter the nested loop, there is a conditional statement that checks if the current instruction is not the first instruction\n\t \t \t \t \t(listeQdr) and whether any changes (mod) have been made to the instruction. If these conditions are met,\n\t \t \t \t \tthe current instruction is removed from the list.\n");
              printf("\t \t \t \t  \tremove instruction at %d \n",j);
              shift(j); 
              nb_shifted++; 
           }
          }
        }
    }
  }
  }
  printf("\t \t \tsecond part of function fixes the adressage of BZ and BR\n");
  p1=quad;
  p2=quad;
  p=&p1[0];
  ps=&p2[0];
  int value;
  char str[10];
  //printf("\t \t \tp[%d]=(%s,%s,%s,%s)\n", 0, p->oper, p->op1, p->op2, p->res);
  //printf("\t \t \tps[%d]=(%s,%s,%s,%s)\n", 0, ps->oper, ps->op1, ps->op2, ps->res);
  printf("\t\t\tshifted=%d\n",nb_shifted);
  if(nb_shifted>0)
  {
    //loop throught the quads
    //if its branchement BR or branchement BZ decrement the op1 by nb_shifted
    for(int i=0;i<qc;i++)
    {
      p=&p1[i];
      if(strcmp(p->oper ,"BZ")==0 || strcmp(p->oper ,"BR")==0 )
      {
        printf("\t \t \tp[%d]=(%s,%s,%s,%s)\n", i, p->oper, p->op1, p->op2, p->res);
        value=atoi(p->op1);
        printf("\t\t\tvalue=%d\n",value);
        value=value-nb_shifted;
        printf("\t\t\tvalue_new=%d\n",value);
        sprintf(str,"%d",value);
        strcpy(p->op1,str);
      }
    }
  }
}
