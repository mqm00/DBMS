#include "bpt.h"

H_P * hp;

page * rt = NULL; //root is declared as global

int fd = -1; //fd is declared as global


H_P * load_header(off_t off) {
    H_P * newhp = (H_P*)calloc(1, sizeof(H_P));
    if (sizeof(H_P) > pread(fd, newhp, sizeof(H_P), 0)) {

        return NULL;
    }
    return newhp;
}


page * load_page(off_t off) {
    page* load = (page*)calloc(1, sizeof(page));
    //if (off % sizeof(page) != 0) printf("load fail : page offset error\n");
    if (sizeof(page) > pread(fd, load, sizeof(page), off)) {

        return NULL;
    }
    return load;
}

int open_table(char * pathname) {
    fd = open(pathname, O_RDWR | O_CREAT | O_EXCL | O_SYNC  , 0775);
    hp = (H_P *)calloc(1, sizeof(H_P));
    if (fd > 0) {
        //printf("New File created\n");
        hp->fpo = 0;
        hp->num_of_pages = 1;
        hp->rpo = 0;
        pwrite(fd, hp, sizeof(H_P), 0);
        free(hp);
        hp = load_header(0);
        return 0;
    }
    fd = open(pathname, O_RDWR|O_SYNC);
    if (fd > 0) {
        //printf("Read Existed File\n");
        if (sizeof(H_P) > pread(fd, hp, sizeof(H_P), 0)) {
            return -1;
        }
        off_t r_o = hp->rpo;
        rt = load_page(r_o);
        return 0;
    }
    else return -1;
}

void reset(off_t off) {
    page * reset;
    reset = (page*)calloc(1, sizeof(page));
    reset->parent_page_offset = 0;
    reset->is_leaf = 0;
    reset->num_of_keys = 0;
    reset->next_offset = 0;
    pwrite(fd, reset, sizeof(page), off);
    free(reset);
    return;
}

void freetouse(off_t fpo) {
    page * reset;
    reset = load_page(fpo);
    reset->parent_page_offset = 0;
    reset->is_leaf = 0;
    reset->num_of_keys = 0;
    reset->next_offset = 0;
    pwrite(fd, reset, sizeof(page), fpo);
    free(reset);
    return;
}

void usetofree(off_t wbf) {
    page * utf = load_page(wbf);
    utf->parent_page_offset = hp->fpo;
    utf->is_leaf = 0;
    utf->num_of_keys = 0;
    utf->next_offset = 0;
    pwrite(fd, utf, sizeof(page), wbf);
    free(utf);
    hp->fpo = wbf;
    pwrite(fd, hp, sizeof(hp), 0);
    free(hp);
    hp = load_header(0);
    return;
}

off_t new_page() {
    off_t newp;
    page * np;
    off_t prev;
    if (hp->fpo != 0) {
        newp = hp->fpo;
        np = load_page(newp);
        hp->fpo = np->parent_page_offset;
        pwrite(fd, hp, sizeof(hp), 0);
        free(hp);
        hp = load_header(0);
        free(np);
        freetouse(newp);
        return newp;
    }
    //change previous offset to 0 is needed
    newp = lseek(fd, 0, SEEK_END);
    //if (newp % sizeof(page) != 0) printf("new page made error : file size error\n");
    reset(newp);
    hp->num_of_pages++;
    pwrite(fd, hp, sizeof(H_P), 0);
    free(hp);
    hp = load_header(0);
    return newp;
}

off_t find_leaf(int64_t key) {
    int i = 0;
    page * p;
    off_t loc = hp->rpo;

    //printf("left = %ld, key = %ld, right = %ld, is_leaf = %d, now_root = %ld\n", rt->next_offset, 
    //  rt->b_f[0].key, rt->b_f[0].p_offset, rt->is_leaf, hp->rpo);

    if (rt == NULL) {
        //printf("Empty tree.\n");
        return 0;
    }
    p = load_page(loc);

    while (!p->is_leaf) {
        i = 0;

        while (i < p->num_of_keys) {
            if (key >= p->b_f[i].key) i++;
            else break;
        }
        if (i == 0) loc = p->next_offset;
        else
            loc = p->b_f[i - 1].p_offset;
        //if (loc == 0)
        // return NULL;

        free(p);
        p = load_page(loc);

    }

    free(p);
    return loc;

}

char * db_find(int64_t key) {
    char * value = (char*)malloc(sizeof(char) * 120);
    int i = 0;
    off_t fin = find_leaf(key);
    if (fin == 0) {
        return NULL;
    }
    page * p = load_page(fin);

    for (; i < p->num_of_keys; i++) {
        if (p->records[i].key == key) break;
    }
    if (i == p->num_of_keys) {
        free(p);
        return NULL;
    }
    else {
        strcpy(value, p->records[i].value);
        free(p);
        return value;
    }
}

