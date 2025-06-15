/******************
Name: Dorian Salomon
ID: 216936880
Assignment: ex6
*******************/
#include "ex6.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

# define INT_BUFFER 128

int main()
{
    mainMenu();
    freeAllOwners();
    return 0;
}

void trimWhitespace(char *str)
{
    // Remove leading spaces/tabs/\r
    int start = 0;
    while (str[start] == ' ' || str[start] == '\t' || str[start] == '\r')
        start++;

    if (start > 0)
    {
        int idx = 0;
        while (str[start])
            str[idx++] = str[start++];
        str[idx] = '\0';
    }

    // Remove trailing spaces/tabs/\r
    int len = (int)strlen(str);
    while (len > 0 && (str[len - 1] == ' ' || str[len - 1] == '\t' || str[len - 1] == '\r'))
    {
        str[--len] = '\0';
    }
}

char *mymyStrdup(const char *src)
{
    if (!src)
        return NULL;
    size_t len = strlen(src);
    char *dest = (char *)malloc(len + 1);
    if (!dest)
    {
        printf("Memory allocation failed in mymyStrdup.\n");
        return NULL;
    }
    strcpy(dest, src);
    return dest;
}

int readIntSafe(const char *prompt)
{
    char buffer[INT_BUFFER];
    int value;
    int success = 0;

    while (!success)
    {
        printf("%s", prompt);

        // If we fail to read, treat it as invalid
        if (!fgets(buffer, sizeof(buffer), stdin))
        {
            printf(" Invalid input.\n");
            clearerr(stdin);
            continue;
        }

        // 1) Strip any trailing \r or \n
        //    so "123\r\n" becomes "123"
        size_t len = strlen(buffer);
        if (len > 0 && (buffer[len - 1] == '\n' || buffer[len - 1] == '\r'))
            buffer[--len] = '\0';
        if (len > 0 && (buffer[len - 1] == '\r' || buffer[len - 1] == '\n'))
            buffer[--len] = '\0';

        // 2) Check if empty after stripping
        if (len == 0)
        {
            printf(" Invalid input.\n");
            continue;
        }

        // 3) Attempt to parse integer with strtol
        char *endptr;
        value = (int)strtol(buffer, &endptr, 10);

        // If endptr didn't point to the end => leftover chars => invalid
        // or if buffer was something non-numeric
        if (*endptr != '\0')
        {
            printf(" Invalid input.\n");
        }
        else
        {
            // We got a valid integer
            success = 1;
        }
    }
    return value;
}

const char *getTypeName(const PokemonType type)
{
    switch (type)
    {
    case GRASS:
        return "GRASS";
    case FIRE:
        return "FIRE";
    case WATER:
        return "WATER";
    case BUG:
        return "BUG";
    case NORMAL:
        return "NORMAL";
    case POISON:
        return "POISON";
    case ELECTRIC:
        return "ELECTRIC";
    case GROUND:
        return "GROUND";
    case FAIRY:
        return "FAIRY";
    case FIGHTING:
        return "FIGHTING";
    case PSYCHIC:
        return "PSYCHIC";
    case ROCK:
        return "ROCK";
    case GHOST:
        return "GHOST";
    case DRAGON:
        return "DRAGON";
    case ICE:
        return "ICE";
    default:
        return "UNKNOWN";
    }
}

char *getDynamicInput()
{
    char *input = NULL;
    size_t size = 0, capacity = 1;
    input = (char *)malloc(capacity);
    if (!input)
    {
        printf("Memory allocation failed.\n");
        return NULL;
    }

    int c;
    while ((c = getchar()) != '\n' && c != EOF)
    {
        if (size + 1 >= capacity)
        {
            capacity *= 2;
            char *temp = (char *)realloc(input, capacity);
            if (!temp)
            {
                printf("Memory reallocation failed.\n");
                free(input);
                return NULL;
            }
            input = temp;
        }
        input[size++] = (char)c;
    }
    input[size] = '\0';

    // Trim any leading/trailing whitespace or carriage returns
    trimWhitespace(input);

    return input;
}

void printPokemonNode(PokemonNode *node)
{
    if (!node)
        return;
    printf("ID: %d, Name: %s, Type: %s, HP: %d, Attack: %d, Can Evolve: %s\n",
           node->data->id,
           node->data->name,
           getTypeName(node->data->TYPE),
           node->data->hp,
           node->data->attack,
           (node->data->CAN_EVOLVE == CAN_EVOLVE) ? "Yes" : "No");
}

