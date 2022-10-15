#include <stdio.h>
#include <stdlib.h>

FILE *in;
FILE *out;

typedef struct node *tree_pointer;
typedef struct node {
    int element;
    tree_pointer left_child;
    tree_pointer right_child;
} Node;
tree_pointer root; 
tree_pointer pos;  //poisition

typedef struct operate {
    char ope;
    int element1;
    int element2;
} Operate;

typedef enum {
    false,
    true
} bool;

/* ------------⇣operation func⇣ ------------*/
void Insert(tree_pointer *root, int value) {
    tree_pointer current;
    tree_pointer backup;
    tree_pointer newNode;

    newNode = (tree_pointer)malloc(sizeof(struct node));
    newNode->element = value;
    newNode->left_child = NULL;
    newNode->right_child = NULL;

    if (*root == NULL) {
        *root = newNode;
        // printf("new:%d\n",(*root)->element); //?????????????????????????????????????
    }
    else {
        current = *root;
        while (current != NULL) {
            backup = current;
            if (current->element > value) {
                current = current->left_child;
            }
            else {  // (current->element < value)
                current = current->right_child;
            }
        }

        if (backup->element > value) {
            backup->left_child = newNode;
        }
        else {
            backup->right_child = newNode;
        }
    }
}

bool FindNode(tree_pointer ptr, int target) {  //perorder
    bool FindIt = false;
    while (ptr) {
        if (ptr->element == target) {
            // printf("\ni find %d\n", (*pos)->element);
            FindIt = true;
            break;
        }
        else if (ptr->element > target) {
            ptr = ptr->left_child;
        }
        else if (ptr->element < target) {
            ptr = ptr->right_child;
        }
    }
    return FindIt;
}

void Preorder(tree_pointer ptr) {  //perorder

    if (ptr) {
        printf("%d ", ptr->element);
        Preorder(ptr->left_child);
        Preorder(ptr->right_child);
    }
}

void delete (tree_pointer *root, int target) {
    tree_pointer parent = NULL;
    tree_pointer this = NULL;
    tree_pointer child = NULL;

    this = parent = *root;
    while (this != NULL) {
        if (this->element == target) {
            break;
        }
        else if (this->element > target) {  //left
            parent = this;
            this = this->left_child;
        }
        else if (this->element < target) {  //right
            parent = this;
            this = this->right_child;
        }
    }
    // printf("parent : %d\n", parent->element);
    // printf("this : %d\n", this->element);

    // if (this == (*root)) {
    //     printf("it is root\n");
    // }
    if (this->left_child == NULL && this->right_child == NULL) {  //leaf
        if (parent->left_child == this)
            parent->left_child = NULL;
        if (parent->right_child == this)
            parent->right_child = NULL;

        this = NULL;
        free(this);
    }
    else if (this->left_child != NULL) {  //have left subtree
        tree_pointer ptr = this;
        tree_pointer pre_ptr = this;
        ptr = ptr->left_child;
        while (ptr->right_child != NULL) {
            pre_ptr = ptr;
            ptr = ptr->right_child;
        }
        // printf("ptr: %d\n", ptr->element);
        // printf("pre ptr: %d\n", pre_ptr->element);

        this->element = ptr->element;  //copy
        if (pre_ptr != this) {
            pre_ptr->right_child = ptr->left_child;
        }
        else {
            pre_ptr->left_child = ptr->left_child;
        }
        free(ptr);
    }
    else if (this->right_child != NULL) {  //have right subtree
        tree_pointer ptr = this;
        tree_pointer pre_ptr = this;
        ptr = ptr->right_child;
        while (ptr->left_child != NULL) {
            pre_ptr = ptr;
            ptr = ptr->left_child;
        }
        // printf("ptr: %d\n", ptr->element);
        // printf("pre ptr: %d\n", pre_ptr->element);

        this->element = ptr->element;  //copy
        if (pre_ptr != this) {
            pre_ptr->left_child = ptr->right_child;
        }
        else {
            pre_ptr->right_child = ptr->right_child;
        }
        free(ptr);
    }
}

void Print_tree(tree_pointer root) {
    tree_pointer ptr;

    ptr = root->left_child;
    printf("列印左子樹:\n");
    while (ptr != NULL) {
        printf("[%2d]\n", ptr->element);
        ptr = ptr->left_child;
    }

    ptr = root->right_child;
    printf("列印右子樹:\n");
    while (ptr != NULL) {
        printf("[%2d]\n", ptr->element);
        ptr = ptr->right_child;
    }
}

int query(int target) {
    int cnt = 0;
    tree_pointer ptr = root;
    while (ptr) {
        cnt++;
        if (target == ptr->element) {
            break;
        }
        else if (target < ptr->element) {
            ptr = ptr->left_child;
        }
        else if (target > ptr->element) {
            ptr = ptr->right_child;
        }
    }
    return cnt;
}

