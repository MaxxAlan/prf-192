/**
 * @file utils.c
 * @brief Utility functions and DataStore implementation
 * @author PMS Team
 * @date 2025
 * @compatible Dev-C++ 6.3, TDM-GCC 9.2.0, C11
 */

#include "../include/utils.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <ctype.h>

#define INITIAL_CATEGORY_CAPACITY 10
#define DATA_FILE "data/products.dat"
#define BACKUP_FILE "data/products.bak"

// ============================================================================
// Helper Functions
// ============================================================================

void trim_string(char* str) {
    if (!str) return;
    
    // Trim leading whitespace
    char* start = str;
    while (*start && (*start == ' ' || *start == '\t' || *start == '\n' || *start == '\r')) {
        start++;
    }
    
    // Trim trailing whitespace
    char* end = start + strlen(start) - 1;
    while (end > start && (*end == ' ' || *end == '\t' || *end == '\n' || *end == '\r')) {
        end--;
    }
    *(end + 1) = '\0';
    
    // Move trimmed string to beginning
    if (start != str) {
        memmove(str, start, strlen(start) + 1);
    }
}

void get_current_timestamp(char* buffer, size_t size) {
    if (!buffer || size < 20) return;
    
    time_t now = time(NULL);
    struct tm* tm_info = localtime(&now);
    
    strftime(buffer, size, "%Y-%m-%d %H:%M:%S", tm_info);
}

void clear_screen(void) {
    system("cls");
}

void pause_screen(void) {
    printf("\nPress Enter to continue...");
    while (getchar() != '\n');
    getchar();
}

bool safe_input_string(const char* prompt, char* buffer, size_t size) {
    if (!buffer || size == 0) return false;
    
    printf("%s", prompt);
    if (fgets(buffer, size, stdin) == NULL) {
        return false;
    }
    
    // Remove newline
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
    }
    
    trim_string(buffer);
    return true;
}

bool safe_input_int(const char* prompt, int* value) {
    if (!value) return false;
    
    char buffer[100];
    printf("%s", prompt);
    
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        return false;
    }
    
    trim_string(buffer);
    
    char* endptr;
    long result = strtol(buffer, &endptr, 10);
    
    if (*endptr != '\0' || endptr == buffer) {
        fprintf(stderr, "Error: Invalid integer input\n");
        return false;
    }
    
    *value = (int)result;
    return true;
}

bool safe_input_float(const char* prompt, float* value) {
    if (!value) return false;
    
    char buffer[100];
    printf("%s", prompt);
    
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        return false;
    }
    
    trim_string(buffer);
    
    char* endptr;
    float result = strtof(buffer, &endptr);
    
    if (*endptr != '\0' || endptr == buffer) {
        fprintf(stderr, "Error: Invalid float input\n");
        return false;
    }
    
    *value = result;
    return true;
}

// ============================================================================
// DataStore Management
// ============================================================================

DataStore datastore_init(void) {
    DataStore store;
    
    store.category_count = 0;
    store.category_capacity = INITIAL_CATEGORY_CAPACITY;
    store.next_category_id = 1;
    store.next_subgroup_id = 1;
    store.next_product_id = 1;
    store.is_modified = false;
    strcpy(store.last_saved, "Never");
    
    // Allocate initial category array
    store.categories = (Category*)malloc(INITIAL_CATEGORY_CAPACITY * sizeof(Category));
    if (!store.categories) {
        fprintf(stderr, "Error: Failed to allocate memory for categories\n");
        store.category_capacity = 0;
    }
    
    return store;
}

void datastore_free(DataStore* store) {
    if (!store) return;
    
    // Free all categories (which will cascade to subgroups and products)
    if (store->categories) {
        for (int i = 0; i < store->category_count; i++) {
            category_free(&store->categories[i]);
        }
        free(store->categories);
        store->categories = NULL;
    }
    
    store->category_count = 0;
    store->category_capacity = 0;
}