int cut(int length) {
    if (length % 2 == 0)
        return length / 2;
    else
        return length / 2 + 1;
}



void start_new_file(record rec) {

    page * root;
    off_t ro;
    ro = new_page();
    rt = load_page(ro);
    hp->rpo = ro;
    pwrite(fd, hp, sizeof(H_P), 0);
    free(hp);
    hp = load_header(0);
    rt->num_of_keys = 1;
    rt->is_leaf = 1;
    rt->records[0] = rec;
    pwrite(fd, rt, sizeof(page), hp->rpo);
    free(rt);
    rt = load_page(hp->rpo);
    //printf("new file is made\n");
}

//******************************************//
 
int db_insert(int64_t key, char * value) { //인서트 key, value
    fflush(stdout);
    record nr;      //삽입할 레코드 struct
    nr.key = key;       //레코드의 Key값에 인자로 받은 key
    strcpy(nr.value, value); //레코드의 value에는 인자로 받은 value
    if (rt == NULL) {       // 루트가 Null이면
        start_new_file(nr); //새로 만들어진 레코드를 가지고 새로운 tree를 만든다.
        return 0;
    }
    char * dupcheck;
    dupcheck = db_find(key); //key값이 같은 것이 있는지 확인
    if (dupcheck != NULL) { //중복된 값이 있다면
        free(dupcheck);
        return -1;      //종료
    }
    free(dupcheck);     //중복된 값이 없다면 일단 dupcheck는 free 해준다.

    off_t leaf = find_leaf(key); //find leaf - Q. 어느 블락에 있는지 확인

    page * leafp = load_page(leaf); //그 leaf에 대한 페이지 가져온다.

    if (leafp->num_of_keys < LEAF_MAX) { //찾은 리프 노드의 키가 max보다 작다면 -> 다 차있지 않다면
        insert_into_leaf(leaf, nr); //그 리프에 nr insert
        free(leafp);
        return 0;
    }
    printf("3");
    insert_into_leaf_as(leaf, nr); //찾은 리프 노드가 이미 꽉 차있다면
    free(leafp);
    //why double free?
    return 0;

}

off_t insert_into_leaf(off_t leaf, record inst) { //리프에 삽입

    page * p = load_page(leaf); // 찾은 리프에 대한 페이지 로드
    //if (p->is_leaf == 0) printf("iil error : it is not leaf page\n");
    int i, insertion_point;
    insertion_point = 0; //인서트 되는 위치
    while (insertion_point < p->num_of_keys && p->records[insertion_point].key < inst.key) {
        //인서트 위치가 노드의 크기보다 작고 그 위치에 있는 key값이 삽입될 key값보다 작다면
        insertion_point++; //한칸 뒤로
    }
    for (i = p->num_of_keys; i > insertion_point; i--) { // 새로운 record가 삽입될 위치보다 크거나 같은 key들을 한칸씩 뒤로 물러준다.
        p->records[i] = p->records[i - 1];
    }
    p->records[insertion_point] = inst; //미리 찾은 위치에 삽입
    p->num_of_keys++; //해당 페이지의 키 갯수 +1
    pwrite(fd, p, sizeof(page), leaf);
    //printf("insertion %ld is complete %d, %ld\n", inst.key, p->num_of_keys, leaf);
    free(p);
    return leaf;
}


