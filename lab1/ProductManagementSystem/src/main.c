/**
 * @file main.c
 * @brief Main logic app for Product Management System
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
    SetConsoleCP(65001);         // Sets input code page to UTF-8
    SetConsoleOutputCP(65001);   // Sets output code page to UTF-8
    
    DataStore store = datastore_init();
    
    // Create data directory if needed (manual step for user)
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
    printf("\n╔════════════════════════════════════════════════════════════╗\n");
    printf  ("║        PRODUCT MANAGEMENT SYSTEM v1.0                      ║\n");
    printf  ("║        Run with Win11 & Dev-C++ 6.3                        ║\n");
    printf  ("║        -> Group 3-PRF192:Lab1                              ║\n");
    printf  ("║    Member: Manh, Khoa, Tuan Anh, Phuong Anh, Thu Phuong    ║\n");
    printf  ("╚════════════════════════════════════════════════════════════╝\n\n");
    printf("  [1] Category Management\n");
    printf("  [2] Subgroup Management\n");
    printf("  [3] Product Management\n");
    printf("  [4] Search & Filter\n");
    printf("  [5] Statistics & Reports\n");
    printf("  [6] View All Data\n");
    printf("  [0] Save & Exit\n");
    printf("\n════════════════════════════════════════════════════════════\n");
}

// ============================================================================
// Category Management
// ============================================================================

void category_management_menu(DataStore* store) {
    int choice;
    bool back = false;
    
    while (!back) {
        clear_screen();
        printf("\n╔════════════════════════════════════════════════════════════╗\n");
        printf  ("║                   CATEGORY MANAGEMENT                      ║\n");
        printf  ("╚════════════════════════════════════════════════════════════╝\n\n");
        printf("  [1] Add Category\n");
        printf("  [2] Edit Category\n");
        printf("  [3] Delete Category\n");
        printf("  [4] List All Categories\n");
        printf("  [0] Back to Main Menu\n");
        printf("\n════════════════════════════════════════════════════════════\n");
        
        if (!safe_input_int("Enter your choice: ", &choice)) {
            printf("Invalid input. Please try again.\n");
            pause_screen();
            continue;
        }
        
        switch (choice) {
            case 1: add_category(store); break;
            case 2: edit_category(store); break;
            case 3: delete_category(store); break;
            case 4: list_categories(store); break;
            case 0: back = true; break;
            default:
                printf("\nInvalid choice.\n");
                pause_screen();
        }
    }
}

void add_category(DataStore* store) {
    clear_screen();
    printf("\n╔════════════════════════════════════════════════════════════╗\n");
    printf  ("║                    ADD NEW CATEGORY                        ║\n");
    printf  ("╚════════════════════════════════════════════════════════════╝\n\n");
    
    char name[50], description[200];
    
    if (!safe_input_string("Category Name: ", name, sizeof(name))) {
        printf("Error reading category name.\n");
        pause_screen();
        return;
    }
    
    if (strlen(name) == 0) {
        printf("Error: Category name cannot be empty.\n");
        pause_screen();
        return;
    }
    
    if (!safe_input_string("Description: ", description, sizeof(description))) {
        printf("Error reading description.\n");
        pause_screen();
        return;
    }
    
    Category category = category_create(store->next_category_id, name, description);
    
    if (datastore_add_category(store, category)) {
        store->next_category_id++;
        printf("\n✓ Category added successfully! (ID: %d)\n", category.id);
    } else {
        printf("\n✗ Failed to add category.\n");
        category_free(&category);
    }
    
    pause_screen();
}

void edit_category(DataStore* store) {
    clear_screen();
    printf("\n╔════════════════════════════════════════════════════════════╗\n");
    printf("║                       EDIT CATEGORY                        ║\n");
    printf("╚════════════════════════════════════════════════════════════╝\n\n");
    
    int id;
    if (!safe_input_int("Enter Category ID to edit: ", &id)) {
        printf("Invalid input.\n");
        pause_screen();
        return;
    }
    
    Category* category = datastore_find_category_by_id(store, id);
    if (!category) {
        printf("\n✗ Category ID %d not found.\n", id);
        pause_screen();
        return;
    }
    
    printf("\nCurrent Category Information:\n");
    printf("  ID: %d\n", category->id);
    printf("  Name: %s\n", category->name);
    printf("  Description: %s\n\n", category->description);
    
    char name[50], description[200];
    
    printf("Enter new values (press Enter to keep current):\n\n");
    
    if (safe_input_string("New Name: ", name, sizeof(name)) && strlen(name) > 0) {
        category_update_name(category, name);
    }
    
    if (safe_input_string("New Description: ", description, sizeof(description))) {
        category_update_description(category, description);
    }
    
    store->is_modified = true;
    printf("\n✓ Category updated successfully!\n");
    pause_screen();
}

void delete_category(DataStore* store) {
    clear_screen();
    printf("\n╔════════════════════════════════════════════════════════════╗\n");
    printf("║                      DELETE CATEGORY                       ║\n");
    printf("╚════════════════════════════════════════════════════════════╝\n\n");
    
    int id;
    if (!safe_input_int("Enter Category ID to delete: ", &id)) {
        printf("Invalid input.\n");
        pause_screen();
        return;
    }
    
    Category* category = datastore_find_category_by_id(store, id);
    if (!category) {
        printf("\n✗ Category ID %d not found.\n", id);
        pause_screen();
        return;
    }
    
    printf("\nCategory to delete:\n");
    printf("  ID: %d\n", category->id);
    printf("  Name: %s\n", category->name);
    printf("  Subgroups: %d\n\n", category->subgroup_count);
    
    if (category->subgroup_count > 0) {
        printf("WARNING: This category contains %d subgroup(s) with products.\n", category->subgroup_count);
        printf("All subgroups and products will be deleted!\n\n");
    }
    
    char confirm[10];
    if (!safe_input_string("Are you sure? (yes/no): ", confirm, sizeof(confirm))) {
        printf("Deletion cancelled.\n");
        pause_screen();
        return;
    }
    
    if (strcmp(confirm, "yes") == 0 || strcmp(confirm, "YES") == 0) {
        if (datastore_remove_category(store, id)) {
            printf("\n✓ Category deleted successfully!\n");
        } else {
            printf("\n✗ Failed to delete category.\n");
        }
    } else {
        printf("\nDeletion cancelled.\n");
    }
    
    pause_screen();
}

void list_categories(DataStore* store) {
    clear_screen();
    printf("\n╔════════════════════════════════════════════════════════════╗\n");
    printf("║                       ALL CATEGORIES                       ║\n");
    printf("╚════════════════════════════════════════════════════════════╝\n\n");
    
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
    
    printf("  └────────┴────────────────────────────────────────────────┴───────────┘\n\n");
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
        printf("\n╔════════════════════════════════════════════════════════════╗\n");
        printf("║                    SUBGROUP MANAGEMENT                     ║\n");
        printf("╚════════════════════════════════════════════════════════════╝\n\n");
        printf("  [1] Add Subgroup\n");
        printf("  [2] Edit Subgroup\n");
        printf("  [3] Delete Subgroup\n");
        printf("  [4] List All Subgroups\n");
        printf("  [0] Back to Main Menu\n");
        printf("\n════════════════════════════════════════════════════════════\n");
        
        if (!safe_input_int("Enter your choice: ", &choice)) {
            printf("Invalid input. Please try again.\n");
            pause_screen();
            continue;
        }
        
        switch (choice) {
            case 1: add_subgroup(store); break;
            case 2: edit_subgroup(store); break;
            case 3: delete_subgroup(store); break;
            case 4: list_subgroups(store); break;
            case 0: back = true; break;
            default:
                printf("\nInvalid choice.\n");
                pause_screen();
        }
    }
}

void add_subgroup(DataStore* store) {
    clear_screen();
    printf("\n╔════════════════════════════════════════════════════════════╗\n");
    printf("║                     ADD NEW SUBGROUP                       ║\n");
    printf("╚════════════════════════════════════════════════════════════╝\n\n");
    
    if (store->category_count == 0) {
        printf("No categories available. Please create a category first.\n");
        pause_screen();
        return;
    }
    
    // Show categories
    printf("Available Categories:\n");
    category_display_table_header();
    for (int i = 0; i < store->category_count; i++) {
        category_display_table_row(&store->categories[i]);
    }
    printf("  └────────┴────────────────────────────────────────────────┴───────────┘\n\n");
    
    int category_id;
    if (!safe_input_int("Select Category ID: ", &category_id)) {
        printf("Invalid input.\n");
        pause_screen();
        return;
    }
    
    Category* category = datastore_find_category_by_id(store, category_id);
    if (!category) {
        printf("\n✗ Category ID %d not found.\n", category_id);
        pause_screen();
        return;
    }
    
    char name[50], description[200];
    
    if (!safe_input_string("Subgroup Name: ", name, sizeof(name))) {
        printf("Error reading subgroup name.\n");
        pause_screen();
        return;
    }
    
    if (strlen(name) == 0) {
        printf("Error: Subgroup name cannot be empty.\n");
        pause_screen();
        return;
    }
    
    if (!safe_input_string("Description: ", description, sizeof(description))) {
        printf("Error reading description.\n");
        pause_screen();
        return;
    }
    
    Subgroup subgroup = subgroup_create(store->next_subgroup_id, category_id, name, description);
    
    if (category_add_subgroup(category, subgroup)) {
        store->next_subgroup_id++;
        store->is_modified = true;
        printf("\n✓ Subgroup added successfully! (ID: %d)\n", subgroup.id);
    } else {
        printf("\n✗ Failed to add subgroup.\n");
        subgroup_free(&subgroup);
    }
    
    pause_screen();
}

void edit_subgroup(DataStore* store) {
    clear_screen();
    printf("\n╔════════════════════════════════════════════════════════════╗\n");
    printf("║                     EDIT SUBGROUP                          ║\n");
    printf("╚════════════════════════════════════════════════════════════╝\n\n");
    
    int id;
    if (!safe_input_int("Enter Subgroup ID to edit: ", &id)) {
        printf("Invalid input.\n");
        pause_screen();
        return;
    }
    
    Subgroup* subgroup = datastore_find_subgroup_by_id(store, id);
    if (!subgroup) {
        printf("\n✗ Subgroup ID %d not found.\n", id);
        pause_screen();
        return;
    }
    
    printf("\nCurrent Subgroup Information:\n");
    printf("  ID: %d\n", subgroup->id);
    printf("  Category ID: %d\n", subgroup->category_id);
    printf("  Name: %s\n", subgroup->name);
    printf("  Description: %s\n\n", subgroup->description);
    
    char name[50], description[200];
    
    printf("Enter new values (press Enter to keep current):\n\n");
    
    if (safe_input_string("New Name: ", name, sizeof(name)) && strlen(name) > 0) {
        subgroup_update_name(subgroup, name);
    }
    
    if (safe_input_string("New Description: ", description, sizeof(description))) {
        subgroup_update_description(subgroup, description);
    }
    
    store->is_modified = true;
    printf("\n✓ Subgroup updated successfully!\n");
    pause_screen();
}

void delete_subgroup(DataStore* store) {
    clear_screen();
    printf("\n╔════════════════════════════════════════════════════════════╗\n");
    printf("║                      DELETE SUBGROUP                       ║\n");
    printf("╚════════════════════════════════════════════════════════════╝\n\n");
    
    int id;
    if (!safe_input_int("Enter Subgroup ID to delete: ", &id)) {
        printf("Invalid input.\n");
        pause_screen();
        return;
    }
    
    Subgroup* subgroup = datastore_find_subgroup_by_id(store, id);
    if (!subgroup) {
        printf("\n✗ Subgroup ID %d not found.\n", id);
        pause_screen();
        return;
    }
    
    Category* category = datastore_find_category_by_id(store, subgroup->category_id);
    
    printf("\nSubgroup to delete:\n");
    printf("  ID: %d\n", subgroup->id);
    printf("  Name: %s\n", subgroup->name);
    printf("  Products: %d\n\n", subgroup->product_count);
    
    if (subgroup->product_count > 0) {
        printf("WARNING: This subgroup contains %d product(s).\n", subgroup->product_count);
        printf("All products will be deleted!\n\n");
    }
    
    char confirm[10];
    if (!safe_input_string("Are you sure? (yes/no): ", confirm, sizeof(confirm))) {
        printf("Deletion cancelled.\n");
        pause_screen();
        return;
    }
    
    if (strcmp(confirm, "yes") == 0 || strcmp(confirm, "YES") == 0) {
        if (category_remove_subgroup(category, id)) {
            store->is_modified = true;
            printf("\n✓ Subgroup deleted successfully!\n");
        } else {
            printf("\n✗ Failed to delete subgroup.\n");
        }
    } else {
        printf("\nDeletion cancelled.\n");
    }
    
    pause_screen();
}

void list_subgroups(DataStore* store) {
    clear_screen();
    printf("\n╔════════════════════════════════════════════════════════════╗\n");
    printf("║                       ALL SUBGROUPS                        ║\n");
    printf("╚════════════════════════════════════════════════════════════╝\n\n");
    
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
    
    printf("  └────────┴──────────────┴──────────────────────────────────────┴──────────┘\n\n");
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
        printf("\n╔════════════════════════════════════════════════════════════╗\n");
        printf("║                       PRODUCT MANAGEMENT                    ║\n");
        printf("╚════════════════════════════════════════════════════════════╝\n\n");
        printf("  [1] Add Product\n");
        printf("  [2] Edit Product\n");
        printf("  [3] Delete Product\n");
        printf("  [4] List All Products\n");
        printf("  [0] Back to Main Menu\n");
        printf("\n════════════════════════════════════════════════════════════\n");
        
        if (!safe_input_int("Enter your choice: ", &choice)) {
            printf("Invalid input. Please try again.\n");
            pause_screen();
            continue;
        }
        
        switch (choice) {
            case 1: add_product(store); break;
            case 2: edit_product(store); break;
            case 3: delete_product(store); break;
            case 4: list_products(store); break;
            case 0: back = true; break;
            default:
                printf("\nInvalid choice.\n");
                pause_screen();
        }
    }
}

void add_product(DataStore* store) {
    clear_screen();
    printf("\n╔════════════════════════════════════════════════════════════╗\n");
    printf("║                       ADD NEW PRODUCT                       ║\n");
    printf("╚════════════════════════════════════════════════════════════╝\n\n");
    
    // Show subgroups
    int total_subgroups = 0;
    for (int i = 0; i < store->category_count; i++) {
        total_subgroups += store->categories[i].subgroup_count;
    }
    
    if (total_subgroups == 0) {
        printf("No subgroups available. Please create a subgroup first.\n");
        pause_screen();
        return;
    }
    
    printf("Available Subgroups:\n");
    subgroup_display_table_header();
    for (int i = 0; i < store->category_count; i++) {
        for (int j = 0; j < store->categories[i].subgroup_count; j++) {
            subgroup_display_table_row(&store->categories[i].subgroups[j]);
        }
    }
    printf("  └────────┴──────────────┴──────────────────────────────────────┴──────────┘\n\n");
    
    int subgroup_id;
    if (!safe_input_int("Select Subgroup ID: ", &subgroup_id)) {
        printf("Invalid input.\n");
        pause_screen();
        return;
    }
    
    Subgroup* subgroup = datastore_find_subgroup_by_id(store, subgroup_id);
    if (!subgroup) {
        printf("\n✗ Subgroup ID %d not found.\n", subgroup_id);
        pause_screen();
        return;
    }
    
    char code[20], name[100], description[200], timestamp[20];
    float price;
    int quantity;
    
    if (!safe_input_string("Product Code: ", code, sizeof(code)) || strlen(code) == 0) {
        printf("Error: Product code cannot be empty.\n");
        pause_screen();
        return;
    }
    
    if (!safe_input_string("Product Name: ", name, sizeof(name)) || strlen(name) == 0) {
        printf("Error: Product name cannot be empty.\n");
        pause_screen();
        return;
    }
    
    if (!safe_input_string("Description: ", description, sizeof(description))) {
        description[0] = '\0';
    }
    
    if (!safe_input_float("Price: ", &price) || price < 0) {
        printf("Error: Invalid price.\n");
        pause_screen();
        return;
    }
    
    if (!safe_input_int("Quantity: ", &quantity) || quantity < 0) {
        printf("Error: Invalid quantity.\n");
        pause_screen();
        return;
    }
    
    get_current_timestamp(timestamp, sizeof(timestamp));
    
    Product product = product_create(store->next_product_id, subgroup_id, code, name, description, price, quantity);
    
    if (subgroup_add_product(subgroup, product)) {
        store->next_product_id++;
        store->is_modified = true;
        printf("\n✓ Product added successfully! (ID: %d)\n", product.id);
    } else {
        printf("\n✗ Failed to add product.\n");
    }
    
    pause_screen();
}

void edit_product(DataStore* store) {
    clear_screen();
    printf("\n╔════════════════════════════════════════════════════════════╗\n");
    printf("║                       EDIT PRODUCT                          ║\n");
    printf("╚════════════════════════════════════════════════════════════╝\n\n");
    
    int id;
    if (!safe_input_int("Enter Product ID to edit: ", &id)) {
        printf("Invalid input.\n");
        pause_screen();
        return;
    }
    
    Product* product = datastore_find_product_by_id(store, id);
    if (!product) {
        printf("\n✗ Product ID %d not found.\n", id);
        pause_screen();
        return;
    }
    
    printf("\nCurrent Product Information:\n");
    product_display(product);
    
    printf("\nEnter new values (press Enter to keep current):\n\n");
    
    char buffer[200];
    float price;
    int quantity;
    
    if (safe_input_string("New Code: ", buffer, sizeof(buffer)) && strlen(buffer) > 0) {
        product_update_code(product, buffer);
    }
    
    if (safe_input_string("New Name: ", buffer, sizeof(buffer)) && strlen(buffer) > 0) {
        product_update_name(product, buffer);
    }
    
    if (safe_input_string("New Description: ", buffer, sizeof(buffer))) {
        product_update_description(product, buffer);
    }
    
    printf("New Price (current: %.2f): ", product->price);
    if (safe_input_float("", &price) && price >= 0) {
        product_update_price(product, price);
    }
    
    printf("New Quantity (current: %d): ", product->quantity);
    if (safe_input_int("", &quantity) && quantity >= 0) {
        product_update_quantity(product, quantity);
    }
    
    product_update_timestamp(product);
    store->is_modified = true;
    printf("\n✓ Product updated successfully!\n");
    pause_screen();
}

void delete_product(DataStore* store) {
    clear_screen();
    printf("\n╔════════════════════════════════════════════════════════════╗\n");
    printf("║                       DELETE PRODUCT                        ║\n");
    printf("╚════════════════════════════════════════════════════════════╝\n\n");
    
    int id;
    if (!safe_input_int("Enter Product ID to delete: ", &id)) {
        printf("Invalid input.\n");
        pause_screen();
        return;
    }
    
    Product* product = datastore_find_product_by_id(store, id);
    if (!product) {
        printf("\n✗ Product ID %d not found.\n", id);
        pause_screen();
        return;
    }
    
    printf("\nProduct to delete:\n");
    product_display(product);
    
    char confirm[10];
    if (!safe_input_string("\nAre you sure? (yes/no): ", confirm, sizeof(confirm))) {
        printf("Deletion cancelled.\n");
        pause_screen();
        return;
    }
    
    if (strcmp(confirm, "yes") == 0 || strcmp(confirm, "YES") == 0) {
        Subgroup* subgroup = datastore_find_subgroup_by_id(store, product->subgroup_id);
        if (subgroup && subgroup_remove_product(subgroup, id)) {
            store->is_modified = true;
            printf("\n✓ Product deleted successfully!\n");
        } else {
            printf("\n✗ Failed to delete product.\n");
        }
    } else {
        printf("\nDeletion cancelled.\n");
    }
    
    pause_screen();
}

void list_products(DataStore* store) {
    clear_screen();
    printf("\n╔════════════════════════════════════════════════════════════╗\n");
    printf("║                       ALL PRODUCTS                          ║\n");
    printf("╚════════════════════════════════════════════════════════════╝\n\n");
    
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
    
    printf("  ────────────────────────────────────────────────────────────────────────────────\n\n");
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
        printf("\n╔════════════════════════════════════════════════════════════╗\n");
        printf("║                       SEARCH & FILTER                       ║\n");
        printf("╚════════════════════════════════════════════════════════════╝\n\n");
        printf("  [1] Search by Name\n");
        printf("  [2] Search by Price Range\n");
        printf("  [3] Search by Quantity Range\n");
        printf("  [0] Back to Main Menu\n");
        printf("\n════════════════════════════════════════════════════════════\n");
        
        if (!safe_input_int("Enter your choice: ", &choice)) {
            printf("Invalid input. Please try again.\n");
            pause_screen();
            continue;
        }
        
        switch (choice) {
            case 1: search_by_name(store); break;
            case 2: search_by_price(store); break;
            case 3: search_by_quantity(store); break;
            case 0: back = true; break;
            default:
                printf("\nInvalid choice.\n");
                pause_screen();
        }
    }
}

void search_by_name(DataStore* store) {
    clear_screen();
    printf("\n╔════════════════════════════════════════════════════════════╗\n");
    printf("║                       SEARCH BY NAME                       ║\n");
    printf("╚════════════════════════════════════════════════════════════╝\n\n");
    
    char name[100];
    if (!safe_input_string("Enter product name (partial match): ", name, sizeof(name))) {
        printf("Invalid input.\n");
        pause_screen();
        return;
    }
    
    SearchResult result = datastore_search_products_by_name(store, name);
    
    printf("\nSearch Results: %d product(s) found\n\n", result.count);
    
    if (result.count > 0) {
        product_display_table_header();
        for (int i = 0; i < result.count; i++) {
            product_display_table_row(&result.products[i]);
        }
        printf("  ────────────────────────────────────────────────────────────────────────────────\n");
    }
    
    search_result_free(&result);
    pause_screen();
}

void search_by_price(DataStore* store) {
    clear_screen();
    printf("\n╔════════════════════════════════════════════════════════════╗\n");
    printf("║                       SEARCH BY PRICE RANGE                ║\n");
    printf("╚════════════════════════════════════════════════════════════╝\n\n");
    
    float min_price, max_price;
    
    if (!safe_input_float("Minimum Price: ", &min_price) || min_price < 0) {
        printf("Invalid input.\n");
        pause_screen();
        return;
    }
    
    if (!safe_input_float("Maximum Price: ", &max_price) || max_price < min_price) {
        printf("Invalid input. Maximum must be >= minimum.\n");
        pause_screen();
        return;
    }
    
    SearchResult result = datastore_search_products_by_price(store, min_price, max_price);
    
    printf("\nSearch Results: %d product(s) found\n\n", result.count);
    
    if (result.count > 0) {
        product_display_table_header();
        for (int i = 0; i < result.count; i++) {
            product_display_table_row(&result.products[i]);
        }
        printf("  ────────────────────────────────────────────────────────────────────────────────\n");
    }
    
    search_result_free(&result);
    pause_screen();
}

void search_by_quantity(DataStore* store) {
    clear_screen();
    printf("\n╔════════════════════════════════════════════════════════════╗\n");
    printf("║                       SEARCH BY QUANTITY RANGE            ║\n");
    printf("╚════════════════════════════════════════════════════════════╝\n\n");
    
    int min_qty, max_qty;
    
    if (!safe_input_int("Minimum Quantity: ", &min_qty) || min_qty < 0) {
        printf("Invalid input.\n");
        pause_screen();
        return;
    }
    
    if (!safe_input_int("Maximum Quantity: ", &max_qty) || max_qty < min_qty) {
        printf("Invalid input. Maximum must be >= minimum.\n");
        pause_screen();
        return;
    }
    
    SearchResult result = datastore_search_products_by_quantity(store, min_qty, max_qty);
    
    printf("\nSearch Results: %d product(s) found\n\n", result.count);
    
    if (result.count > 0) {
        product_display_table_header();
        for (int i = 0; i < result.count; i++) {
            product_display_table_row(&result.products[i]);
        }
        printf("  ────────────────────────────────────────────────────────────────────────────────\n");
    }
    
    search_result_free(&result);
    pause_screen();
}

// ============================================================================
// Statistics & Reports
// ============================================================================

void statistics_menu(DataStore* store) {
    clear_screen();
    printf("\n╔════════════════════════════════════════════════════════════╗\n");
    printf("║                       STATISTICS & REPORTS                ║\n");
    printf("╚════════════════════════════════════════════════════════════╝\n\n");
    
    Statistics stats = datastore_get_statistics(store);
    
    printf("  ┌──────────────────────────────────────────────────────┐\n");
    printf("  │ SYSTEM STATISTICS                                    │\n");
    printf("  ├──────────────────────────────────────────────────────┤\n");
    printf("  │ Total Categories:    %-10d                     │\n", stats.total_categories);
    printf("  │ Total Subgroups:     %-10d                     │\n", stats.total_subgroups);
    printf("  │ Total Products:      %-10d                     │\n", stats.total_products);
    printf("  │ Total Inventory:     %-10d units               │\n", stats.total_quantity);
    printf("  │ Total Value:         $%-10.2f                  │\n", stats.total_value);
    printf("  │ Average Price:       $%-10.2f                  │\n", stats.average_price);
    printf("  └──────────────────────────────────────────────────────┘\n\n");
    
    pause_screen();
}