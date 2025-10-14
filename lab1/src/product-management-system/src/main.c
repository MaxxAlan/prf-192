/**
 * @file main.c
 * @brief Main program entry point and menu system
 * @details Integrates all modules and provides user interface
 * @author PMS Team - Member 5
 * @date 14/10/2025
 * 
 * @mainpage Product Management System (PMS)
 * 
 * @section intro_sec Introduction
 * PMS is a comprehensive product management system with hierarchical organization:
 * Category → Subgroup → Product
 * 
 * @section features_sec Features
 * - CRUD operations for products, subgroups, and categories
 * - Binary file storage for data persistence
 * - Input validation and error handling
 * - Modular architecture for team development
 * 
 * @section usage_sec Usage
 * Compile: gcc src/*.c -Iinclude -o build/ProductManagement
 * Run: ./build/ProductManagement
 */

#include "utils.h"
#include <stdio.h>
#include <stdlib.h>

/* ========================================================================
 * GLOBAL DATA
 * ======================================================================== */

DataStore g_store;

/* ========================================================================
 * FUNCTION PROTOTYPES - MENU SYSTEMS
 * ======================================================================== */

void displayMainMenu(void);
void displayCategoryMenu(void);
void displaySubgroupMenu(Category* category);
void displayProductMenu(Subgroup* subgroup);

void handleCategoryOperations(void);
void handleSubgroupOperations(void);
void handleProductOperations(void);
void handleSearch(void);
void handleStatistics(void);

/* ========================================================================
 * FUNCTION PROTOTYPES - OPERATIONS
 * ======================================================================== */

void addCategory(void);
void viewCategories(void);
void removeCategory(void);

void addSubgroup(Category* category);
void viewSubgroups(Category* category);
void removeSubgroup(Category* category);

void addProduct(Subgroup* subgroup);
void viewProducts(Subgroup* subgroup);
void updateProduct(Subgroup* subgroup);
void removeProduct(Subgroup* subgroup);

void searchProductById(void);
void searchProductByName(void);

void showSystemStatistics(void);
void showCategoryStatistics(void);

/* ========================================================================
 * MAIN FUNCTION
 * ======================================================================== */

/**
 * @brief Program entry point
 */
int main(void) {
    // Load data from file
    printf("================================================================================\n");
    printf("              PRODUCT MANAGEMENT SYSTEM - INITIALIZING                         \n");
    printf("================================================================================\n\n");
    
    if (!Utils_loadData(&g_store)) {
        fprintf(stderr, "Warning: Could not load data. Starting with empty system.\n");
        g_store = Utils_createDataStore();
    }
    
    printf("\nSystem ready!\n");
    Utils_pause();
    
    // Main menu loop
    int choice;
    do {
        displayMainMenu();
        choice = Utils_getValidInt("Enter your choice: ", 0, 6);
        
        switch (choice) {
            case 1:
                handleCategoryOperations();
                break;
            case 2:
                handleSubgroupOperations();
                break;
            case 3:
                handleProductOperations();
                break;
            case 4:
                handleSearch();
                break;
            case 5:
                handleStatistics();
                break;
            case 6:
                DataStore_printSummary(&g_store);
                Utils_pause();
                break;
            case 0:
                printf("\nSaving data...\n");
                if (Utils_saveData(&g_store)) {
                    printf("Data saved successfully!\n");
                } else {
                    fprintf(stderr, "Warning: Failed to save data!\n");
                }
                printf("Thank you for using Product Management System!\n");
                break;
        }
    } while (choice != 0);
    
    // Cleanup
    Utils_freeDataStore(&g_store);
    
    return 0;
}

/* ========================================================================
 * MENU DISPLAY FUNCTIONS
 * ======================================================================== */

/**
 * @brief Displays main menu
 */
void displayMainMenu(void) {
    printf("\n");
    printf("================================================================================\n");
    printf("                         PRODUCT MANAGEMENT SYSTEM                              \n");
    printf("                                MAIN MENU                                       \n");
    printf("================================================================================\n");
    printf("  1. Category Management\n");
    printf("  2. Subgroup Management\n");
    printf("  3. Product Management\n");
    printf("  4. Search\n");
    printf("  5. Statistics & Reports\n");
    printf("  6. View All Data\n");
    printf("  0. Save & Exit\n");
    printf("================================================================================\n");
}

