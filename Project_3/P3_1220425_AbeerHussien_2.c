/*Abeer Salah Hussien
1220425
sec 1 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

typedef struct Town *Town;
struct Town {
    char district[50];
    char townName[50];
    int population;
    int elevation;
    char municipality[4];
    int isDeleted;
};
typedef struct HashTable *HashTable;
 struct HashTable{
    Town*array;
    int size;
    int capacity;
};
typedef struct AVLnode *AVLNode;
struct AVLnode{
int Element;
AVLNode Left;
AVLNode Right;
int Height;
char district[50] ;
char town[50];
int population;
int elevation;
char hasMunicipality[4];
};

int IsEmpty(AVLNode T){
    return T == NULL;
}

AVLNode MakeEmpty( AVLNode T ){
    if(!IsEmpty(T))
    {
        MakeEmpty( T->Left );
        MakeEmpty( T->Right );
        free( T );
    }
    return NULL;
}

AVLNode Find(char *town, AVLNode T) {//find function
    if (T == NULL) {
        printf("town not found\n");
        return NULL;
    }
    if (strcmp(town, T->town) < 0) {//if less than
        return Find(town, T->Left);
    }
    else if (strcmp(town, T->town) > 0) {//if greater than
        return Find(town, T->Right);
    }
    else {
        printf("found town is %s\n", T->town);
        return T;
    }
}

AVLNode FindMin( AVLNode T ){
if( T == NULL )
return NULL;
else if( T->Left == NULL )
return T;
else
return FindMin( T->Left );
}

AVLNode FindMax( AVLNode T ){
if( T != NULL )
while( T->Right != NULL )
T = T->Right;
return T;
}

int Height( AVLNode P ){
if( P == NULL )
return -1;
else
return P->Height;
}

int Max( int Lhs, int Rhs ){
return Lhs > Rhs ? Lhs : Rhs;
}

AVLNode SingleRotateWithLeft( AVLNode K2 ){
AVLNode K1;

K1 = K2->Left;
K2->Left = K1->Right;
K1->Right = K2;

K2->Height = Max( Height( K2->Left ), Height( K2->Right ) ) + 1;
K1->Height = Max( Height( K1->Left ), K2->Height ) + 1;

return K1; /* New root */
}

AVLNode SingleRotateWithRight( AVLNode K1 ){
AVLNode K2;

K2 = K1->Right;
K1->Right = K2->Left;
K2->Left = K1;

K1->Height = Max( Height( K1->Left ), Height( K1->Right ) ) + 1;
K2->Height = Max( Height( K2->Right ), K1->Height ) + 1;

return K2; /* New root */
}

AVLNode DoubleRotateWithLeft( AVLNode K3 ){
/* Rotate between K1 and K2 */
K3->Left = SingleRotateWithRight( K3->Left );

/* Rotate between K3 and K2 */
return SingleRotateWithLeft( K3 );
}

AVLNode DoubleRotateWithRight( AVLNode K1 ){
/* Rotate between K3 and K2 */
K1->Right = SingleRotateWithLeft( K1->Right );

/* Rotate between K1 and K2 */
return SingleRotateWithRight( K1 );
}

