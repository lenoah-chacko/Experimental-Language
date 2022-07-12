int variableAddr=1;
struct node* Head;


int registers[20]={0};
int labelnum=1;


typedef struct node
{
    char * varname;
    int address;
    struct node* next;
} node;
struct node* makeNode(char *varname, int address)
{
    struct node *temp;
    temp = (struct node*)malloc(sizeof(struct node));
    int length=strlen(varname)+1;
    temp->varname=(char*)malloc(length);
    strcpy(temp->varname,varname);
    temp->address = address;
    temp->next=NULL;
    return temp;
}

void traverse()
{
    node *temp=Head;
    //printf("SymbolTable:");
    while(temp!=NULL)
    {
        //printf("%s: %d > ",temp->varname,temp->address);
        temp=temp->next;
    }
    //printf("\n");
}

struct node* pushNode(char *varname, int address)
{
    struct node* returnable;
    if(Head==NULL)
    {
        //printf("new Head\n");
        Head=makeNode(varname,address);
        returnable=Head;
    }
    else
    {
        struct node *newNode=makeNode(varname,address);
        struct node* temp=Head;
        while(temp->next!=NULL)
        {
            temp=temp->next;
        }
        temp->next=newNode;
        returnable=newNode;
    }
    traverse();
    return returnable;
}

int search(char *varname)
{
    node *temp=Head;
    while(temp!=NULL)
    {
        //printf("%s vs %s\n",varname,temp->varname);
        if(!strcmp(temp->varname,varname))
            return temp->address;
        temp=temp->next;
    }
    return -1;
}

struct node* searchNode(char *varname)
{
    node *temp=Head;
    while(temp!=NULL)
    {
        //printf("%s vs %s\n",varname,temp->varname);
        if(!strcmp(temp->varname,varname))
            return temp;
        temp=temp->next;
    }
    return NULL;
}

void popAllNodes(char *varname)
{
    node *selected=Head, *temp;
    char *val;
    while(selected!=NULL)
    {
        temp=selected;
        selected=selected->next;
        free(temp);
    }
    return;
}

int getLength()
{
    node *temp=Head;
    int length=0;
    while(temp!=NULL)
    {
        length++;
        temp=temp->next;
    }
    return length;
}


void ending(FILE *F)
{
    
    fprintf(F,"MOV R1, \"Write\" \nPUSH R1 \nMOV R1, -2 \nPUSH R1 \nPUSH R0 \nPUSH R1 \nPUSH R1 \nCALL 0 \n");
    fprintf(F,"POP R1 \nPOP R1 \nPOP R0 \nPOP R1 \nPOP R1 \n BRKP\n");
    fprintf(F,"MOV R1, \"Exit\"\nPUSH R1\nPUSH R1\nPUSH R0\nPUSH R1\nPUSH R1\nCALL 0\nPOP R0\nPOP R1\nPOP R1\nPOP R1\nPOP R1\n");
    fprintf(F,"L0:\n"); //function body starts
    fprintf(F,"PUSH BP\n");     //save old return address for memory addressing when you come back
    fprintf(F,"MOV BP,SP\n");     //new BP for addressing memory
    for(int i=0;i<getLength()-1;i++)
        fprintf(F,"PUSH SP\n");
}

void entry(FILE *F)
{
	fprintf(F,"0 \n2056 \n0 \n0 \n0 \n0 \n0 \n0 \nMOV SP, 4097\nMOV BP, SP\n");
    fprintf(F,"MOV R0, 4096\nMOV R1, \"Read\" \nPUSH R1 \nMOV R1, -1 \nPUSH R1 \nPUSH R0 \nPUSH R1 \nPUSH R1 \nCALL 0 \n");
    fprintf(F,"POP R1 \nPOP R1 \nPOP R0 \nPOP R1 \nPOP R1 \n BRKP\n");
    fprintf(F,"MOV R0, [4096]\nPUSH R0\n");
    fprintf(F,"PUSH R0\n");
    fprintf(F,"CALL L0\n");
    fprintf(F,"POP R0\n");
    fprintf(F,"POP R1\n");
    ending(F);
}


struct tnode* makeLeafNode(int n, int type)
{
    struct tnode *temp;
    temp = (struct tnode*)malloc(sizeof(struct tnode));
    temp->val = n;
    temp->type = type;
    temp->nodetype = 7;
    temp->left = NULL;
    temp->right = NULL;
    return temp;
}

void print_tnode(struct tnode* t)
{
	//printf("{\n\tvarname:%s,\n\taddress:%d\n}\n",t->varname,t->symbol->address);
}
struct tnode* makeLeafNodeID(int type, char *name)
{
    //printf("Making ID node.\n");
    struct tnode *temp;
    temp = (struct tnode*)malloc(sizeof(struct tnode));
    temp->type = type;
    temp->nodetype = 8;
    temp->varname = (char*)malloc(strlen(name)+1);
    strcpy(temp->varname,name);
    temp->left = NULL;
    temp->right = NULL;
    //temp->symbol=NULL;
    //printf("Calling push function.\n");
    if(search(name)==-1 && !strcmp(name,"argc"))
    {
        temp->symbol=pushNode("argc",-3);
    }
    else if(search(name)==-1)
    {
        temp->symbol=pushNode(name,variableAddr++);
    }
    else
        temp->symbol=searchNode(name);
    //printf("Finished pushing\n");
    //print_tnode(temp);
    return temp;
}

