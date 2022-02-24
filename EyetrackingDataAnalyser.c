#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct AOIs   //Areas Of Interests
{
    char name[5];
    int topLeftX;
    int width;
    int topLeftY;
    int height;
    int fixationCount;
    int dwellTime;
};

struct txtFile     
{
    int index;
    int x;
    int y;
    int duration;
};

struct AOIs *initializeAOIsTable(char *,int *);
struct txtFile *initializeTxtFiles(int,int *);
void computeFixationCount(struct AOIs *,int, struct txtFile *, int);
void computeDwellTime(struct AOIs *,int, struct txtFile *, int);
void heapSort(struct AOIs *, int, int);
void heapifyFixationCount(struct AOIs *, int, int);
void heapifyDwellTime(struct AOIs *, int, int);
void swap(struct AOIs *,struct AOIs *);
void display(struct AOIs *,int);

int main(){ //int argc, char *argv[]

    char *filename = "aois.txt"; //char *filename = argv[2];
    int AOIsSize,txtFileSize;
    int numberOfTxtFiles; //= atoi(argv[4])
    int selection; //atoi(argv[1])
    struct AOIs *AOIsTable;
    struct txtFile *txtTable;

    
    printf("\nplease write number of txt file: ");   
    scanf("%d",&numberOfTxtFiles);
    
    
    AOIsTable = initializeAOIsTable(filename,&AOIsSize);//argv[2],&AOIsSize
  	int i;
    for(i = 1; i <= numberOfTxtFiles; i++){
        txtTable = initializeTxtFiles(i,&txtFileSize);
        computeFixationCount(AOIsTable,AOIsSize,txtTable,txtFileSize);
        computeDwellTime(AOIsTable,AOIsSize,txtTable,txtFileSize);
    }

    printf("\nplease write yor selection sorting for FixationCount(1) sorting for Dwell Time(2): ");   
    scanf("%d",&selection);

    if(selection == 1){             //atoi(argv[1]) == 1     
        heapSort(AOIsTable,AOIsSize,1);
    }
    if(selection == 2){             //atoi(argv[1]) == 2
        heapSort(AOIsTable,AOIsSize,2);
    }
    display(AOIsTable,AOIsSize);

    getch();
    return 0;

}

void display(struct AOIs *AOIsTable, int listLength){
	int j;
    printf("AOI Fixation-count Dwell-time\n");
    for(j = 0; j < listLength; j++) {
        printf("%s\t%d\t\t%d\n",(AOIsTable+j)->name,(AOIsTable+j)->fixationCount,(AOIsTable+j)->dwellTime);
    }
    printf("%d",9/2);
}

struct AOIs *initializeAOIsTable(char *fileName, int *size){

    FILE *fileIn = fopen(fileName,"r");
    if(fileIn == NULL)
    {
        printf("FAILED!\n");
        exit(1);
    }

    int listLength=0;
    char charNum;
    while ((charNum=fgetc(fileIn))!=EOF){
        if (charNum=='\n'){
            listLength++;
        }
    }

    fclose(fileIn);
    struct AOIs *AOIsTable = malloc(sizeof(struct AOIs) * listLength);
    FILE *file1In = fopen(fileName,"r");
    if(file1In == NULL)
    {
        printf("FAILED!\n");
        exit(1);
    }
    int i=0;
    while (fscanf(file1In, "%s %d %d %d %d %d %d\n", (AOIsTable+i)->name,&(AOIsTable+i)->topLeftX,&(AOIsTable+i)->width,&(AOIsTable+i)->topLeftY,&(AOIsTable+i)->height,&(AOIsTable+i)->fixationCount,&(AOIsTable+i)->dwellTime)!=EOF){
        i++;
    }
    fclose(file1In);
    
  
    *size = i;
    return AOIsTable;
}

struct txtFile *initializeTxtFiles(int j,int *size){

    char tempFileName []= ".txt";
    char fileName[3];
    sprintf(fileName, "%d", j);
    strcat(fileName,tempFileName);

    FILE *fileIn = fopen(fileName,"r");
    if(fileIn == NULL)
    {
        printf("FAILED!\n");
        exit(1);
    }

    int listLength=0;
    char charNum;
    while ((charNum=fgetc(fileIn))!=EOF){
        if (charNum=='\n'){
            listLength++;
        }
    }
    fclose(fileIn);
    struct txtFile *txtTable = malloc(sizeof(struct txtFile) * listLength);

    FILE *file1In = fopen(fileName,"r");
    if(file1In == NULL)
    {
        printf("FAILED!\n");
        exit(1);
    }
    int i=0;   
    while (fscanf(file1In, "%d %d %d %d \n", &(txtTable+i)->index,&(txtTable+i)->x,&(txtTable+i)->y,&(txtTable+i)->duration)!=EOF){
        i++;  
    }
    fclose(file1In);
 
    
    *size = i;
    return txtTable;
}

