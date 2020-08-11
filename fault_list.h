struct fault_list{
  int signal_no;
  unsigned char fault_ptn;
  struct fault_list *prev; //前のデータがある場所
  struct fault_list *next; //次のデータがある場所
} FAULT_LIST;
//常にsignal_no順に格納される。0,1は順不同

struct fault_list *list_make(int signal_no,unsigned char fault_ptn){
  struct fault_list *new_list;
  
  new_list = malloc(sizeof(struct fault_list));
  if(new_list ==NULL){
    printf("List make malloc Error.\n");
    exit(-1);
  }

  new_list->signal_no = signal_no;
  new_list->fault_ptn = fault_ptn;
  new_list->prev = NULL;
  new_list->next = NULL;

  return new_list;
}

struct fault_list *list_add(struct fault_list *end,int signal_no,unsigned char fault_ptn){
  //リストに要素を追加。最後の要素のポインタを返す
  struct fault_list *new_list,*now,*next;
  if(end == NULL){
    return list_make(signal_no,fault_ptn);
  }
  new_list = malloc(sizeof(struct fault_list));
  if(new_list == NULL){
    printf("List Add malloc Error.\n");
    exit(-1);
  }
  new_list->signal_no = signal_no;
  new_list->fault_ptn = fault_ptn;
  now = end;
  next = NULL;
  while((now->signal_no) > signal_no && now->prev != NULL){
    next = now;
    now = now->prev; //nowがsignal_no以下となるようにもどっていく
  }
  if(next == NULL && (now->signal_no < signal_no)){ //最後に追加する場合
    new_list->prev = end;
    new_list->next = NULL;
    end->next = new_list;
    return new_list;
  }else if(next == NULL){ //最後から2番目に追加
    if(now->signal_no == signal_no && now->fault_ptn == fault_ptn){
      //nothing do
      free(new_list);
      return end;
    }
    if(now->prev != NULL){
      if(now->prev->signal_no == signal_no){ //2連続
	//nothing do
	free(new_list);
	return end;
      }
      now->prev->next = new_list;
    }
    new_list->next = now;
    new_list->prev = now->prev;
    now->prev = new_list;
    return now;
  }else if(now->prev == NULL){ //最初まで戻った場合
    if((now->signal_no == signal_no) && (now->fault_ptn == fault_ptn)){
      //nothing do
      free(new_list);
      return end;
    }else if(now->signal_no <= signal_no){
      new_list->prev = now;
      new_list->next = next;
      next->prev = new_list;
      now->next = new_list;
      return end;
    }else{ //頭に追加
      now->prev = new_list;
      new_list->next = now;
      new_list->prev = NULL;
      return end;
    }
  }else{
    if((now->prev->signal_no == signal_no) || ((now->signal_no == signal_no) && (now->fault_ptn == fault_ptn))){
      //2連続できた場合(0,1両方が既に存在)か同じデータが存在した場合は何もしない
      free(new_list);
      return end;
    }else{
      new_list->prev = now;
      new_list->next = next;
      now->next = new_list;
      next->prev = new_list;
      return end;
    }
  }
}

struct fault_list *list_rm(struct fault_list *end,int signal_no,unsigned char fault_ptn){
  //リストから要素を削除。削除した要素のポインタを返す
  struct fault_list *tmp,*now,*next;

  now = end;
  next = NULL;
  while((now->signal_no) > signal_no && now->prev != NULL){
    next = now;
    now = now->prev; //nowがsignal_no以下となるようにもどっていく
  }
  if(now->signal_no != signal_no){
    //何もしない
    return end;
  }else if ((now->signal_no == signal_no) && (now->fault_ptn == fault_ptn)){
    if(next != NULL){
      next->prev = now->prev;
      now->prev->next = next;
      free(now);
      return end;
    }else{
      tmp = now->prev;
      now->prev->next = NULL;
      free(now);
      return tmp;
    }
  }else if(now->prev == NULL){
    //nothing do
    return end;
  }else if(now->prev->signal_no == signal_no){ //2連続の場合必ずある
    tmp = now->prev;
    now->prev = tmp->prev;
    tmp->prev->next = now;
    free(tmp);
    return end;
  }else{
    //nothing do
    return end;
  }
}