off_t insert_into_leaf_as(off_t leaf, record inst) { //리프가 꽉 차 있을 때 삽입
    off_t new_leaf;
    record * temp;
    int insertion_index, split, i, j;
    int64_t new_key;
    new_leaf = new_page(); //새로운 리프
    //printf("\n%ld is new_leaf offset\n\n", new_leaf);
    page * nl = load_page(new_leaf); //새로운 리프에 대한 페이지
    nl->is_leaf = 1;
    temp = (record *)calloc(LEAF_MAX + 1, sizeof(record));  //레코드를 리프맥스+1 저장할 수 있는 공간 할당해라 -> 꽉 찬 상태에서 일단 삽입한 후에 다음 오퍼레이션 진행을 위함
    if (temp == NULL) {
        perror("Temporary records array");
        exit(EXIT_FAILURE);
    }
    insertion_index = 0;
    page * ol = load_page(leaf); //인자로 들어온 리프 -> 원래 삽입하려고 찾아놓은 리프 -> 꽉 찬 상태의 리프
    while (insertion_index < LEAF_MAX && ol->records[insertion_index].key < inst.key) {
                                                                        //삽입 위치 찾기, 리프 맥스보다 작고 해당 위치의 key값이 삽입하려는 key값보다 작다면
        insertion_index++; //위치 +1 칸
    }
    for (i = 0, j = 0; i < ol->num_of_keys; i++, j++) { //i가 ol, 즉 삽입하려고 찾은 리프의 키보다 작다면; i++,j++
        if (j == insertion_index) j++;  //j 가 삽입하려는 위치라면 j++ -> 한칸 건너 뛴다
        temp[j] = ol->records[i];       //temp는 전부다 밀어넣는 공간, ol과 같은 순서로 record를 넣는다.
    }
    temp[insertion_index] = inst; //그리고 삽입될 위치에 삽입
    ol->num_of_keys = 0; //ol은 이제 빈 리프
    split = cut(LEAF_MAX); //leaf_max가 2의 배수라면 max/2, 아니라면 max/2+1

    for (i = 0; i < split; i++) { //i가 split보다 작다면
        ol->records[i] = temp[i]; // temp에는 현재 삽입하려한 key와 그 키가 들어갈 리프에 있던 key들이 순서에 맞게 들어가있다.
        ol->num_of_keys++; //그래서 ol에 다시 순서에 맞게 가져온다.
    }

    for (i = split, j = 0; i < LEAF_MAX + 1; i++, j++) { //split 기준에 i가 도달했다면 거기서부터 끝까지를 새로운 리프에 저장한다.
        nl->records[j] = temp[i];
        nl->num_of_keys++;
    }

    free(temp);

    nl->next_offset = ol->next_offset; // 원래 리프가 왼쪽 새로 만들어진 리프가 오른쪽에 위치한다. -> 원래 리프가 가리키던 offset을 이제는 새로 만들어진 리프가 가리켜야한다.
    ol->next_offset = new_leaf; //그리고 원래 있던 리프의 offset은 새로 만들어진 리프가 된다.

    for (i = ol->num_of_keys; i < LEAF_MAX; i++) { //i가 원래 리프의 키의 갯수이고, 그런 i가 리프 맥스보다 작다면,
        ol->records[i].key = 0; // 리프에 key값을 옮기고 나서 남은 나머지 부분은 0으로 채운다
        //strcpy(ol->records[i].value, NULL);
    }

    for (i = nl->num_of_keys; i < LEAF_MAX; i++) { // nl도 마찬가지로 남은 부분은 0으로 채운다.
        nl->records[i].key = 0;
        //strcpy(nl->records[i].value, NULL);
    }

    nl->parent_page_offset = ol->parent_page_offset; //parent pointer를 옮겨주는 것. ol의 부모였던 offset이 nl의 부모 offset이 된다. 그리고 ol의 부모는 nl에서 올라가야하는데 이를 insert_into_parent에서 할 것으로 추측
    new_key = nl->records[0].key; //그 새로운 키는 새로 만들어진 리프의 가장 첫번째 키

    pwrite(fd, nl, sizeof(page), new_leaf);
    pwrite(fd, ol, sizeof(page), leaf);
    free(ol);
    free(nl);
    //printf("split_leaf is complete\n");

    return insert_into_parent(leaf, new_key, new_leaf);

}

off_t insert_into_parent(off_t old, int64_t key, off_t newp) { //old: split후 왼쪽에 위치한 리프, key: split후 새로 만들어진 리프의 첫번째 key, newp: split후 새로 만들어진 리프

    int left_index;
    off_t bumo;
    page * left;
    left = load_page(old); //split후 왼쪽에 위치한 리프를 left에 할당

    bumo = left->parent_page_offset; //left의 parent offset을 부모
    free(left);

    if (bumo == 0)      //  Q. 부모가 0이다 -> left의 parent offset이 0이다 -> root
        return insert_into_new_root(old, key, newp); //루트가 split된 경우이므로 새로운 root를 삽입한다.

    left_index = get_left_index(old); //Q

    page * parent = load_page(bumo); //bumo가 위치한 페이지 로드
    //printf("\nbumo is %ld\n", bumo);
    if (parent->num_of_keys < INTERNAL_MAX) { // 부모가 위치한 페이지의 key의 갯수가 internal_max보다 작다면
        free(parent);
        //printf("\nuntil here is ok\n");
        return insert_into_internal(bumo, left_index, key, newp); // 그냥 internal에 넣으면 된다.
    }
    free(parent);
    return insert_into_internal_as(bumo, left_index, key, newp); //그게 아니라면 internal에서도 split이 일어난다.
}
//스플릿한 후에 부모 노드에 넣을 땐 부모에서 원래 노드를 가리키던 포인터 오른쪽에 키와 포인터를 넣는다.
//그렇다면 get_left_index가 부모에서 원래 노드를 가리키던 포인터를 찾는 함수인가?

