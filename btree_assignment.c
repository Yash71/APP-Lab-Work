#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

struct node {
    int *keys;
    int t;  
    struct node **children;
    int n;
};

struct node *newNode(int t, bool isLeaf) {
    struct node *BTnode = (struct node *)malloc(sizeof(struct node));
    BTnode->keys = (int *)malloc((2 * t - 1) * sizeof(int));
    BTnode->children = (struct node **)malloc(2 * t * sizeof(struct node *));
    for (int i = 0; i < 2 * t; i++) {
        BTnode->children[i] = NULL;
    }
    BTnode->t = t;
    BTnode->n = 0;
    return BTnode;
}

int isLeaf(struct node* nd) {
    for (int i = 0; i <= nd->n; i++) {
        if (nd->children[i] != NULL) return 0;
    }
    return 1;
}

void splitChild(struct node *parent, int i, struct node *child) {
    int t = child->t;
    struct node *newBTnode = newNode(t, isLeaf(child));
    newBTnode->n = t - 1;

    for (int j = 0; j < t - 1; j++)
        newBTnode->keys[j] = child->keys[j + t];

    if (!isLeaf(child)) {
        for (int j = 0; j < t; j++)
            newBTnode->children[j] = child->children[j + t];
    }

    child->n = t - 1;

    for (int j = parent->n; j >= i + 1; j--)
        parent->children[j + 1] = parent->children[j];

    parent->children[i + 1] = newBTnode;

    for (int j = parent->n - 1; j >= i; j--)
        parent->keys[j + 1] = parent->keys[j];

    parent->keys[i] = child->keys[t - 1];
    parent->n++;
}

void insertNonFull(struct node *BTnode, int k) {
    int i = BTnode->n - 1;

    if (isLeaf(BTnode)) {
        while (i >= 0 && k < BTnode->keys[i]) {
            BTnode->keys[i + 1] = BTnode->keys[i];
            i--;
        }
        BTnode->keys[i + 1] = k;
        BTnode->n++;
    } else {
        while (i >= 0 && k < BTnode->keys[i])
            i--;

        if (BTnode->children[i + 1]->n == 2 * BTnode->t - 1) {
            splitChild(BTnode, i + 1, BTnode->children[i + 1]);

            if (k > BTnode->keys[i + 1])
                i++;
        }
        insertNonFull(BTnode->children[i + 1], k);
    }
}

struct node *insert(struct node *root, int t, int k) {
    if (root == NULL) {
        root = newNode(t, true);
        root->keys[0] = k;
        root->n = 1;
    } else {
        if (root->n == 2 * t - 1) {
            struct node *newRoot = newNode(t, false);
            newRoot->children[0] = root;
            splitChild(newRoot, 0, root);

            int i = 0;
            if (newRoot->keys[0] < k)
                i++;
            insertNonFull(newRoot->children[i], k);

            root = newRoot;
        } else {
            insertNonFull(root, k);
        }
    }
    return root;
}

int findKey(struct node* root, int k) {
    int index = 0;
    while (index < root->n && root->keys[index] < k) {
        index++;
    }
    return index;
}

void merge(struct node *root, int idx) {
    struct node *child = root->children[idx];
    struct node *sibling = root->children[idx + 1];

    child->keys[child->t - 1] = root->keys[idx];

    for (int i = 0; i < sibling->n; i++)
        child->keys[child->t + i] = sibling->keys[i];

    if (!isLeaf(sibling)) {
        for (int i = 0; i <= sibling->n; i++)
            child->children[child->t + i] = sibling->children[i];
    }

    for (int i = idx + 1; i < root->n; i++)
        root->keys[i - 1] = root->keys[i];

    for (int i = idx + 2; i <= root->n; i++)
        root->children[i - 1] = root->children[i];

    child->n += sibling->n + 1;
    root->n--;

    free(sibling);
}

