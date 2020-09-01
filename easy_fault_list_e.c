#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>
#include"fault_list.h"

#define SIZE 50000 //キューのサイズ。SIZE-1まで格納できる

struct queue{
  unsigned int list[SIZE];
  unsigned int head; //最初のデータの場所を覚える(書き込み済み)
  unsigned int tail; //次に書き込む場所を覚える(未書き込み)
};
//空状態は表現できる、満状態は表現しない


//関数のプロトタイプ宣言
int enqueue(struct queue *qp,int data);
int dequeue(struct queue *qp);
void show_queue(struct queue *qp);
char logic_cal(int (*signal_line)[6],int line_no,unsigned int *pointa_list);

//e回路用
int main (int argc, char *argv[]){
  int i,j,now_no;
  char input[255];
  char filename[511];
  FILE *fp;
  int input_rd;

  int (*signal_line)[6];
  unsigned int *input_list;
  unsigned int *output_list;
  unsigned int signal_lines;
  unsigned int input_lists;
  unsigned int output_lists;
  unsigned int *pointa_list;
  unsigned int pointa_lists;

  unsigned char **test_ptn;
  unsigned int test_ptns;
  unsigned int ptn_i;

  unsigned int (*fault_ptn)[2];
  unsigned int fault_ptns;
  unsigned int fault_i;

  struct fault_list *end_p;
  struct fault_list *start_p;

  unsigned char **true_out;

  unsigned int found_faults;
  char flg;

  int *cal_no;
  int *cal_no_inv;
  
  struct queue que;
  que.head = 0;
  que.tail = 0;

  //回路テーブルの読み込み
  if(argc < 2){
    printf("No argument Error.\n");
    exit(-1);
  } 
  strcpy(filename,"/mnt/tiger/home/test_db/comb/benchmark/iscas85/");
  strcat(filename,argv[1]);
  strcat(filename,".tbl");

  if((fp = fopen(filename,"r")) == NULL){
    printf("tbl File open Error.\n");
    exit(-1);
  }

  //信号線読み込み
  if(fscanf(fp,"%d",&signal_lines) != 1){
    printf("tbl File read Error.\n");
    exit(-1);
  }

  signal_line = malloc(signal_lines*6*sizeof(int));

  if(signal_line == NULL){
    printf("signal line malloc Error.\n");
    exit(-1);
  }
  
  for(i=0;i<signal_lines;i++){
    for(j=0;j<5;j++){
      if(fscanf(fp,"%d",&signal_line[i][j]) != 1){
	printf("tbl File read Error.\n");
	exit(-1);
      }
    }
  }

  //ポインタリスト読み込み
  if(fscanf(fp,"%d",&pointa_lists) != 1){
    printf("tbl File read Error.\n");
    exit(-1);
  }

  pointa_list = malloc(pointa_lists*sizeof(unsigned int));

  if(pointa_list == NULL){
    printf("pointa list malloc Error.\n");
    exit(-1);
  }
  
  for(i=0;i<pointa_lists;i++){
    if(fscanf(fp,"%d",&pointa_list[i]) != 1){
      printf("tbl File read Error.\n");
      exit(-1);
    }
  }

  //入力リスト読み込み
  if(fscanf(fp,"%d",&input_lists) != 1){
    printf("tbl File read Error.\n");
    exit(-1);
  }

  input_list = malloc(input_lists*sizeof(unsigned int));

  if(input_list == NULL){
    printf("input list malloc Error.\n");
    exit(-1);
  }
  
  for(i=0;i<input_lists;i++){
    if(fscanf(fp,"%d",&input_list[i]) != 1){
      printf("tbl File read Error.\n");
      exit(-1);
    }
  }
  
  //出力リスト読み込み
  if(fscanf(fp,"%d",&output_lists) != 1){
    printf("tbl File read Error.\n");
    exit(-1);
  }

  output_list = malloc(output_lists*sizeof(unsigned int));

  if(output_list == NULL){
    printf("output list malloc Error.\n");
    exit(-1);
  }
  
  for(i=0;i<output_lists;i++){
    if(fscanf(fp,"%d",&output_list[i]) != 1){
      printf("tbl File read Error.\n");
      exit(-1);
    }
  }

  fclose(fp);

  //テストパターン読み込み
  strcpy(filename,"/mnt/tiger/home/test_db/comb/benchmark/iscas85/Pattern/");
  strcat(filename,argv[1]);
  strcat(filename,".pat");

  if((fp = fopen(filename,"r")) == NULL){
    printf("pat File open Error.\n");
    exit(-1);
  }

  if(fscanf(fp,"%d",&test_ptns) != 1){
    printf("pat File read Error.\n");
    exit(-1);
  }

  if((test_ptn = malloc(test_ptns*sizeof(unsigned char *)))==NULL){
    printf("test ptn malloc Error.\n");
    exit(-1);
  }
  for(i=0;i<test_ptns;i++){
    if((test_ptn[i] = malloc(input_lists*sizeof(unsigned char))) == NULL){
      printf("test ptn malloc Error.\n");
      exit(-1);  
    }    
  }
  
  for(i=0;i<test_ptns;i++){
    for(j=0;j<input_lists;j++){
      if(fscanf(fp,"%d",&input_rd) != 1){
	printf("pat File read Error.\n");
	exit(-1);
      }else{
	test_ptn[i][j] = (unsigned char)input_rd;
      }
    }
  }

  fclose(fp);

  //計算順番を格納する配列を確保、初期化
  cal_no = malloc((signal_lines+1)*sizeof(int));

  if(cal_no == NULL){
    printf("cal no malloc Error.\n");
    exit(-1);
  }
  for(i=0;i<signal_lines;i++){
    cal_no[i] = -1;
  }
  cal_no_inv = malloc(signal_lines*sizeof(int));

  if(cal_no_inv == NULL){
    printf("cal no inv malloc Error.\n");
    exit(-1);
  }

  //正しい出力を記録する配列を確保
  if((true_out = malloc(test_ptns*sizeof(unsigned char *)))==NULL){
    printf("true out malloc Error.\n");
    exit(-1);
  }
  for(i=0;i<test_ptns;i++){
    if((true_out[i] = malloc(signal_lines*sizeof(unsigned char))) == NULL){
      printf("true out malloc Error.\n");
      exit(-1);  
    }    
  }

  //計算順番決定
  //出力線を未定義へ
  for(i=0;i<signal_lines;i++){
    signal_line[i][5] = -1;
  }
  //テストデータ(ALL1)で仮設定
  for(i=0;i<input_lists;i++){
    if(signal_line[(input_list[i]-1)][0] != 0){
      printf("input set Error.\n");
      exit(-1);
    }else{
      signal_line[(input_list[i]-1)][5] = 1;
    }
  }
  
  now_no = 0;
  //最初の信号線出力計算
  for(i=1;i<=signal_lines;i++){
    if(signal_line[(i-1)][5] == -1){ //信号線が既に決まっている場合はスキップ
      if(logic_cal(signal_line,i,pointa_list) == -1){
	if(enqueue(&que,i) == -1){
	  printf("queue full Error\n");
	  exit(-1);
	}
      }else{ //計算できた場合は順番を覚える
	cal_no[now_no] = i;
	cal_no_inv[i-1] = now_no;
	now_no++;
      }
    }
  }
  
  //キューから取り出しながら再計算
  while((i = dequeue(&que)) != -1){
    if(logic_cal(signal_line,i,pointa_list) == -1){
      if(enqueue(&que,i) == -1){
	printf("queue full Error\n");
	exit(-1);
      }
    }else{ //計算できた場合は順番を覚える
      cal_no[now_no] = i;
      cal_no_inv[i-1] = now_no;
      now_no++;
    }
  }
  //ここまで順番決定
  
  /* ここから正規出力計算 */
  //テストパターン抽出
  for(ptn_i=0;ptn_i<test_ptns;ptn_i++){
    //信号線出力を未定義(-1)へ
    for(i=0;i<signal_lines;i++){
      signal_line[i][5] = -1;
    }

    //テストパターンを入力信号線へ設定
    for(i=0;i<input_lists;i++){
      if(signal_line[(input_list[i]-1)][0] != 0){
	printf("input set Error.\n");
	exit(-1);
      }else{
	signal_line[(input_list[i]-1)][5] = test_ptn[ptn_i][i];
      }
    }
    
    i = 0;
    while(cal_no[i] != -1){
      if(logic_cal(signal_line,cal_no[i],pointa_list) == -1){
	printf("logic cal roop(more than 2) Error.\n");
	exit(-1);
      }
      i++;
    }
    
    //出力をtrue_out に保存
    for(i=0;i<signal_lines;i++){
      if(signal_line[i][5] == -1){
	printf("output line Error(Line %d).\n",i);
	exit(-1);
      }else{
	true_out[ptn_i][i] = (unsigned char)signal_line[i][5];
      }
    }
    
  //テストパターンループの終わり
  }
  /* ここまで正規出力計算 */

  //故障パターンファイル読み込み
  strcpy(filename,"/mnt/tiger/home/test_db/comb/benchmark/iscas85/Faultset/");
  strcat(filename,argv[1]);
  strcat(filename,"f.rep");

  if((fp = fopen(filename,"r")) == NULL){
    printf("fault ptn File open Error.\n");
    exit(-1);
  }

  //故障パターン読み込み
  if(fscanf(fp,"%d",&fault_ptns) != 1){
    printf("tbl File read Error.\n");
    exit(-1);
  }
  
  fault_ptn = malloc(fault_ptns*2*sizeof(unsigned int));

  if(fault_ptn == NULL){
    printf("signal line malloc Error.\n");
    exit(-1);
  }

  end_p = NULL;
  start_p = NULL;
  
  for(i=0;i<fault_ptns;i++){
    for(j=0;j<2;j++){
      if(fscanf(fp,"%d",&fault_ptn[i][j]) != 1){
	printf("rep File read Error.\n");
	exit(-1);
      }
    }
    end_p = list_add(end_p,cal_no_inv[fault_ptn[i][0]-1],(unsigned char)fault_ptn[i][1]);
    if(start_p == NULL){
      start_p = end_p;
    }
    while(start_p ->prev != NULL){
      start_p = start_p->prev;
    }
  }

  found_faults = 0;

  /* ここから故障出力計算 */
  for(fault_i=0;fault_i<fault_ptns;fault_i++){
    flg = 0; //検出済み用フラグ
    //テストパターン抽出
    for(ptn_i=0;ptn_i<test_ptns;ptn_i++){
      //信号線出力を正常状態から読み出す
      for(i=0;i<signal_lines;i++){
	signal_line[i][5] = true_out[ptn_i][i];
      }

      //故障信号線の定義
      signal_line[(fault_ptn[fault_i][0]-1)][5] = fault_ptn[fault_i][1];
      
      i = 0;
      if(signal_line[(fault_ptn[fault_i][0]-1)][0] != 0){ //計算対象(input以外)の場合は
	while(cal_no[i] != fault_ptn[fault_i][0]){
	  i++; //故障信号線より前の計算はしない
	}
	i++; //故障箇所の計算もしない
      }
      while(cal_no[i] != -1){
	if(logic_cal(signal_line,cal_no[i],pointa_list) == -1){
	  printf("logic cal roop(more than 2) Error.\n");
	  exit(-1);
	}
	i++;
      }

      //出力をtrue_outと比較
      for(i=0;i<output_lists;i++){
	if(signal_line[(output_list[i]-1)][5] == -1){
	  printf("output line Error2.\n");
	  exit(-1);
	}else{
	  if(true_out[ptn_i][(output_list[i]-1)] != signal_line[(signal_line[(output_list[i]-1)][4])-1][5]){
	    found_faults ++;
	    flg = 1;
	    break;
	  }
	}
      }
      if(flg == 1){ //既に検出した場合は次の故障パターンへ
	break;
      }
    //テストパターンループの終わり
    }
  //故障パターンループ終わり
  }
  /* ここまで正規出力計算 */

  printf("合計故障数：%d　検出故障数：%d　故障検出率：%.2f　",fault_ptns,found_faults,(double)found_faults*100/fault_ptns);

  printf("処理時間：%.2f\n",(double)clock()/CLOCKS_PER_SEC);

  free(signal_line);
  free(pointa_list);
  free(input_list);
  free(output_list);
  for(i=0;i<test_ptns;i++){
    free(test_ptn[i]);
  }
  free(test_ptn);
  for(i=0;i<test_ptns;i++){
    free(true_out[i]);
  }
  free(true_out);
  free(fault_ptn);

  return 0;
}

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