int get_left_index(off_t left) {        //insert_into_internal은 부모의 레프트 인덱스에 새로운 키와 새로운 offset을 넣는 것. 거기서 레프트 인덱스를 찾는 함수
    page * child = load_page(left);        //인자로 들어온 offset의 페이지를 로드
    off_t po = child->parent_page_offset; //그 페이지의 부모페이지의 offset을 po에 할당
    free(child);
    page * parent = load_page(po);      //부모 페이지 로드
    int i = 0;
    if (left == parent->next_offset) return -1; // 인자로 받은 레프트가 부모의 다음 offset이라면 return -1 ->부모의 오른쪽 offset이라면 잘못된 것
    for (; i < parent->num_of_keys; i++) {      // i가 부모페이지의 키의 갯수보다 작다면 i++ -> 즉 부모 노드의 offset을 하나씩 비교하는 것
        if (parent->b_f[i].p_offset == left) break; //부모의 i번째 offset이 left와 같다면 break -> Q. 부모에서 인자로 들어온 페이지, 즉 원래 노드를 가리키는 offset의 키 값을 찾는 것?
    }

    if (i == parent->num_of_keys) { //i가 부모의 마지막 키 위치와 같다면? -> 마지막 record라면 ->
        free(parent);       //
        return -10;
    }
    free(parent);
    return i;
}

off_t insert_into_new_root(off_t old, int64_t key, off_t newp) { //새로운 root로 삽입

    off_t new_root;
    new_root = new_page();
    page * nr = load_page(new_root);
    nr->b_f[0].key = key;
    nr->next_offset = old;
    nr->b_f[0].p_offset = newp;
    nr->num_of_keys++;
    //printf("key = %ld, old = %ld, new = %ld, nok = %d, nr = %ld\n", key, old, newp, 
    //  nr->num_of_keys, new_root);
    page * left = load_page(old);
    page * right = load_page(newp);
    left->parent_page_offset = new_root;
    right->parent_page_offset = new_root;
    pwrite(fd, nr, sizeof(page), new_root);
    pwrite(fd, left, sizeof(page), old);
    pwrite(fd, right, sizeof(page), newp);
    free(nr);
    nr = load_page(new_root);
    rt = nr;
    hp->rpo = new_root;
    pwrite(fd, hp, sizeof(H_P), 0);
    free(hp);
    hp = load_header(0);
    free(left);
    free(right);
    return new_root;

}

off_t insert_into_internal(off_t bumo, int left_index, int64_t key, off_t newp) { //자식 노드에서 split이 일어났을 때 부모 노드에 자리가 있는 경우, 부모의 offset, 원래 노드를 가리키던 부모의 offset, 삽입된 key값, split으로 인해 새로 만들어진 노드

    page * parent = load_page(bumo); //parent = bumo 페이지
    int i;

    for (i = parent->num_of_keys; i > left_index + 1; i--) { //i가 부모의 키 갯수, i가 left_index + 1보다 크다면 i-- -> 뒤에서부터 삽입될 위치 탐색
        parent->b_f[i] = parent->b_f[i - 1]; //Q. b_f가 뭐지
    }
    //부모노드에 새로 삽입될 위치를 찾았다면, i에 저장되어있음
    parent->b_f[left_index + 1].key = key; //원래 가리키던 record 위치의 오른쪽에 삽입된 key값 넣는다.
    parent->b_f[left_index + 1].p_offset = newp; //원래 노드를 가리키던 위치에서 +1한 곳의 offset은 새로 만들어진 노드를 가리키도록 한다
    parent->num_of_keys++; //부모의 키 갯수 +1 -> 새로운 키 값 들어왔으니까
    pwrite(fd, parent, sizeof(page), bumo);
    free(parent);
    if (bumo == hp->rpo) {
        free(rt);
        rt = load_page(bumo);
        //printf("\nrt->numofkeys%lld\n", rt->num_of_keys);

    }
    return hp->rpo;
}