void displayMenu(const OwnerNode *owner)
{
    if (!owner->pokedexRoot)
    {
        printf("Pokedex is empty.\n");
        return;
    }

    printf("Display:\n");
    printf("1. BFS (Level-Order)\n");
    printf("2. Pre-Order\n");
    printf("3. In-Order\n");
    printf("4. Post-Order\n");
    printf("5. Alphabetical (by name)\n");

    const int choice = readIntSafe("Your choice: ");

    switch (choice)
    {
    case BFS:
        displayBFS(owner->pokedexRoot);
        break;
    case PRE_ORDER:
        preOrderTraversal(owner->pokedexRoot);
        break;
    case IN_ORDER:
        inOrderTraversal(owner->pokedexRoot);
        break;
    case POST_ORDER:
        postOrderTraversal(owner->pokedexRoot);
        break;
    case ALPHABET:
        displayAlphabetical(owner->pokedexRoot);
        break;
    default:
        printf("Invalid choice.\n");
    }
}

void linkOwnerInCircularList(OwnerNode *newOwner) {
    if (!newOwner) return;

    if (ownerHead == NULL) {
        newOwner->next = newOwner;
        newOwner->prev = newOwner;
        ownerHead = newOwner;
    } else {
        OwnerNode *tail = ownerHead->prev;
        tail->next       = newOwner;
        newOwner->prev   = tail;
        newOwner->next   = ownerHead;
        ownerHead->prev  = newOwner;
    }
}


void removeOwnerFromCircularList(OwnerNode *target) {
    if (!ownerHead || !target) return;
    if (target == ownerHead && ownerHead->next == ownerHead) {
        ownerHead = NULL;
        return;
    }
    OwnerNode *cur = ownerHead;
    do {
        if (cur->next == target) {
            cur->next = target->next;
            target->next->prev = cur;
            if (target == ownerHead) ownerHead = target->next;
            return;
        }
        cur = cur->next;
    } while (cur != ownerHead);
}


void printOwnersCircular() {
    const OwnerNode * cur = ownerHead;
    if (!cur) {
        printf(" No owners.\n");
        return;
    }

    char direction;
    printf(" Enter direction (F or B): ");
    while(TRUE) {
        scanf("%c", &direction);
        if (direction=='F' || direction=='f' ||
            direction=='B' || direction=='b') {
            break;
            }
        printf("Invalid direction, must be F or B.\n");
    }

    const int move = (direction=='F' || direction=='f') ? FORWARD : BACKWARD;
    getchar();
    const int steps = readIntSafe("How many prints? ");
    for(int i = 0 ;i < steps;i++) {
        printf("[%d] %s\n",i+1,cur->ownerName);
        cur = (move == FORWARD) ? cur->next : cur->prev;
    }
}

void printOwnersList() {
    const OwnerNode * cur = ownerHead;
    int counter = 1;
    if(cur == NULL) {
        printf("No existing Pokedexes.");
        return;
    }

    while (cur->next != ownerHead) {
        printf("%d. %s\n",counter,cur->ownerName);
        cur = cur->next;
        counter++;
    }
    printf("%d. %s\n",counter,cur->ownerName);
}

void freeEntireQueue(QueuePokemon *head) {
    while (head != NULL) {
        QueuePokemon *next = head->next;
        free(head);
        head = next;
    }
}

int add(QueuePokemon** q,PokemonNode* pokemonNode) {
    if (q == NULL || pokemonNode == NULL) {
        return TRUE;
    }

    QueuePokemon *newNodeToQ = malloc(sizeof(QueuePokemon));
    if (newNodeToQ == NULL) {
        return FAIL;
    }
    newNodeToQ->data = pokemonNode;
    newNodeToQ->next = NULL;

    if (*q == NULL) {
        *q = newNodeToQ;
        return TRUE;
    }

    QueuePokemon *current = *q;
    while (current->next != NULL) {
        current = current->next;
    }

    current->next = newNodeToQ;
    return TRUE;
}

typedef void (*VisitNodeFunc)(PokemonNode *);


