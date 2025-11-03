/**
 * @file main.c
 * @brief Main logic app for Product Management System (FIXED UI)
 * @author PMS Team3, PRF192. 
 * @date 24-10-2025
 * @compatible Dev-C++ 6.3, TDM-GCC 9.2.0, C11
 */

#include "../include/utils.h"
#include "../include/category.h"
#include "../include/subgroup.h"
#include "../include/product.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

#define DATA_FILE "data/products.dat"

// Forward declarations
void display_main_menu(void);
void category_management_menu(DataStore* store);
void subgroup_management_menu(DataStore* store);
void product_management_menu(DataStore* store);
void search_menu(DataStore* store);
void statistics_menu(DataStore* store);

// Category menu functions
void add_category(DataStore* store);
void edit_category(DataStore* store);
void delete_category(DataStore* store);
void list_categories(DataStore* store);

// Subgroup menu functions
void add_subgroup(DataStore* store);
void edit_subgroup(DataStore* store);
void delete_subgroup(DataStore* store);
void list_subgroups(DataStore* store);

// Product menu functions
void add_product(DataStore* store);
void edit_product(DataStore* store);
void delete_product(DataStore* store);
void list_products(DataStore* store);

// Search functions
void search_by_name(DataStore* store);
void search_by_price(DataStore* store);
void search_by_quantity(DataStore* store);

// Statistics functions
void display_statistics(DataStore* store);

int main(void) {
    // Store original console code pages
    UINT originalInputCP = GetConsoleCP();
    UINT originalOutputCP = GetConsoleOutputCP();
    
    // Set console to UTF-8 mode
    SetConsoleCP(65001);
    SetConsoleOutputCP(65001);
    
    DataStore store = datastore_init();
    
    printf("Initializing Product Management System...\n");
    printf("Please ensure 'data' directory exists in the project folder.\n\n");
    
    // Load existing data
    datastore_load(&store, DATA_FILE);
    
    int choice;
    bool running = true;
    
    while (running) {
        clear_screen();
        display_main_menu();
        
        if (!safe_input_int("Enter your choice: ", &choice)) {
            printf("Invalid input. Please try again.\n");
            pause_screen();
            continue;
        }
        
        switch (choice) {
            case 1:
                category_management_menu(&store);
                break;
            case 2:
                subgroup_management_menu(&store);
                break;
            case 3:
                product_management_menu(&store);
                break;
            case 4:
                search_menu(&store);
                break;
            case 5:
                statistics_menu(&store);
                break;
            case 6:
                datastore_display_all(&store);
                pause_screen();
                break;
            case 0:
                if (store.is_modified) {
                    printf("\nData has been modified. Save before exit? (y/n): ");
                    char confirm[10];
                    if (safe_input_string("", confirm, sizeof(confirm))) {
                        if (confirm[0] == 'y' || confirm[0] == 'Y') {
                            datastore_save(&store, DATA_FILE);
                        }
                    }
                }
                running = false;
                printf("\nThank you for using Product Management System!\n");
                break;
            default:
                printf("\nInvalid choice. Please try again.\n");
                pause_screen();
        }
    }
    
    datastore_free(&store);
    
    // Restore original console code pages
    SetConsoleCP(originalInputCP);
    SetConsoleOutputCP(originalOutputCP);
    
    return 0;
}

void display_main_menu(void) {
    set_color(COLOR_HEADER);
    printf("\n");
    printf("  ╔══════════════════════════════════════════════════════════╗\n");
    printf("  ║        PRODUCT MANAGEMENT SYSTEM v1.0                    ║\n");
    printf("  ║        Run with Win11 & Dev-C++ 6.3                      ║\n");
    printf("  ║        -> Group 3-PRF192:Lab1                            ║\n");
    printf("  ║    Member: Manh, Khoa, Tuan Anh, Phuong Anh, Thu Phuong  ║\n");
    printf("  ╚══════════════════════════════════════════════════════════╝\n");
    set_color(COLOR_RESET);
    printf("\n");
    printf("  ┌──────────────────────────────────────────────────────────┐\n");
    printf("  │  [1] Category Management                                 │\n");
    printf("  │  [2] Subgroup Management                                 │\n");
    printf("  │  [3] Product Management                                  │\n");
    printf("  │  [4] Search & Filter                                     │\n");
    printf("  │  [5] Statistics & Reports                                │\n");
    printf("  │  [6] View All Data                                       │\n");
    printf("  │  [0] Save & Exit                                         │\n");
    printf("  └──────────────────────────────────────────────────────────┘\n");
    printf("\n");
}

// ============================================================================
// Category Management
// ============================================================================

