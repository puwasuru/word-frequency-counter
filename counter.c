#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <getopt.h>

typedef struct characters{

	char lett;
	int count;
    float freq;
	struct characters *next;  

} charnode;

typedef struct words{

    char *word;
    int count;
    float freq;
    struct words *next;  

} wordnode;


charnode*createLetterHead();
wordnode*createWordHead();
void addNewLetterToEnd(charnode * head, char letter);
void addNewWordToEnd(wordnode * head, char *str);
void addToWordlen(wordnode * head, char *str, float fre);
void increaseLetterCount(charnode *head,char letter);
void increaseWordCount(wordnode *head,char *str);
int isLetterInList(charnode* head, char letter);
int isWordInList(wordnode* head, char *str);
int totalLetters(charnode * head);
int totalWordCount(wordnode* head);
int maxword(wordnode * head);
int countFre(float freq);
int lengthOfLetterBar(float fre);
int lengthOfLetterBarScaled(float fre, float maximumfreq);
int lengthOfWordBar(float fre,int maxlength);
int lengthOfWordBarScaled(float fre,int maxlength, float maximumfreq);
void printLetterRaw(char letter,float fre,float maximumfreq,int scaled);
void printOneRaw(int maxlength,float fre,char * word,float maximumfreq,int scaled);
void endLetterGraph();
void bottomBar(int maxlength);
void printingLetterGraph(charnode * head,int charnum,int scaled);
void printGraph(wordnode * head,int scaled);
void setCharFreq(charnode* head);
void setWordFreq(wordnode* head);
wordnode* sortWordList(wordnode * head,int charnum);
void letterFrequency(charnode * head,FILE * testfile);
void wordFrequency(wordnode* head,FILE * testfile);
int isDigit(char * arg);



wordnode * topw; 
charnode * topc;  

int main(int argc, char **argv){

    int length,op,index,scaled,prvind,Scaled,Char,Wor;

    struct option optn[] = {{"scaled",no_argument,&scaled,1}};
    

    length = 10;    
    Scaled = 0;       
    Char = 0;  

    if(argc == 1){
        printf("No input files were given\n");
        printf("usage: %s [-l length] [-w | -c] [--scaled] filename1 filename2 ..\n",argv[0]);
        return 0;
    }
    opterr = 0;
    prvind = optind;
    while ((op = getopt_long(argc, argv, "l:wc", optn, NULL)) != -1){
        switch (op) {
            case 0:
                break;
            case 'l':
                if(!isDigit(optarg)){
                    printf("Invalid options for [-l]\n");
                    printf("usage: %s [-l length] [-w | -c] [--scaled] filename1 filename2 ..\n",argv[0]);
                    return 0;
                }
                
                length = atoi(optarg);
                
                break;
            case 'w':
                Wor = 1;
                break;
            case 'c':
                Char = 1;
                break;
            case '?':
                if(optopt == 'l'){
                    printf("Not enough options for [-%c]\n",optopt);
                    printf("usage: %s [-l length] [-w | -c] [--scaled] filename1 filename2 ..\n",argv[0]);
                }else{
                    printf("Invalid options [%s]\n",argv[prvind+1]);
                    printf("usage: %s [-l length] [-w | -c] [--scaled] filename1 filename2 ..\n",argv[0]);
                }
                return 0;
            default:
                return 0;
       }
   }
   if(Char == 1 && Wor == 1){
        printf("[-c] and [-w] cannot use together\n");
        printf("usage: %s [-l length] [-w | -c] [--scaled] filename1 filename2 ..\n",argv[0]);
        return 0;
   }
    if(scaled == 1){
        Scaled = 1;
    }
    if(optind == argc){
        printf("No input files were given\n");
        printf("usage: %s [-l length] [-w | -c] [--scaled] filename1 filename2 ..\n",argv[0]);
        return 0;
    }

    if(Char==1){
        topc=createLetterHead();  
    }else{
        topw=createWordHead();
    }


    for (index = optind; index < argc; index++){
        FILE* filename=fopen(argv[index], "r");

        if(filename == NULL){
            printf("Cannot open one or more given files\n");
            return 0;
        }
        
        if(Char==1){
            letterFrequency(topc,filename);
        }else{
            wordFrequency(topw,filename);
        }
        fclose(filename);
    
    }
    if((Char==1 && topc->count==0) || (Char==0 && topw->count==0)){
        printf("No data to process\n");
        return 0;
    }


    if(Char==1){
        setCharFreq(topc);
        printingLetterGraph(topc,length,Scaled);
        endLetterGraph();
  
    }else{
        setWordFreq(topw);
        topw=sortWordList(topw,length);
        printGraph(topw,Scaled);
    }

    
    return 0;
}