/**
 * @brief Displays category menu
 */
void displayCategoryMenu(void) {
    printf("\n");
    printf("========================================\n");
    printf("       CATEGORY MANAGEMENT              \n");
    printf("========================================\n");
    printf("  1. Add Category\n");
    printf("  2. View Categories\n");
    printf("  3. Remove Category\n");
    printf("  0. Back to Main Menu\n");
    printf("========================================\n");
}

/**
 * @brief Displays subgroup menu
 */
void displaySubgroupMenu(Category* category) {
    printf("\n");
    printf("========================================\n");
    printf("       SUBGROUP MANAGEMENT              \n");
    printf("       Category: %s\n", category->name);
    printf("========================================\n");
    printf("  1. Add Subgroup\n");
    printf("  2. View Subgroups\n");
    printf("  3. Remove Subgroup\n");
    printf("  0. Back\n");
    printf("========================================\n");
}

/**
 * @brief Displays product menu
 */
void displayProductMenu(Subgroup* subgroup) {
    printf("\n");
    printf("========================================\n");
    printf("       PRODUCT MANAGEMENT               \n");
    printf("       Subgroup: %s\n", subgroup->name);
    printf("========================================\n");
    printf("  1. Add Product\n");
    printf("  2. View Products\n");
    printf("  3. Update Product\n");
    printf("  4. Remove Product\n");
    printf("  0. Back\n");
    printf("========================================\n");
}

/* ========================================================================
 * HANDLER FUNCTIONS
 * ======================================================================== */

/**
 * @brief Handles category operations submenu
 */
void handleCategoryOperations(void) {
    int choice;
    do {
        displayCategoryMenu();
        choice = Utils_getValidInt("Enter your choice: ", 0, 3);
        
        switch (choice) {
            case 1:
                addCategory();
                break;
            case 2:
                viewCategories();
                break;
            case 3:
                removeCategory();
                break;
        }
    } while (choice != 0);
}

/**
 * @brief Handles subgroup operations submenu
 */
void handleSubgroupOperations(void) {
    if (g_store.category_count == 0) {
        printf("\nNo categories available. Please create a category first.\n");
        Utils_pause();
        return;
    }
    
    char cat_name[MAX_CATEGORY_NAME_LENGTH];
    Utils_getValidString("Enter category name: ", cat_name, MAX_CATEGORY_NAME_LENGTH);
    
    Category* category = DataStore_findCategory(&g_store, cat_name);
    if (category == NULL) {
        printf("Category not found!\n");
        Utils_pause();
        return;
    }
    
    int choice;
    do {
        displaySubgroupMenu(category);
        choice = Utils_getValidInt("Enter your choice: ", 0, 3);
        
        switch (choice) {
            case 1:
                addSubgroup(category);
                break;
            case 2:
                viewSubgroups(category);
                break;
            case 3:
                removeSubgroup(category);
                break;
        }
    } while (choice != 0);
}

/**
 * @brief Handles product operations submenu
 */
void handleProductOperations(void) {
    if (g_store.category_count == 0) {
        printf("\nNo categories available. Please create a category first.\n");
        Utils_pause();
        return;
    }
    
    char cat_name[MAX_CATEGORY_NAME_LENGTH];
    Utils_getValidString("Enter category name: ", cat_name, MAX_CATEGORY_NAME_LENGTH);
    
    Category* category = DataStore_findCategory(&g_store, cat_name);
    if (category == NULL) {
        printf("Category not found!\n");
        Utils_pause();
        return;
    }
    
    if (category->subgroup_count == 0) {
        printf("\nNo subgroups available. Please create a subgroup first.\n");
        Utils_pause();
        return;
    }
    
    char sub_name[MAX_SUBGROUP_NAME_LENGTH];
    Utils_getValidString("Enter subgroup name: ", sub_name, MAX_SUBGROUP_NAME_LENGTH);
    
    Subgroup* subgroup = Category_findSubgroupByName(category, sub_name);
    if (subgroup == NULL) {
        printf("Subgroup not found!\n");
        Utils_pause();
        return;
    }
    
    int choice;
    do {
        displayProductMenu(subgroup);
        choice = Utils_getValidInt("Enter your choice: ", 0, 4);
        
        switch (choice) {
            case 1:
                addProduct(subgroup);
                break;
            case 2:
                viewProducts(subgroup);
                break;
            case 3:
                updateProduct(subgroup);
                break;
            case 4:
                removeProduct(subgroup);
                break;
        }
    } while (choice != 0);
}