void BFSGeneric(PokemonNode *root, const VisitNodeFunc visit) {
    if (!root)
        return;

    QueuePokemon *queue = NULL;
    if (add(&queue, root) == FAIL) {
        exit(1);
    }

    while (queue != NULL) {
        QueuePokemon *frontNode = queue;
        PokemonNode  *current = frontNode->data;
        queue = queue->next;
        free(frontNode);

        visit(current);

        if (current->left != NULL) {
            if (add(&queue, current->left) == FAIL) {
                while (queue != NULL) {
                    QueuePokemon *tmp = queue;
                    queue = queue->next;
                    free(tmp);
                }
                exit(1);
            }
        }

        if (current->right != NULL) {
            if (add(&queue, current->right) == FAIL) {
                while (queue != NULL) {
                    QueuePokemon *tmp = queue;
                    queue = queue->next;
                    free(tmp);
                }
                exit(1);
            }
        }
    }
}

void preOrderGeneric(PokemonNode *root, VisitNodeFunc visit) {
    if (root == NULL || visit == NULL) {
        return;
    }
    visit(root);
    preOrderGeneric(root->left, visit);
    preOrderGeneric(root->right, visit);
}

void inOrderGeneric(PokemonNode *root, VisitNodeFunc visit) {
    if (root == NULL || visit == NULL) {
        return;
    }
    inOrderGeneric(root->left, visit);
    visit(root);
    inOrderGeneric(root->right, visit);
}

void postOrderGeneric(PokemonNode *root, VisitNodeFunc visit) {
    if (root == NULL || visit == NULL) {
        return;
    }
    postOrderGeneric(root->left, visit);
    postOrderGeneric(root->right, visit);
    visit(root);
}


void displayBFS(PokemonNode *root) {
    BFSGeneric(root,printPokemonNode);
}

void inOrderTraversal(PokemonNode *root) {
    inOrderGeneric(root,printPokemonNode);
}

void preOrderTraversal(PokemonNode *root) {
    preOrderGeneric(root,printPokemonNode);
}

void postOrderTraversal(PokemonNode *root) {
    postOrderGeneric(root,printPokemonNode);
}

int compareByNameNode(const void *a, const void *b) {
    // Cast a and b to pointer to PokemonNode*
    const PokemonNode * const *pnodeA = a;
    const PokemonNode * const *pnodeB = b;

    // Dereference to get the actual PokemonNode*
    const PokemonNode *nodeA = *pnodeA;
    const PokemonNode *nodeB = *pnodeB;


    if (nodeA == NULL && nodeB == NULL) {
        return 0;
    }
    if (nodeA == NULL) {
        return SMALL;
    }
    if (nodeB == NULL) {
        return BIGGER;
    }

    return strcmp(nodeA->data->name, nodeB->data->name);
}

void initNodeArray(NodeArray *na, const int cap) {
    if (na == NULL || cap <= 0) {
        return;
    }
    na->nodes = malloc(sizeof(PokemonNode *) * cap);
    if (na->nodes == NULL) {
        freeAllOwners();
        exit(1);
    }
    na->size = 0;
    na->capacity = cap;
}

void freeArray(NodeArray *na) {
    if(!na) { return ;}
    if (na->nodes) free(na->nodes);
    na->nodes = NULL;
    na->size = 0;
    na->capacity = 0;
}

void addNode(NodeArray *na, PokemonNode *node) {
    if (na == NULL || node == NULL) {
        return;
    }
    // If array is full, double its capacity
    if (na->size >= na->capacity) {
        const int newCap = na->capacity * DOUBLE;
        PokemonNode **temp = realloc(na->nodes, sizeof(PokemonNode *) * newCap);
        if (temp == NULL) {
            freeArray(na);
            freeAllOwners();
            exit(1);
        }
        na->nodes = temp;
        na->capacity = newCap;
    }
    na->nodes[na->size++] = node;
}

void collectAll(PokemonNode *root, NodeArray *na) {
    if (root == NULL || na == NULL) {
        return;
    }
    addNode(na, root);
    collectAll(root->left, na);
    collectAll(root->right, na);
}

void displayAlphabetical(PokemonNode *root) {
    if (!root)
        return;
    NodeArray* pokdex = malloc(sizeof(NodeArray));
    initNodeArray(pokdex, START_SIZE);
    collectAll(root, pokdex);
    bubbleSortByNames(pokdex);
    printArray(pokdex);
    freeArray(pokdex);
    free(pokdex);
}
void printArray(const NodeArray* pokdex) {
    if(!pokdex) return;
    for(int i = 0;i<pokdex->size;i++) {
        printPokemonNode(pokdex->nodes[i]);
    }
}