int isDigit(char * arg){
    int len,i;
    len=strlen(arg);
    for(i=0;i<len;i++){
        if(!isdigit(arg[i])){
            return 0;
        }
    }
    return 1;
} 


charnode*createLetterHead(){
    static charnode* head = NULL;
    head = (charnode *)malloc(sizeof(charnode));
    head->count=0;
    head->next=NULL;
    return head;
}

wordnode*createWordHead(){
    static wordnode* head = NULL;
    head = (wordnode *)malloc(sizeof(wordnode));
    head->count=0;
    head->next=NULL;
    return head;
}


void addNewLetterToEnd(charnode * head, char letter) {
    charnode* current = head;

    head->count=head->count+1;

    while (current->next != NULL) {
        current = current->next;
    }

    current->next = (charnode*)malloc(sizeof(charnode));
    current->next->lett = letter;
    current->next->count = 1;
    current->next->freq = 0;
    current->next->next = NULL;
}

void addNewWordToEnd(wordnode * head, char *str) {
    wordnode* current = head;

    head->count=head->count+1;

    while (current->next != NULL) {
        current = current->next;
    }

    current->next = (wordnode*)malloc(sizeof(wordnode));
    current->next->word=(char *)malloc(strlen(str)+1);
    strcpy((current->next->word),str);
    current->next->count = 1;
    current->next->freq = 0;
    current->next->next = NULL;
}

void addToWordlen(wordnode * head, char *str, float fre) {
    wordnode* current = head;

    while (current->next != NULL) {
        current = current->next;
    }

    current->next = (wordnode*)malloc(sizeof(wordnode));
    current->next->word=(char *)malloc(strlen(str)+1);
    strcpy((current->next->word),str);
    current->next->count = strlen(str);
    current->next->freq = fre;
    current->next->next = NULL;
}

void increaseLetterCount(charnode *head,char letter){
    charnode* current = head->next;
    while(current->lett!=letter){
        current=current->next;
    }

    current->count=(current->count)+1;

}

void increaseWordCount(wordnode *head,char *str){
    wordnode* current = head->next;
    while(strcmp((current->word),str)!=0){
        current=current->next;
    }

    current->count=(current->count)+1;

}

int isLetterInList(charnode* head, char letter){
    charnode * current = head->next;
    
    while(current!=NULL){
        if (current->lett==letter){
            return 1;
        }
        current=current->next;
    }

    return 0;
            
}

int isWordInList(wordnode* head, char *str){
    wordnode * current = head->next;
    
    while(current!=NULL){
        if (strcmp(current->word,str)==0){
            return 1;
        }
        current=current->next;
    }

        return 0;
}

int totalLetters(charnode * head){
    int total=0;
    charnode *current=head->next;

    while(current!=NULL){
        total = total + current->count;
        current=current->next;
    }

    return total;

}

int totalWordCount(wordnode* head){
    int total=0;
    wordnode *current=head->next;

    while(current!=NULL){
        total = total + current->count;
        current=current->next;
    }

    return total;
}

int maxword(wordnode * head){
    wordnode * current=head->next;
    int maxlength=0;
    while(current!=NULL){

        if(maxlength<current->count){
            maxlength=current->count;
        }

        current=current->next;
    }

    return maxlength;
}

int countFre(float freq){
    int fre=freq;
    char str[5];
    sprintf(str, "%d", fre);
    return (int)strlen(str);
}

int lengthOfLetterBar(float fre){
    return (int)(((72-countFre(fre)))*(fre/100.0));
}

int lengthOfLetterBarScaled(float fre, float maximumfreq){
    return (int)(((72-countFre(fre)))*(fre/maximumfreq));
}

int lengthOfWordBar(float fre,int maxlength){
    return (int)((73-countFre(fre)-maxlength)*(fre/100.0));
}