bool datastore_add_category(DataStore* store, Category category) {
    if (!store) {
        fprintf(stderr, "Error: DataStore pointer is NULL\n");
        return false;
    }
    
    if (!category_is_valid(&category)) {
        fprintf(stderr, "Error: Invalid category data\n");
        return false;
    }
    
    // Check if capacity needs to be expanded
    if (store->category_count >= store->category_capacity) {
        int new_capacity = store->category_capacity * 2;
        Category* new_categories = (Category*)realloc(store->categories, new_capacity * sizeof(Category));
        
        if (!new_categories) {
            fprintf(stderr, "Error: Failed to expand category array\n");
            return false;
        }
        
        store->categories = new_categories;
        store->category_capacity = new_capacity;
    }
    
    // Add category
    store->categories[store->category_count] = category;
    store->category_count++;
    store->is_modified = true;
    
    return true;
}

bool datastore_remove_category(DataStore* store, int category_id) {
    if (!store) {
        fprintf(stderr, "Error: DataStore pointer is NULL\n");
        return false;
    }
    
    // Find category index
    int index = -1;
    for (int i = 0; i < store->category_count; i++) {
        if (store->categories[i].id == category_id) {
            index = i;
            break;
        }
    }
    
    if (index == -1) {
        fprintf(stderr, "Error: Category ID %d not found\n", category_id);
        return false;
    }
    
    // Free category resources before removing
    category_free(&store->categories[index]);
    
    // Shift categories left to fill gap
    for (int i = index; i < store->category_count - 1; i++) {
        store->categories[i] = store->categories[i + 1];
    }
    
    store->category_count--;
    store->is_modified = true;
    
    return true;
}

Category* datastore_find_category_by_id(DataStore* store, int category_id) {
    if (!store) return NULL;
    
    for (int i = 0; i < store->category_count; i++) {
        if (store->categories[i].id == category_id) {
            return &store->categories[i];
        }
    }
    
    return NULL;
}

// ============================================================================
// Search Functions
// ============================================================================

SearchResult datastore_search_products_by_name(DataStore* store, const char* name) {
    SearchResult result = {NULL, 0};
    
    if (!store || !name) return result;
    
    char search_lower[100];
    strncpy(search_lower, name, sizeof(search_lower) - 1);
    search_lower[sizeof(search_lower) - 1] = '\0';
    
    // Convert to lowercase for case-insensitive search
    for (int i = 0; search_lower[i]; i++) {
        search_lower[i] = tolower(search_lower[i]);
    }
    
    // Count matching products
    int count = 0;
    for (int i = 0; i < store->category_count; i++) {
        for (int j = 0; j < store->categories[i].subgroup_count; j++) {
            for (int k = 0; k < store->categories[i].subgroups[j].product_count; k++) {
                char product_name_lower[100];
                strncpy(product_name_lower, store->categories[i].subgroups[j].products[k].name, sizeof(product_name_lower) - 1);
                product_name_lower[sizeof(product_name_lower) - 1] = '\0';
                
                for (int l = 0; product_name_lower[l]; l++) {
                    product_name_lower[l] = tolower(product_name_lower[l]);
                }
                
                if (strstr(product_name_lower, search_lower) != NULL) {
                    count++;
                }
            }
        }
    }
    
    if (count == 0) return result;
    
    // Allocate result array
    result.products = (Product*)malloc(count * sizeof(Product));
    if (!result.products) {
        fprintf(stderr, "Error: Failed to allocate memory for search results\n");
        return result;
    }
    
    // Fill result array
    int index = 0;
    for (int i = 0; i < store->category_count; i++) {
        for (int j = 0; j < store->categories[i].subgroup_count; j++) {
            for (int k = 0; k < store->categories[i].subgroups[j].product_count; k++) {
                char product_name_lower[100];
                strncpy(product_name_lower, store->categories[i].subgroups[j].products[k].name, sizeof(product_name_lower) - 1);
                product_name_lower[sizeof(product_name_lower) - 1] = '\0';
                
                for (int l = 0; product_name_lower[l]; l++) {
                    product_name_lower[l] = tolower(product_name_lower[l]);
                }
                
                if (strstr(product_name_lower, search_lower) != NULL) {
                    result.products[index++] = store->categories[i].subgroups[j].products[k];
                }
            }
        }
    }
    
    result.count = count;
    return result;
}