/**
 * @brief Handles search submenu
 */
void handleSearch(void) {
    printf("\n");
    printf("========================================\n");
    printf("       SEARCH                           \n");
    printf("========================================\n");
    printf("  1. Search by Product ID (SKU)\n");
    printf("  2. Search by Product Name\n");
    printf("  0. Back\n");
    printf("========================================\n");
    
    int choice = Utils_getValidInt("Enter your choice: ", 0, 2);
    
    switch (choice) {
        case 1:
            searchProductById();
            break;
        case 2:
            searchProductByName();
            break;
    }
}

/**
 * @brief Handles statistics submenu
 */
void handleStatistics(void) {
    printf("\n");
    printf("========================================\n");
    printf("       STATISTICS & REPORTS             \n");
    printf("========================================\n");
    printf("  1. System Statistics\n");
    printf("  2. Category Statistics\n");
    printf("  0. Back\n");
    printf("========================================\n");
    
    int choice = Utils_getValidInt("Enter your choice: ", 0, 2);
    
    switch (choice) {
        case 1:
            showSystemStatistics();
            break;
        case 2:
            showCategoryStatistics();
            break;
    }
}

/* ========================================================================
 * CATEGORY OPERATIONS
 * ======================================================================== */

/**
 * @brief Adds a new category
 */
void addCategory(void) {
    printf("\n--- Add New Category ---\n");
    
    char name[MAX_CATEGORY_NAME_LENGTH];
    Utils_getValidString("Enter category name: ", name, MAX_CATEGORY_NAME_LENGTH);
    
    Category category = Category_create(name);
    
    if (DataStore_addCategory(&g_store, category)) {
        printf("✓ Category '%s' added successfully!\n", name);
    } else {
        printf("✗ Failed to add category!\n");
    }
    
    Utils_pause();
}

/**
 * @brief Views all categories
 */
void viewCategories(void) {
    printf("\n--- View Categories ---\n");
    
    if (g_store.category_count == 0) {
        printf("No categories available.\n");
    } else {
        DataStore_printSummary(&g_store);
    }
    
    Utils_pause();
}

/**
 * @brief Removes a category
 */
void removeCategory(void) {
    printf("\n--- Remove Category ---\n");
    
    if (g_store.category_count == 0) {
        printf("No categories available.\n");
        Utils_pause();
        return;
    }
    
    viewCategories();
    
    char name[MAX_CATEGORY_NAME_LENGTH];
    Utils_getValidString("Enter category name to remove: ", name, MAX_CATEGORY_NAME_LENGTH);
    
    printf("Are you sure? This will delete all subgroups and products! (y/n): ");
    char confirm;
    scanf(" %c", &confirm);
    Utils_clearInputBuffer();
    
    if (confirm == 'y' || confirm == 'Y') {
        if (DataStore_removeCategory(&g_store, name)) {
            printf("✓ Category '%s' removed successfully!\n", name);
        } else {
            printf("✗ Failed to remove category!\n");
        }
    } else {
        printf("Operation cancelled.\n");
    }
    
    Utils_pause();
}

/* ========================================================================
 * SUBGROUP OPERATIONS
 * ======================================================================== */

/**
 * @brief Adds a new subgroup to a category
 */
void addSubgroup(Category* category) {
    printf("\n--- Add New Subgroup ---\n");
    
    char name[MAX_SUBGROUP_NAME_LENGTH];
    Utils_getValidString("Enter subgroup name: ", name, MAX_SUBGROUP_NAME_LENGTH);
    
    Subgroup subgroup = Subgroup_create(name);
    
    if (Category_addSubgroup(category, subgroup)) {
        printf("✓ Subgroup '%s' added successfully to category '%s'!\n", 
               name, category->name);
    } else {
        printf("✗ Failed to add subgroup!\n");
    }
    
    Utils_pause();
}