AVLNode InsertByName(int population, char* town, char* district, int elevation, char* hasMunicipality, AVLNode T) {//insert and order the townd by alpha
    if (T==NULL) {
        T=(struct AVLNode*)malloc(sizeof(struct AVLnode));
        if (T==NULL) {
            printf("Out of space!!!\n");
            return NULL;
        }
        T->population = population;
        strcpy(T->town, town);
        strcpy(T->district, district);
        strcpy(T->hasMunicipality, hasMunicipality);
        T->elevation = elevation;
        T->Height = 0;
        T->Left = T->Right = NULL;
    }
else if (strcmp(town,T->town)< 0 ||(strcmp(town,T->town)== 0&& population < T->population)) {//if less than (insert to the left)
                                                                                      //if the same town insert will order by the population
        T->Left=InsertByName(population, town,district,elevation,hasMunicipality,T->Left);
        if (Height(T->Left) - Height(T->Right) == 2) {//rebalanc
if (strcmp(town, T->Left->town)< 0||(strcmp(town, T->Left->town)==0&& population < T->Left->population)) {
                T = SingleRotateWithLeft(T);//rotate with left
            }
             else {
                T = DoubleRotateWithLeft(T);//double rotate
            }
        }
    }
else if (strcmp(town, T->town)>0||(strcmp(town, T->town)==0&& population>T->population)) {//if greater than (insert to the right)
                                                                                            //if the same town insert will order by the population
        T->Right = InsertByName(population, town, district, elevation, hasMunicipality, T->Right);//insert to the right
        if (Height(T->Right) - Height(T->Left) == 2) {
            if (strcmp(town, T->Right->town) > 0 ||(strcmp(town, T->Right->town) == 0 && population > T->Right->population)) {
                T = SingleRotateWithRight(T);//rotate right
            } else {
                T = DoubleRotateWithRight(T);//double rotation
            }
        }
    }
    T->Height = Max(Height(T->Left), Height(T->Right)) + 1;//update the hight
    return T;
}

void listTownsByMunicipalityInOrder(AVLNode T) {//list by Municipality
    if (T == NULL) {
        return;
    }
    listTownsByMunicipality(T, "yes");//the towns has municipality first
    listTownsByMunicipality(T, "no");//then the towns has not Municipality
}

void listTownsByMunicipality(AVLNode T,char* hasMunicipality) {//used in list by Municipality function
    if (T == NULL) {
        return;
    }
    listTownsByMunicipality(T->Left, hasMunicipality);//left side
    if (strcmp(T->hasMunicipality, hasMunicipality) == 0) {
        printf("District: %s   Town: %s   Population: %d   Elevation: %d   Municipality: %s\n",
               T->district, T->town, T->population, T->elevation, T->hasMunicipality);
    }
    listTownsByMunicipality(T->Right, hasMunicipality);//right side
}

void PrintInOrder( AVLNode t){//print in order function
if( t != NULL)
{
PrintInOrder( t->Left );//left first
printf("district :%s   town:%s   pop:%d   elevation:%d   muni:%s  ", t->district,t->town,t->population,t->elevation,t->hasMunicipality);
printf("\n");
PrintInOrder( t->Right );//then the right
}
}

void update(AVLNode T,int population,int elevation,char*hasMunicipality){//update town function
if(T==NULL){
        printf("the town is not found !\n");
    return NULL;
}
 T->population=population;
 T->elevation=elevation;
 strcpy(T->hasMunicipality,hasMunicipality) ;
printf("the town updated !\n");
}

void ListTownsByPopulation(AVLNode T, int minPopulation) {//list by population function
    if (T == NULL) {
        return;
    }
    ListTownsByPopulation(T->Left, minPopulation);//left first
    if (T->population > minPopulation) {
        printf("District: %s, Town: %s, Population: %d, Elevation: %d, Municipality: %s\n",
               T->district, T->town, T->population, T->elevation, T->hasMunicipality);
    }
    ListTownsByPopulation(T->Right, minPopulation);//then the right
}

void displayMenu(){//menu function
printf("Menu:\n");
printf("1. Read districts file\n");
printf("2. Insert a new town with all its associated information. \n");
printf("3. Find a town and update.\n");
printf("4. order towns in alphabetical order with their associated information \n");
printf("5. List all towns that have population greater than an input number. \n");
printf("6. List all towns that have municipalities or not.\n");
printf("7. Delete a town from the tree. \n");
printf("8. Save all information to a file called towns.txt.\n");
printf("9. Hash Part.\n");
printf("10.Exit.\n");
}