off_t insert_into_internal_as(off_t bumo, int left_index, int64_t key, off_t newp) { //스플릿 후 키를 부모 노드에 올렸을 때 부모 노드에서 또 다시 split이 일어나야되는 상황
//부모노드, 부모노드에서 split전 노드를 가리키던 offset의 위치, 새로 삽입될 key, 새로 만들어진 Node
    int i, j, split;
    int64_t k_prime;
    off_t new_p, child;
    I_R * temp;

    temp = (I_R *)calloc(INTERNAL_MAX + 1, sizeof(I_R)); //일단 새로운 키가 올라가서 부모노드에 삽입되었을 때의 상황을 만들기 위한 공간

    page * old_parent = load_page(bumo); //bumo 노드를 로드

    for (i = 0, j = 0; i < old_parent->num_of_keys; i++, j++) { //
        if (j == left_index + 1) j++; //temp공간에 복사하는 건데 나뉘어지는 부분, 즉 부모의 split으로 올라가는 key -> 부모의 부모로 올라가는 key는 일단 건너뛴다.
        temp[j] = old_parent->b_f[i];
    }

    temp[left_index + 1].key = key; //자식노드에서 부모 노드로 올라간 key
    temp[left_index + 1].p_offset = newp;

    split = cut(INTERNAL_MAX); //반으로 나누는 기준 index를 얻는다.
    new_p = new_page(); //본격적으로 부모 노드 split
    page * new_parent = load_page(new_p);
    old_parent->num_of_keys = 0;
    for (i = 0; i < split; i++) { //split 기준보다 작다면 부모 노드에서 새로 올라간 key보다 작은 값들이 복사
        old_parent->b_f[i] = temp[i];
        old_parent->num_of_keys++;
    }
    k_prime = temp[i].key; // K_prime -> 부모의 부모로 올라가는 key -> 수업자료 lamport 삽입할 때의 gold
    new_parent->next_offset = temp[i].p_offset; //kim의 next offset은 gold의 offset이다.
    for (++i, j = 0; i < INTERNAL_MAX + 1; i++, j++) { //그리고 이제 gold 뒷부분에 있는 record를 새로 만들어진 부모 노드에 넣는다.
        new_parent->b_f[j] = temp[i];
        new_parent->num_of_keys++;
    }

    new_parent->parent_page_offset = old_parent->parent_page_offset; //새로 만들어진 부모노드의 부모 offset은 split전 부모 노드의 부모의 Offset -> 수업자료에서 Mozart
    page * nn;
    nn = load_page(new_parent->next_offset); //부모노드에서 next_offset -> 이 모든 것의 시작이 된 자식 노드 -> kim, lamport
    nn->parent_page_offset = new_p; //Kim, lamport의 부모는 부모노드에서 split되어 새로 만들어진 노드가 된다.
    pwrite(fd, nn, sizeof(page), new_parent->next_offset);
    free(nn);
    for (i = 0; i < new_parent->num_of_keys; i++) { //
        child = new_parent->b_f[i].p_offset; //새로 만들어진 부모노드와 새로 만들어진 자식 노드를 각각 index에 맞게 이어준다.
        page * ch = load_page(child);
        ch->parent_page_offset = new_p;
        pwrite(fd, ch, sizeof(page), child);
        free(ch);
    }

    pwrite(fd, old_parent, sizeof(page), bumo);
    pwrite(fd, new_parent, sizeof(page), new_p);
    free(old_parent);
    free(new_parent);
    free(temp);
    //printf("split internal is complete\n");
    return insert_into_parent(bumo, k_prime, new_p); //그리고 부모노드에서 스플릿되었으므로 부모노드의 부모노드로 스플릿된 Key값 삽입 -> recursive하게 만들어진다.
}

int db_delete(int64_t key) { //delete key

    if (rt->num_of_keys == 0) { //root가 empty일 때
        //printf("root is empty\n");
        return -1;
    }
    char * check = db_find(key);//삭제할 키가 있는지 확인, 없다면 끝
    if (check== NULL) {
        free(check);
        //printf("There are no key to delete\n");
        return -1;
    }
    free(check);
    off_t deloff = find_leaf(key); //삭제할 키가 있는 leaf 찾기
    delete_entry(key, deloff); //키와 offset 삭제하기
    return 0;

}//fin