/**
 * @brief Views all subgroups in a category
 */
void viewSubgroups(Category* category) {
    printf("\n--- View Subgroups ---\n");
    
    if (category->subgroup_count == 0) {
        printf("No subgroups in category '%s'.\n", category->name);
    } else {
        Category_print(category);
    }
    
    Utils_pause();
}

/**
 * @brief Removes a subgroup from a category
 */
void removeSubgroup(Category* category) {
    printf("\n--- Remove Subgroup ---\n");
    
    if (category->subgroup_count == 0) {
        printf("No subgroups available.\n");
        Utils_pause();
        return;
    }
    
    viewSubgroups(category);
    
    char name[MAX_SUBGROUP_NAME_LENGTH];
    Utils_getValidString("Enter subgroup name to remove: ", name, MAX_SUBGROUP_NAME_LENGTH);
    
    printf("Are you sure? This will delete all products! (y/n): ");
    char confirm;
    scanf(" %c", &confirm);
    Utils_clearInputBuffer();
    
    if (confirm == 'y' || confirm == 'Y') {
        if (Category_removeSubgroup(category, name)) {
            printf("✓ Subgroup '%s' removed successfully!\n", name);
        } else {
            printf("✗ Failed to remove subgroup!\n");
        }
    } else {
        printf("Operation cancelled.\n");
    }
    
    Utils_pause();
}

/* ========================================================================
 * PRODUCT OPERATIONS
 * ======================================================================== */

/**
 * @brief Adds a new product to a subgroup
 */
void addProduct(Subgroup* subgroup) {
    printf("\n--- Add New Product ---\n");
    
    int id = Utils_getValidInt("Enter product SKU (ID): ", 1, 999999);
    
    if (Subgroup_productExists(subgroup, id)) {
        printf("✗ Product with ID %d already exists!\n", id);
        Utils_pause();
        return;
    }
    
    char name[MAX_NAME_LENGTH];
    Utils_getValidString("Enter product name: ", name, MAX_NAME_LENGTH);
    
    double price = Utils_getValidDouble("Enter price: $", 0.0, 999999.99);
    
    int quantity = Utils_getValidInt("Enter quantity: ", 0, 999999);
    
    char description[MAX_DESCRIPTION_LENGTH];
    Utils_getValidString("Enter description: ", description, MAX_DESCRIPTION_LENGTH);
    
    Product product = Product_create(id, name, price, quantity, description);
    
    if (Subgroup_addProduct(subgroup, product)) {
        printf("✓ Product '%s' (SKU: %d) added successfully!\n", name, id);
    } else {
        printf("✗ Failed to add product!\n");
    }
    
    Utils_pause();
}

/**
 * @brief Views all products in a subgroup
 */
void viewProducts(Subgroup* subgroup) {
    printf("\n--- View Products ---\n");
    
    if (subgroup->product_count == 0) {
        printf("No products in subgroup '%s'.\n", subgroup->name);
    } else {
        Subgroup_print(subgroup);
    }
    
    Utils_pause();
}

/**
 * @brief Updates an existing product
 */