int lengthOfWordBarScaled(float fre,int maxlength, float maximumfreq){
    return (int)((73-countFre(fre)-maxlength)*(fre/maximumfreq));
}


void printLetterRaw(char letter,float fre,float maximumfreq,int scaled){
    int i;
    if(scaled==1){
        printf("   ");
        printf("\u2502");
        for(i=0;i<lengthOfLetterBarScaled(fre,maximumfreq);i++){
            printf("\u2591");    
        }
        printf("\n");
        printf(" %c ",letter);
        printf("\u2502");
        for(i=0;i<lengthOfLetterBarScaled(fre,maximumfreq);i++){
            printf("\u2591");    
        }
        printf("%.2f%%\n",fre);
        printf("   ");
        printf("\u2502");
        for(i=0;i<lengthOfLetterBarScaled(fre,maximumfreq);i++){
            printf("\u2591");    
        }
        printf("\n");
        printf("   ");
        printf("\u2502\n");

    }else{

        printf("   ");
        printf("\u2502");
        for(i=0;i<lengthOfLetterBar(fre);i++){
            printf("\u2591");    
        }
        printf("\n");
        printf(" %c ",letter);
        printf("\u2502");
        for(i=0;i<lengthOfLetterBar(fre);i++){
            printf("\u2591");    
        }
        printf("%.2f%%\n",fre);
        printf("   ");
        printf("\u2502");
        for(i=0;i<lengthOfLetterBar(fre);i++){
            printf("\u2591");    
        }
        printf("\n");
        printf("   ");
        printf("\u2502\n");
    }
}

void printOneRaw(int maxlength,float fre,char * word,float maximumfreq,int scaled){

    int i,wordlen;
    
    
    wordlen=strlen(word);

    if(scaled==1){
        for(i=0;i<(maxlength+2);i++){
            printf(" ");
        }
        printf("\u2502");

        for(i=0;i<lengthOfWordBarScaled(fre,maxlength,maximumfreq);i++){
            printf("\u2591");
        }
        printf("\n");

        printf(" %s",word);
        for(i=0;i<(maxlength-wordlen+1);i++){
            printf(" ");
        }   
        printf("\u2502");
        for(i=0;i<lengthOfWordBarScaled(fre,maxlength,maximumfreq);i++){
            printf("\u2591");
        }
        printf("%.2f%%\n",fre);
        
        for(i=0;i<(maxlength+2);i++){
            printf(" ");
        }
        printf("\u2502");

        for(i=0;i<lengthOfWordBarScaled(fre,maxlength,maximumfreq);i++){
            printf("\u2591");
        }
        printf("\n");
        for(i=0;i<(maxlength+2);i++){
            printf(" ");
        }
        printf("\u2502\n");

    }else{

        for(i=0;i<(maxlength+2);i++){
            printf(" ");
        }
        printf("\u2502");

        for(i=0;i<lengthOfWordBar(fre,maxlength);i++){
            printf("\u2591");
        }
        printf("\n");

        printf(" %s",word);
        for(i=0;i<(maxlength-wordlen+1);i++){
            printf(" ");
        }   
        printf("\u2502");
        for(i=0;i<lengthOfWordBar(fre,maxlength);i++){
            printf("\u2591");
        }
        printf("%.2f%%\n",fre);
        
        for(i=0;i<(maxlength+2);i++){
            printf(" ");
        }
        printf("\u2502");

        for(i=0;i<lengthOfWordBar(fre,maxlength);i++){
            printf("\u2591");
        }
        printf("\n");
        for(i=0;i<(maxlength+2);i++){
            printf(" ");
        }
        printf("\u2502\n");
    }
}

void endLetterGraph(){
    int i;
    printf("   \u2514");
    for(i=0;i<76;i++){
        printf("\u2500");
    }
    printf("\n");
}

void bottomBar(int maxlength){
    int i;
    for(i=0;i<(maxlength+2);i++){
        printf(" ");
    }
    printf("\u2514");
    for(i=0;i<(77-maxlength);i++){
        printf("\u2500");
    }
    printf("\n");

}

