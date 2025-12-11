/* helpers.c
 * Stubs for BST and GymMember helper functions.
 * Each function includes inline comments describing the algorithm and
 * implementation notes suitable for a beginner. These stubs intentionally
 * avoid full implementations so you can implement them step-by-step.
 */

#include "helpers.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

GymMember constructMember(const char *firstName,
                          const char *lastName,
                          unsigned memberId,
                          unsigned short newStatus,
                          bool paid)
{

    // Algorithm:
    // 1. Create a GymMember on the stack (return by value).
    GymMember m = {NULL, NULL, 0, 0, false};
    // 2. Duplicate firstName and lastName into heap-allocated strings
    //    (use strdup or malloc + strcpy). Check for NULL returns.
    //    Caller is responsible for freeing these strings in the GymMember struct.
    m.firstName = malloc(strlen(firstName) + 1);
    if (m.firstName != NULL) {
        strcpy(m.firstName, firstName);
    }
    m.lastName = malloc(strlen(lastName) + 1);
    if (m.lastName != NULL) {
        strcpy(m.lastName, lastName);
    }
    // 3. Set memberId, memberLevel, and paid fields from parameters.
    m.memberId = memberId;
    m.memberLevel = newStatus;
    m.paid = paid;
    // 4. Return m by value. Caller keeps strings valid and frees later.

    return m; // TODO: implement string duplication and field assignments
}

GymLeaf *addLeaf(GymLeaf *root, const GymMember *newMember)
{
    // Algorithm:
    // 1. If root is NULL: allocate new GymLeaf, copy newMember (dup strings),
    //    set left/right to NULL, return new node as root.
    if(root == NULL) {
        GymLeaf *newNode = (GymLeaf *)malloc(sizeof(GymLeaf));
        if (newNode != NULL) {
            newNode->value.firstName = strdup(newMember->firstName);
            newNode->value.lastName = strdup(newMember->lastName);
            newNode->value.memberId = newMember->memberId;
            newNode->value.memberLevel = newMember->memberLevel;
            newNode->value.paid = newMember->paid;
            newNode->higher = NULL;
            newNode->lower = NULL;
        }
        return newNode; // New root
    }
    // 2. Else: compare newMember->memberId with root->value.memberId:
    //    - If smaller: recursively insert into root->lower.
    //    - If larger: recursively insert into root->higher.
    //    - If equal: decide policy (reject duplicate or update).
    else {
        if (newMember->memberId < root->value.memberId) {
            root->lower = addLeaf(root->lower, newMember);
        } else if (newMember->memberId > root->value.memberId) {
            root->higher = addLeaf(root->higher, newMember);
        } else {
            // Duplicate ID found; for this implementation, we do nothing.
            // Alternatively, we could update the existing member's data.
        }
    }
    // 3. Return the (possibly updated) root pointer.
    // Note: use malloc for nodes, strdup for strings, check allocations.
    return root; // TODO: implement BST insertion logic
}

GymMember *search(GymLeaf *root, unsigned searchId)
{
    // Algorithm:
    // 1. Start at root.
    GymLeaf *node = root;
    // 2. While node is not NULL:
    //    - If searchId == node->value.memberId: return &node->value.
    //    - If searchId < node->value.memberId: move to node->lower.
    //    - Else: move to node->higher.
    while (node != NULL) {
        if (searchId == node->value.memberId) {
            return &node->value; // Found
        } else if (searchId < node->value.memberId) {
            node = node->lower; // Go left
        } else {
            node = node->higher; // Go right
        }
    }
    // 3. If not found, return NULL.
    return NULL; // TODO: implement iterative or recursive search
}

const GymMember *search_const(const GymLeaf *root, unsigned searchId)
{
    // Algorithm: Same as search() but using const pointers.
    const GymLeaf *node = root;
    while (node != NULL) {
        if (searchId == node->value.memberId) {
            return &node->value; // Found
        } else if (searchId < node->value.memberId) {
            node = node->lower; // Go left
        } else {
            node = node->higher; // Go right
        }
    }
    return NULL; /* not found */
}

unsigned short changeStatus(GymMember *member, unsigned short newStatus)
{
    // Algorithm:
    // 1. Check member is not NULL.
    if (member == NULL) {
        return 0; // Or some error code
    }
    // 2. Set member->memberLevel = newStatus.
    member->memberLevel = newStatus;
    // 3. Return the updated level (or previous level if preferred).
    return newStatus; // TODO: implement status update
}