void deleteFromNode(struct node *root, int k) {
    int idx = findKey(root, k);

    if (idx < root->n && root->keys[idx] == k) {
        if (isLeaf(root)) {
            for (int i = idx + 1; i < root->n; i++)
                root->keys[i - 1] = root->keys[i];
            root->n--;
        } else {
            if (root->children[idx]->n >= root->t) {
                struct node *cur = root->children[idx];
                while (!isLeaf(cur))
                    cur = cur->children[cur->n];
                int pred = cur->keys[cur->n - 1];
                root->keys[idx] = pred;
                deleteFromNode(root->children[idx], pred);
            } else if (root->children[idx + 1]->n >= root->t) {
                struct node *cur = root->children[idx + 1];
                while (!isLeaf(cur))
                    cur = cur->children[0];
                int succ = cur->keys[0];
                root->keys[idx] = succ;
                deleteFromNode(root->children[idx + 1], succ);
            } else {
                merge(root, idx);
                deleteFromNode(root->children[idx], k);
            }
        }
    } else {
        if (isLeaf(root)) {
            printf("The key %d is not in the tree\n", k);
            return;
        }

        bool flag = (idx == root->n);

        if (root->children[idx]->n < root->t) {
            if (idx != 0 && root->children[idx - 1]->n >= root->t) {
                struct node *child = root->children[idx];
                struct node *sibling = root->children[idx - 1];

                for (int i = child->n - 1; i >= 0; --i)
                    child->keys[i + 1] = child->keys[i];

                if (!isLeaf(child)) {
                    for (int i = child->n; i >= 0; --i)
                        child->children[i + 1] = child->children[i];
                }

                child->keys[0] = root->keys[idx - 1];
                if (!isLeaf(sibling))
                    child->children[0] = sibling->children[sibling->n];

                root->keys[idx - 1] = sibling->keys[sibling->n - 1];

                child->n++;
                sibling->n--;
            } else if (idx != root->n && root->children[idx + 1]->n >= root->t) {
                struct node *child = root->children[idx];
                struct node *sibling = root->children[idx + 1];

                child->keys[child->n] = root->keys[idx];

                if (!isLeaf(sibling))
                    child->children[child->n + 1] = sibling->children[0];

                root->keys[idx] = sibling->keys[0];

                for (int i = 1; i < sibling->n; ++i)
                    sibling->keys[i - 1] = sibling->keys[i];

                if (!isLeaf(sibling)) {
                    for (int i = 1; i <= sibling->n; ++i)
                        sibling->children[i - 1] = sibling->children[i];
                }

                child->n++;
                sibling->n--;
            } else {
                if (idx != root->n) {
                    merge(root, idx);
                } else {
                    merge(root, idx - 1);
                    idx--;
                }
            }
        }

        if (flag && idx > root->n)
            deleteFromNode(root->children[idx - 1], k);
        else
            deleteFromNode(root->children[idx], k);
    }
}

struct node* deleteKey(struct node* root, int k) {
    if (!root) {
        return NULL;
    }
    deleteFromNode(root, k);
    if (root->n == 0) {
        struct node* tempNode = root;
        if (isLeaf(root)) root = NULL;
        else root = root->children[0];

        free(tempNode);
    }
    return root;
}

// void levelOrderTraversal(struct node *root) {
//     if (root == NULL)
//         return;

//     struct node* queue[100];
//     int front = 0, rear = 0;

//     queue[rear++] = root;

//     while (front < rear) {
//         struct node *current = queue[front++];

//         for (int i = 0; i < current->n; i++) {
//             printf("%d ", current->keys[i]);
//         }

//         if (!isLeaf(current)) {
//             for (int i = 0; i <= current->n; i++) {
//                 queue[rear++] = current->children[i];
//             }
//         }
//     }
// }
void levelOrderTraversal(struct node *root) {
    if (root == NULL)
        return;

    struct node **queue = (struct node **)malloc(1000 * sizeof(struct node *));
    int front = 0, rear = 0;

    queue[rear++] = root;

    while (front < rear) {
        struct node *current = queue[front++];
        for (int i = 0; i < current->n; i++) {
            printf("%d ", current->keys[i]);
        }

        if (!isLeaf(current)) {
            for (int i = 0; i <= current->n; i++) {
                queue[rear++] = current->children[i];
            }
        }
    }

    free(queue);
}


int main() {
    int t, x;
    scanf("%d", &t);

    struct node *root = NULL;

    while (1) {
        scanf("%d", &x);
        if (x == 0)
            break;
        if (x > 0) {
            root = insert(root, t, x);
        }
        else{
            root = deleteKey(root,abs(-x));
        }
        
    }
    
    levelOrderTraversal(root);

    return 0;
}