struct tnode* makeOperatorNode(int c,struct tnode *l,struct tnode *r){
    struct tnode *temp;
    temp = (struct tnode*)malloc(sizeof(struct tnode));
    //temp->op = malloc(sizeof(char));
    temp->nodetype = c;
    temp->left = l;
    temp->right = r;
    //printf("returning from %d\n",c);
    return temp;
}
int getReg()
{
    for(int i=0;i<20;i++)
    {
        if(registers[i]==0)
        {
            //printf("Reg: %d\n",i);
            registers[i]=1;
            return i;
        }
    }
    //printf("Pineapple\n");
    return -1;
}

void printReg()
{
    for(int i=0;i<20;i++)
    {
            //printf("Register%d: %d\n",i,registers[i]);
    }
    return;
}

int freeReg(int freeit)
{
    registers[freeit]=0;
    //printf("Freed R%d\n",freeit);
}
int evaluate(FILE *F,struct tnode *t){
    int p,q,ind;
    char *name;
    int label1,label2;
    //printf("%d++++\n",t->nodetype);
    if(t->nodetype == 7)
    {
        p=getReg();
        fprintf(F, "MOV R%d, %d\n", p,t->val);
        return p;
    }
    else if(t->nodetype == 8)
    {
        p=getReg();
        name=t->varname;
        ind=t->symbol->address;
        print_tnode(t);
        fprintf(F, "MOV R%d, BP \nADD R%d,%d\n", p, p, ind);
        fprintf(F, "MOV R%d, [R%d] \n", p, p);
        //printf("Identifier at %d\n",p);
        return p;
    }
    else{
        switch(t->nodetype){
            case 3 : ;
                        p=evaluate(F,t->left);
                        q=evaluate(F,t->right);
                        fprintf(F,"ADD R%d, R%d\n",p,q);
                        freeReg(q);
                        return p;
                       break;
            case 5 :  ;
                        p=evaluate(F,t->left);
                        q=evaluate(F,t->right);
                        fprintf(F,"SUB R%d, R%d\n",p,q);
                        freeReg(q);
                        return p;
                       break;
            case 4 : ;
                        p=evaluate(F,t->left);
                        q=evaluate(F,t->right);
                        fprintf(F,"MUL R%d, R%d\n",p,q);
                        freeReg(q);
                        return p;
                       break;
            case 6 : ;
                        p=evaluate(F,t->left);
                        q=evaluate(F,t->right);
                        fprintf(F,"DIV R%d, R%d\n",p,q);
                        freeReg(q);
                        return p;
                       break;
            case 9 : ; 
                        name=t->left->varname;
                        //printf("Calling push function.\n");
                        if(search(name)==-1 && !strcmp(name,"argc"))
                        {
                            t->left->symbol=pushNode("argc",-3);
                        }
                        else if(search(name)==-1)
                        {
                            fprintf(F,"PUSH SP\n");
                            t->left->symbol=pushNode(name,variableAddr++);
                        }
                        else
                            t->left->symbol=searchNode(name);
                        //printf("Finished pushing\n");
                        traverse();
                        ind=search(name);
                        p=getReg();
                        fprintf(F, "MOV R%d, BP \nADD R%d,%d\n", p, p, ind);
                        //printf("Assignment executing for %s\n",name);
                        traverse(Head);
                        q=evaluate(F,t->right);
                        fprintf(F,"MOV [R%d], R%d\n",p,q);
                        //fprintf(F,"MOV [R%d], R%d\n",q,p);
                        freeReg(p);
                        freeReg(q);
                        return -1;
                       break;
            case 1 : ;
                        //printf("Reading from stdin\n");
                        name=t->left->varname;
                        //printf("Name= %s\n",name);
                        ind=search(name);
                        p=getReg();
                        fprintf(F, "MOV R%d, BP \nADD R%d,%d\n", p, p, ind);
                        q=getReg();
                        fprintf(F,"MOV R%d, \"Read\" \nPUSH R%d \nMOV R%d, -1 \nPUSH R%d \nPUSH R%d \nPUSH R%d \nPUSH R%d \nCALL 0 \n",q,q,q,q,p,q,q);
                        fprintf(F,"POP R%d \nPOP R%d \nPOP R%d \nPOP R%d \nPOP R%d \n BRKP\n",p,q,q,q,q);
                        freeReg(p);
                        freeReg(q);
                        return p;
                       break;
            case 2 : ; 
                        //printf("Writing to stdout\n");
                        p=evaluate(F,t->left);
                        q=getReg();
                        fprintf(F,"MOV R%d, \"Write\" \nPUSH R%d \nMOV R%d, -2 \nPUSH R%d \nPUSH R%d \nPUSH R%d \nPUSH R%d \nCALL 0 \n",q,q,q,q,p,q,q);
                        fprintf(F,"POP R%d \nPOP R%d \nPOP R%d \nPOP R%d \nPOP R%d \n BRKP\n",p,q,q,q,q);
                        freeReg(p);
                        freeReg(q);
                        return -1;
                       break;
            case 0 : ; 
                        p=evaluate(F,t->left);
                        q=evaluate(F,t->right);
                        return -1;
                       break;
            case 12 : ; //LT
                        p=evaluate(F,t->left);
                        q=evaluate(F,t->right);
                        fprintf(F,"LT R%d, R%d\n",p,q);
                        freeReg(q);
                        return p;
                       break;
            case 13 : ; //GT
                        p=evaluate(F,t->left);
                        q=evaluate(F,t->right);
                        fprintf(F,"GT R%d, R%d\n",p,q);
                        freeReg(q);
                        return p;
                       break;
            case 14 : ; //EQCHECK
                        p=evaluate(F,t->left);
                        q=evaluate(F,t->right);
                        fprintf(F,"EQ R%d, R%d\n",p,q);
                        freeReg(q);
                        return p;
                       break;
            case 15 : ; //LE
                        p=evaluate(F,t->left);
                        q=evaluate(F,t->right);
                        fprintf(F,"LE R%d, R%d\n",p,q);
                        freeReg(q);
                        return p;
                       break;
            case 16 : ; //GE
                        p=evaluate(F,t->left);
                        q=evaluate(F,t->right);
                        fprintf(F,"GE R%d, R%d\n",p,q);
                        freeReg(q);
                        return p;
                       break;
            case 17 : ;  //NE
                        p=evaluate(F,t->left);
                        q=evaluate(F,t->right);
                        fprintf(F,"NE R%d, R%d\n",p,q);
                        freeReg(q);
                        return p;
                       break;
            case 10 : ;  //if
                        p=evaluate(F,t->left);
                        //q=evaluate(F,t->right);
                        label1=labelnum++;
                        fprintf(F,"JZ R%d, L%d\n",p,label1);
                        q=evaluate(F,t->right);
                        fprintf(F,"L%d:\n",label1);
                        freeReg(q);
                        return p;
                       break;
            case 11 : ;  //ifelse
                        //printf("evaluating ifelse\n");
                        label1=labelnum++;
                        label2=labelnum++;
                        p=evaluate(F,t->left);
                        fprintf(F,"JZ R%d, L%d\n",p,label1);
                        q=evaluate(F,t->right->left);
                        fprintf(F,"JMP L%d\n",label2);
                        fprintf(F,"L%d:\n",label1);
                        freeReg(q);
                        q=evaluate(F,t->right->right);
                        fprintf(F,"L%d:\n",label2);
                        freeReg(q);
                        return p;
                       break;
            case 18 : ;  //while
                        //printf("evaluating while\n");
                        label1=labelnum++;
                        fprintf(F,"L%d:\n",label1);
                        p=evaluate(F,t->right);
                        q=evaluate(F,t->left);
                        fprintf(F,"JNZ R%d, L%d\n",q,label1);
                        freeReg(q);
                        return p;
                       break;
            case 19 : ;  //FUN
                        //printf("evaluating function\n");
                        int registers_copy[20]={0};
                        p=getReg();
                        //printf("Reserved R%d for return value\n",p);
                        for (int i=0;i<20;i++)
                        {
                            if(registers[i]==1 && i!=p)
                            {
                                //printf("Cleared %d\n",i);
                                fprintf(F,"PUSH R%d\n",i);
                                registers[i]=0;
                                registers_copy[i]=1;
                            }
                        }
                        printReg();
                        q=evaluate(F,t->left);
                        printReg();
                        //printf("Evaluated argument and it is in R%d\n",q);
                        fprintf(F,"PUSH R%d\n",q);  //argument
                        freeReg(q);
                        fprintf(F,"PUSH R0\n");     //space for return
                        fprintf(F,"CALL L0\n");
                        //function called
                        //printf("Function called\n");
                        q=getReg();
                        fprintf(F,"POP R%d\n",p);   //return
                        fprintf(F,"POP R%d\n",q);     //argument
                        freeReg(q);
                        for(int i=19;i>=0;i--)
                        {
                            if(registers_copy[i]==1 && i!=p)
                            {
                                fprintf(F,"POP R%d\n",i);
                                registers[i]=1;
                                registers_copy[i]=0;
                            }
                        }
                        return p;
                       break;
            case 20 : ;  //return
                        //printf("evaluating return\n");
                        p=evaluate(F,t->left);
                        q=getReg();
                        for(int i=getLength()-1; i>0; i--)
                        {
                            fprintf(F,"POP SP\n");
                        }
                        fprintf(F,"MOV R%d, BP\nSUB R%d,2\n",q,q);
                        fprintf(F,"MOV [R%d], R%d\nPOP BP\nRET\n",q,p);
                        freeReg(q);
                        return p;
                       break;
        }
    }
}