SearchResult datastore_search_products_by_price(DataStore* store, float min_price, float max_price) {
    SearchResult result = {NULL, 0};
    
    if (!store) return result;
    
    // Count matching products
    int count = 0;
    for (int i = 0; i < store->category_count; i++) {
        for (int j = 0; j < store->categories[i].subgroup_count; j++) {
            for (int k = 0; k < store->categories[i].subgroups[j].product_count; k++) {
                float price = store->categories[i].subgroups[j].products[k].price;
                if (price >= min_price && price <= max_price) {
                    count++;
                }
            }
        }
    }
    
    if (count == 0) return result;
    
    // Allocate result array
    result.products = (Product*)malloc(count * sizeof(Product));
    if (!result.products) {
        fprintf(stderr, "Error: Failed to allocate memory for search results\n");
        return result;
    }
    
    // Fill result array
    int index = 0;
    for (int i = 0; i < store->category_count; i++) {
        for (int j = 0; j < store->categories[i].subgroup_count; j++) {
            for (int k = 0; k < store->categories[i].subgroups[j].product_count; k++) {
                float price = store->categories[i].subgroups[j].products[k].price;
                if (price >= min_price && price <= max_price) {
                    result.products[index++] = store->categories[i].subgroups[j].products[k];
                }
            }
        }
    }
    
    result.count = count;
    return result;
}

SearchResult datastore_search_products_by_quantity(DataStore* store, int min_qty, int max_qty) {
    SearchResult result = {NULL, 0};
    
    if (!store) return result;
    
    // Count matching products
    int count = 0;
    for (int i = 0; i < store->category_count; i++) {
        for (int j = 0; j < store->categories[i].subgroup_count; j++) {
            for (int k = 0; k < store->categories[i].subgroups[j].product_count; k++) {
                int qty = store->categories[i].subgroups[j].products[k].quantity;
                if (qty >= min_qty && qty <= max_qty) {
                    count++;
                }
            }
        }
    }
    
    if (count == 0) return result;
    
    // Allocate result array
    result.products = (Product*)malloc(count * sizeof(Product));
    if (!result.products) {
        fprintf(stderr, "Error: Failed to allocate memory for search results\n");
        return result;
    }
    
    // Fill result array
    int index = 0;
    for (int i = 0; i < store->category_count; i++) {
        for (int j = 0; j < store->categories[i].subgroup_count; j++) {
            for (int k = 0; k < store->categories[i].subgroups[j].product_count; k++) {
                int qty = store->categories[i].subgroups[j].products[k].quantity;
                if (qty >= min_qty && qty <= max_qty) {
                    result.products[index++] = store->categories[i].subgroups[j].products[k];
                }
            }
        }
    }
    
    result.count = count;
    return result;
}

void search_result_free(SearchResult* result) {
    if (!result) return;
    
    if (result->products) {
        free(result->products);
        result->products = NULL;
    }
    result->count = 0;
}

// ============================================================================
// Statistics Functions
// ============================================================================

Statistics datastore_get_statistics(DataStore* store) {
    Statistics stats = {0, 0, 0, 0.0f, 0.0f, 0};
    
    if (!store) return stats;
    
    stats.total_categories = store->category_count;
    
    float total_price_sum = 0.0f;
    
    for (int i = 0; i < store->category_count; i++) {
        stats.total_subgroups += store->categories[i].subgroup_count;
        
        for (int j = 0; j < store->categories[i].subgroup_count; j++) {
            stats.total_products += store->categories[i].subgroups[j].product_count;
            
            for (int k = 0; k < store->categories[i].subgroups[j].product_count; k++) {
                Product* p = &store->categories[i].subgroups[j].products[k];
                stats.total_value += p->price * p->quantity;
                stats.total_quantity += p->quantity;
                total_price_sum += p->price;
            }
        }
    }
    
    if (stats.total_products > 0) {
        stats.average_price = total_price_sum / stats.total_products;
    }
    
    return stats;
}