void category_management_menu(DataStore* store) {
    int choice;
    bool back = false;
    
    while (!back) {
        clear_screen();
        set_color(COLOR_HEADER);
        printf("\n");
        printf("  ╔══════════════════════════════════════════════════════════╗\n");
        printf("  ║                   CATEGORY MANAGEMENT                    ║\n");
        printf("  ╚══════════════════════════════════════════════════════════╝\n");
        set_color(COLOR_RESET);
        printf("\n");
        printf("  ┌──────────────────────────────────────────────────────────┐\n");
        printf("  │  [1] Add Category                                        │\n");
        printf("  │  [2] Edit Category                                       │\n");
        printf("  │  [3] Delete Category                                     │\n");
        printf("  │  [4] List All Categories                                 │\n");
        printf("  │  [0] Back to Main Menu                                   │\n");
        printf("  └──────────────────────────────────────────────────────────┘\n");
        printf("\n");
        
        set_color(COLOR_INPUT);
        if (!safe_input_int("  Enter your choice: ", &choice)) {
            set_color(COLOR_ERROR);
            printf("  Invalid input. Please try again.\n");
            set_color(COLOR_RESET);
            pause_screen();
            continue;
        }
        set_color(COLOR_RESET);
        
        switch (choice) {
            case 1: add_category(store); break;
            case 2: edit_category(store); break;
            case 3: delete_category(store); break;
            case 4: list_categories(store); break;
            case 0: back = true; break;
            default:
                set_color(COLOR_ERROR);
                printf("\n  Invalid choice.\n");
                set_color(COLOR_RESET);
                pause_screen();
        }
    }
}

void add_category(DataStore* store) {
    clear_screen();
    set_color(COLOR_HEADER);
    printf("\n");
    printf("  ╔══════════════════════════════════════════════════════════╗\n");
    printf("  ║                    ADD NEW CATEGORY                      ║\n");
    printf("  ╚══════════════════════════════════════════════════════════╝\n");
    set_color(COLOR_RESET);
    printf("\n");
    
    char name[50], description[200];
    
    set_color(COLOR_INPUT);
    if (!safe_input_string("  Category Name: ", name, sizeof(name))) {
        set_color(COLOR_ERROR);
        printf("  Error reading category name.\n");
        set_color(COLOR_RESET);
        pause_screen();
        return;
    }
    set_color(COLOR_RESET);
    
    if (strlen(name) == 0) {
        set_color(COLOR_ERROR);
        printf("  Error: Category name cannot be empty.\n");
        set_color(COLOR_RESET);
        pause_screen();
        return;
    }
    
    set_color(COLOR_INPUT);
    if (!safe_input_string("  Description: ", description, sizeof(description))) {
        set_color(COLOR_ERROR);
        printf("  Error reading description.\n");
        set_color(COLOR_RESET);
        pause_screen();
        return;
    }
    set_color(COLOR_RESET);
    
    Category category = category_create(store->next_category_id, name, description);
    
    if (datastore_add_category(store, category)) {
        store->next_category_id++;
        set_color(COLOR_SUCCESS);
        printf("\n  ✓ Category added successfully! (ID: %d)\n", category.id);
        set_color(COLOR_RESET);
    } else {
        set_color(COLOR_ERROR);
        printf("\n  ✗ Failed to add category.\n");
        set_color(COLOR_RESET);
        category_free(&category);
    }
    
    pause_screen();
}

void edit_category(DataStore* store) {
    clear_screen();
    set_color(COLOR_HEADER);
    printf("\n");
    printf("  ╔══════════════════════════════════════════════════════════╗\n");
    printf("  ║                       EDIT CATEGORY                      ║\n");
    printf("  ╚══════════════════════════════════════════════════════════╝\n");
    set_color(COLOR_RESET);
    printf("\n");
    
    int id;
    set_color(COLOR_INPUT);
    if (!safe_input_int("  Enter Category ID to edit: ", &id)) {
        set_color(COLOR_ERROR);
        printf("  Invalid input.\n");
        set_color(COLOR_RESET);
        pause_screen();
        return;
    }
    set_color(COLOR_RESET);
    
    Category* category = datastore_find_category_by_id(store, id);
    if (!category) {
        set_color(COLOR_ERROR);
        printf("\n  ✗ Category ID %d not found.\n", id);
        set_color(COLOR_RESET);
        pause_screen();
        return;
    }
    
    printf("\n  Current Category Information:\n");
    printf("  ┌──────────────────────────────────────────────────────────┐\n");
    printf("  │  ID: %-52d│\n", category->id);
    printf("  │  Name: %-49s│\n", category->name);
    printf("  │  Description: %-44s│\n", category->description);
    printf("  └──────────────────────────────────────────────────────────┘\n");
    printf("\n");
    
    char name[50], description[200];
    
    printf("  Enter new values (press Enter to keep current):\n\n");
    
    set_color(COLOR_INPUT);
    if (safe_input_string("  New Name: ", name, sizeof(name)) && strlen(name) > 0) {
        category_update_name(category, name);
    }
    
    if (safe_input_string("  New Description: ", description, sizeof(description))) {
        category_update_description(category, description);
    }
    set_color(COLOR_RESET);
    
    store->is_modified = true;
    set_color(COLOR_SUCCESS);
    printf("\n  ✓ Category updated successfully!\n");
    set_color(COLOR_RESET);
    pause_screen();
}

