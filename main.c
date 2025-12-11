// main.c
// Main entry point for the gym membership database program.
// Provides a menu-driven interface for user interaction with the BST.

#include "helpers.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Helper function to read a line from stdin safely.
// Approach: use fgets with size limit to avoid buffer overflows.
void read_line(char *buffer, size_t size)
{
    fgets(buffer, size, stdin);
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
    }
}

// Helper function to display the main menu and get user choice.
// Approach: print menu options and return selected mode as int.
int display_menu(void)
{
    printf("\n=== Gym Membership Database ===\n");
    printf("1. Lookup member by ID\n");
    printf("2. Add new member\n");
    printf("3. Edit member (change level or payment status)\n");
    printf("4. Cancel/delete member\n");
    printf("5. Save and exit\n");
    printf("Enter your choice (1-5): ");
    int choice;
    scanf("%d", &choice);
    getchar();
    return choice;
}

// Handle lookup mode: search for a member by ID and display.
// Approach: prompt for ID, search tree, print member or "not found" message.
void mode_lookup(GymLeaf *root)
{
    printf("Enter member ID to search: ");
    int memberId;
    scanf("%d", &memberId);
    getchar();
    GymMember *found = search(root, memberId);
    if (found != NULL) {
        printMember(found);
    } else {
        printf("Member not found.\n");
    }
}

// Handle new member mode: prompt for details and add member to tree.
// Approach: prompt for firstName, lastName, ID, level, paid status.
//           construct member, call addLeaf, update root pointer.
GymLeaf *mode_add_member(GymLeaf *root)
{
    char firstName[50];
    char lastName[50];
    int memberId;
    int level;
    int paid;
    printf("Enter first name: ");
    read_line(firstName, sizeof(firstName));
    printf("Enter last name: ");
    read_line(lastName, sizeof(lastName));
    printf("Enter member ID: ");
    scanf("%d", &memberId);
    getchar();
    printf("Enter member level (0-3): ");
    scanf("%d", &level);
    getchar();
    printf("Is paid? (1 for yes, 0 for no): ");
    scanf("%d", &paid);
    getchar();
    GymMember member = constructMember(firstName, lastName, memberId, level, paid);
    root = addLeaf(root, &member);
    printf("Member added successfully.\n");
    return root;
}

// Handle edit member mode: change level or payment status.
// Approach: prompt for ID, search, then modify found member.
void mode_edit_member(GymLeaf *root)
{
    printf("Enter member ID to edit: ");
    int memberId;
    scanf("%d", &memberId);
    getchar();
    GymMember *found = search(root, memberId);
    if (found == NULL) {
        printf("Member not found.\n");
        return;
    }
    printf("What would you like to change?\n");
    printf("1. Change level\n");
    printf("2. Change paid status\n");
    printf("Enter choice (1-2): ");
    int choice;
    scanf("%d", &choice);
    getchar();
    if (choice == 1) {
        unsigned int lvl = 0;
        printf("Enter new level (0-3): ");
        scanf("%u", &lvl);
        getchar();
        found->memberLevel = (unsigned short)lvl;
        printf("Level updated.\n");
    } else if (choice == 2) {
        int paid = 0;
        printf("Is paid? (1 for yes, 0 for no): ");
        scanf("%d", &paid);
        getchar();
        found->paid = (paid != 0);
        printf("Payment status updated.\n");
    }
}

// Handle cancel/delete member mode: remove member from tree.
// Approach: prompt for ID, call removeMember, update root pointer.
GymLeaf *mode_cancel_member(GymLeaf *root)
{
    printf("Enter member ID to delete: ");
    int memberId;
    scanf("%d", &memberId);
    getchar();
    root = removeMember(root, memberId);
    printf("Member deleted successfully.\n");
    return root;
}

// Save data to CSV file and prepare for exit.
// Approach: call save_to_csv, check return value, free tree, exit.
void cleanup_and_exit(GymLeaf *root)
{
    printf("Saving to data.csv...\n");
    if (save_to_csv("data.csv", root) == 0) {
        printf("Data saved successfully.\n");
    } else {
        printf("Error saving data.\n");
    }
    free_tree(root);
    printf("Goodbye!\n");
    exit(0);
}

// Main program loop.
int main(void)
{
    GymLeaf *root = load_from_csv("data.csv");
    
    while (1) {
        int choice = display_menu();
        switch (choice) {
            case 1:
                mode_lookup(root);
                break;
            case 2:
                root = mode_add_member(root);
                break;
            case 3:
                mode_edit_member(root);
                break;
            case 4:
                root = mode_cancel_member(root);
                break;
            case 5:
                cleanup_and_exit(root);
                break;
            default:
                printf("Invalid choice, try again.\n");
        }
    }
    return 0;
}