AVLNode buildTreeFromFile() {//build the tree from tree (by the town alpha order)
    FILE *file = fopen("districts.txt", "r"); // Open the file for reading
    if (file == NULL) {
        printf("the file not found !\n");
        return NULL;
    }
    AVLNode T = NULL;
    char line[256];
    while (fgets(line, sizeof(line), file)) {
        char district[50], town[50], municipality[4];
        int population, elevation;
        if (sscanf(line, "%[^:]:%[^:]:%d:%d:%s", district, town, &population, &elevation, municipality) == 5) {
//%[^:]== read every thing until :
//sscanf returns the number of strings reads
            T = InsertByName(population, town, district, elevation, municipality,T);//build the tree by the town alpha order
        }
    }
    fclose(file);
    PrintInOrder(T);
    return T;
}

AVLNode insertByUser(AVLNode mainTree) {//insert by user function
    char district[10];
    char town[10];
    int population;
    int elevation;
    char hasMunicipality[4];
printf("Enter district name, town name, population, elevation, municipality (yes/no):\n ");
scanf(" %s %s %d %d %s",district, town, &population, &elevation, hasMunicipality);
    mainTree=InsertByName(population,town,district,elevation,hasMunicipality,mainTree);
    return mainTree;
}

AVLNode DeleteTown(char *townName, AVLNode T) {//delete town function
    if (T == NULL) {
        printf("Town not found\n");
        return T;
    }
    if (strcmp(townName, T->town) < 0) {//if less than then move to the left
        T->Left = DeleteTown(townName, T->Left);
    }
     else if (strcmp(townName, T->town) > 0) {//else if greater than then move to the right
        T->Right = DeleteTown(townName, T->Right);
    }
    else { //find the town to delete
        if (T->Left == NULL && T->Right == NULL) {//in case the node with no children
            free(T);
            return NULL;
        }
        else if (T->Left == NULL) { //in case the node with only right child
            AVLNode temp = T->Right;
            free(T);
            return temp;
        }
        else if (T->Right == NULL) {// in case the node with only left child
            AVLNode temp = T->Left;
            free(T);
            return temp;
        }
        else {//in case the node with two children
            AVLNode temp = FindMin(T->Right);//minimum in the right side
            strcpy(T->district, temp->district);
            strcpy(T->town, temp->town);
            strcpy(T->hasMunicipality, temp->hasMunicipality);
            T->population = temp->population;
            T->elevation = temp->elevation;
            T->Right = DeleteTown(temp->town, T->Right);
        }
    }
    T->Height = Max(Height(T->Left), Height(T->Right)) + 1;//update the hight

    int balance = Height(T->Left) - Height(T->Right);//update the tree balance

    if (balance > 1) {//the right greater than the left
        if (Height(T->Left->Left) >= Height(T->Left->Right)) {
            return SingleRotateWithLeft(T);
        } else {
            return DoubleRotateWithLeft(T);
        }
    }
    if (balance < -1) {//the left greater than the right
        if (Height(T->Right->Right) >= Height(T->Right->Left)) {
            return SingleRotateWithRight(T);
        } else {
            return DoubleRotateWithRight(T);
        }
    }
    return T;
}

void writeToFile(FILE* file,AVLNode T) {//write to the town.txt file
if(T==NULL)
    return;
writeToFile(file,T->Left);//left first
 fprintf(file, "District: %s   Town: %s   Population: %d   Elevation: %d   Municipality: %s\n",
            T->district, T->town, T->population, T->elevation, T->hasMunicipality);
    writeToFile(file, T->Right);//then the right side
}

///Hash functions part
void hashMenu(){//menu function for the hash part
printf("\nMenu:\n");
printf("1. Print Hash Table\n");
printf("2. Print Size and Load Factor\n");
printf("3. Insert a New Record\n");
printf("4. Search for a Town\n");
printf("5. Delete a Record\n");
printf("6. Save Data to File\n");
printf("7. Exit\n");
}