void delete_category(DataStore* store) {
    clear_screen();
    set_color(COLOR_HEADER);
    printf("\n");
    printf("  ╔══════════════════════════════════════════════════════════╗\n");
    printf("  ║                      DELETE CATEGORY                     ║\n");
    printf("  ╚══════════════════════════════════════════════════════════╝\n");
    set_color(COLOR_RESET);
    printf("\n");
    
    int id;
    set_color(COLOR_INPUT);
    if (!safe_input_int("  Enter Category ID to delete: ", &id)) {
        set_color(COLOR_ERROR);
        printf("  Invalid input.\n");
        set_color(COLOR_RESET);
        pause_screen();
        return;
    }
    set_color(COLOR_RESET);
    
    Category* category = datastore_find_category_by_id(store, id);
    if (!category) {
        set_color(COLOR_ERROR);
        printf("\n  ✗ Category ID %d not found.\n", id);
        set_color(COLOR_RESET);
        pause_screen();
        return;
    }
    
    printf("\n  Category to delete:\n");
    printf("  ┌──────────────────────────────────────────────────────────┐\n");
    printf("  │  ID: %-52d│\n", category->id);
    printf("  │  Name: %-49s│\n", category->name);
    printf("  │  Subgroups: %-45d│\n", category->subgroup_count);
    printf("  └──────────────────────────────────────────────────────────┘\n");
    printf("\n");
    
    if (category->subgroup_count > 0) {
        set_color(COLOR_WARNING);
        printf("  ⚠ WARNING: This category contains %d subgroup(s) with products.\n", category->subgroup_count);
        printf("  All subgroups and products will be deleted!\n\n");
        set_color(COLOR_RESET);
    }
    
    char confirm[10];
    set_color(COLOR_INPUT);
    if (!safe_input_string("  Are you sure? (yes/no): ", confirm, sizeof(confirm))) {
        set_color(COLOR_ERROR);
        printf("  Deletion cancelled.\n");
        set_color(COLOR_RESET);
        pause_screen();
        return;
    }
    set_color(COLOR_RESET);
    
    if (strcmp(confirm, "yes") == 0 || strcmp(confirm, "YES") == 0) {
        if (datastore_remove_category(store, id)) {
            set_color(COLOR_SUCCESS);
            printf("\n  ✓ Category deleted successfully!\n");
            set_color(COLOR_RESET);
        } else {
            set_color(COLOR_ERROR);
            printf("\n  ✗ Failed to delete category.\n");
            set_color(COLOR_RESET);
        }
    } else {
        set_color(COLOR_ERROR);
        printf("\n  Deletion cancelled.\n");
        set_color(COLOR_RESET);
    }
    
    pause_screen();
}

void list_categories(DataStore* store) {
    clear_screen();
    set_color(COLOR_HEADER);
    printf("\n");
    printf("  ╔══════════════════════════════════════════════════════════╗\n");
    printf("  ║                       ALL CATEGORIES                     ║\n");
    printf("  ╚══════════════════════════════════════════════════════════╝\n");
    set_color(COLOR_RESET);
    printf("\n");
    
    if (store->category_count == 0) {
        printf("  No categories found.\n\n");
        pause_screen();
        return;
    }
    
    printf("  Total Categories: %d\n\n", store->category_count);
    category_display_table_header();
    
    for (int i = 0; i < store->category_count; i++) {
        category_display_table_row(&store->categories[i]);
    }
    
    category_display_table_footer();
    printf("\n");
    pause_screen();
}

// ============================================================================
// Subgroup Management
// ============================================================================

void subgroup_management_menu(DataStore* store) {
    int choice;
    bool back = false;
    
    while (!back) {
        clear_screen();
        set_color(COLOR_HEADER);
        printf("\n");
        printf("  ╔══════════════════════════════════════════════════════════╗\n");
        printf("  ║                    SUBGROUP MANAGEMENT                   ║\n");
        printf("  ╚══════════════════════════════════════════════════════════╝\n");
        set_color(COLOR_RESET);
        printf("\n");
        printf("  ┌──────────────────────────────────────────────────────────┐\n");
        printf("  │  [1] Add Subgroup                                        │\n");
        printf("  │  [2] Edit Subgroup                                       │\n");
        printf("  │  [3] Delete Subgroup                                     │\n");
        printf("  │  [4] List All Subgroups                                  │\n");
        printf("  │  [0] Back to Main Menu                                   │\n");
        printf("  └──────────────────────────────────────────────────────────┘\n");
        printf("\n");
        
        set_color(COLOR_INPUT);
        if (!safe_input_int("  Enter your choice: ", &choice)) {
            set_color(COLOR_ERROR);
            printf("  Invalid input. Please try again.\n");
            set_color(COLOR_RESET);
            pause_screen();
            continue;
        }
        set_color(COLOR_RESET);
        
        switch (choice) {
            case 1: add_subgroup(store); break;
            case 2: edit_subgroup(store); break;
            case 3: delete_subgroup(store); break;
            case 4: list_subgroups(store); break;
            case 0: back = true; break;
            default:
                set_color(COLOR_ERROR);
                printf("\n  Invalid choice.\n");
                set_color(COLOR_RESET);
                pause_screen();
        }
    }
}

