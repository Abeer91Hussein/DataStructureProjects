/*name:Abeer Salah Hussien
ID:1220425
sec:2*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct polyStack* createStack();
struct polynomial* calculateLikeTerms(struct polynomial* p);
struct polynomial*addPolynomials(struct polynomial*p1, struct polynomial*p2);

struct term{
int coef;
int exp;
struct term*next;
};
struct polynomial{
struct term*termHead;//pointer to term list.
struct polynomial*next;
};
struct polyStack{
struct polynomial*polyPtr;//pointer to polynomial list.
struct polyStack*next;
};

struct polynomial*creatPolyList(){//create list of polynomials
struct polynomial*l=(struct polynomial*)malloc(sizeof(struct polynomial));
if(l==NULL){
    printf("out of memory!\n");
    return NULL;
}
l->next=NULL;
l->termHead=NULL;
  return l;
}
void printPolynomial(struct polynomial* poly) {//print the terms in the polynomial
    struct term* term= poly->termHead;
    printf("polynomial: ");
    while (term != NULL) {
        printf("%d x^ %d ",term->coef,term->exp);
       if (term->next != NULL) {
            printf("+ ");
        }
        term = term->next;
    }
    printf("\n");
}
void displayMenu(){//display main menu
printf("Menu:\n");
printf("1. Load the polynomials file\n");
printf("2. Print the polynomials\n");
printf("3. Add the polynomials\n");
printf("4. Subtract the polynomials\n");
printf("5. Multiply the polynomials\n");
printf("6. Print the results to the screen\n");
printf("7. Print the results to file\n");
printf("8. Exit\n");
}
void insertTermAtLast(int coefficient,int exponent,struct term*T) {//insert to term list
    struct term*p=T;
    while(p->next!=NULL){//main point in insertion at last.
    p=p->next;
}
    struct term*temp = (struct term*)malloc(sizeof(struct term));
    if (temp == NULL) {
        printf("Out of memory!\n");
        return;
    }
   temp->coef=coefficient;
   temp->exp=exponent;
     temp->next = p->next;
        p->next = temp;
}
struct term*createTerm() {//create list of terms
    struct term*temp= (struct term*)malloc(sizeof(struct term));
if(temp==NULL)
{
    printf("out of memory!\n");
    return;
}
    temp->next = NULL;
    temp->coef=0;
    temp->exp=0;
    return temp;
}
void insertTermListIntoPolynomial(struct polynomial* polyList, struct term* termList) {//insert term to the polynomial list
    struct polynomial* poly = (struct polynomial*)malloc(sizeof(struct polynomial));
    poly->termHead = termList->next;
    poly->next = NULL;
    if (polyList== NULL) {
        polyList= poly;//new poly node
    } else {
        struct polynomial*temp= polyList;
        while (temp->next != NULL) {
             temp=temp->next;
        }
        temp->next=poly;
    }
}
struct polyStack* loadFile(struct polynomial* polyList){//load the file and sort the polynomials list into stack.
    polyList=creatPolyList();
    FILE* file = fopen("polynomials.txt", "r");
    if (file == NULL) {
        printf("Error opening file.\n");
        return;
    }
    char line[100];
    while (fgets(line,sizeof(line),file)) {
        struct term* termList = createTerm();
        char* term= line;
        char* nextTerm;
        int coef, exp;
        while (*term) {//check for both `+` and `-` sign
            coef = 1,exp = 0;//initial values for exp and coef
            char sign = '+';
            if (*term== '+' || *term== '-') {
                sign = *term;  // determine the sign of the term.
                term++;
            }
            nextTerm= strpbrk(term, "+-\n");//(strpbrk)work as split the term until the next `+` or `-` or end of string
            char*x=strchr(term, 'x'); //(strchr) is used to locate 'x' in the term
            if (x) {
                if (x==term)
                    coef = 1;// if term starts with 'x' so coef = 1 or -1
                 else
                    coef = atoi(term);//else 'x' have coef

                if (sign == '-') //'-' sign
                    coef = -coef;
                 char* ex = strchr(x+ 1,'^');//locate '^' in the term
                if (ex)
              exp = atoi(ex+ 1); //for exponent not equal 1
               else
             exp = 1; // default exponent is 1
            }
            else {
                coef = atoi(term);
                if (sign == '-')
                    coef = -coef;
                exp = 0;
            }
            if(coef!=0)//if coef equal zero don't insert this term to the list
            insertTermAtLast(coef, exp, termList);
            if (nextTerm) {
               if (*nextTerm=='+')
              *nextTerm='+';
            else
             *nextTerm= '-';
            term= nextTerm;
            }
            else {
                break;//when term ==NULL then break
            }
        }
        insertTermListIntoPolynomial(polyList,termList);
    }
    fclose(file);
    printf("The file has been loaded!\n");
struct polyStack*s=createStack();
   struct polynomial*polyPtr= polyList->next;
    while (polyPtr!= NULL) { //push all polynomials into the stack.
        Push(polyPtr, s);
        polyPtr=polyPtr->next;
    }
return s;
}
void PrintThePolynomialsFile(){//print the input file.
    FILE *file = fopen("polynomials.txt", "r");
    if (file == NULL) {
        printf("Error opening file\n");
        return;
    }
    char line[150];
    while (fgets(line, sizeof(line), file)) {
           if (line[strlen(line)-1] == '\n') {
            line[strlen(line)-1] = '\0';
        }
        if (strlen(line) > 0) {
            printf("Polynomial: %s\n", line);//print it without \n
        }
}
fclose(file);
}
void insertTermToPolyAtLast(int coefficient,int exponent,struct polynomial*p){//insert terms to polynomials list.
    struct term* term = createTerm();
    term->coef =coefficient;
    term->exp = exponent;
    term->next = NULL;
    if (p->termHead == NULL) {
        p->termHead = term;
    } else {
        struct term*temp=p->termHead;
        while (temp->next!=NULL) {
            temp=temp->next;
        }
    temp->next=term;//to insert at last
    }
}
int IsEmptyStack(struct polyStack*s){//check if the stack is empty or not.
return s->next==NULL;
}
struct polyStack* createStack(){//create stack function.
struct polyStack* s=(struct polyStack*)malloc(sizeof(struct polyStack));
if(s==NULL){
    printf("out of memory!\n");
    return;
}
s->next=NULL;
return s;
}
void Push(struct polynomial*p, struct polyStack*s){//push polynomials to a stack.
struct polyStack* temp;
temp=(struct polyStack* )malloc(sizeof(struct polyStack));
if( temp==NULL)
printf("out of memory!\n");
else{
temp->polyPtr=p;
temp->next=s->next;
s->next=temp;
}
}
struct polyStack*Pop(struct polyStack*s){//pop stack function.
struct polyStack* firstCell;
struct polynomial*p=NULL;
if (!IsEmptyStack(s)){
firstCell=s->next;
p=s->next->polyPtr;
s->next =s->next->next;
free(firstCell);
return p;
}
else
   return NULL ;
}
int StackSize(struct polyStack*s){//find the size of the stack.
    int size=0;
    struct polyStack*ptrs= s;
    while (ptrs!=NULL) {
        size++;
        ptrs=ptrs->next;
    }
    return size;
}
struct polynomial*addPolynomials(struct polynomial*p1, struct polynomial*p2) {
struct polynomial* result = creatPolyList();
    struct term* t1 = p1->termHead;
    struct term* t2 = p2->termHead;
    while (t1 != NULL || t2 != NULL) {
        if (t1 == NULL) {// If t1 NULL so insert t2 to the result lest
            insertTermToPolyAtLast(t2->coef, t2->exp, result);
            t2 = t2->next;
        } else if (t2 == NULL) {// If t2 NULL so insert t1 to the result lest
            insertTermToPolyAtLast(t1->coef, t1->exp,result);
            t1 = t1->next;
        } else if (t1->exp == t2->exp) {// If exponents are the same so sum the coefficients
            int sum = t1->coef + t2->coef;
            if (sum != 0) {
                insertTermToPolyAtLast(sum, t1->exp,result);
            }
            t1 = t1->next;
            t2 = t2->next;
        } else if (t1->exp > t2->exp) {// If exponent of t1 is greater so insert t1 term
            insertTermToPolyAtLast(t1->coef, t1->exp, result);
            t1 = t1->next;
        } else {
            insertTermToPolyAtLast(t2->coef, t2->exp,result); // If exponent of t2 is greater so insert t2 term
            t2 = t2->next;
        }
    }
    return result;
}
struct polynomial* subtractPolynomials(struct polynomial* p1, struct polynomial* p2) {
    struct polynomial* result = creatPolyList();
    struct term* t1 = p1->termHead;
    struct term* t2 = p2->termHead;
    while (t1 != NULL || t2 != NULL) {
        if (t1 == NULL) {//if t1 NULL so insert -t2 to the result lest
            insertTermToPolyAtLast(-t2->coef, t2->exp, result);
            t2 = t2->next;
        }
        else if (t2 == NULL) {//if t2 NULL so insert t1 to the result lest
            insertTermToPolyAtLast(t1->coef, t1->exp, result);
            t1 = t1->next;
        }
        else if (t1->exp == t2->exp) {// If exponents are the same so subtract the coefficients
            int sub = t1->coef - t2->coef;
            if (sub!= 0) {
                insertTermToPolyAtLast(sub, t1->exp, result);
            }
            t1 = t1->next;
            t2 = t2->next;
        }
        else if (t1->exp > t2->exp) {  // If exponent of t1 is greater so insert t1 term
            insertTermToPolyAtLast(t1->coef, t1->exp, result);
            t1 = t1->next;
        }
        else {// If exponent of t2 is greater so subtract t2 term
            insertTermToPolyAtLast(-t2->coef, t2->exp, result);
            t2 = t2->next;
        }
    }
    return result;
}
struct polynomial* mulPolynomials(struct polynomial* p1, struct polynomial* p2) {
    struct polynomial* result = creatPolyList();
    struct term* t1 = p1->termHead;
    struct term* t2 = p2->termHead;
    while (t1 != NULL) {
        t2 = p2->termHead;  //t2 point to the start of p2 for each term in p1
        while (t2 != NULL) {
            int newCoef = t1->coef * t2->coef;// multiply the coefficients
            int newExp = t1->exp + t2->exp;//add the exponents
            insertTermToPolyAtLast(newCoef, newExp, result);
            t2 = t2->next;  // move to the next term in p2
        }
        t1 = t1->next;  // move to the next term in p1
    }
    result = calculateLikeTerms(result);
    return result;
}
struct polynomial*calculateLikeTerms(struct polynomial* p) {// calculate the similar terms in a polynomial
    struct term* t1 = p->termHead;
    while (t1!=NULL) {
        struct term* t2=t1->next;
        struct term* prev=t1;
        while (t2!=NULL) {
            if (t1->exp == t2->exp) { // if exponents are the same, add the coefficients
                t1->coef += t2->coef;
                prev->next = t2->next;  // remove the duplicate term
                free(t2); // remove the duplicate term
                t2 = prev->next;
            } else {
                prev = t2;//move to the next term
                t2 = t2->next;
            }
        }
        t1 = t1->next;
    }
    return p;
}
struct polyStack*stackStor(struct polyStack*s) {//function to store the main stack to another one
    if (s== NULL)
        return NULL;
    struct polyStack*secondStack=NULL;
    struct polyStack*temp=NULL;
    struct polyStack*ptrs=s;
    while (ptrs!= NULL) {
        struct polyStack*newStack= (struct polyStack*)malloc(sizeof(struct polyStack));
        newStack->polyPtr =ptrs->polyPtr; // Point to the same polynomial
        newStack->next =temp;
        temp =newStack;
        ptrs=ptrs->next;
    }
    while (temp != NULL) {
        struct polyStack* newStack=(struct polyStack*)malloc(sizeof(struct polyStack));
        newStack->polyPtr=temp->polyPtr;
        newStack->next=secondStack;
        secondStack=newStack;
        struct polyStack*p=temp;
        temp=temp->next;
        free(p);
    }
    return secondStack;
}
void printResultsToFile(struct polynomial* addResult, struct polynomial* subResult, struct polynomial* multResult) {
    FILE* resultFile = fopen("polynomial_results.txt", "w");
    if (resultFile == NULL) {
        printf("can't opening file for writing the results!\n");
        return;
    }
    fprintf(resultFile, "operations results :\n");
    fprintf(resultFile, "addition result: ");// add result
    if (addResult == NULL) {
        fprintf(resultFile, "Empty\n");
    } else
        writePolynomialToFile(resultFile, addResult);

    fprintf(resultFile, "subtraction result: "); // subtract result
    if (subResult == NULL) {
        fprintf(resultFile, "Empty\n");
    } else
        writePolynomialToFile(resultFile, subResult);

    fprintf(resultFile, "multiplication result: ");// multiply result
    if (multResult == NULL) {
        fprintf(resultFile, "Empty\n");
    } else
        writePolynomialToFile(resultFile, multResult);
    fclose(resultFile);
    printf("results have been written to the output file\n");
}
void writePolynomialToFile(FILE* file, struct polynomial* poly) {//write the polynomials to the output file .
    struct term* currentTerm = poly->termHead;
    while (currentTerm != NULL) {
        fprintf(file, "%d x^%d", currentTerm->coef, currentTerm->exp);
        if (currentTerm->next != NULL) {
            fprintf(file, " + ");
        }
        currentTerm = currentTerm->next;
    }
    fprintf(file, "\n");
}
int main(){
struct polynomial* addResult = NULL;
struct polynomial* subResult = NULL;
struct polynomial* multResult = NULL;
    struct polyStack*mainStack=createStack();
    int operation;
    printf("Please choose the operation that you want:\n");
    displayMenu();
    scanf("%d",&operation);
     if(operation!=1){
        while(operation!=1){//Loading the file first.
            printf("\nYou can't chose any operation before load the file!\n\n");
            printf("Please enter the number of operation that you want\n");
            displayMenu();
            scanf("%d",&operation);
        }
    }
    while(operation!=8){
        if(operation<0&&operation>8){
            printf("Invalid operation.Please try different number!\n");
            displayMenu();
            scanf("%d",&operation);
            }
switch(operation){
case 1:printf("1. Load the polynomials file\n");
struct polynomial* polyListHead = creatPolyList();
   mainStack = loadFile(polyListHead);
break;
case 2: printf("2. Print the polynomials\n");
PrintThePolynomialsFile();
break;
case 3:printf("3. Add the polynomials\n");
    struct polyStack* addStack = stackStor(mainStack);
    if (StackSize(addStack) >= 2) {
        while (!IsEmptyStack(addStack)) {
            struct polynomial*Poly1=Pop(addStack);
            struct polynomial*poly2=Pop(addStack);
            if (Poly1&&poly2) {
                addResult=addPolynomials(Poly1,poly2);
                Push(addResult, addStack);
            }
            else
              printf("The addition operation has been performed\n");//end of the stack
        }
    }
    else
    printf("Stack does not have enough elements for addition.\n");
    break;

case 4: printf("4. Subtract the polynomials\n");
    struct polyStack* subStack = stackStor(mainStack);
    if (StackSize(subStack) >= 2) {
        while (!IsEmptyStack(subStack)) {
            struct polynomial*poly1 =Pop(subStack);
            struct polynomial*poly2=Pop(subStack);
            if (poly1&&poly2) {
                subResult = subtractPolynomials(poly1,poly2);
                Push(subResult, subStack);
            }
            else
                printf("The subtraction operation has been performed\n");//end of the stack
            }
            } else
        printf("Stack does not have enough elements for subtraction.\n");
    break;
case 5: printf("5. Multiply the polynomials\n");
    struct polyStack* multStack =stackStor(mainStack);
    if (StackSize(multStack) >= 2) {
        while (!IsEmptyStack(multStack)) {
            struct polynomial*poly1=Pop(multStack);
            struct polynomial*poly2=Pop(multStack);
            if (poly1&&poly2) {
                multResult = mulPolynomials(poly1,poly2);
                Push(multResult, multStack);
            }
            else
                printf("The multiplication operation has been performed\n");//end of the stack
        }
    } else
        printf("Stack does not have enough elements for multiplication.\n");
    break;
case 6:printf("6. Print the results to the screen\n");
            printf("Addition Result: ");
            if (addResult != NULL)
                printPolynomial(addResult);
            else
                printf("Empty list\n");
            printf("Subtraction Result: ");
            if (subResult != NULL)
                printPolynomial(subResult);
             else
                printf("Empty list\n");

            printf("Multiplication Result: ");
            if (multResult != NULL)
                printPolynomial(multResult);
             else
                printf("Empty list\n");
break;
case 7:printf("7. Print the results to file\n");
printResultsToFile(addResult, subResult, multResult);
    break;
default :
    printf("Invalid operation number.Please try again...\n!");
}
printf("Enter the next operation \n");
displayMenu();
scanf("%d",&operation);
}
    printf("Thank you for using my applecation!\n");
    return 0;
}