int hashFun1(char* key, int tableSize) {//first hash function
    int hashValue = 0;
    while (*key != '\0') {
        hashValue = (hashValue << 5) + *key++; // hashValue = hashValue * 31 + *key++
    }
    return (hashValue %tableSize);
}

int hashFun2(char* key, int tableSize) {//second hash function (double hashing used)
    int hashValue = 0;
    while (*key != '\0') {
        hashValue = (hashValue << 5) + *key++; // hashValue = hashValue *31+*key++
    }
    return (tableSize - (hashValue % tableSize));  //second function eq
}
Town createTown(char *district,char* name, int population, int elevation, char* municipality) {//create town
    Town town = (struct Town*)malloc(sizeof(struct Town));
    strcpy(town->district,district);
    strcpy(town->townName,name);
     strcpy(town->municipality, municipality);
    town->population = population;
    town->elevation = elevation;
    return town;
}

void loadDataFromFile(HashTable table) {//load data from town.txt file
    FILE *file = fopen("towns.txt", "r");
    if (file==NULL) {
        printf("Unable to open file!\n");
        return;
    }
    char town[50],municipality[4],district[50];
    int population, elevation;
while(fscanf(file,"District: %s Town: %s Population: %d Elevation: %d Municipality: %s\n",district,
             town, &population, &elevation, municipality) == 5) {//fscanf returns the number of items that read successfully
        insertRecord(table,district, town, population, elevation, municipality);//load
    }
    fclose(file);
}
int countLines() {//calculate the lines in the file
    FILE* file = fopen("towns.txt", "r");
    if (file == NULL) {
        perror("Error opening file");
        return ;
    }
    int count = 0;
    char c[256];
    while (fgets(c, sizeof(c), file) != NULL) {
        count++; // Increment line count for each line read
    }
    fclose(file);
    return count;
}

HashTable createHashTable(int capacity) {//create hash table
    HashTable table=(HashTable*)malloc(sizeof(HashTable));
    table->capacity =capacity;
    table->size=0;
    table->array =(struct town*)malloc(sizeof(struct Town) * table->capacity);//malloc by multiply by the table capacity
    for (int i = 0; i < table->capacity; i++) {
        table->array[i] = NULL;//initialize with empty table
    }
    return table;
}

float calculateLoadFactor(HashTable table) {//find the load factor
   float loadFactor=(float)table->size / table->capacity;
    return loadFactor;
}

int nextPrime(int num) {//used for rehash function
    while (1) {
        num++;
        int isPrime = 1;
        for (int i = 2; i * i <= num; i++) {
            if (num % i == 0) {
                isPrime = 0;
                break;
            }
        }
        if (isPrime)
            return num;
    }
}

void rehashTable(HashTable table) {//rehash function
    int capacity = table->capacity;
    int newCapacity = nextPrime(capacity *2);
    Town *newArray = table->array;
    table->array = (Town *)malloc(sizeof(struct Town ) * newCapacity);
    table->capacity = newCapacity;
    table->size = 0;
    for (int i = 0; i < newCapacity; i++) {
        table->array[i] = NULL;
    }
    for (int i = 0; i <capacity; i++) {
        if (newArray[i] != NULL){
insertRecord(table,newArray[i]->district,newArray[i]->townName,newArray[i]->population,newArray[i]->elevation,newArray[i]->municipality);
            free(newArray[i]);  //free the rehashed element
        }
    }
    free(newArray);  //free the old array
}

void printHashTable(HashTable table) {//print hash table function
    for (int i = 0; i < table->capacity; i++) {
        if (table->array[i] != NULL) {
            printf("index [%d]: town: %s, Population: %d, Elevation: %d, Municipality: %s\n",
                   i, table->array[i]->townName, table->array[i]->population,
                   table->array[i]->elevation, table->array[i]->municipality);
        }
         else {
            printf("index [%d]:----\n", i);
        }
    }
}