void add_subgroup(DataStore* store) {
    clear_screen();
    set_color(COLOR_HEADER);
    printf("\n");
    printf("  ╔══════════════════════════════════════════════════════════╗\n");
    printf("  ║                     ADD NEW SUBGROUP                     ║\n");
    printf("  ╚══════════════════════════════════════════════════════════╝\n");
    set_color(COLOR_RESET);
    printf("\n");
    
    if (store->category_count == 0) {
        set_color(COLOR_ERROR);
        printf("  No categories available. Please create a category first.\n");
        set_color(COLOR_RESET);
        pause_screen();
        return;
    }
    
    printf("  Available Categories:\n");
    category_display_table_header();
    for (int i = 0; i < store->category_count; i++) {
        category_display_table_row(&store->categories[i]);
    }
    category_display_table_footer();
    printf("\n");
    
    int category_id;
    set_color(COLOR_INPUT);
    if (!safe_input_int("  Select Category ID: ", &category_id)) {
        set_color(COLOR_ERROR);
        printf("  Invalid input.\n");
        set_color(COLOR_RESET);
        pause_screen();
        return;
    }
    set_color(COLOR_RESET);
    
    Category* category = datastore_find_category_by_id(store, category_id);
    if (!category) {
        set_color(COLOR_ERROR);
        printf("\n  ✗ Category ID %d not found.\n", category_id);
        set_color(COLOR_RESET);
        pause_screen();
        return;
    }
    
    char name[50], description[200];
    
    set_color(COLOR_INPUT);
    if (!safe_input_string("  Subgroup Name: ", name, sizeof(name))) {
        set_color(COLOR_ERROR);
        printf("  Error reading subgroup name.\n");
        set_color(COLOR_RESET);
        pause_screen();
        return;
    }
    set_color(COLOR_RESET);
    
    if (strlen(name) == 0) {
        set_color(COLOR_ERROR);
        printf("  Error: Subgroup name cannot be empty.\n");
        set_color(COLOR_RESET);
        pause_screen();
        return;
    }
    
    set_color(COLOR_INPUT);
    if (!safe_input_string("  Description: ", description, sizeof(description))) {
        set_color(COLOR_ERROR);
        printf("  Error reading description.\n");
        set_color(COLOR_RESET);
        pause_screen();
        return;
    }
    set_color(COLOR_RESET);
    
    Subgroup subgroup = subgroup_create(store->next_subgroup_id, category_id, name, description);
    
    if (category_add_subgroup(category, subgroup)) {
        store->next_subgroup_id++;
        store->is_modified = true;
        set_color(COLOR_SUCCESS);
        printf("\n  ✓ Subgroup added successfully! (ID: %d)\n", subgroup.id);
        set_color(COLOR_RESET);
    } else {
        set_color(COLOR_ERROR);
        printf("\n  ✗ Failed to add subgroup.\n");
        set_color(COLOR_RESET);
        subgroup_free(&subgroup);
    }
    
    pause_screen();
}

void edit_subgroup(DataStore* store) {
    clear_screen();
    set_color(COLOR_HEADER);
    printf("\n");
    printf("  ╔══════════════════════════════════════════════════════════╗\n");
    printf("  ║                     EDIT SUBGROUP                        ║\n");
    printf("  ╚══════════════════════════════════════════════════════════╝\n");
    set_color(COLOR_RESET);
    printf("\n");
    
    int id;
    set_color(COLOR_INPUT);
    if (!safe_input_int("  Enter Subgroup ID to edit: ", &id)) {
        set_color(COLOR_ERROR);
        printf("  Invalid input.\n");
        set_color(COLOR_RESET);
        pause_screen();
        return;
    }
    set_color(COLOR_RESET);
    
    Subgroup* subgroup = datastore_find_subgroup_by_id(store, id);
    if (!subgroup) {
        set_color(COLOR_ERROR);
        printf("\n  ✗ Subgroup ID %d not found.\n", id);
        set_color(COLOR_RESET);
        pause_screen();
        return;
    }
    
    printf("\n  Current Subgroup Information:\n");
    printf("  ┌──────────────────────────────────────────────────────────┐\n");
    printf("  │  ID: %-52d│\n", subgroup->id);
    printf("  │  Category ID: %-44d│\n", subgroup->category_id);
    printf("  │  Name: %-49s│\n", subgroup->name);
    printf("  │  Description: %-44s│\n", subgroup->description);
    printf("  └──────────────────────────────────────────────────────────┘\n");
    printf("\n");
    
    char name[50], description[200];
    
    printf("  Enter new values (press Enter to keep current):\n\n");
    
    set_color(COLOR_INPUT);
    if (safe_input_string("  New Name: ", name, sizeof(name)) && strlen(name) > 0) {
        subgroup_update_name(subgroup, name);
    }
    
    if (safe_input_string("  New Description: ", description, sizeof(description))) {
        subgroup_update_description(subgroup, description);
    }
    set_color(COLOR_RESET);
    
    store->is_modified = true;
    set_color(COLOR_SUCCESS);
    printf("\n  ✓ Subgroup updated successfully!\n");
    set_color(COLOR_RESET);
    pause_screen();
}