void bubbleSortByNames(const NodeArray* pokdex) {
    if (pokdex == NULL || pokdex->size < 2) {
        return;
    }

    const int n = pokdex->size;
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - 1 - i; j++) {
            if (compareByNameNode(
                &pokdex->nodes[j],
                    &pokdex->nodes[j + 1]
                ) > 0)
            {
                PokemonNode* temp       = pokdex->nodes[j];
                pokdex->nodes[j] = pokdex->nodes[j + 1];
                pokdex->nodes[j + 1] = temp;
            }
        }
    }
}


PokemonNode *searchPokemonBFS(PokemonNode *root, const int id) {
    if (!root)
        return NULL;

    QueuePokemon *queue = NULL;
    if (add(&queue, root) == FAIL) {
        exit(1);
    }

    while (queue != NULL) {
        QueuePokemon *frontNode = queue;
        PokemonNode  *current   = frontNode->data;
        queue = queue->next;
        free(frontNode);

        if (current->data->id == id) {
            freeEntireQueue(queue);
            return current;
        }

        if (current->left != NULL) {
            if (add(&queue, current->left) == FAIL) {
                freeEntireQueue(queue);
                exit(1);
            }
        }
        if (current->right != NULL) {
            if (add(&queue, current->right) == FAIL) {
                freeEntireQueue(queue);
                exit(1);
            }
        }
    }
    return NULL;
}

void freePokemon(OwnerNode *owner) {
    if (!owner)
        return;

    if (owner->pokedexRoot == NULL) {
        printf("No Pokemon to release.\n");
        return;
    }

    const int id = readIntSafe("Enter Pokemon ID to release: ");

    const PokemonNode *found = searchPokemonBFS(owner->pokedexRoot, id);
    if (!found) {
        printf("No Pokemon with ID %d found.\n", id);
        return;
    }
    printf("Removing Pokemon %s (ID %d).\n", found->data->name, id);
    owner->pokedexRoot = removePokemonByID(owner->pokedexRoot, id);
}

OwnerNode *findOwnerByName(const char *name) {
    if(!ownerHead) return NULL;
    OwnerNode * current = ownerHead;
    while (current->next != ownerHead) {
        if(strcmp(current->ownerName,name) == 0) {
            return current;
        }
        current = current->next;
    }
    if(strcmp(current->ownerName,name) == 0) {
        return current;
    }
    return NULL;
}

void mergePokedexMenu() {
    if(ownerHead == NULL || ownerHead->next == NULL || ownerHead->next == ownerHead) {
        printf(" Not enough owners to merge.\n");
        return;
    }

    printf("\n=== Merge Pokedexes ===\n");
    printf("Enter name of first owner:");
    char* firstOwnerName = getDynamicInput();
    printf(" Enter name of second owner:");
    char* secondOwnerName = getDynamicInput();

    if(firstOwnerName == NULL||secondOwnerName == NULL) {
        free(firstOwnerName);
        free(secondOwnerName);
        freeAllOwners();
        exit(1);
    }

    OwnerNode* first = findOwnerByName(firstOwnerName);
    OwnerNode* second = findOwnerByName(secondOwnerName);

    if (first == NULL || second == NULL) {
        printf(" One or both owners not found.\n");
        free(firstOwnerName);
        free(secondOwnerName);
        return;
    }

    printf(" Merging %s and %s...\n",firstOwnerName,secondOwnerName);
    if(mergeTwoPokadex(first, second->pokedexRoot) == -1) {
        free(firstOwnerName);
        free(secondOwnerName);
        freeAllOwners();
        exit(1);
    }
    printf("Merge completed.\n");
    removeOwnerFromCircularList(second);


    printf("Owner '%s' has been removed after merging.\n",secondOwnerName);
    freeOwnerNode(second);
    free(second);
    second = NULL;
}