// ============================================================================
// Display Functions
// ============================================================================

void datastore_display_all(DataStore* store) {
    if (!store) {
        printf("Error: DataStore pointer is NULL\n");
        return;
    }
    
    clear_screen();
    printf("\n╔════════════════════════════════════════════════════════════╗\n");
    printf("║  ALL DATA - HIERARCHICAL VIEW                              ║\n");
    printf("╚════════════════════════════════════════════════════════════╝\n\n");
    
    if (store->category_count == 0) {
        printf("  No data available.\n\n");
        return;
    }
    
    printf("  Total Categories: %d\n", store->category_count);
    printf("  Last Saved: %s\n", store->last_saved);
    printf("  Modified: %s\n\n", store->is_modified ? "Yes" : "No");
    
    for (int i = 0; i < store->category_count; i++) {
        category_display(&store->categories[i]);
    }
}

// ============================================================================
// File I/O Functions
// ============================================================================

bool datastore_save(DataStore* store, const char* filename) {
    if (!store || !filename) {
        fprintf(stderr, "Error: Invalid parameters for save\n");
        return false;
    }
    
    // Create backup first
    FILE* original = fopen(filename, "rb");
    if (original) {
        FILE* backup = fopen(BACKUP_FILE, "wb");
        if (backup) {
            char buffer[4096];
            size_t bytes;
            while ((bytes = fread(buffer, 1, sizeof(buffer), original)) > 0) {
                fwrite(buffer, 1, bytes, backup);
            }
            fclose(backup);
        }
        fclose(original);
    }
    
    // Open file for writing
    FILE* file = fopen(filename, "wb");
    if (!file) {
        fprintf(stderr, "Error: Cannot open file %s for writing\n", filename);
        return false;
    }
    
    // Write header info
    fwrite(&store->category_count, sizeof(int), 1, file);
    fwrite(&store->next_category_id, sizeof(int), 1, file);
    fwrite(&store->next_subgroup_id, sizeof(int), 1, file);
    fwrite(&store->next_product_id, sizeof(int), 1, file);
    
    // Write each category
    for (int i = 0; i < store->category_count; i++) {
        Category* cat = &store->categories[i];
        
        // Write category data
        fwrite(&cat->id, sizeof(int), 1, file);
        fwrite(cat->name, sizeof(char), 50, file);
        fwrite(cat->description, sizeof(char), 200, file);
        fwrite(&cat->subgroup_count, sizeof(int), 1, file);
        
        // Write each subgroup
        for (int j = 0; j < cat->subgroup_count; j++) {
            Subgroup* sub = &cat->subgroups[j];
            
            // Write subgroup data
            fwrite(&sub->id, sizeof(int), 1, file);
            fwrite(&sub->category_id, sizeof(int), 1, file);
            fwrite(sub->name, sizeof(char), 50, file);
            fwrite(sub->description, sizeof(char), 200, file);
            fwrite(&sub->product_count, sizeof(int), 1, file);
            
            // Write each product
            for (int k = 0; k < sub->product_count; k++) {
                Product* prod = &sub->products[k];
                fwrite(prod, sizeof(Product), 1, file);
            }
        }
    }
    
    fclose(file);
    
    // Update last saved timestamp
    get_current_timestamp(store->last_saved, sizeof(store->last_saved));
    store->is_modified = false;
    
    printf("Data saved successfully to %s\n", filename);
    return true;
}

