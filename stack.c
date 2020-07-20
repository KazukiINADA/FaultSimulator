#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>

#define SIZE 10

struct stack{
  unsigned int list[SIZE];
  int last_index;
};

int push(struct stack *sp,int data){
  if((sp->last_index) >= (SIZE-2)){
    //すでにスタックはいっぱい(SIZE-1個まで格納可能)
    return -1;
  }
  (sp->last_index)++;
  sp->list[sp->last_index] = data;
  return 0;
}

int pop(struct stack *sp){
  if((sp->last_index) < 0){
    //スタックは空
    return -1;
  }
  (sp->last_index)--;
  return (int)sp->list[((sp->last_index) +1)];
}

void show_stack(struct stack *sp){
  int i;
  if((sp->last_index) < 0){
    printf("Empty\n");
    return;
  }
  else{
    for(i=0;i<=(sp->last_index);i++){
      printf("%d ",sp->list[i]);
    }
    printf("\n");
    return;
  }
}

int main (int argc, char *argv[]){
  int i,data;
  char input[255];
  struct stack stk;
  stk.last_index = -1;

  srand((unsigned int)time(NULL));

  for(i=0;i<5;i++){
    push(&stk,rand()%101);
  }

  printf("Stack:");
  show_stack(&stk);

  do{
    printf("> ");
    scanf("%s",input);

    if(strncmp(input,"pop",4)==0){
      if((data = pop(&stk)) < 0){
	printf("Stack is Empty.\n");
	break;
      }else{
	printf("popped data:%d\n",data);
	printf("Stack:");
	show_stack(&stk);
      }
    }else if(strncmp(input,"push",5)==0){
      printf("data: >");
      scanf("%d",&data);
      if(data > 100 || data < 0){
	printf("%d is too large or too small.\n",data);
	continue;
      }
      if(push(&stk,data) < 0){
        printf("Stack is Full.\n");
	break;
      }else{
        printf("Stack:");
	show_stack(&stk);
      }
    }else{
	printf("Please enter \"push\"or\"pop\".\n");
    }
  }while(1);
return 0;
}