void delete_entry(int64_t key, off_t deloff) { //키와 offset 삭제, delete시 merge되는 것을 유심히 보자

    remove_entry_from_page(key, deloff); //일단 page에서 삭제 -> 디비에서 삭제

    if (deloff == hp->rpo) { //삭제하려는 것이 루트 노드에 있을 때
        adjust_root(deloff);
        return;
    }
    
    
    page * not_enough = load_page(deloff); //삭제할 페이지 로드
    int check = not_enough->is_leaf ? cut(LEAF_MAX) : cut(INTERNAL_MAX); //삭제할 키가 있는 노드가 leaf인지 internal인지에 따라 길이가 바뀐다. 비플 트리의 기준에 맞게, 리프는 ceiling n-1/2, internal ceiling n/2
    if (not_enough->num_of_keys >= check){ //삭제한 노드의 키의 갯수가 check보다 크거나 같다면 그냥 삭제하고 냅두면 된다.
      free(not_enough);
      //printf("just delete\n");
      return;  
    }
    
    int neighbor_index, k_prime_index; //왼쪽 이웃 -> 최소 갯수 부족할 땐 무조건 왼쪽으로 모는 것
    off_t neighbor_offset, parent_offset;
    int64_t k_prime;
    parent_offset = not_enough->parent_page_offset; //현재 갯수가 부족한 노드의 부모 노드
    page * parent = load_page(parent_offset); //그 부모 노드의 페이지 로드

    if (parent->next_offset == deloff) { //부모의 다음 포인터가 키를 삭제한 노드라면 -> 삭제가 이루어진 노드가 가장 왼쪽 노드라면
        neighbor_index = -2;        // 왼쪽 형제 인덱스 = -2
        neighbor_offset = parent->b_f[0].p_offset; //이웃의 포인터 = 부모의 첫번째의 키의 오른쪽 포인터 -> 즉 원래는 삭제가 일어나서 조건이 미달되면 삭제가 이루어진 노드와 그 왼쪽 노드를 merge하지만 삭제가 이루어진 노드가 가장 왼쪽의 노드일 경우에만 그것의 오른쪽 노드와 병합해준다. 이를 나타낸 코드
        k_prime = parent->b_f[0].key; //부모의 키는 0번째 1
        k_prime_index = 0; //부모의 키 인덱스는 0번
    }
    else if(parent->b_f[0].p_offset == deloff) { //왼쪽 형제 노드가 맨 왼쪽 노드일 경우. 삭제가 이루어진 노드가 가장 왼쪽에서 두번째 노드일 경우
        neighbor_index = -1; //인덱스에 -1
        neighbor_offset = parent->next_offset; //부모의 왼쪽 offset 할당
        k_prime_index = 0;
        k_prime = parent->b_f[0].key;
    }
    else { //이외의 경우 -> 삭제가 일어나는 노드 >= 0 (가장 왼쪽을 -2, 두번째를 -1로 할당했으므로)
        int i;

        for (i = 0; i <= parent->num_of_keys; i++) //i가 부모의 키 갯수보다 작다면
            if (parent->b_f[i].p_offset == deloff) break; //부모 노드에서 삭제가 일어나는 노드를 가리키는 offset을 찾았다면 break;
        neighbor_index = i - 1; //neighbor는 그 왼쪽 형제 노드
        neighbor_offset = parent->b_f[i - 1].p_offset; //그 왼쪽 옆 offset
        k_prime_index = i; //인덱스와 키도 그에 맞게 할당
        k_prime = parent->b_f[i].key;
    }

    page * neighbor = load_page(neighbor_offset); //형제노드 로드
    int max = not_enough->is_leaf ? LEAF_MAX : INTERNAL_MAX - 1; //삭제가 이루어진 노드가 리프인지 internal인지에 따라 노드의 Max값 할당
	//******************************** 수정한 부분
	if(!(not_enough->is_leaf)){ //internal이라면
        if(not_enough->num_of_keys != 0) return; //0이상이라면 그냥 끝낸다
		 if(neighbor->num_of_keys <= 1){ //이웃노드가 1개보다 적게 갖고 있다면 merge
			return coalesce_pages(deloff, neighbor_index, neighbor_offset, parent_offset, k_prime);
     } else{ //1개보다 많다면 재분배
			return redistribute_pages(deloff, neighbor_index, neighbor_offset, parent_offset, k_prime, k_prime_index); }
	 }
	else{
    int why = neighbor->num_of_keys + not_enough->num_of_keys; //이웃의 키의 갯수와 삭제가 이루어진 노드의 키 갯수의 합 ->왜? 여기서 합쳤을 때 max를 넘을 수도 있음
    //printf("%d %d\n",why, max);
    if (why <= max) { //작거나 같다면 그냥 합치고 끝
        free(not_enough);
        free(parent);
        free(neighbor);
        coalesce_pages(deloff, neighbor_index, neighbor_offset, parent_offset, k_prime);
    }
    else { //크다면 재분배
        free(not_enough);
        free(parent);
        free(neighbor);
        redistribute_pages(deloff, neighbor_index, neighbor_offset, parent_offset, k_prime, k_prime_index);

		}
	}
    return;
}
void redistribute_pages(off_t need_more, int nbor_index, off_t nbor_off, off_t par_off, int64_t k_prime, int k_prime_index) {
    //갯수가 부족한 노드의 Offset, neighbor index, offset, parent offset, parent key, parent key의 index
    page *need, *nbor, *parent;
    int i;
    need = load_page(need_more);
    nbor = load_page(nbor_off);
    parent = load_page(par_off);
    if (nbor_index != -2) { //삭제가 이루어진 노드가 가장 왼쪽 노드가 아닐 경우 -> 왼쪽 형제와 한다
        
        if (!need->is_leaf) { //인터널에서 이루어진 경우
            //printf("redis average interal\n");
            for (i = need->num_of_keys; i > 0; i--) //한칸씩 뒤로 당긴다. 새로운 key가 들어올 자리 만들어준다.
                need->b_f[i] = need->b_f[i - 1];
            
            need->b_f[0].key = k_prime; //첫번째 키 부분에 부모 노드의 키를 복사한다.
            need->b_f[0].p_offset = need->next_offset; //원래 왼쪽 자식이 오른쪽 자식으로 바뀐다 -> 키로 들어온 것의 오른쪽 자식이 오른쪽 형제 노드로 들어가면서 왼쪽 자식으로 바뀐다.
            need->next_offset = nbor->b_f[nbor->num_of_keys - 1].p_offset; //왼쪽 자식은 가장 왼쪽 형제노드의 가장 오른쪽의 오른쪽 Offset이 된다.
            page * child = load_page(need->next_offset); // 자식 페이지는 새로 들어온 왼쪽 자식 -> 오른쪽 형제의 가장 오른쪽 자식
            child->parent_page_offset = need_more;      // 그 노드의 부모 노드는 이제 키가 필요했던 노드가 된다.
            pwrite(fd, child, sizeof(page), need->next_offset); //
            free(child);
            parent->b_f[k_prime_index].key = nbor->b_f[nbor->num_of_keys - 1].key; //부모에서 바뀐 인덱스의 키는 왼쪽 노드의 가장 오른쪽의 키가 된다.
            
        }
        else { //리프에서 일어난 경우
            //printf("redis average leaf\n");
            for (i = need->num_of_keys; i > 0; i--){ //삭제되고 부족하니까 빌려와야하고 그 공간을 만들어준다. 특이 케이스 제외 무조건 왼쪽에서 빌려오기 때문에 노드의 가장 왼쪽으로 들어갈 것이고 때문에 한칸씩 미뤄준다.
                need->records[i] = need->records[i - 1];
            }
            need->records[0] = nbor->records[nbor->num_of_keys - 1]; //필요한 노드의 첫번째는 왼쪽 노드의 가장 오른쪽
            nbor->records[nbor->num_of_keys - 1].key = 0; //그리고 그것은 왼쪽 노드에서 지워준다.
            parent->b_f[k_prime_index].key = need->records[0].key; //부모 노드에서도 바뀐 부분의 키를 옮겨진 키로 바꿔준다.
        }

    }
    else { //가장 왼쪽이었을 경우 -> 오른쪽과 해야지
        
        if (need->is_leaf) { //리프일 때
            //printf("redis leftmost leaf\n");
            need->records[need->num_of_keys] = nbor->records[0]; //필요한 노드의 가장 오른쪽에 그 이웃의 첫번째 레코드를 가져온다.
            for (i = 0; i < nbor->num_of_keys - 1; i++) // 맨 앞 하나가 없어졌으니까 한칸씩 앞으로 당긴다
                nbor->records[i] = nbor->records[i + 1];
            parent->b_f[k_prime_index].key = nbor->records[0].key; //가장 앞에 온 레코드를 부모한테 올려준다.
            
           
        }
        else {  //리프가 아닐 때
            //printf("redis leftmost internal\n");
            need->b_f[need->num_of_keys].key = k_prime;     // 필요한 노드의 가장 오른쪽 인덱스의 키에 k_prime
            need->b_f[need->num_of_keys].p_offset = nbor->next_offset; //키가 필요한 노드에 존재하는 마지막 키의 오른쪽 오프셋은 오른쪽 이웃의 왼쪽 offset이 가리키는 노드가 된다. ->자식도 가져와야 함
            page * child = load_page(need->b_f[need->num_of_keys].p_offset); //자식도 부모 바꿔줘야한다.
            child->parent_page_offset = need_more; //
            pwrite(fd, child, sizeof(page), need->b_f[need->num_of_keys].p_offset);
            free(child);
            
            parent->b_f[k_prime_index].key = nbor->b_f[0].key; //부모 노드도 업데이트 해주어야 함 -> 오른쪽 노드의 첫번째 키로
            nbor->next_offset = nbor->b_f[0].p_offset; // 왼쪽 오프셋에 오른쪽 오프셋을 넣어준다.
            for (i = 0; i < nbor->num_of_keys - 1 ; i++) //그리고 레코드값 하나씩 댕겨준다.
                nbor->b_f[i] = nbor->b_f[i + 1];
            
        }
    }
    nbor->num_of_keys--;
    need->num_of_keys++;
    pwrite(fd, parent, sizeof(page), par_off);
    pwrite(fd, nbor, sizeof(page), nbor_off);
    pwrite(fd, need, sizeof(page), need_more);
    free(parent); free(nbor); free(need);
    return;
}