void delete_subgroup(DataStore* store) {
    clear_screen();
    set_color(COLOR_HEADER);
    printf("\n");
    printf("  ╔══════════════════════════════════════════════════════════╗\n");
    printf("  ║                      DELETE SUBGROUP                     ║\n");
    printf("  ╚══════════════════════════════════════════════════════════╝\n");
    set_color(COLOR_RESET);
    printf("\n");
    
    int id;
    set_color(COLOR_INPUT);
    if (!safe_input_int("  Enter Subgroup ID to delete: ", &id)) {
        set_color(COLOR_ERROR);
        printf("  Invalid input.\n");
        set_color(COLOR_RESET);
        pause_screen();
        return;
    }
    set_color(COLOR_RESET);
    
    Subgroup* subgroup = datastore_find_subgroup_by_id(store, id);
    if (!subgroup) {
        set_color(COLOR_ERROR);
        printf("\n  ✗ Subgroup ID %d not found.\n", id);
        set_color(COLOR_RESET);
        pause_screen();
        return;
    }
    
    Category* category = datastore_find_category_by_id(store, subgroup->category_id);
    
    printf("\n  Subgroup to delete:\n");
    printf("  ┌──────────────────────────────────────────────────────────┐\n");
    printf("  │  ID: %-52d│\n", subgroup->id);
    printf("  │  Name: %-49s│\n", subgroup->name);
    printf("  │  Products: %-45d│\n", subgroup->product_count);
    printf("  └──────────────────────────────────────────────────────────┘\n");
    printf("\n");
    
    if (subgroup->product_count > 0) {
        set_color(COLOR_WARNING);
        printf("  ⚠ WARNING: This subgroup contains %d product(s).\n", subgroup->product_count);
        printf("  All products will be deleted!\n\n");
        set_color(COLOR_RESET);
    }
    
    char confirm[10];
    set_color(COLOR_INPUT);
    if (!safe_input_string("  Are you sure? (yes/no): ", confirm, sizeof(confirm))) {
        set_color(COLOR_ERROR);
        printf("  Deletion cancelled.\n");
        set_color(COLOR_RESET);
        pause_screen();
        return;
    }
    set_color(COLOR_RESET);
    
    if (strcmp(confirm, "yes") == 0 || strcmp(confirm, "YES") == 0) {
        if (category_remove_subgroup(category, id)) {
            store->is_modified = true;
            set_color(COLOR_SUCCESS);
            printf("\n  ✓ Subgroup deleted successfully!\n");
            set_color(COLOR_RESET);
        } else {
            set_color(COLOR_ERROR);
            printf("\n  ✗ Failed to delete subgroup.\n");
            set_color(COLOR_RESET);
        }
    } else {
        set_color(COLOR_ERROR);
        printf("\n  Deletion cancelled.\n");
        set_color(COLOR_RESET);
    }
    
    pause_screen();
}

void list_subgroups(DataStore* store) {
    clear_screen();
    set_color(COLOR_HEADER);
    printf("\n");
    printf("  ╔══════════════════════════════════════════════════════════╗\n");
    printf("  ║                       ALL SUBGROUPS                      ║\n");
    printf("  ╚══════════════════════════════════════════════════════════╝\n");
    set_color(COLOR_RESET);
    printf("\n");
    
    int total = 0;
    for (int i = 0; i < store->category_count; i++) {
        total += store->categories[i].subgroup_count;
    }
    
    if (total == 0) {
        printf("  No subgroups found.\n\n");
        pause_screen();
        return;
    }
    
    printf("  Total Subgroups: %d\n\n", total);
    subgroup_display_table_header();
    
    for (int i = 0; i < store->category_count; i++) {
        for (int j = 0; j < store->categories[i].subgroup_count; j++) {
            subgroup_display_table_row(&store->categories[i].subgroups[j]);
        }
    }
    
    subgroup_display_table_footer();
    printf("\n");
    pause_screen();
}

// ============================================================================
// Product Management
// ============================================================================

void product_management_menu(DataStore* store) {
    int choice;
    bool back = false;
    
    while (!back) {
        clear_screen();
        set_color(COLOR_HEADER);
        printf("\n");
        printf("  ╔══════════════════════════════════════════════════════════╗\n");
        printf("  ║                       PRODUCT MANAGEMENT                 ║\n");
        printf("  ╚══════════════════════════════════════════════════════════╝\n");
        set_color(COLOR_RESET);
        printf("\n");
        printf("  ┌──────────────────────────────────────────────────────────┐\n");
        printf("  │  [1] Add Product                                         │\n");
        printf("  │  [2] Edit Product                                        │\n");
        printf("  │  [3] Delete Product                                      │\n");
        printf("  │  [4] List All Products                                   │\n");
        printf("  │  [0] Back to Main Menu                                   │\n");
        printf("  └──────────────────────────────────────────────────────────┘\n");
        printf("\n");
        
        set_color(COLOR_INPUT);
        if (!safe_input_int("  Enter your choice: ", &choice)) {
            set_color(COLOR_ERROR);
            printf("  Invalid input. Please try again.\n");
            set_color(COLOR_RESET);
            pause_screen();
            continue;
        }
        set_color(COLOR_RESET);
        
        switch (choice) {
            case 1: add_product(store); break;
            case 2: edit_product(store); break;
            case 3: delete_product(store); break;
            case 4: list_products(store); break;
            case 0: back = true; break;
            default:
                set_color(COLOR_ERROR);
                printf("\n  Invalid choice.\n");
                set_color(COLOR_RESET);
                pause_screen();
        }
    }
}