int mergeTwoPokadex(OwnerNode* first, PokemonNode *pokdexSecond) {
    if (!pokdexSecond) return TRUE;

    if (mergeTwoPokadex(first, pokdexSecond->left) == FAIL)
        return FAIL;

    first->pokedexRoot = insertPokemonNode(first->pokedexRoot, pokdexSecond);

    if (mergeTwoPokadex(first, pokdexSecond->right) == FAIL)
        return FAIL;
    return TRUE;
}

PokemonNode *insertPokemonNode(PokemonNode *root, PokemonNode *newNode){
    if (root == NULL && newNode == NULL) {
        return NULL;
    }

    if (root == NULL) {
        PokemonNode *node = malloc(sizeof(PokemonNode));
        if (!node) return NULL;
        node->left = NULL;
        node->right = NULL;

        PokemonData *copyData = malloc(sizeof(PokemonData));
        if (!copyData) {
            free(node);
            return NULL;
        }
        copyData->name = NULL;
        if (copyPokemonData(copyData, newNode->data) == FAIL) {
            free(copyData);
            free(node);
            return NULL;
        }
        node->data = copyData;
        return node;
    }

    if (!newNode || !newNode->data) {
        return root;
    }

    const int cmp = (newNode->data->id - root->data->id);
    if (cmp < 0) {
        // Insert into left subtree
        root->left = insertPokemonNode(root->left, newNode);
    }
    else if (cmp > 0) {
        // Insert into right subtree
        root->right = insertPokemonNode(root->right, newNode);
    }
    return root;
}

void swapOwnerData(OwnerNode *a, OwnerNode *b) {
    char *tmpName    = a->ownerName;
    a->ownerName     = b->ownerName;
    b->ownerName     = tmpName;

    PokemonNode *tmpRoot = a->pokedexRoot;
    a->pokedexRoot       = b->pokedexRoot;
    b->pokedexRoot       = tmpRoot;
}

void sortOwners() {
    if (ownerHead == NULL || ownerHead->next == ownerHead) {
        printf(" 0 or 1 owners only => no need to sort.\n");
        return;
    }

    OwnerNode *head = ownerHead;
    OwnerNode *tail = ownerHead->prev;

    tail->next = NULL;
    head->prev = NULL;

    int swapped;
    do {
        swapped = 0;
        OwnerNode *ptr = head;

        while (ptr->next != NULL) {
            if (strcmp(ptr->ownerName, ptr->next->ownerName) > 0) {
                swapOwnerData(ptr, ptr->next);
                swapped = 1;
            }
            ptr = ptr->next;
        }
    } while (swapped);


    OwnerNode *newTail = head;
    while (newTail->next) {
        newTail = newTail->next;
    }

    newTail->next = head;
    head->prev    = newTail;

    ownerHead = head;
    printf(" Owners sorted by name.\n");
}

PokemonNode *removePokemonByID(PokemonNode *root, int id) {
    const PokemonNode *found = searchPokemonBFS(root, id);
    if (found == NULL) {
        return root;
    }
    return removeNodeBST(root, id);
}
PokemonNode *removeNodeBST(PokemonNode *root, int id) {
    if (root == NULL) {
        return NULL;
    }

    if (id < root->data->id) {
        root->left = removeNodeBST(root->left, id);
    }
    else if (id > root->data->id) {
        root->right = removeNodeBST(root->right, id);
    }
    else {
        // Case 1: No children
        if (root->left == NULL && root->right == NULL) {
            freePokemonNode(root);
            return NULL;
        }

        // Case 2: Only right child
        if (root->left == NULL) {
            PokemonNode *temp = root->right;
            freePokemonNode(root);
            return temp;
        }

        // Case 3: Only left child
        if (root->right == NULL) {
            PokemonNode *temp = root->left;
            freePokemonNode(root);
            return temp;
        }

        // Case 4: Two children
        const PokemonNode *minNode = findMinNode(root->right);
        if (copyPokemonData(root->data, minNode->data) == FAIL) {
            freeAllOwners();
            exit(1);
        }
        root->right = removeNodeBST(root->right, minNode->data->id);
    }

    return root;
}

PokemonNode *findMinNode(PokemonNode *root) {
    while (root->left != NULL) {
        root = root->left;
    }
    return root;
}

int copyPokemonData(PokemonData *dst, const PokemonData *src) {
    dst->id  = src->id;
    dst->TYPE  = src->TYPE;
    dst->hp = src->hp;
    dst->attack = src->attack;
    dst->CAN_EVOLVE = src->CAN_EVOLVE;

    if(dst->name != NULL) {
        free(dst->name);
        dst->name = NULL;
    }


    dst->name = mymyStrdup(src->name);
    if (!dst->name) {
        return FAIL;
    }
    return TRUE;
}

