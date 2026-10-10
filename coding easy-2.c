#include <stdio.h>
#include <stdlib.h>

/* ---------- 1. 定义节点类型 ---------- */
typedef struct Node {
    int          data;   /* 数据域 */
    struct Node *next;   /* 指针域：存下一个节点的地址 */
} Node;

/* ---------- 2. 定义一个节点：传入数据，返回一个新节点 ---------- */
Node *create_node(int data)
{
    Node *p = (Node *)malloc(sizeof(Node));

    if (p == NULL) {                 /* 申请失败就退出 */
        printf("内存分配失败\n");
        exit(1);
    }
    p->data = data;
    p->next = NULL;
    return p;
}

/* ---------- 3. 头插：把新节点放到最前面 ---------- */
Node *push_front(Node *head, int n)
{
    Node *a = create_node(n);

    a->next = head;      /* 新节点指向原来的头 */
    return a;            /* 返回新的头：调用方必须写 head = push_front(head, n); */
}

/* ---------- 4. 尾插：把新节点接到最后面 ---------- */
Node *push_back(Node *head, int n)
{
    Node *a = create_node(n);
    Node *p = head;

    if (head == NULL) {          /* 空链表：新节点直接当第一个 */
        return a;
    }
    while (p->next != NULL) {    /* 一直走到最后一个节点 */
        p = p->next;
    }
    p->next = a;
    return head;                 /* 头没变，原样返回 */
}

/* ---------- 5. 打印链表：只读，不会改变头指针 ---------- */
void print_list(Node *head)
{
    Node *p = head;

    while (p != NULL) {
        printf("%d -> ", p->data);
        p = p->next;
    }
    printf("NULL\n");
}

/* ---------- 6. 查找：返回它是第几个节点；没找到返回 0 ---------- */
int find(Node *head, int n)
{
    Node *p = head;
    int   i = 1;

    while (p != NULL) {
        if (p->data == n) {
            return i;            /* 找到了：返回序号 */
        }
        p = p->next;
        i++;
    }
    return 0;                    /* 走到 NULL 说明没有 */
}

/* ---------- 7. 删除第 n 个节点：成功返回 1，失败返回 0 ----------
 * 形参写成 Node **phead（二级指针）的原因：
 *   删掉第 1 个节点时，头指针本身要变。C 是值传递，
 *   只有传进"这个变量的地址"，函数里才能改到调用方的 head。
 * ------------------------------------------------------------ */
int delete_at(Node **phead, int n)
{
    Node *prev = NULL;       /* 前一个节点 */
    Node *cur  = *phead;     /* 当前节点（*phead 就是调用方的 head） */
    int   i    = 1;

    if (n < 1) {
        return 0;            /* n 非法 */
    }
    while (cur != NULL && i < n) {   /* 两个指针一起往前挪 */
        prev = cur;
        cur  = cur->next;
        i++;
    }
    if (cur == NULL) {       /* 还没数到第 n 个就走完了 → 节点数少于 n */
        return 0;
    }

    if (prev == NULL) {      /* prev 是空，说明删的是第 1 个节点 */
        *phead = cur->next;  /* 直接改头指针 */
    } else {
        prev->next = cur->next;   /* 让前一个节点跳过它 */
    }
    free(cur);
    return 1;
}

/* ---------- 8. 反转：返回新的头指针 ---------- */
Node *reverse(Node *head)
{
    Node *prev = NULL;
    Node *cur  = head;
    Node *next = NULL;

    while (cur != NULL) {
        next = cur->next;    /* 先记住下一个，不然改完就找不到了 */
        cur->next = prev;    /* 掉头：指向前一个 */
        prev = cur;          /* 两个指针各前进一格 */
        cur = next;
    }
    return prev;             /* 原来的最后一个节点成了新头 */
}

/* ---------- main---------- */
int main(void)
{
    Node *head;
    Node *p;

    head = create_node(0);          /* 初始状态：head -> 0 -> NULL */
    printf("初始状态: ");
    print_list(head);

    /* 头插 1、2、3 */
    head = push_front(head, 1);
    head = push_front(head, 2);
    head = push_front(head, 3);
    printf("头插后:   ");
    print_list(head);               /* 3 -> 2 -> 1 -> 0 */

    /* 尾插 4、5 */
    head = push_back(head, 4);
    head = push_back(head, 5);
    printf("尾插后:   ");
    print_list(head);               /* 3 -> 2 -> 1 -> 0 -> 4 -> 5 */

    /* 查找 */
    printf("找 0: 第 %d 个节点\n", find(head, 0));
    printf("找 9: 返回 %d（0 就是没找到）\n", find(head, 9));

    /* 修改：先找到节点，再改它的 data */
    p = head;
    p = p->next;                    /* 走一步，到第 2 个节点 */
    p->data = 99;
    printf("改第2个:  ");
    print_list(head);

    /* 删除第 1 个节点 */
    if (delete_at(&head, 1)) {
        printf("删除第 1 个成功: ");
    }
    print_list(head);

    /* 删除第 100 个节点（节点数不够） */
    if (delete_at(&head, 100) == 0) {
        printf("删除第 100 个失败：节点数不到 100\n");
    }

    /* 反转 */
    printf("反转前:   ");
    print_list(head);
    head = reverse(head);
    printf("反转后:   ");
    print_list(head);

    /* 释放内存 */
    while (head != NULL) {
        Node *t = head;
        head = head->next;
        free(t);
    }

    return 0;
}
