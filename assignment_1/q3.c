#include "stack.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
int main(){
    char a[81];
    while(1){
    scanf("%s",a);
    if(strcmp(a,"quit")==0){break;}
    stackADT s=EmptyStack();
    int i=0;
    while(a[i]!='\0'){
        Push(s,a[i]);
        i++;
    }
    if(StackDepth(s)%2!=0){
        printf("no\n\n");
        continue;
    }
    else{
        stackADT up=EmptyStack();
        while(!StackIsEmpty(s)){
            char k=Pop(s);
            if(k<='Z'&&k>='A'){
                Push(up,k);
            }
            else{
                Push(s,k);
                break;
            }
        }
        if(StackDepth(up)!=StackDepth(s)){
            printf("no\n\n");
            continue;
        }
        int n=StackDepth(up);
        int same=1;
        for(i=0;i<n;i++){
            if(Pop(s)-Pop(up)!=32){
                same=0;
                break;
            }
        }
        if(same){printf("yes\n\n");}else{printf("no\n\n");}
    }
}
    return 0;
}
