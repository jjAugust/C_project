#include <stdio.h>
#include <stdlib.h>
typedef int Type;

typedef struct AVLTreeNode{
    Type key;                    // 关键字(键值)
    int height;
    struct AVLTreeNode *left;    // 左孩子
    struct AVLTreeNode *right;    // 右孩子
}Node, *AVLTree;
#define HEIGHT(p)    ( (p==NULL) ? -1 : (((Node *)(p))->height) )
#define MAX(a, b)    ( (a) > (b) ? (a) : (b) )
#define TBL_SIZE(a) ( (sizeof(a)) / (sizeof(a[0])) )



void Create (AVLTree Bst)
{
  Bst->key=NULL;
}

int avltree_height(AVLTree tree)
{
    return HEIGHT(tree);
}

void preorder_avltree(AVLTree tree)
{
    if(tree != NULL)
    {
        printf("%d ", tree->key);
        preorder_avltree(tree->left);
        preorder_avltree(tree->right);
    }
}


void inorder_avltree(AVLTree tree)
{
    if(tree != NULL)
    {
        inorder_avltree(tree->left);
        printf("%d ", tree->key);
        inorder_avltree(tree->right);
    }
}


void postorder_avltree(AVLTree tree)
{
    if(tree != NULL)
    {
        postorder_avltree(tree->left);
        postorder_avltree(tree->right);
        printf("%d ", tree->key);
    }
}


Node* Search(AVLTree x, Type key)
{
    if (x==NULL || x->key==key)
        return x;

    if (key < x->key)
        return Search(x->left, key);
    else
        return Search(x->right, key);
}


Node* iterative_Search(AVLTree x, Type key)
{
    while ((x!=NULL) && (x->key!=key))
    {
        if (key < x->key)
            x = x->left;
        else
            x = x->right;
    }

    return x;
}


Node* avltree_minimum(AVLTree tree)
{
    if (tree == NULL)
        return NULL;

    while(tree->left != NULL)
        tree = tree->left;
    return tree;
}


Node* avltree_maximum(AVLTree tree)
{
    if (tree == NULL)
        return NULL;

    while(tree->right != NULL)
        tree = tree->right;
    return tree;
}


static Node* left_left_rotation(AVLTree k2)
{
    AVLTree k1;

    k1 = k2->left;
    k2->left = k1->right;
    k1->right = k2;

    k2->height = MAX( HEIGHT(k2->left), HEIGHT(k2->right)) + 1;
    k1->height = MAX( HEIGHT(k1->left), k2->height) + 1;

    return k1;
}

static Node* right_right_rotation(AVLTree k1)
{
    AVLTree k2;

    k2 = k1->right;
    k1->right = k2->left;
    k2->left = k1;

    k1->height = MAX( HEIGHT(k1->left), HEIGHT(k1->right)) + 1;
    k2->height = MAX( HEIGHT(k2->right), k1->height) + 1;

    return k2;
}

static Node* left_right_rotation(AVLTree k3)
{
    k3->left = right_right_rotation(k3->left);

    return left_left_rotation(k3);
}

static Node* right_left_rotation(AVLTree k1)
{
    k1->right = left_left_rotation(k1->right);

    return right_right_rotation(k1);
}

static Node* CreateNode(Type key, Node *left, Node* right)
{
    Node* p;

    if ((p = (Node *)malloc(sizeof(Node))) == NULL)
        return NULL;
    p->key = key;
    p->height = 0;
    p->left = left;
    p->right = right;

    return p;
}

Node* Insert(AVLTree tree, Type key)
{
    if (tree == NULL)
    {
        // 新建节点
        tree = CreateNode(key, NULL, NULL);
        if (tree==NULL)
        {
            printf("ERROR: create avltree node failed!\n");
            return NULL;
        }
    }
    else if (key < tree->key) // 应该将key插入到"tree的左子树"的情况
    {
        tree->left = Insert(tree->left, key);
        // 插入节点后，若AVL树失去平衡，则进行相应的调节。
        if (HEIGHT(tree->left) - HEIGHT(tree->right) == 2)
        {
            if (key < tree->left->key)
                tree = left_left_rotation(tree);
            else
                tree = left_right_rotation(tree);
        }
    }
    else if (key > tree->key) // 应该将key插入到"tree的右子树"的情况
    {
        tree->right = Insert(tree->right, key);
        // 插入节点后，若AVL树失去平衡，则进行相应的调节。
        if (HEIGHT(tree->right) - HEIGHT(tree->left) == 2)
        {
            if (key > tree->right->key)
                tree = right_right_rotation(tree);
            else
                tree = right_left_rotation(tree);
        }
    }
    else //key == tree->key
    {
        printf("添加失败：不允许添加相同的节点！\n");
    }

    tree->height = MAX( HEIGHT(tree->left), HEIGHT(tree->right)) + 1;

    return tree;
}