GymLeaf *removeMember(GymLeaf *root, unsigned memberId)
{
    // BST deletion algorithm (three cases):
    // 1. Leaf node: free it, return NULL to parent.
    // 2. One child: replace node with child and free node.
    // 3. Two children: find inorder successor (min in right subtree),
    //    copy its value to current node, delete successor recursively.
    if (root == NULL) {
        return NULL; // Member not found; no changes
    }
    
    if (memberId < root->value.memberId) {
        root->lower = removeMember(root->lower, memberId);
    } else if (memberId > root->value.memberId) {
        root->higher = removeMember(root->higher, memberId);
    } else {
        // Node to delete found
        // Case 1: No children (leaf node)
        if (root->lower == NULL && root->higher == NULL) {
            free(root->value.firstName);
            free(root->value.lastName);
            free(root);
            return NULL;
        }
        // Case 2: One child
        if (root->lower == NULL) {
            GymLeaf *temp = root->higher;
            free(root->value.firstName);
            free(root->value.lastName);
            free(root);
            return temp;
        }
        if (root->higher == NULL) {
            GymLeaf *temp = root->lower;
            free(root->value.firstName);
            free(root->value.lastName);
            free(root);
            return temp;
        }
        // Case 3: Two children - find inorder successor
        GymLeaf *successor = root->higher;
        while (successor->lower != NULL) {
            successor = successor->lower;
        }
        root->value = successor->value;
        root->higher = removeMember(root->higher, successor->value.memberId);
    }
    return root;
}

GymLeaf *load_from_csv(const char *filename)
{
    // Algorithm:
    // 1. fopen(filename, "r").
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        return NULL; // File not found; return empty tree
    }
    // 2. Read lines with fgets, parse fields (firstName,lastName,memberId,
    //    memberLevel,paid) using strtok or sscanf.
    char buffer[256];
    GymLeaf *root = NULL;
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        // 3. For each record: construct GymMember, then addLeaf to root.
        char *copy = malloc(strlen(buffer) + 1);
        if (copy == NULL) continue;
        strcpy(copy, buffer);
        
        char *firstName = strtok(copy, ",");
        char *lastName = strtok(NULL, ",");
        char *memberId_str = strtok(NULL, ",");
        char *memberLevel_str = strtok(NULL, ",");
        char *paid_str = strtok(NULL, ",");
        
        if (firstName && lastName && memberId_str && memberLevel_str && paid_str) {
            unsigned memberId = atoi(memberId_str);
            unsigned short memberLevel = atoi(memberLevel_str);
            bool paid = atoi(paid_str) != 0;
            
            GymMember member = constructMember(firstName, lastName, memberId, memberLevel, paid);
            root = addLeaf(root, &member);
            
            free(member.firstName);
            free(member.lastName);
        }
        free(copy);
    }
    // 4. fclose and return root.
    fclose(file);
    // Error handling: if file doesn't exist, return NULL (empty tree).
    return root;
}

void save_to_csv_helper(FILE *file, const GymLeaf *root)
{
    if (root == NULL) {
        return;
    }
    save_to_csv_helper(file, root->lower);
    fprintf(file, "%s,%s,%u,%hu,%d\n", 
            root->value.firstName, root->value.lastName, 
            root->value.memberId, root->value.memberLevel, 
            root->value.paid);
    save_to_csv_helper(file, root->higher);
}

int save_to_csv(const char *filename, const GymLeaf *root)
{
    // Algorithm:
    // 1. fopen(filename, "w") to create/overwrite file.
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        return -1; // Error opening file
    }
    // 2. Inorder traversal: visit left, process node, visit right.
    //    This gives memberId ordering.
    //    Use a helper function for recursion.
    //    For each node, write a line to the file.
    //    Format: firstName,lastName,memberId,memberLevel,paid
    
    // 3. For each node, write CSV line: firstName,lastName,memberId,
    //    memberLevel,paid.
    // 4. fclose and return 0 on success, -1 on error.
    save_to_csv_helper(file, root);
    if(fclose(file))    
        return -1;
    return 0;
}

void free_tree(GymLeaf *root)
{
    // Algorithm: post-order traversal (free left, free right, then node).
    // For each node: free firstName, free lastName, then free node.
    if (root == NULL) {
        return;
    }
    free_tree(root->lower);
    free_tree(root->higher);
    free(root->value.firstName);
    free(root->value.lastName);
    free(root);
}

void printMember(const GymMember *m)
{
    // Print member in readable format:
    // Example: "ID: 42, Name: Ada Lovelace, Level: PLATINUM, Paid: yes"
    if (m == NULL) {
        return;
    }
    char level[20];
    switch (m->memberLevel) {
        case 0: strcpy(level, "INACTIVE"); break;
        case 1: strcpy(level, "BASIC"); break;
        case 2: strcpy(level, "GOLD"); break;
        case 3: strcpy(level, "PLATINUM"); break;
        default: strcpy(level, "UNKNOWN"); break;
    }
    const char *paid = m->paid ? "yes" : "no";
    printf("ID: %u, Name: %s %s, Level: %s, Paid: %s\n", 
           m->memberId, m->firstName, m->lastName, level, paid);
}

size_t tree_size(const GymLeaf *root)
{
    // Return node count via recursion: 1 + size(left) + size(right).
    // Base case: if root is NULL, return 0.
    if (root == NULL) {
        return 0;
    }
    return 1 + tree_size(root->lower) + tree_size(root->higher);
}