void evolvePokemon(const OwnerNode *owner) {
    if (!owner)
        return;
    PokemonNode* ownerPokedex = owner->pokedexRoot;
    if(ownerPokedex == NULL) {
        printf("Cannot evolve. Pokedex empty.\n");
        return;
    }
    const int id = readIntSafe("Enter ID of Pokemon to evolve:");
    PokemonNode* found = searchPokemonBFS(ownerPokedex,id);
    if(found == NULL) {
        printf("Pokedex is empty.\n");
        return;
    }
    if(found->data->CAN_EVOLVE) {
        const int existBefore = searchPokemonBFS(ownerPokedex,id+1)!=NULL;

        if(existBefore) {
            printf("Evolution ID %d (%s) already in the Pokedex. Releasing %s (ID %d).\n",
                   id+1,pokedex[id+1].name,pokedex[id].name,id);
            printf(" Removing Pokemon %s (ID %d).\n",found->data->name,found->data->id);
            ownerPokedex = removePokemonByID(ownerPokedex,id);
        }
        else {
            printf(" Removing Pokemon %s (ID %d).\n",found->data->name,found->data->id);
            ownerPokedex = removePokemonByID(ownerPokedex,id);
            PokemonNode *newPokemon = malloc(sizeof(PokemonNode));
            PokemonData *newPokemonData = malloc(sizeof(PokemonData));
            newPokemon->data = newPokemonData;
            newPokemon->data->name = mymyStrdup(pokedex[id].name);
            newPokemon->data->hp = pokedex[id].hp;
            newPokemon->data->attack = pokedex[id].attack;
            newPokemon->data->TYPE = pokedex[id].TYPE;
            newPokemon->data->CAN_EVOLVE = pokedex[id].CAN_EVOLVE;
            newPokemon->data->id = pokedex[id].id;
            newPokemon->left = NULL;
            newPokemon->right = NULL;
            insertPokemonNode(ownerPokedex,newPokemon);
            printf("Pokemon evolved from %s (ID %d) to %s (ID %d).\n"
            ,pokedex[id-1].name,id,newPokemon->data->name,id+1);
        }

    }
    else {
        printf("Pokemon cant evolved.\n");
    }
    
}

void enterExistingPokedexMenu()
{
    if(!ownerHead) {
        printf(" No existing Pokedexes.\n");
        return;
    }
    printf("\nExisting Pokedexes:\n");
    printOwnersList();
    const int id = readIntSafe("Choose a Pokedex by number:");
    OwnerNode * cur = ownerHead;
    for(int i = 1; i < id; i++) {
        cur = cur->next;
    }
    printf("\nEntering %s's Pokedex...\n", cur->ownerName);

    int subChoice;
    do
    {
        printf("-- %s's Pokedex Menu --\n", cur->ownerName);
        printf("1. Add Pokemon\n");
        printf("2. Display Pokedex\n");
        printf("3. Release Pokemon (by ID)\n");
        printf("4. Pokemon Fight!\n");
        printf("5. Evolve Pokemon\n");
        printf("6. Back to Main\n");

        subChoice = readIntSafe("Your choice: ");

        switch (subChoice)
        {
        case ADD_POKEMON:
            addPokemon(cur);
            break;
        case DISPLAY_POKEDEX:
            displayMenu(cur);
            break;
        case REMOVE_POKEMON:
            freePokemon(cur);
            break;
        case FIGHT:
            pokemonFight(cur);
            break;
        case EVOLVE_POKEMON:
            evolvePokemon(cur);
            break;
        case BACK:
            printf("Back to Main Menu.\n");
            break;
        default:
            printf("Invalid choice.\n");
        }
    } while (subChoice != BACK);
}

OwnerNode* getOwnerByIndex(const int id) {
    if(ownerHead == NULL) {
        return NULL;
    }
    OwnerNode* cur = ownerHead;
    for(int i = 1; i < id; i++) {
        cur = cur->next;
    }
    return cur;
}