void coalesce_pages(off_t will_be_coal, int nbor_index, off_t nbor_off, off_t par_off, int64_t k_prime) {
    
    page *wbc, *nbor, *parent;
    off_t newp, wbf;

    if (nbor_index == -2) {
        //printf("leftmost\n");
        wbc = load_page(nbor_off); nbor = load_page(will_be_coal); parent =  load_page(par_off);
        newp = will_be_coal; wbf = nbor_off; //leftmost면 coal의 기준이 되는 노드가 오른쪽 형제 노드가 된다.
    }
    else {
        wbc = load_page(will_be_coal); nbor = load_page(nbor_off); parent = load_page(par_off);
        newp = nbor_off; wbf = will_be_coal;
    }

    int point = nbor->num_of_keys; //합치는 노드의 키 갯수
    int le = wbc->num_of_keys; //합쳐지는 노드의 키 갯수
    int i, j;
    if (!wbc->is_leaf) { //internal node일 경우
        //printf("coal internal\n");
        nbor->b_f[point].key = k_prime; //합치는 노드의 마지막 키는 그에 대한 부모 노드의 키가 된다.
        nbor->b_f[point].p_offset = wbc->next_offset; //합치는 노드의 가장 오른쪽 offset은 합쳐지는 노드의 가장 왼쪽 offset이 된다.
        nbor->num_of_keys++;

        for (i = point + 1, j = 0; j < le; i++, j++) { //합치는 노드에 복사
            nbor->b_f[i] = wbc->b_f[j];
            nbor->num_of_keys++;
            wbc->num_of_keys--;
        }

        for (i = point; i < nbor->num_of_keys; i++) { //offset 가져오기
            page * child = load_page(nbor->b_f[i].p_offset);
            child->parent_page_offset = newp;
            pwrite(fd, child, sizeof(page), nbor->b_f[i].p_offset);
            free(child);
        }

    }
    else { // 리프라면
        //printf("coal leaf\n");
        int range = wbc->num_of_keys; //합쳐지는 노드의 키 갯수
        for (i = point, j = 0; j < range; i++, j++) {
            
            nbor->records[i] = wbc->records[j]; //레코드 복사
            nbor->num_of_keys++;
            wbc->num_of_keys--;
        }
        nbor->next_offset = wbc->next_offset;
    }
    pwrite(fd, nbor, sizeof(page), newp);
    
    delete_entry(k_prime, par_off);
    free(wbc);
    usetofree(wbf);
    free(nbor);
    free(parent);
    return;

}//fin