void add_product(DataStore* store) {
    clear_screen();
    set_color(COLOR_HEADER);
    printf("\n");
    printf("  ╔══════════════════════════════════════════════════════════╗\n");
    printf("  ║                       ADD NEW PRODUCT                    ║\n");
    printf("  ╚══════════════════════════════════════════════════════════╝\n");
    set_color(COLOR_RESET);
    printf("\n");
    
    // Check if subgroups exist
    int total_subgroups = 0;
    for (int i = 0; i < store->category_count; i++) {
        total_subgroups += store->categories[i].subgroup_count;
    }
    
    if (total_subgroups == 0) {
        set_color(COLOR_ERROR);
        printf("  No subgroups available. Please create a subgroup first.\n");
        set_color(COLOR_RESET);
        pause_screen();
        return;
    }
    
    printf("  Available Subgroups:\n");
    subgroup_display_table_header();
    for (int i = 0; i < store->category_count; i++) {
        for (int j = 0; j < store->categories[i].subgroup_count; j++) {
            subgroup_display_table_row(&store->categories[i].subgroups[j]);
        }
    }
    subgroup_display_table_footer();
    printf("\n");
    
    int subgroup_id;
    set_color(COLOR_INPUT);
    if (!safe_input_int("  Select Subgroup ID: ", &subgroup_id)) {
        set_color(COLOR_ERROR);
        printf("  Invalid input.\n");
        set_color(COLOR_RESET);
        pause_screen();
        return;
    }
    set_color(COLOR_RESET);
    
    Subgroup* subgroup = datastore_find_subgroup_by_id(store, subgroup_id);
    if (!subgroup) {
        set_color(COLOR_ERROR);
        printf("\n  ✗ Subgroup ID %d not found.\n", subgroup_id);
        set_color(COLOR_RESET);
        pause_screen();
        return;
    }
    
    char code[20], name[100], description[200], timestamp[20];
    float price;
    int quantity;
    
    set_color(COLOR_INPUT);
    if (!safe_input_string("  Product Code: ", code, sizeof(code)) || strlen(code) == 0) {
        set_color(COLOR_ERROR);
        printf("  Error: Product code cannot be empty.\n");
        set_color(COLOR_RESET);
        pause_screen();
        return;
    }
    
    if (!safe_input_string("  Product Name: ", name, sizeof(name)) || strlen(name) == 0) {
        set_color(COLOR_ERROR);
        printf("  Error: Product name cannot be empty.\n");
        set_color(COLOR_RESET);
        pause_screen();
        return;
    }
    
    if (!safe_input_string("  Description: ", description, sizeof(description))) {
        description[0] = '\0';
    }
    set_color(COLOR_RESET);
    
    // Price input with better prompt
    set_color(COLOR_INPUT);
    printf("  Price (example: 20000 or 99.99): ");
    if (!safe_input_float("", &price) || price < 0) {
        set_color(COLOR_ERROR);
        printf("\n  ✗ Error: Invalid price. Please enter a valid number (e.g., 20000 or 99.99)\n");
        set_color(COLOR_RESET);
        pause_screen();
        return;
    }
    set_color(COLOR_RESET);
    
    // Quantity input with better prompt
    set_color(COLOR_INPUT);
    printf("  Quantity (must be whole number, e.g., 5): ");
    if (!safe_input_int("", &quantity) || quantity < 0) {
        set_color(COLOR_ERROR);
        printf("\n  ✗ Error: Invalid quantity. Please enter a whole number (e.g., 5 not 5.0)\n");
        printf("     Hint: Quantity must be an integer without decimal point.\n");
        set_color(COLOR_RESET);
        pause_screen();
        return;
    }
    set_color(COLOR_RESET);
    
    get_current_timestamp(timestamp, sizeof(timestamp));
    
    Product product = product_create(store->next_product_id, subgroup_id, code, name, description, price, quantity);
    
    if (subgroup_add_product(subgroup, product)) {
        store->next_product_id++;
        store->is_modified = true;
        set_color(COLOR_SUCCESS);
        printf("\n  ✓ Product added successfully! (ID: %d)\n", product.id);
        set_color(COLOR_RESET);
    } else {
        set_color(COLOR_ERROR);
        printf("\n  ✗ Failed to add product.\n");
        set_color(COLOR_RESET);
    }
    
    pause_screen();
}

void edit_product(DataStore* store) {
    clear_screen();
    set_color(COLOR_HEADER);
    printf("\n");
    printf("  ╔══════════════════════════════════════════════════════════╗\n");
    printf("  ║                       EDIT PRODUCT                       ║\n");
    printf("  ╚══════════════════════════════════════════════════════════╝\n");
    set_color(COLOR_RESET);
    printf("\n");
    
    int id;
    set_color(COLOR_INPUT);
    if (!safe_input_int("  Enter Product ID to edit: ", &id)) {
        set_color(COLOR_ERROR);
        printf("  Invalid input.\n");
        set_color(COLOR_RESET);
        pause_screen();
        return;
    }
    set_color(COLOR_RESET);
    
    Product* product = datastore_find_product_by_id(store, id);
    if (!product) {
        set_color(COLOR_ERROR);
        printf("\n  ✗ Product ID %d not found.\n", id);
        set_color(COLOR_RESET);
        pause_screen();
        return;
    }
    
    printf("\n  Current Product Information:\n");
    product_display(product);
    
    printf("\n  Enter new values (press Enter to keep current):\n\n");
    
    char buffer[200];
    float price;
    int quantity;
    
    set_color(COLOR_INPUT);
    if (safe_input_string("  New Code: ", buffer, sizeof(buffer)) && strlen(buffer) > 0) {
        product_update_code(product, buffer);
    }
    
    if (safe_input_string("  New Name: ", buffer, sizeof(buffer)) && strlen(buffer) > 0) {
        product_update_name(product, buffer);
    }
    
    if (safe_input_string("  New Description: ", buffer, sizeof(buffer))) {
        product_update_description(product, buffer);
    }
    
    printf("  New Price (current: %.2f, press Enter to skip): ", product->price);
    if (safe_input_float("", &price) && price >= 0) {
        product_update_price(product, price);
    }
    
    printf("  New Quantity (current: %d, press Enter to skip): ", product->quantity);
    if (safe_input_int("", &quantity) && quantity >= 0) {
        product_update_quantity(product, quantity);
    }
    set_color(COLOR_RESET);
    
    product_update_timestamp(product);
    store->is_modified = true;
    set_color(COLOR_SUCCESS);
    printf("\n  ✓ Product updated successfully!\n");
    set_color(COLOR_RESET);
    pause_screen();
}