void freeOwnerNode(OwnerNode *owner) {
    if (!owner)
        return;
    if(owner->ownerName != NULL) {
        free(owner->ownerName);
        owner->ownerName = NULL;
    }
    freePokemonTree(owner->pokedexRoot);
    owner->pokedexRoot = NULL;
}

void deletePokedex() {
    if(!ownerHead) {
        printf(" No existing Pokedexes to delete.\n");
        return;
    }
    printf("\n=== Delete a Pokedex ===\n");
    printOwnersList();
    const int id = readIntSafe("Choose a Pokedex to delete by number: ");
    OwnerNode *cur = getOwnerByIndex(id);
    if (!cur) { printf("Invalid selection.\n"); return; }
    printf("Deleting %s's entire Pokedex...\n", cur->ownerName);
    removeOwnerFromCircularList(cur);
    freeOwnerNode(cur);
    free(cur);
    printf("Pokedex deleted.\n");
}

void pokemonFight(const OwnerNode *owner) {
    if(owner->pokedexRoot == NULL) {
        printf("Pokedex is empty.\n");
        return;
    }
    const int firstId = readIntSafe("Enter ID of the first Pokemon:");
    const int secondId=readIntSafe(" Enter ID of the second Pokemon:") ;

    if(firstId < 0 || secondId < 0 || firstId >= SIZE || secondId >= SIZE) {
        printf("Invalid Pokemon fight.\n");
        return;
    }
    const PokemonNode* first =  searchPokemonBFS(owner->pokedexRoot, firstId);
    const PokemonNode* second =  searchPokemonBFS(owner->pokedexRoot, secondId);
    if(first == NULL || second == NULL) {
        printf("One or both Pokemon IDs not found.\n");
        return;
    }
    const double scoreOne = powerPokemonFight(first->data->attack,first->data->hp);
    const double scoreSecond = powerPokemonFight(second->data->attack,second->data->hp);
    printf(" Pokemon 1: %s (Score = %.2f)\n"
           "Pokemon 2: %s (Score = %.2f)\n",first->data->name,scoreOne,second->data->name,scoreSecond);
    if(scoreOne>scoreSecond) {
        printf("%s wins!\n",first->data->name);
    }
    else if(scoreOne < scoreSecond) {
        printf("%s wins!\n",second->data->name);
    }
    else {
        printf("tie!\n");;
    }
}

double powerPokemonFight(const int attack,const int hp) {
    return attack * ATTACK_RATIO + hp * HP_RATIO;
}

void freePokemonData(PokemonData *cur) {
    if(cur == NULL) {
        return;
    }
    free(cur->name);
    cur->name = NULL;
}

void addPokemon(const OwnerNode *owner) {
    const int choice = readIntSafe("Enter ID to add:");
    if(choice < 1 || choice > SIZE ) {
        printf("Invalid choice.\n");
        return;
    }

    if (searchPokemonBFS(owner->pokedexRoot, choice) != NULL) {
        printf(" Pokemon with ID %d is already in the Pokedex. No changes made.\n", choice);
        return;
    }
    PokemonData* pd = calloc(1,sizeof(PokemonData));
    if (pd == NULL) {
        freeAllOwners();
        exit(1);
    }
    const int res = copyPokemonData(pd,&pokedex[choice-1]);
    if(res == FAIL) {
        freePokemonData(pd);
        free(pd);
        freeAllOwners();
        exit(1);
    }
    OwnerNode *temp = ownerHead;
    while(temp != owner) {
        temp = temp->next;
    }
    PokemonNode* pdNode = malloc(sizeof(PokemonNode));
    if(pdNode == NULL) {
        freePokemonData(pd);
        free(pd);
        freeAllOwners();
        exit(1);
    }
    pdNode->data = pd;
    pdNode->left = NULL;
    pdNode->right = NULL;
    temp->pokedexRoot = insertPokemonNode(temp->pokedexRoot,pdNode);
    printf(" Pokemon %s (ID %d) added.\n",pd->name,pd->id);
}