char logic_cal(int (*signal_line)[6],int line_no,unsigned int *pointa_list){
  //信号線の出力を計算、入力未定義の場合は-1を返す
  int res;
  int i;
  int pointa_head;

  switch(signal_line[line_no-1][0]){
  case 0: //外部入力
    //nothing do
    return (char)signal_line[line_no-1][5];
    break;
  case 1: //or
    if(signal_line[line_no-1][1] <= 1){ //入力数
      //printf("Logic cal OR Error(%dth Line).\n",line_no);
      //exit(-1);
      signal_line[line_no-1][5] = signal_line[(signal_line[line_no-1][2])-1][5];
      return (char)signal_line[line_no-1][5];
    }else{
      res = 0;
      pointa_head = signal_line[line_no-1][2];
      for(i=0;i<signal_line[line_no-1][1];i++){
	if(signal_line[(pointa_list[(pointa_head-1+i)])-1][5] == -1){
	  signal_line[line_no-1][5] = -1;
	  return -1;
	}else{
	  res += signal_line[(pointa_list[(pointa_head-1+i)])-1][5];
	}
      }
      if(res != 0){
	res = 1;
      }
      signal_line[line_no-1][5] = res;
      return (char)res;
    }
    break;
  case 2: // AND
    if(signal_line[line_no-1][1] <= 1){ //入力数
      //printf("Logic cal AND Error(%dth Line).\n",line_no);
      //exit(-1);
      signal_line[line_no-1][5] = signal_line[(signal_line[line_no-1][2])-1][5];
      return (char)signal_line[line_no-1][5];
    }else{
      res = 1;
      pointa_head = signal_line[line_no-1][2];
      for(i=0;i<signal_line[line_no-1][1];i++){
	if(signal_line[(pointa_list[(pointa_head-1+i)])-1][5] == -1){
	  signal_line[line_no-1][5] = -1;
	  return -1;
	}else{
	  res *= signal_line[(pointa_list[(pointa_head-1+i)])-1][5];
	}
      }
      if(res != 0){
	res = 1;
      }
      signal_line[line_no-1][5] = res;
      return (char)res;
    }
    break;
  case 3: //分岐の枝
    if(signal_line[line_no-1][1] > 1){ //入力数
      printf("Logic cal branch Error(%dth Line).\n",line_no);
      exit(-1);
    }else{
      signal_line[line_no-1][5] = signal_line[(signal_line[line_no-1][2])-1][5];
      return (char)signal_line[line_no-1][5];
    }
    break;
  case 4: //外部出力
    if(signal_line[line_no-1][1] > 1){ //入力数
      printf("Logic cal Point Out Error(%dth Line).\n",line_no);
      exit(-1);
    }else{
      signal_line[line_no-1][5] = signal_line[(signal_line[line_no-1][2])-1][5];
      return (char)signal_line[line_no-1][5];
    }
    break;
  case 5: //EXOR
    if(signal_line[line_no-1][1] <= 1){ //入力数
      printf("Logic cal EXOR Error(%dth Line).\n",line_no);
      exit(-1);
    }else{
      res = 0;
      pointa_head = signal_line[line_no-1][2];
      for(i=0;i<signal_line[line_no-1][1];i++){
	if(signal_line[(pointa_list[(pointa_head-1+i)])-1][5] == -1){
	  signal_line[line_no-1][5] = -1;
	  return -1;
	}else{
	  if(signal_line[(pointa_list[(pointa_head-1+i)])-1][5] == 1){
	    if(res == 0){
	      res = 1;
	    }else{
	      res = 0;
	    }
	  } //入力0の時は何もしない
	}
      }
      signal_line[line_no-1][5] = res;
      return (char)res;
    }
    break;
  case -1: //NAND
    if(signal_line[line_no-1][1] <= 1){ //入力数
      //printf("Logic cal NAND Error(%dth Line).\n",line_no);
      //exit(-1);
      switch(signal_line[(signal_line[line_no-1][2])-1][5]){
      case -1:
	res = -1;
	break;
      case 0:
	res = 1;
	break;
      case 1:
	res = 0;
	break;
      default:
	res = -1;
	printf("Error :Line %d NOT gate inputed undefined signal(%d)\n",line_no,signal_line[(signal_line[line_no-1][2])-1][5]);
	exit(-1);
	break;
      }
      signal_line[line_no-1][5] = res;
      return (char)signal_line[line_no-1][5];
    }else{
      res = 1;
      pointa_head = signal_line[line_no-1][2];
      for(i=0;i<signal_line[line_no-1][1];i++){
	if(signal_line[(pointa_list[(pointa_head-1+i)])-1][5] == -1){
	  signal_line[line_no-1][5] = -1;
	  return -1;
	}else{
	  res *= signal_line[(pointa_list[(pointa_head-1+i)])-1][5];
	}
      }
      if(res != 0){
	res = 0;
      }else{
	res = 1;
      }
      signal_line[line_no-1][5] = res;
      return (char)res;
    }
    break;
  case -2: //NOR
    if(signal_line[line_no-1][1] <= 1){ //入力数
      //printf("Logic cal NOR Error(%dth Line).\n",line_no);
      //exit(-1);
      switch(signal_line[(signal_line[line_no-1][2])-1][5]){
      case -1:
	res = -1;
	break;
      case 0:
	res = 1;
	break;
      case 1:
	res = 0;
	break;
      default:
	res = -1;
	printf("Error :Line %d NOR gate inputed undefined signal(%d)\n",line_no,signal_line[(signal_line[line_no-1][2])-1][5]);
	exit(-1);
	break;
      }
      signal_line[line_no-1][5] = res;
      return (char)signal_line[line_no-1][5];
    }else{
      res = 0;
      pointa_head = signal_line[line_no-1][2];
      for(i=0;i<signal_line[line_no-1][1];i++){
	if(signal_line[(pointa_list[(pointa_head-1+i)])-1][5] == -1){
	  signal_line[line_no-1][5] = -1;
	  return -1;
	}else{
	  res += signal_line[(pointa_list[(pointa_head-1+i)])-1][5];
	}
      }
      if(res != 0){
	res = 0;
      }else{
	res = 1;
      }
      signal_line[line_no-1][5] = res;
      return (char)res;
    }
    break;
  case -3: //NOT
    if(signal_line[line_no-1][1] > 1){ //入力数
      printf("Logic cal branch Error(%dth Line).\n",line_no);
      exit(-1);
    }else{
      switch(signal_line[(signal_line[line_no-1][2])-1][5]){
      case -1:
	res = -1;
	break;
      case 0:
	res = 1;
	break;
      case 1:
	res = 0;
	break;
      default:
	res = -1;
	printf("Error :Line %d NOT gate inputed undefined signal(%d)\n",line_no,signal_line[(signal_line[line_no-1][2])-1][5]);
	exit(-1);
	break;
      }
      signal_line[line_no-1][5] = res;
      return (char)signal_line[line_no-1][5];
    }
    break;
  default:
    printf("Logic cal Error(%dth Line).\n",line_no);
    exit(-1);
    break;
  }
}