void delete_product(DataStore* store) {
    clear_screen();
    set_color(COLOR_HEADER);
    printf("\n");
    printf("  ╔══════════════════════════════════════════════════════════╗\n");
    printf("  ║                       DELETE PRODUCT                     ║\n");
    printf("  ╚══════════════════════════════════════════════════════════╝\n");
    set_color(COLOR_RESET);
    printf("\n");
    
    int id;
    set_color(COLOR_INPUT);
    if (!safe_input_int("  Enter Product ID to delete: ", &id)) {
        set_color(COLOR_ERROR);
        printf("  Invalid input.\n");
        set_color(COLOR_RESET);
        pause_screen();
        return;
    }
    set_color(COLOR_RESET);
    
    Product* product = datastore_find_product_by_id(store, id);
    if (!product) {
        set_color(COLOR_ERROR);
        printf("\n  ✗ Product ID %d not found.\n", id);
        set_color(COLOR_RESET);
        pause_screen();
        return;
    }
    
    printf("\n  Product to delete:\n");
    product_display(product);
    
    char confirm[10];
    set_color(COLOR_INPUT);
    if (!safe_input_string("\n  Are you sure? (yes/no): ", confirm, sizeof(confirm))) {
        set_color(COLOR_ERROR);
        printf("  Deletion cancelled.\n");
        set_color(COLOR_RESET);
        pause_screen();
        return;
    }
    set_color(COLOR_RESET);
    
    if (strcmp(confirm, "yes") == 0 || strcmp(confirm, "YES") == 0) {
        Subgroup* subgroup = datastore_find_subgroup_by_id(store, product->subgroup_id);
        if (subgroup && subgroup_remove_product(subgroup, id)) {
            store->is_modified = true;
            set_color(COLOR_SUCCESS);
            printf("\n  ✓ Product deleted successfully!\n");
            set_color(COLOR_RESET);
        } else {
            set_color(COLOR_ERROR);
            printf("\n  ✗ Failed to delete product.\n");
            set_color(COLOR_RESET);
        }
    } else {
        set_color(COLOR_ERROR);
        printf("\n  Deletion cancelled.\n");
        set_color(COLOR_RESET);
    }
    
    pause_screen();
}

void list_products(DataStore* store) {
    clear_screen();
    set_color(COLOR_HEADER);
    printf("\n");
    printf("  ╔══════════════════════════════════════════════════════════╗\n");
    printf("  ║                       ALL PRODUCTS                       ║\n");
    printf("  ╚══════════════════════════════════════════════════════════╝\n");
    set_color(COLOR_RESET);
    printf("\n");
    
    Statistics stats = datastore_get_statistics(store);
    
    if (stats.total_products == 0) {
        printf("  No products found.\n\n");
        pause_screen();
        return;
    }
    
    printf("  Total Products: %d\n\n", stats.total_products);
    product_display_table_header();
    
    for (int i = 0; i < store->category_count; i++) {
        for (int j = 0; j < store->categories[i].subgroup_count; j++) {
            for (int k = 0; k < store->categories[i].subgroups[j].product_count; k++) {
                product_display_table_row(&store->categories[i].subgroups[j].products[k]);
            }
        }
    }
    
    product_display_table_footer();
    printf("\n");
    pause_screen();
}

// ============================================================================
// Search & Filter
// ============================================================================

void search_menu(DataStore* store) {
    int choice;
    bool back = false;
    
    while (!back) {
        clear_screen();
        set_color(COLOR_HEADER);
        printf("\n");
        printf("  ╔══════════════════════════════════════════════════════════╗\n");
        printf("  ║                       SEARCH & FILTER                    ║\n");
        printf("  ╚══════════════════════════════════════════════════════════╝\n");
        set_color(COLOR_RESET);
        printf("\n");
        printf("  ┌──────────────────────────────────────────────────────────┐\n");
        printf("  │  [1] Search by Name                                      │\n");
        printf("  │  [2] Search by Price Range                               │\n");
        printf("  │  [3] Search by Quantity Range                            │\n");
        printf("  │  [0] Back to Main Menu                                   │\n");
        printf("  └──────────────────────────────────────────────────────────┘\n");
        printf("\n");
        
        set_color(COLOR_INPUT);
        if (!safe_input_int("  Enter your choice: ", &choice)) {
            set_color(COLOR_ERROR);
            printf("  Invalid input. Please try again.\n");
            set_color(COLOR_RESET);
            pause_screen();
            continue;
        }
        set_color(COLOR_RESET);
        
        switch (choice) {
            case 1: search_by_name(store); break;
            case 2: search_by_price(store); break;
            case 3: search_by_quantity(store); break;
            case 0: back = true; break;
            default:
                set_color(COLOR_ERROR);
                printf("\n  Invalid choice.\n");
                set_color(COLOR_RESET);
                pause_screen();
        }
    }
}