static Node* delete_node(AVLTree tree, Node *z)
{
    // 根为空 或者 没有要删除的节点，直接返回NULL。
    if (tree==NULL || z==NULL)
        return NULL;

    if (z->key < tree->key)        // 待删除的节点在"tree的左子树"中
    {
        tree->left = delete_node(tree->left, z);
        // 删除节点后，若AVL树失去平衡，则进行相应的调节。
        if (HEIGHT(tree->right) - HEIGHT(tree->left) == 2)
        {
            Node *r =  tree->right;
            if (HEIGHT(r->left) > HEIGHT(r->right))
                tree = right_left_rotation(tree);
            else
                tree = right_right_rotation(tree);
        }
    }
    else if (z->key > tree->key)// 待删除的节点在"tree的右子树"中
    {
        tree->right = delete_node(tree->right, z);
        if (HEIGHT(tree->left) - HEIGHT(tree->right) == 2)
        {
            Node *l =  tree->left;
            if (HEIGHT(l->right) > HEIGHT(l->left))
                tree = left_right_rotation(tree);
            else
                tree = left_left_rotation(tree);
        }
    }
    else   
    {
        if ((tree->left) && (tree->right))
        {
            if (HEIGHT(tree->left) > HEIGHT(tree->right))
            {
                Node *max = avltree_maximum(tree->left);
                tree->key = max->key;
                tree->left = delete_node(tree->left, max);
            }
            else
            {
                Node *min = avltree_maximum(tree->right);
                tree->key = min->key;
                tree->right = delete_node(tree->right, min);
            }
        }
        else
        {
            Node *tmp = tree;
            tree = tree->left ? tree->left : tree->right;
            free(tmp);
        }
    }

    return tree;
}

Node* Delete(AVLTree tree, Type key)
{
    Node *z;

    if ((z = Search(tree, key)) != NULL)
        tree = delete_node(tree, z);
    return tree;
}

void destroy_avltree(AVLTree tree)
{
    if (tree==NULL)
        return ;

    if (tree->left != NULL)
        destroy_avltree(tree->left);
    if (tree->right != NULL)
        destroy_avltree(tree->right);

    free(tree);
}

void Traverse(AVLTree tree, Type key, int direction)
{
    if(tree != NULL)
    {
        if(direction==0)    // tree是根节点
            printf("%2d is root\n", tree->key, key);
        else                // tree是分支节点
            printf("%2d is %2d's %6s child\n", tree->key, key, direction==1?"right" : "left");

        Traverse(tree->left, tree->key, -1);
        Traverse(tree->right,tree->key,  1);
    }
}

int main()
{
	static int arr[]= {3,2,1,4,5,6,7,16,15,14,13,12,11,10,8,9};
    int i,ilen;
    AVLTree root=NULL;

    printf("== height: %d\n", avltree_height(root));

    printf("== insert: ");
    ilen = TBL_SIZE(arr);
    for(i=0; i<ilen; i++)
    {
        printf("%d ", arr[i]);
        root = Insert(root, arr[i]);
    }

    printf("\n== pre order: ");
    preorder_avltree(root);

    printf("\n== middle order: ");
    inorder_avltree(root);

    printf("\n== back order: ");
    postorder_avltree(root);
    printf("\n");

    printf("== height: %d\n", avltree_height(root));
    printf("== min: %d\n", avltree_minimum(root)->key);
    printf("== max: %d\n", avltree_maximum(root)->key);
    printf("== tree_detail: \n");
    Traverse(root, root->key, 0);

    i = 8;
    printf("\n== delete: %d", i);
    root = Delete(root, i);

    printf("\n== height: %d", avltree_height(root));
    printf("\n== middle order: ");
    inorder_avltree(root);
    printf("\n== tree_detail: \n");
    Traverse(root, root->key, 0);

    destroy_avltree(root);
}