void printTableSizeLoadFactor(HashTable table) {//print the table info
    float lF = calculateLoadFactor(table);
    printf("hash table size: %d\n", table->size);
    printf("hash table capacity: %d\n", table->capacity);
    printf("Load Factor: %.2f\n", lF);
}

void insertRecord(HashTable table,char *district, char* townName, int population, int elevation, char* municipality) {//insert new record
    float lf=calculateLoadFactor(table);
    if (lf >= 0.7) {//0.7 as a maximum load factor
        printf("the load factor is %.2f greater than 0.7 ,rehash the table \n",lf);
        rehashTable(table);
    }
    int idx = hashFun1(townName, table->capacity);
    int next = hashFun2(townName, table->capacity);
    int i = 0;
    while (table->array[(idx +i*next) % table->capacity] != NULL) {//if the index not fill so move to the next one
        i++;
    }
    int nextIndex=(idx+i *next)%table->capacity;
    table->array[nextIndex] = createTown(district,townName, population, elevation, municipality);//create town
    table->size++;//increment the size
}

void searchTown(HashTable table, char* townName) {//search for specific town
    int idx = hashFun1(townName, table->capacity);
    int next = hashFun2(townName, table->capacity);
    int i = 0;

    while (table->array[(idx + i *next) % table->capacity] != NULL) {
        if (strcmp(table->array[(idx + i *next) % table->capacity]->townName, townName) == 0) {//if the compare function return 0 so there are equal
            printf("town %s found in index %d after %d collisions\n", townName,(idx + i *next) % table->capacity, i);
            return;
        }
        i++;
    }
    printf("town %s not found in the hash table\n", townName);
}

void deleteRecord(HashTable table, char* townName) {//delete specific  record
    int idx = hashFun1(townName, table->capacity);
    int next = hashFun2(townName, table->capacity);
    int i = 0;
    while (i < table->capacity) {
        int nextIndex = (idx +i*next)%table->capacity;
        if (nextIndex < 0) {
            nextIndex += table->capacity;
        }
        Town currentNode = table->array[nextIndex];//town
        if (currentNode == NULL) {//move to the next index
            i++;
            continue;
        }
        if (strcmp(currentNode->townName, townName) == 0) {
            printf("Found town %s at index %d\n", townName, nextIndex);
            free(currentNode);
            table->array[nextIndex] = NULL;
            table->size--;//dicreamente the size
            printf("Town %s deleted from the hash table\n", townName);
            return;
        }
        i++;
    }
    printf("Town %s not found in hash table\n", townName);
}

void saveDataToFile(HashTable table) {//save the data in the town.txt file
    FILE *file = fopen("towns.txt", "w");
    if (file==NULL) {
        printf(" can't open the file\n");
        return;
    }
    for (int i = 0; i < table->capacity; i++) {
        if (table->array[i] != NULL) {
            fprintf(file, "District: %s Town: %s Population: %d Elevation: %d Municipality: %s\n",table->array[i]->district,
                    table->array[i]->townName, table->array[i]->population,
                    table->array[i]->elevation, table->array[i]->municipality);
        }
    }
    fclose(file);
    printf("data saved in the town file \n");
}