void adjust_root(off_t deloff) {

    if (rt->num_of_keys > 0)
        return;
    if (!rt->is_leaf) {
        off_t nr = rt->next_offset;
        page * nroot = load_page(nr);
        nroot->parent_page_offset = 0;
        usetofree(hp->rpo);
        hp->rpo = nr;
        pwrite(fd, hp, sizeof(H_P), 0);
        free(hp);
        hp = load_header(0);
        
        pwrite(fd, nroot, sizeof(page), nr);
        free(nroot);
        free(rt);
        rt = load_page(nr);

        return;
    }
    else {
        free(rt);
        rt = NULL;
        usetofree(hp->rpo);
        hp->rpo = 0;
        pwrite(fd, hp, sizeof(hp), 0);
        free(hp);
        hp = load_header(0);
        return;
    }
}//fin

void remove_entry_from_page(int64_t key, off_t deloff) {
    
    int i = 0;
    page * lp = load_page(deloff);
    if (lp->is_leaf) {
        //printf("remove leaf key %ld\n", key);
        while (lp->records[i].key != key)
            i++;

        for (++i; i < lp->num_of_keys; i++)
            lp->records[i - 1] = lp->records[i];
        lp->num_of_keys--;
        pwrite(fd, lp, sizeof(page), deloff);
        if (deloff == hp->rpo) {
            free(lp);
            free(rt);
            rt = load_page(deloff);
            return;
        }
        
        free(lp);
        return;
    }
    else {
        //printf("remove interanl key %ld\n", key);
        while (lp->b_f[i].key != key)
            i++;
        for (++i; i < lp->num_of_keys; i++)
            lp->b_f[i - 1] = lp->b_f[i];
        lp->num_of_keys--;
        pwrite(fd, lp, sizeof(page), deloff);
        if (deloff == hp->rpo) {
            free(lp);
            free(rt);
            rt = load_page(deloff);
            return;
        }
        
        free(lp);
        return;
    }
    
}//fin