void search_by_name(DataStore* store) {
    clear_screen();
    set_color(COLOR_HEADER);
    printf("\n");
    printf("  ╔══════════════════════════════════════════════════════════╗\n");
    printf("  ║                       SEARCH BY NAME                     ║\n");
    printf("  ╚══════════════════════════════════════════════════════════╝\n");
    set_color(COLOR_RESET);
    printf("\n");
    
    char name[100];
    set_color(COLOR_INPUT);
    if (!safe_input_string("  Enter product name (partial match): ", name, sizeof(name))) {
        set_color(COLOR_ERROR);
        printf("  Invalid input.\n");
        set_color(COLOR_RESET);
        pause_screen();
        return;
    }
    set_color(COLOR_RESET);
    
    SearchResult result = datastore_search_products_by_name(store, name);
    
    printf("\n  Search Results: %d product(s) found\n\n", result.count);
    
    if (result.count > 0) {
        product_display_table_header();
        for (int i = 0; i < result.count; i++) {
            product_display_table_row(&result.products[i]);
        }
        product_display_table_footer();
    }
    
    search_result_free(&result);
    pause_screen();
}

void search_by_price(DataStore* store) {
    clear_screen();
    set_color(COLOR_HEADER);
    printf("\n");
    printf("  ╔══════════════════════════════════════════════════════════╗\n");
    printf("  ║                  SEARCH BY PRICE RANGE                   ║\n");
    printf("  ╚══════════════════════════════════════════════════════════╝\n");
    set_color(COLOR_RESET);
    printf("\n");
    
    float min_price, max_price;
    
    set_color(COLOR_INPUT);
    if (!safe_input_float("  Minimum Price: ", &min_price) || min_price < 0) {
        set_color(COLOR_ERROR);
        printf("  Invalid input.\n");
        set_color(COLOR_RESET);
        pause_screen();
        return;
    }
    
    if (!safe_input_float("  Maximum Price: ", &max_price) || max_price < min_price) {
        set_color(COLOR_ERROR);
        printf("  Invalid input. Maximum must be >= minimum.\n");
        set_color(COLOR_RESET);
        pause_screen();
        return;
    }
    set_color(COLOR_RESET);
    
    SearchResult result = datastore_search_products_by_price(store, min_price, max_price);
    
    printf("\n  Search Results: %d product(s) found\n\n", result.count);
    
    if (result.count > 0) {
        product_display_table_header();
        for (int i = 0; i < result.count; i++) {
            product_display_table_row(&result.products[i]);
        }
        product_display_table_footer();
    }
    
    search_result_free(&result);
    pause_screen();
}

void search_by_quantity(DataStore* store) {
    clear_screen();
    set_color(COLOR_HEADER);
    printf("\n");
    printf("  ╔══════════════════════════════════════════════════════════╗\n");
    printf("  ║                SEARCH BY QUANTITY RANGE                  ║\n");
    printf("  ╚══════════════════════════════════════════════════════════╝\n");
    set_color(COLOR_RESET);
    printf("\n");
    
    int min_qty, max_qty;
    
    set_color(COLOR_INPUT);
    if (!safe_input_int("  Minimum Quantity: ", &min_qty) || min_qty < 0) {
        set_color(COLOR_ERROR);
        printf("  Invalid input.\n");
        set_color(COLOR_RESET);
        pause_screen();
        return;
    }
    
    if (!safe_input_int("  Maximum Quantity: ", &max_qty) || max_qty < min_qty) {
        set_color(COLOR_ERROR);
        printf("  Invalid input. Maximum must be >= minimum.\n");
        set_color(COLOR_RESET);
        pause_screen();
        return;
    }
    set_color(COLOR_RESET);
    
    SearchResult result = datastore_search_products_by_quantity(store, min_qty, max_qty);
    
    printf("\n  Search Results: %d product(s) found\n\n", result.count);
    
    if (result.count > 0) {
        product_display_table_header();
        for (int i = 0; i < result.count; i++) {
            product_display_table_row(&result.products[i]);
        }
        product_display_table_footer();
    }
    
    search_result_free(&result);
    pause_screen();
}

// ============================================================================
// Statistics & Reports
// ============================================================================

void statistics_menu(DataStore* store) {
    clear_screen();
    set_color(COLOR_HEADER);
    printf("\n");
    printf("  ╔══════════════════════════════════════════════════════════╗\n");
    printf("  ║                  STATISTICS & REPORTS                    ║\n");
    printf("  ╚══════════════════════════════════════════════════════════╝\n");
    set_color(COLOR_RESET);
    printf("\n");
    
    Statistics stats = datastore_get_statistics(store);
    
    printf("  ┌──────────────────────────────────────────────────────────┐\n");
    printf("  │  SYSTEM STATISTICS                                       │\n");
    printf("  ├──────────────────────────────────────────────────────────┤\n");
    printf("  │  Total Categories:    %-31d    │\n", stats.total_categories);
    printf("  │  Total Subgroups:     %-31d    │\n", stats.total_subgroups);
    printf("  │  Total Products:      %-31d    │\n", stats.total_products);
    printf("  │  Total Inventory:     %-26d units   │\n", stats.total_quantity);
    printf("  │  Total Value:         $%-30.2f    │\n", stats.total_value);
    printf("  │  Average Price:       $%-30.2f    │\n", stats.average_price);
    printf("  └──────────────────────────────────────────────────────────┘\n");
    printf("\n");
    
    pause_screen();
}