void mainMenu()
{
    int choice;
    do
    {
        printf("=== Main Menu ===\n");
        printf("1. New Pokedex\n");
        printf("2. Existing Pokedex\n");
        printf("3. Delete a Pokedex\n");
        printf("4. Merge Pokedexes\n");
        printf("5. Sort Owners by Name\n");
        printf("6. Print Owners in a direction X times\n");
        printf("7. Exit\n");
        choice = readIntSafe("Your choice:");

        switch (choice)
        {
        case ADD:
            openPokedexMenu();
            break;
        case EXISTING_POKEDEX:
            enterExistingPokedexMenu();
            break;
        case REMOVE:
            deletePokedex();
            break;
        case MERGE_POKEDEX:
            mergePokedexMenu();
            break;
        case SORT_OWNER_BY_NAME:
            sortOwners();
            break;
        case PRINT:
            printOwnersCircular();
            break;
        case EXIT:
            printf(" Goodbye!\n");
            break;
        default:
            printf(" Invalid.\n");
        }
    } while (choice != EXIT);
}

PokemonNode *createPokemonNode(const PokemonData *data) {
    PokemonNode* curNode = malloc(sizeof(PokemonNode));
    if (curNode == NULL) {
        return NULL;
    }
    curNode->data = malloc(sizeof(PokemonData));
    if(curNode->data == NULL) {
        free(curNode);
        return NULL;
    }
    curNode->data->name = mymyStrdup(data->name);
    curNode->data->attack = data->attack;
    curNode->data->hp = data->hp;
    curNode->data->id = data->id;
    curNode->data->TYPE = data->TYPE;
    curNode->data->CAN_EVOLVE = data->CAN_EVOLVE;
    curNode->left = curNode->right = NULL;
    return curNode;
}

void openPokedexMenu(void)
{
    printf(" Your name: ");
    char* name = getDynamicInput();
    if (!name) {
        freeAllOwners();
        exit(1);
    }
    if(findOwnerByName(name) != NULL) {
        printf("Owner '%s' already exists. Not creating a new Pokedex.\n",name);
        free(name);
        return;
    }
    const int choice = readIntSafe("Choose Starter:\n1. Bulbasaur\n2. Charmander\n3. Squirtle\n");
    printf("Your choice: ");
    PokemonData* pd = malloc(sizeof(PokemonData));
    if (pd == NULL) {
        free(name);
        name = NULL;
        freeAllOwners();
        exit(1);
    }
    pd->name = NULL;
    int res = 0;
    switch (choice) {
        case BULBASAUR_CASE:
            res = copyPokemonData(pd,&pokedex[BULBASAUR]);
            break;
        case CHARMANDER_CASE:
            res = copyPokemonData(pd,&pokedex[CHARMANDER]);
            break;
        case SQUIRTLE_CASE:
            res = copyPokemonData(pd,&pokedex[SQUIRTLE]);
            break;
        default:
            printf("Invalid choice.\n");
            free(name);
            return;
    }
    if(res == FAIL) {
        free(name);
        freeAllOwners();
        free(pd);
        exit(1);
    }
    PokemonNode *pokemonNode = createPokemonNode(pd);
    if(pokemonNode == NULL) {
        free(name);
        freeAllOwners();
        exit(1);
    }
    OwnerNode *cur = createOwner(name, pokemonNode);
    if(!cur) {
        free(name);
        free(pokemonNode);
        freeAllOwners();
        exit(1);
    }
    linkOwnerInCircularList(cur);
    printf("New Pokedex created for %s with starter %s.\n", name, pokemonNode->data->name);
}

OwnerNode *createOwner(char *ownerName, PokemonNode *starter) {
    OwnerNode *newNode = malloc(sizeof(OwnerNode));
    if (!newNode) return NULL;
    newNode->ownerName   = ownerName;
    newNode->pokedexRoot = starter;
    newNode->next = NULL;
    newNode->prev = NULL;
    return newNode;
}

void freeAllOwners() {
    if (!ownerHead)
        return;

    OwnerNode *tail = ownerHead->prev;
    tail->next = NULL;


    OwnerNode *cur = ownerHead;
    while (cur) {
        OwnerNode *next = cur->next;
        freeOwnerNode(cur);
        free(cur);
        cur = next;
    }

    ownerHead = NULL;
}

void freePokemonTree(PokemonNode *root) {
    if (root == NULL)
        return;
    freePokemonTree(root->left);
    freePokemonTree(root->right);
    freePokemonNode(root);
}

void freePokemonNode(PokemonNode *node)
{
    if (node == NULL)
        return;
    free(node->data->name);
    node->data->name = NULL;
    free(node->data);
    node->data = NULL;
    free(node);
    node = NULL;
}