void updateProduct(Subgroup* subgroup) {
    printf("\n--- Update Product ---\n");
    
    if (subgroup->product_count == 0) {
        printf("No products available.\n");
        Utils_pause();
        return;
    }
    
    viewProducts(subgroup);
    
    int id = Utils_getValidInt("Enter product SKU (ID) to update: ", 1, 999999);
    
    Product* product = Subgroup_findProductById(subgroup, id);
    if (product == NULL) {
        printf("✗ Product not found!\n");
        Utils_pause();
        return;
    }
    
    printf("\nCurrent product details:\n");
    Product_print(product);
    
    printf("\nWhat would you like to update?\n");
    printf("  1. Name\n");
    printf("  2. Price\n");
    printf("  3. Quantity\n");
    printf("  4. Description\n");
    printf("  5. All fields\n");
    printf("  0. Cancel\n");
    
    int choice = Utils_getValidInt("Enter your choice: ", 0, 5);
    
    switch (choice) {
        case 1: {
            char new_name[MAX_NAME_LENGTH];
            Utils_getValidString("Enter new name: ", new_name, MAX_NAME_LENGTH);
            Product_updateName(product, new_name);
            printf("✓ Name updated!\n");
            break;
        }
        case 2: {
            double new_price = Utils_getValidDouble("Enter new price: $", 0.0, 999999.99);
            if (Product_updatePrice(product, new_price)) {
                printf("✓ Price updated!\n");
            }
            break;
        }
        case 3: {
            int new_quantity = Utils_getValidInt("Enter new quantity: ", 0, 999999);
            if (Product_updateQuantity(product, new_quantity)) {
                printf("✓ Quantity updated!\n");
            }
            break;
        }
        case 4: {
            char new_desc[MAX_DESCRIPTION_LENGTH];
            Utils_getValidString("Enter new description: ", new_desc, MAX_DESCRIPTION_LENGTH);
            Product_updateDescription(product, new_desc);
            printf("✓ Description updated!\n");
            break;
        }
        case 5: {
            char new_name[MAX_NAME_LENGTH];
            Utils_getValidString("Enter new name: ", new_name, MAX_NAME_LENGTH);
            Product_updateName(product, new_name);
            
            double new_price = Utils_getValidDouble("Enter new price: $", 0.0, 999999.99);
            Product_updatePrice(product, new_price);
            
            int new_quantity = Utils_getValidInt("Enter new quantity: ", 0, 999999);
            Product_updateQuantity(product, new_quantity);
            
            char new_desc[MAX_DESCRIPTION_LENGTH];
            Utils_getValidString("Enter new description: ", new_desc, MAX_DESCRIPTION_LENGTH);
            Product_updateDescription(product, new_desc);
            
            printf("✓ All fields updated!\n");
            break;
        }
        case 0:
            printf("Update cancelled.\n");
            break;
    }
    
    Utils_pause();
}

/**
 * @brief Removes a product from a subgroup
 */
void removeProduct(Subgroup* subgroup) {
    printf("\n--- Remove Product ---\n");
    
    if (subgroup->product_count == 0) {
        printf("No products available.\n");
        Utils_pause();
        return;
    }
    
    viewProducts(subgroup);
    
    int id = Utils_getValidInt("Enter product SKU (ID) to remove: ", 1, 999999);
    
    printf("Are you sure you want to remove this product? (y/n): ");
    char confirm;
    scanf(" %c", &confirm);
    Utils_clearInputBuffer();
    
    if (confirm == 'y' || confirm == 'Y') {
        if (Subgroup_removeProduct(subgroup, id)) {
            printf("✓ Product removed successfully!\n");
        } else {
            printf("✗ Failed to remove product!\n");
        }
    } else {
        printf("Operation cancelled.\n");
    }
    
    Utils_pause();
}

/* ========================================================================
 * SEARCH OPERATIONS
 * ======================================================================== */

/**
 * @brief Searches for a product by ID across all categories
 */
void searchProductById(void) {
    printf("\n--- Search by Product ID ---\n");
    
    int id = Utils_getValidInt("Enter product SKU (ID): ", 1, 999999);
    
    int found = 0;
    
    for (int i = 0; i < g_store.category_count; i++) {
        Category* cat = &g_store.categories[i];
        
        for (int j = 0; j < cat->subgroup_count; j++) {
            Subgroup* sub = &cat->subgroups[j];
            Product* product = Subgroup_findProductById(sub, id);
            
            if (product != NULL) {
                printf("\n✓ Product found!\n");
                printf("Category: %s\n", cat->name);
                printf("Subgroup: %s\n", sub->name);
                Product_print(product);
                found = 1;
                break;
            }
        }
        
        if (found) break;
    }
    
    if (!found) {
        printf("✗ Product with ID %d not found.\n", id);
    }
    
    Utils_pause();
}

/**
 * @brief Searches for products by name (partial match)
 */
