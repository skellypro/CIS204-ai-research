/* helpers.h
 * Header for simple gym-members BST helpers.
 * Contains type definitions and function prototypes used by main.c and helpers.c
 */

#ifndef HELPERS_H
#define HELPERS_H

#include <stdbool.h>
#include <stddef.h>

/* Member levels */
enum MemberLevel {
    LEVEL_INACTIVE = 0,
    LEVEL_BASIC = 1,
    LEVEL_GOLD = 2,
    LEVEL_PLATINUM = 3
};

/* Gym member data stored in each tree node. Strings are allocated dynamically. */
typedef struct _gym_member_data_ {
    char *firstName;
    char *lastName;
    unsigned memberId;
    unsigned short memberLevel; /* use enum MemberLevel values */
    bool paid;
} GymMember;

/* Forward declare GymLeaf so prototypes can use it. */
typedef struct _tree_node_type_gym_member_ GymLeaf;

/* Tree node definition */
struct _tree_node_type_gym_member_ {
    GymMember value;
    GymLeaf *higher; /* nodes with larger memberId */
    GymLeaf *lower;  /* nodes with smaller memberId */
};

/*
 * Function prototypes
 * - pointer-returning function: returns pointers into the tree (e.g., addLeaf)
 * - value-returning function: returns a GymMember by value (e.g., constructMember)
 */

/* Construct a GymMember value from given strings and fields.
 * The function returns a GymMember by value; caller is responsible for
 * later allocating/duplicating strings into a node (helpers.c will provide helpers).
 */
GymMember constructMember(const char *firstName,
                          const char *lastName,
                          unsigned memberId,
                          unsigned short newStatus,
                          bool paid);

/* Insert a member into the BST. Returns the (possibly new) root pointer. */
GymLeaf *addLeaf(GymLeaf *root, const GymMember *newMember);

/* Search for a member by ID. Returns a pointer to the GymMember inside the tree
 * or NULL if not found. Two variants: const and non-const. */
GymMember *search(GymLeaf *root, unsigned searchId);
const GymMember *search_const(const GymLeaf *root, unsigned searchId);

/* Change the membership level (or other status). Returns the updated level. */
unsigned short changeStatus(GymMember *member, unsigned short newStatus);

/* Remove a member with given ID from the tree. Returns new root. */
GymLeaf *removeMember(GymLeaf *root, unsigned memberId);

/* CSV persistence helpers */
GymLeaf *load_from_csv(const char *filename);
int save_to_csv(const char *filename, const GymLeaf *root);

/* Utility helpers */
void free_tree(GymLeaf *root);
void printMember(const GymMember *m);
size_t tree_size(const GymLeaf *root);

#endif /* HELPERS_H */
