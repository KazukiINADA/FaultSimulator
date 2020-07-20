#include<stdio.h>
#include<stdlib.h>
#include<time.h>

//関数のプロトタイプ宣言
void mk_rand_file(int numlong,int max);
void shaker_sort(int list[],int numlong);

int main (int argc, char *argv[]){
  FILE *fp;
  int input,i;
  int list[100];
  clock_t start_time,end_time;

  mk_rand_file(100,1000);

  if((fp = fopen("rand_list.txt","r")) == NULL){
    printf("File open Error,\n");
    exit(-1);
  }

  for(i=0;i<100;i++){
    if(fscanf(fp,"%d",&input) != 1){
      printf("File read Error.\n");
      exit(-1);
    }
    list[i] = input;
  }
  
  fclose(fp);

  //start_time = clock();
  shaker_sort(list,100);
  //  end_time = clock();
  //  printf("処理時間：%f秒\n",(double)(end_time-start_time));

  if((fp = fopen("sorted_list.txt","w")) == NULL){
    printf("File open Error,\n");
    exit(-1);
  }

  for(i=0;i<100;i++){
    fprintf(fp,"%d\n",list[i]);
  }
  
  fclose(fp);
  
  return 0;
}

void mk_rand_file(int numlong,int max){
  //numlong個の数字(0以上max以下の数字)の重複なし数字リストファイルを作成
  int i,j;
  char exist_fg;
  int *rand_list;
  FILE *fp;

  srand((unsigned int)time(NULL));

  if((numlong+1) >= max){
    printf("function argument error\n");
    exit(-1);
  }

  rand_list = malloc(numlong*sizeof(int));
  if((fp = fopen("rand_list.txt","w")) == NULL){
    printf("file open error\n");
    exit(-1);
  }
  
  rand_list[0] = rand()%(max+1);

  for(i=1;i<numlong;i++){
    do{
      exist_fg = 0;
      rand_list[i] = rand()%(max+1);
      for(j=0;j<i;j++){
	if(rand_list[i] == rand_list[j]){
	  exist_fg = 1;
	  break;
	}
      }
    }while(exist_fg == 1);
  }

  for(i=0; i < numlong; i++){
    fprintf(fp,"%d\n",rand_list[i]);
  }

  fclose(fp);
  free(rand_list);
}

void shaker_sort(int list[],int numlong){
  //list内のnumlong個の数字をシェーカーソートで昇順に並び替える
  int tmp,i,head,tail;
  char swp_flg;

  head=0;
  tail=numlong-1;

  while(1){
    //headからtailへ
    swp_flg = 0;
    for(i=head;i<tail;i++){
      if(list[i] > list[i+1]){
	swp_flg = 1;
	tmp = list[i];
	list[i] = list[i+1];
	list[i+1] = tmp;
      }
    }
    tail--;
    if(head == tail || swp_flg == 0){
      break;
    }
    //tailからheadへ
    swp_flg = 0;
    for(i=tail;i>head;i--){
      if(list[i-1] > list[i]){
	swp_flg = 1;
	tmp = list[i];
	list[i] = list[i-1];
	list[i-1] = tmp;
      }
    }
    head++;
    if(head == tail || swp_flg == 0){
      break;
    }
  }  
}