int main(){//main function
AVLNode mainTree=NULL;
mainTree=MakeEmpty(mainTree);
    int operation;
    printf("Please choose the operation that you want:\n");
    displayMenu();
    scanf("%d",&operation);
     if(operation!=1){
        while(operation!=1){//Loading the file first.
            printf("you can't chose any operation before load the file!\n");
            printf("please enter the number of operation that you want\n");
            displayMenu();
            scanf("%d",&operation);
        }
    }
    while(operation!=10){
        if(operation<0&&operation>10){
            printf("Invalid operation.Please try different number!\n");
            displayMenu();
            scanf("%d",&operation);
            }
switch(operation){
case 1:printf("1. Load the  districts file\n");
mainTree=buildTreeFromFile();
break;

case 2: printf("2. insert town \n");
mainTree=insertByUser(mainTree);
printf("after insertion :\n\n");
PrintInOrder(mainTree);
break;

case 3:printf("3. Find the town and update its information (if found)\n");
char townName[10],choice[4],municipality[4];
printf("enter the town name do you want to find it :\n");
scanf("%s",townName);
AVLNode t=Find(townName,mainTree);
int population,elevation;
 if(t!=NULL){
    printf("the town is founded \n"
          "do you want to update it's information ?\n ");
          scanf("%s",choice);
         if(strcmp(choice,"yes")==0){
            printf("update the population:\n");
            scanf("%d",&population);
                  printf("update the elevation:\n");
                  scanf("%d",&elevation);
                   printf("it has a municipality or not?");
            scanf("%s",municipality);
            update(t,population,elevation,municipality);
            PrintInOrder(mainTree);
         }
 }
 else if(t==NULL)
    printf("the town does not found \n");
    break;
case 4:
    printf("order the tree based on the town name :\n\n");
    PrintInOrder(mainTree);
    break;
case 5 :
 printf("Enter the minimum population:\n");
    int minPopulation;
    scanf("%d", &minPopulation);
    printf("Towns with population greater than %d:\n", minPopulation);
    ListTownsByPopulation(mainTree, minPopulation);
    break;
case 6:
     printf("List all towns, ordered by municipality (yes then no):\n");
                listTownsByMunicipalityInOrder(mainTree);
    break;
case 7:printf("delete town \n");
char deleteTown[10];
    printf("enter the town name do you want to delete it :\n");
    scanf("%s",deleteTown);
mainTree=  DeleteTown(deleteTown,mainTree);
if(!IsEmpty(mainTree))
    PrintInOrder(mainTree);
    else
        printf("Empty tree!!!\n");
    break;
case 8:
    printf("Save the information :");
    FILE*townTxt=fopen("towns.txt","w");
writeToFile(townTxt,mainTree);
    break;
case 9:
 printf("hash part\n");
    int option;
    char townNamee[50];
    char district[50];
    int pop, elev;
    char hasmunicipality[4];
 int count=countLines();
    HashTable *table = createHashTable(count); // Initial table size
    printf("First load the town file and check the load factor \n\n");
    loadDataFromFile(table);
    hashMenu();
    printf("Enter your option :\n");
    scanf("%d",&option);
    while (option!=7){
    if(option <1 &&option >7){
        printf("Invalid operation number.Please try again...\n!");
              hashMenu();
    scanf("%d",&option);
    }
        switch (option) {
            case 1:
                printHashTable(table);
                break;
            case 2:
                printTableSizeLoadFactor(table);
                break;
            case 3:
                printf("Enter district name, town name, population, elevation, municipality (yes/no):\n ");
                scanf(" %s %s %d %d %s",district, townNamee, &pop, &elev, hasmunicipality);
                insertRecord(table,district,townNamee, pop, elev, hasmunicipality);
                break;
            case 4:
                printf("Enter Town Name to Search: ");
                scanf("%s", townNamee);
                searchTown(table, townNamee);
                break;
            case 5:
                printf("Enter Town Name to Delete: ");
                scanf("%s", townNamee);
                deleteRecord(table, townNamee);
                break;
            case 6:
                saveDataToFile(table);
                break;
            case 7:
                printf("Exit\n");
                break;
            default:
                printf("Invalid option. Try again.\n");
        }
        printf("Enter the next operation \n");
hashMenu();
scanf("%d",&option);
    }
break;
case 10:
   printf("Exit\n");
break;
default :
    printf("Invalid operation number.Please try again...\n!");
}
printf("Enter the next operation \n");
displayMenu();
scanf("%d",&operation);
}
    printf("Thank you for using my application!\n");
    return 0;
}