bool datastore_load(DataStore* store, const char* filename) {
    if (!store || !filename) {
        fprintf(stderr, "Error: Invalid parameters for load\n");
        return false;
    }
    
    FILE* file = fopen(filename, "rb");
    if (!file) {
        printf("No existing data file found. Starting with empty database.\n");
        return true; // Not an error, just no existing file
    }
    
    // Free existing data
    datastore_free(store);
    
    // Read header info
    if (fread(&store->category_count, sizeof(int), 1, file) != 1) {
        fprintf(stderr, "Error: Failed to read category count\n");
        fclose(file);
        return false;
    }
    
    fread(&store->next_category_id, sizeof(int), 1, file);
    fread(&store->next_subgroup_id, sizeof(int), 1, file);
    fread(&store->next_product_id, sizeof(int), 1, file);
    
    // Allocate category array
    store->category_capacity = store->category_count > INITIAL_CATEGORY_CAPACITY ? 
                               store->category_count : INITIAL_CATEGORY_CAPACITY;
    store->categories = (Category*)malloc(store->category_capacity * sizeof(Category));
    
    if (!store->categories) {
        fprintf(stderr, "Error: Failed to allocate memory for categories\n");
        fclose(file);
        return false;
    }
    
    // Read each category
    for (int i = 0; i < store->category_count; i++) {
        Category* cat = &store->categories[i];
        
        // Read category data
        fread(&cat->id, sizeof(int), 1, file);
        fread(cat->name, sizeof(char), 50, file);
        fread(cat->description, sizeof(char), 200, file);
        fread(&cat->subgroup_count, sizeof(int), 1, file);
        
        // Allocate subgroup array
        cat->subgroup_capacity = cat->subgroup_count > INITIAL_CATEGORY_CAPACITY ? 
                                 cat->subgroup_count : INITIAL_CATEGORY_CAPACITY;
        cat->subgroups = (Subgroup*)malloc(cat->subgroup_capacity * sizeof(Subgroup));
        
        if (!cat->subgroups) {
            fprintf(stderr, "Error: Failed to allocate memory for subgroups\n");
            fclose(file);
            return false;
        }
        
        // Read each subgroup
        for (int j = 0; j < cat->subgroup_count; j++) {
            Subgroup* sub = &cat->subgroups[j];
            
            // Read subgroup data
            fread(&sub->id, sizeof(int), 1, file);
            fread(&sub->category_id, sizeof(int), 1, file);
            fread(sub->name, sizeof(char), 50, file);
            fread(sub->description, sizeof(char), 200, file);
            fread(&sub->product_count, sizeof(int), 1, file);
            
            // Allocate product array
            sub->product_capacity = sub->product_count > INITIAL_CATEGORY_CAPACITY ? 
                                    sub->product_count : INITIAL_CATEGORY_CAPACITY;
            sub->products = (Product*)malloc(sub->product_capacity * sizeof(Product));
            
            if (!sub->products) {
                fprintf(stderr, "Error: Failed to allocate memory for products\n");
                fclose(file);
                return false;
            }
            
            // Read each product
            for (int k = 0; k < sub->product_count; k++) {
                Product* prod = &sub->products[k];
                fread(prod, sizeof(Product), 1, file);
            }
        }
    }
    
    fclose(file);
    
    get_current_timestamp(store->last_saved, sizeof(store->last_saved));
    store->is_modified = false;
    
    printf("Data loaded successfully from %s\n", filename);
    printf("Categories: %d, Next IDs - Cat: %d, Sub: %d, Prod: %d\n",
           store->category_count, store->next_category_id,
           store->next_subgroup_id, store->next_product_id);

    return true;
}
Subgroup* datastore_find_subgroup_by_id(DataStore* store, int subgroup_id) {
    if (!store) return NULL;

    for (int i = 0; i < store->category_count; i++) {
        Subgroup* subgroup = category_find_subgroup_by_id(&store->categories[i], subgroup_id);
        if (subgroup) {
            return subgroup;
        }
    }

    return NULL;
}

Product* datastore_find_product_by_id(DataStore* store, int product_id) {
    if (!store) return NULL;

    for (int i = 0; i < store->category_count; i++) {
        for (int j = 0; j < store->categories[i].subgroup_count; j++) {
            Product* product = subgroup_find_product_by_id(&store->categories[i].subgroups[j], product_id);
            if (product) {
                return product;
            }
        }
    }

    return NULL;
}