#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>

#define SIZE 10

struct queue{
  unsigned int list[SIZE];
  unsigned int head; //最初のデータの場所を覚える(書き込み済み)
  unsigned int tail; //次に書き込む場所を覚える(未書き込み)
};
//空状態は表現できる、満状態は表現しない

int enqueue(struct queue *qp,int data){
  if(((qp->head)-1 == (qp->tail)) || ((qp->head)==0 && (qp->tail)==(SIZE-1)) ){
    //すでに循環キューはいっぱい
    return -1;
  }
  qp->list[qp->tail] = data;
  qp->tail = (((qp->tail) +1)%SIZE);
  return 0;
}

int dequeue(struct queue *qp){
  if((qp->head)==(qp->tail)){
    //循環キューは空
    return -1;
  }
  (qp->head)++;
  if((qp->head) >= SIZE){
    qp->head = 0;
    return (int)qp->list[(SIZE-1)];
  }else{
    return (int)qp->list[((qp->head) - 1)];
  }
}

void show_queue(struct queue *qp){
  int i;
  if((qp->head)==(qp->tail)){
    printf("Empty\n");
    return;
  }
  else{
    i=0;
    do{
      printf("%d ",qp->list[((qp->head)+i)%SIZE]);
      i++;
    }while(qp->list[qp->tail] != qp->list[((qp->head)+i)%SIZE]);
    printf("\n");
    return;
  }
}

int main (int argc, char *argv[]){
  int i,data;
  char input[255];
  struct queue que;
  que.head = 0;
  que.tail = 0;

  srand((unsigned int)time(NULL));

  for(i=0;i<5;i++){
    enqueue(&que,rand()%101);
  }

  printf("Queue:");
  show_queue(&que);

  do{
    printf("> ");
    scanf("%s",input);

    if(strncmp(input,"dequeue",8)==0){
      if((data = dequeue(&que)) < 0){
	printf("Queue is Empty.\n");
	break;
      }else{
	printf("defueued data:%d\n",data);
	printf("Queue:");
	show_queue(&que);
      }
    }else if(strncmp(input,"enqueue",8)==0){
      printf("data: >");
      scanf("%d",&data);
      if(data > 100 || data < 0){
	printf("%d is too large or too small.\n",data);
	continue;
      }
      if(enqueue(&que,data) < 0){
        printf("Queue is Full.\n");
	break;
      }else{
        printf("Queue:");
	show_queue(&que);
      }
    }else{
      printf("Please enter \"enqueue\"or\"dequeue\".\n");
    }
  }while(1);
  return 0;
}