int Path(tree_pointer *root, int from, int to) {
    int lpath = 0, rpath = 0;
    tree_pointer split = (*root);
    if (from > to) {
        int tmp = from;
        from = to;
        to = tmp;
    }
    while (!(from <= split->element && to >= split->element)) {
        if (from < split->element && to < split->element) {
            split = split->left_child;
        }
        else if (from > split->element && to > split->element) {
            split = split->right_child;
        }
       
    }

    //printf("split: %d\n", split->element);

    /*path of left tree*/
    tree_pointer ptr = split;
    while (ptr->element != from) {
        if (from < ptr->element) {
            ptr = ptr->left_child;
        }
        else if (from > ptr->element) {
            ptr = ptr->right_child;
        }

        if (ptr->element > 0) {
            lpath += ptr->element;
        }
    }

    /*path of right tree*/
    ptr = split;
    while (ptr->element != to) {
        if (to < ptr->element) {
            ptr = ptr->left_child;
        }
        else if (to > ptr->element) {
            ptr = ptr->right_child;
        }
        if (ptr->element > 0) {
            rpath += ptr->element;
        }
    }
    if (split->element > 0) {
        return rpath + lpath + split->element;
    }
    else {
        return rpath + lpath;
    }
}
/*------------ ⇡operation func⇡ ------------*/

int main() {
    in = fopen("input_1.txt", "r");
    out = fopen("my_output_1.txt", "w");

    if (in == NULL) {
        printf("can't open input_1.txt\n");
    }
    else {
        printf("input_1.txt open!!!\n\n");
        if (out == NULL) {
            printf("can't open my_output_1.txt\n\n");
        }
        else {
            printf("my_output_1.txt open!!!\n\n");

            /* Read txt */
            int dataCNT = -1, opeCNT = -1;
            char c;
            int cnt = 0;

            while (1) {
                root = NULL;  
                pos = NULL;

                int ele1, ele2;
                fscanf(in, "%d%d", &dataCNT, &opeCNT);
                printf("----%d %d----\n", dataCNT, opeCNT);
                if (dataCNT == 0 && opeCNT == 0) {
                    break;
                }
                fprintf(out, "# %d\n", ++cnt);
                printf("# %d\n", cnt);

                
                for (int i = 0; i < dataCNT; i++) {
                    fscanf(in, "%d", &ele1);
                    /*------------ ⇣Create Tree⇣ ------------*/

                    if (FindNode(root, ele1) == false) {
                        Insert(&root, ele1);
                    }
                    /*------------ ⇡Create Tree⇡ ------------*/
                    printf("%d ", ele1);
                }
                printf("\n");
                // Preorder(root);
                printf("\n");
                printf("\n............ %d ............\n", root->element); //?????????????????????????????????????

                for (int i = 0; i < opeCNT; i++) {
                    fscanf(in, "%c", &c);
                    while (c > 'Z' || c < 'A') {
                        fscanf(in, "%c", &c);
                    }

                    if (c == 'I' || c == 'D' || c == 'Q') {

                        fscanf(in, "%d", &ele1);
                        printf("%c %d\n", c, ele1);
                        if (c == 'I') {
                            if (FindNode(root, ele1) == false) {
                                Insert(&root, ele1);
                                //Preorder(root);
                                //printf("\n\n");
                            }
                            else {
                               // printf("EXIST\n");
                            }
                        }
                        else if (c == 'D') {
                            if (FindNode(root, ele1)) {
                                //printf("doing delete...\n");
                                delete (&root, ele1);
                                // Preorder(root);
                               // printf("\n\n");
                            }
                            else {
                                //printf("ignore D\n");
                            }
                        }
                        else if (c == 'Q') {
                            if (FindNode(root, ele1)) {
                                // printf("doing query...\n");
                                int depth = query(ele1);
                                printf("depth: %d\n\n", depth);
                                fprintf(out, "%d\n", depth);
                            }
                            else {
                                //printf("ignore Q\n\n");
                            }
                        }
                    }
                    else if (c == 'P') {
                 
                        fscanf(in, "%d %d", &ele1, &ele2);
                        printf("%c %d %d\n", c, ele1, ele2);

                        if (FindNode(root, ele1) && FindNode(root, ele2)) {  //element1

                            int pathlen = 0;

                            if (ele1 == ele2) {  //same
                                if (ele1 > 0) {
                                    pathlen = ele1;
                                }
                                else {
                                    pathlen = 0;
                                }
                            }
                            else {  //from ~ to
                               pathlen = Path(&root, ele1, ele2);
                            }
                            printf("（Path: %d）\n", pathlen);
                            fprintf(out, "%d\n", pathlen);
                        }
                        else {
                           // fprintf(out, "-\n");
                            printf("（ignore p）\n");
                        }
                    }

                    else {
                        printf("read error\n");
                    }
                }
            }
        }
    }
    fclose(in);
    fclose(out);
    return 0;
}