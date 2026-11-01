#include "lexer.h"
int main(){
    FILE *fp=fopen("sample.c","r");
    if(fp==NULL){
        printf("Unable to open a file\n");
        return 1;
    }
    analyze_file(fp);
    fclose(fp);
    return 0;
}

