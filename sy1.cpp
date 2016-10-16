#include <stdio.h>
#include <stdlib.h>
typedef enum { false=0, true } bool ;
typedef int type;
typedef struct node
{
	type value;
	struct node *left;
	struct node *right;
}Search_tree;

void Create (Search_tree *Bst)
{
  Bst->value=NULL;
}


Search_tree *CreateNode(type value)
{
	Search_tree *p = (Search_tree*)malloc(sizeof(Search_tree));
	if(NULL == p)
		return NULL;
	p->value = value;
	p->left = p->right = NULL;
	return p;
}

void Destroy(Search_tree *t)
{
	if(t)
	{
		if(t->left)
			Destroy(t->left);
		if(t->right)
			Destroy(t->right);
		free(t);
		t = NULL;
	}
}

 Search_tree *Search(Search_tree *t, type value)
 {
 	if(t == NULL)
 		return NULL;
 	if(t->value == value)
 		return t;
 	else if(t->value > value)
 		return Search(t->left, value);
 	else
 		return Search(t->right, value);
 }

Search_tree *Search_min(Search_tree *t)
{
	if(NULL == t)
		return NULL;
	Search_tree *p = t;
	while(p->left)
		p = p->left;
	return p;
}

Search_tree *Search_max(Search_tree *t)
{
	if(NULL == t)
		return NULL;
	Search_tree *p = t;
	while(p->right)
		p = p->right;
	return p;
}

Search_tree *Insert(Search_tree *t, type value)
{
	if(t == NULL)// no root
		t = CreateNode(value);
	else if(t->value > value)
		t->left = Insert(t->left, value);
	else if(t->value < value)
		t->right = Insert(t->right, value);
	return t;
}

bool Delete(Search_tree *t, type value)
{
	if(t == NULL)
		return false;
	Search_tree *p=t,*prev;
	while(p && p->value!=value)
	{
		prev=p;
		if(p->value>value)
			p=p->left;
		else
			p=p->right;
	}
	if(p == NULL)
		return false;

	if(p->left && p->right)// left right  are all exist
	{
	    Search_tree *min=p->right;//右子树最小节点
	    prev=p;
	    while(min->left)//找到右子树最小节点
	    {
	    	prev=min;
	    	min=min->left;
	    }
        p->value=min->value;//把找到的右子树的最值与p进行值交换

        /*将min节点的子节点作为prev的子节点，并将min所指向的节点删除*/
        if(prev->right==min)
            prev->right=min->right;//
        else
        	prev->left=min->right;
    }
    else
    {
	    if(prev->left==p)//p为左
	    {
	    	prev->left=p->left ? p->left:p->right;
	    }
        else if(prev->right==p)//p为右
        {
        	prev->right=p->left ? p->left:p->right;
        }
        free(p);
        p=NULL;
    }
    return true;
}

void Traverse(Search_tree *t)//中序输出
{
	if(t == NULL)
		return ;
	Traverse(t->left);
	printf("%d ", t->value);
	Traverse(t->right);
}

int main()
{
	Search_tree *root = CreateNode(11);
	Insert(root, 8);
	Insert(root, 15);
	Insert(root, 4);
	Insert(root, 9);
	Insert(root, 6);
	Insert(root, 18);

	printf("min: %d",Search_min(root)->value);
	printf("\n");

	printf("max: %d",Search_max(root)->value);
	printf("\n");
	
	printf("search 6: %d",Search(root, 6)->value);
	printf("\n");
	
    if(Search(root, 100)==NULL){
        printf("search 100: null\n");
    }


	Traverse(root);
	Delete(root, 11);
	printf("\n");
	Traverse(root);
	Delete(root, 4);
	printf("\n");
	Traverse(root);
	Destroy(root);
	printf("\n");
	Traverse(root);
	return 0;
}