void searchProductByName(void) {
    printf("\n--- Search by Product Name ---\n");
    
    char search_term[MAX_NAME_LENGTH];
    Utils_getValidString("Enter product name (or part of it): ", search_term, MAX_NAME_LENGTH);
    
    int found_count = 0;
    
    printf("\nSearch results:\n");
    printf("+----------+--------------------------------+-------------+------------+\n");
    printf("| %-8s | %-30s | %-11s | %-10s |\n", "SKU", "Product Name", "Price", "Quantity");
    printf("+----------+--------------------------------+-------------+------------+\n");
    
    for (int i = 0; i < g_store.category_count; i++) {
        Category* cat = &g_store.categories[i];
        
        for (int j = 0; j < cat->subgroup_count; j++) {
            Subgroup* sub = &cat->subgroups[j];
            
            for (int k = 0; k < sub->product_count; k++) {
                Product* product = &sub->products[k];
                
                // Case-insensitive partial match
                if (strcasestr(product->name, search_term) != NULL) {
                    Product_printTableRow(product);
                    printf("  Location: %s > %s\n", cat->name, sub->name);
                    found_count++;
                }
            }
        }
    }
    
    printf("+----------+--------------------------------+-------------+------------+\n");
    printf("Total results: %d\n", found_count);
    
    if (found_count == 0) {
        printf("✗ No products found matching '%s'.\n", search_term);
    }
    
    Utils_pause();
}

/* ========================================================================
 * STATISTICS OPERATIONS
 * ======================================================================== */

/**
 * @brief Shows system-wide statistics
 */
void showSystemStatistics(void) {
    printf("\n");
    printf("================================================================================\n");
    printf("                         SYSTEM STATISTICS                                      \n");
    printf("================================================================================\n");
    
    int total_products = 0;
    int total_quantity = 0;
    double total_value = 0.0;
    
    for (int i = 0; i < g_store.category_count; i++) {
        Category* cat = &g_store.categories[i];
        total_products += Category_getTotalProductCount(cat);
        total_value += Category_getTotalValue(cat);
        
        for (int j = 0; j < cat->subgroup_count; j++) {
            Subgroup* sub = &cat->subgroups[j];
            for (int k = 0; k < sub->product_count; k++) {
                total_quantity += sub->products[k].quantity;
            }
        }
    }
    
    printf("Total Categories:       %d\n", g_store.category_count);
    printf("Total Products (SKUs):  %d\n", total_products);
    printf("Total Items in Stock:   %d\n", total_quantity);
    printf("Total Inventory Value:  $%.2f\n", total_value);
    printf("================================================================================\n");
    
    if (total_products > 0) {
        printf("\nAverage Product Value:  $%.2f\n", total_value / total_products);
        printf("Average Stock per SKU:  %.1f units\n", (double)total_quantity / total_products);
    }
    
    Utils_pause();
}

/**
 * @brief Shows statistics for a specific category
 */
void showCategoryStatistics(void) {
    printf("\n--- Category Statistics ---\n");
    
    if (g_store.category_count == 0) {
        printf("No categories available.\n");
        Utils_pause();
        return;
    }
    
    char name[MAX_CATEGORY_NAME_LENGTH];
    Utils_getValidString("Enter category name: ", name, MAX_CATEGORY_NAME_LENGTH);
    
    Category* category = DataStore_findCategory(&g_store, name);
    if (category == NULL) {
        printf("✗ Category not found!\n");
        Utils_pause();
        return;
    }
    
    printf("\n");
    printf("================================================================================\n");
    printf("CATEGORY: %s - STATISTICS\n", category->name);
    printf("================================================================================\n");
    printf("Total Subgroups:        %d\n", category->subgroup_count);
    printf("Total Products:         %d\n", Category_getTotalProductCount(category));
    printf("Total Category Value:   $%.2f\n", Category_getTotalValue(category));
    printf("================================================================================\n");
    
    if (category->subgroup_count > 0) {
        printf("\nSubgroup Breakdown:\n");
        printf("%-30s | %-10s | %-15s\n", "Subgroup Name", "Products", "Value");
        printf("--------------------------------------------------------------------------------\n");
        
        for (int i = 0; i < category->subgroup_count; i++) {
            Subgroup* sub = &category->subgroups[i];
            printf("%-30s | %-10d | $%-14.2f\n",
                   sub->name,
                   sub->product_count,
                   Subgroup_getTotalValue(sub));
        }
    }
    
    Utils_pause();
}