void computeFixationCount(struct AOIs *AOIsTable, int AOIsSize, struct txtFile * txtTable, int txtSize){

	int i,j;
    for(i = 0;i < AOIsSize; i++){
       for(j = 0;j < txtSize; j++){
           if((txtTable+j)->x >= (AOIsTable+i)->topLeftX && (txtTable+j)->x <= ((AOIsTable+i)->topLeftX + (AOIsTable+i)->width) &&  (txtTable+j)->y >= (AOIsTable+i)->topLeftY && (txtTable+j)->y <= ((AOIsTable+i)->topLeftY+ (AOIsTable+i)->height))
                (AOIsTable+i)->fixationCount = (AOIsTable+i)->fixationCount + 1;
       }  
    }
}

void computeDwellTime(struct AOIs *AOIsTable, int AOIsSize, struct txtFile * txtTable, int txtSize){

	int i,j;
    for(i = 0;i < AOIsSize; i++){
       for(j = 0;j < txtSize; j++){
           if((txtTable+j)->x >= (AOIsTable+i)->topLeftX && (txtTable+j)->x <= ((AOIsTable+i)->topLeftX + (AOIsTable+i)->width) &&  (txtTable+j)->y >= (AOIsTable+i)->topLeftY && (txtTable+j)->y <= ((AOIsTable+i)->topLeftY+ (AOIsTable+i)->height))
                (AOIsTable+i)->dwellTime = (AOIsTable+i)->dwellTime + (txtTable+j)->duration;
       }  
    }
}

void heapSort(struct AOIs *AOIsTable, int AOIsSize, int selection){
    int i,j;
    if(selection == 1 ){
        for(i = AOIsSize / 2 - 1; i >= 0; i--)
            heapifyFixationCount(AOIsTable, AOIsSize, i);
        for (i = AOIsSize - 1; i >= 0; i--) {
            swap((AOIsTable+0), (AOIsTable+i));
            heapifyFixationCount(AOIsTable, i, 0);
        }

    }
    if(selection == 2){
        for(j = AOIsSize / 2 - 1; j >= 0; j--)
            heapifyDwellTime(AOIsTable, AOIsSize, j);
        for (j = AOIsSize - 1; j >= 0; j--) {
            swap((AOIsTable+0), (AOIsTable+j));
            heapifyDwellTime(AOIsTable, j, 0);
        }

    }


}
void heapifyFixationCount(struct AOIs *AOIsTable, int AOIsSize, int i){
    int max = i;
    int leftChild = 2 * i;
    int rightChild = 2 * i + 1;
    if (leftChild < AOIsSize && (AOIsTable+leftChild)->fixationCount > (AOIsTable+max)->fixationCount)
        max = leftChild;
    
    if (rightChild < AOIsSize && (AOIsTable+rightChild)->fixationCount > (AOIsTable+max)->fixationCount)
        max = rightChild;
    
    if (max != i) {
        swap((AOIsTable+i), (AOIsTable+max));
        heapifyFixationCount(AOIsTable, AOIsSize, max);
    }

} 
void heapifyDwellTime(struct AOIs *AOIsTable, int AOIsSize, int i){
    int max = i;
    int leftChild = 2 * i ;
    int rightChild = 2 * i + 1;
    if (leftChild < AOIsSize && (AOIsTable+leftChild)->dwellTime > (AOIsTable+max)->dwellTime)
        max = leftChild;
    
    if (rightChild < AOIsSize && (AOIsTable+rightChild)->dwellTime > (AOIsTable+max)->dwellTime)
        max = rightChild;

    if (max != i) {
        swap((AOIsTable+i), (AOIsTable+max));
        heapifyDwellTime(AOIsTable, AOIsSize, max);
    }

} 

void swap(struct AOIs *a, struct AOIs *b){

    
    char name[5];
    int topLeftX;
    int width;
    int topLeftY;
    int height;
    int fixationCount;
    int dwellTime;


    strcpy(name, a->name);
    topLeftX = a->topLeftX;
    width = a->width;
    topLeftY = a->topLeftY;
    height = a->height;
    fixationCount = a->fixationCount;
    dwellTime = a->dwellTime;


    strcpy(a->name, b->name);
    a->topLeftX = b->topLeftX;
    a->width = b->width;
    a->topLeftY = b->topLeftY;
    a->height = b->height;
    a->fixationCount = b->fixationCount;
    a->dwellTime = b->dwellTime;

    strcpy(b->name, name);
    b->topLeftX = topLeftX;
    b->width = width;
    b->topLeftY = topLeftY;
    b->height = height;
    b->fixationCount = fixationCount;
    b->dwellTime = dwellTime;
    

}