void printingLetterGraph(charnode * head,int charnum,int scaled){
    int i,maxc;
    float maxf,maximumfreq;
    char k;
    charnode * temp=NULL;
    charnode * current=head->next;

    if(charnum>head->count){
        charnum=head->count;
    }

    printf("\n");
    for(i=0;i<charnum;i++){
        maxc=0;
        maxf=0;
        current=head->next;
        
        while(current!=NULL){
            if(current->count>maxc){
                k=current->lett;
                maxc=current->count;
                maxf=current->freq;
            }
            current=current->next;
        }
        current=head;
        while(current->next->lett!=k){

            current=current->next;

        }
        temp=current->next;
        if(temp->next==NULL){
            current->next=NULL;
        }else{
            current->next=temp->next;
        }
        
        free(temp);
        if(i==0){
            maximumfreq=maxf;
        }
        printLetterRaw(k,maxf,maximumfreq,scaled);

    }


}

void printGraph(wordnode * head,int scaled){
    wordnode * current=head->next;
    float maximumfreq = current->freq;
    int maxwordlen=maxword(head);

    
    printf("\n");
    while(current!=NULL){
        printOneRaw(maxwordlen,current->freq,current->word,maximumfreq,scaled);

        current=current->next;
    }
    bottomBar(maxwordlen);
}

void setCharFreq(charnode* head){

    int total = totalLetters(head);

    charnode* current=head->next;
    while(current!=NULL){
        current->freq=((current->count)/(float)total)*100;
        current=current->next;
    }
}

void setWordFreq(wordnode* head){

    int total=totalWordCount(head);

    wordnode* current=head->next;
    while(current!=NULL){
        current->freq=((current->count)/(float)total)*100;
        current=current->next;
    }
}


wordnode* sortWordList(wordnode * head,int charnum){
    int i,maxc;
    float maxf;
    char k[100];

    
    wordnode * temp=NULL;
    wordnode * current=head->next;
    wordnode * wordlen=createWordHead();
    
    if(charnum>head->count){
        charnum=head->count;
    }

    for(i=0;i<charnum;i++){
        maxc=0;
        maxf=0;

        current=head->next;
        
        while(current!=NULL){

            if(current->count>maxc){
                strcpy(k,(current->word));
                maxc=current->count;
                maxf=current->freq;
            }
            current=current->next;
        }
        current=head;
        while(strcmp(current->next->word,k)!=0){

            current=current->next;

        }
        temp=current->next;
        if(temp->next==NULL){
            current->next=NULL;
        }else{
            current->next=temp->next;
        }
        
        free(temp);
        addToWordlen(wordlen,k,maxf);
    }
    return wordlen;
}

void letterFrequency(charnode * head,FILE* f1){

    int ch1=getc(f1);
    
    while(!isalpha(ch1)){
        if(ch1==EOF){
            return ;
        }
        ch1=getc(f1); 
    }
       
    
    while(ch1!=EOF){

        if(isalnum(ch1)){
            
            if(isupper(ch1)){
                ch1=tolower(ch1);
            }
            if(isLetterInList(head,ch1)){
                increaseLetterCount(head,ch1);
            }else{
                addNewLetterToEnd(head,ch1);
            }
            
        }
        ch1=getc(f1);
    }
    
    
    
}

void wordFrequency(wordnode* head,FILE* f2){
    char buff[100];
    int index;
    
    char ch2=getc(f2);
    while(!isalnum(ch2)){
        if(ch2==EOF){
            return ;
        }
        ch2=getc(f2); 
    }

    while(ch2!=EOF){
        
        index=0;
        while(isspace(ch2) || (!isalnum(ch2))){
            ch2=getc(f2);
            if(ch2==EOF){
                break;
            }
        }

        if(ch2==EOF){
            break;
        }

        if(isalnum(ch2)){
            while(!isspace(ch2)){
                if(isalnum(ch2)){
                    if(isupper(ch2)){
                        ch2=tolower(ch2);
                    }   

                    buff[index]=ch2;
                }
                ch2=getc(f2);

                if(isspace(ch2) || ch2==EOF){
                    buff[index+1]='\0';
                    break;
                }
                if(isalnum(ch2)){
                    index++;
                }
                
            }
        }

        if(isWordInList(head,buff)){
            increaseWordCount(head,buff);
        }else{
            addNewWordToEnd(head,buff);
        }
        
        ch2=getc(f2);
        
    }
    
}

