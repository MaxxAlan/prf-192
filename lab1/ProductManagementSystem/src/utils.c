/**
 * @file utils.c
 * @brief Utility functions and DataStore implementation (FIXED - COMPLETE)
 * @author PMS Team
 * @date 2025
 * @compatible Dev-C++ 6.3, TDM-GCC 9.2.0, C11
 * 
 * MAJOR FIXES:
 * - Fixed input buffer handling in safe_input_string()
 * - Fixed pause_screen() double-enter bug
 * - Added error checking in datastore_load()
 * - Improved datastore_save() with atomic write
 * - Optimized search functions (single-pass)
 * - Fixed memory management in remove operations
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

/**
 * ✅ FIXED: Pause screen now works correctly
 */
void pause_screen(void) {
    printf("\nPress Enter to continue...");
    fflush(stdout);
    
    // Clear all pending input first
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
    
    // If we cleared to EOF, need to clear error and wait for new input
    if (c == EOF) {
        clearerr(stdin);
        while (getchar() != '\n');
    }
}

/**
 * ✅ FIXED: Input buffer handling
 */
bool safe_input_string(const char* prompt, char* buffer, size_t size) {
    if (!buffer || size == 0) return false;
    
    printf("%s", prompt);
    fflush(stdout);
    
    if (fgets(buffer, size, stdin) == NULL) {
        clearerr(stdin);
        buffer[0] = '\0';
        return false;
    }
    
    // Remove trailing newline
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
    } else {
        // Input was too long - clear the rest of the line
        int c;
        while ((c = getchar()) != '\n' && c != EOF);
        if (c == EOF) clearerr(stdin);
    }
    
    trim_string(buffer);
    return true;
}

bool safe_input_int(const char* prompt, int* value) {
    if (!value) return false;
    
    char buffer[100];
    printf("%s", prompt);
    fflush(stdout);
    
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        clearerr(stdin);
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
    fflush(stdout);
    
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        clearerr(stdin);
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
    
    // Free category resources
    category_free(&store->categories[index]);
    
    // Use swap-and-pop (consistent with other remove operations)
    int last_index = store->category_count - 1;
    if (index < last_index) {
        store->categories[index] = store->categories[last_index];
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

// ============================================================================
// Search Functions (OPTIMIZED - Single Pass)
// ============================================================================

SearchResult datastore_search_products_by_name(DataStore* store, const char* name) {
    SearchResult result = {NULL, 0};
    
    if (!store || !name) return result;
    
    char search_lower[100];
    strncpy(search_lower, name, sizeof(search_lower) - 1);
    search_lower[sizeof(search_lower) - 1] = '\0';
    for (int i = 0; search_lower[i]; i++) {
        search_lower[i] = tolower(search_lower[i]);
    }
    
    // Calculate max possible products
    int max_products = 0;
    for (int i = 0; i < store->category_count; i++) {
        for (int j = 0; j < store->categories[i].subgroup_count; j++) {
            max_products += store->categories[i].subgroups[j].product_count;
        }
    }
    
    if (max_products == 0) return result;
    
    result.products = (Product*)malloc(max_products * sizeof(Product));
    if (!result.products) {
        fprintf(stderr, "Error: Failed to allocate memory for search results\n");
        return result;
    }
    
    // Single pass: find and copy
    int count = 0;
    for (int i = 0; i < store->category_count; i++) {
        for (int j = 0; j < store->categories[i].subgroup_count; j++) {
            for (int k = 0; k < store->categories[i].subgroups[j].product_count; k++) {
                Product* p = &store->categories[i].subgroups[j].products[k];
                
                char product_name_lower[100];
                strncpy(product_name_lower, p->name, sizeof(product_name_lower) - 1);
                product_name_lower[sizeof(product_name_lower) - 1] = '\0';
                for (int l = 0; product_name_lower[l]; l++) {
                    product_name_lower[l] = tolower(product_name_lower[l]);
                }
                
                if (strstr(product_name_lower, search_lower) != NULL) {
                    result.products[count++] = *p;
                }
            }
        }
    }
    
    // Resize to actual size
    if (count < max_products && count > 0) {
        Product* resized = (Product*)realloc(result.products, count * sizeof(Product));
        if (resized) {
            result.products = resized;
        }
    } else if (count == 0) {
        free(result.products);
        result.products = NULL;
    }
    
    result.count = count;
    return result;
}

SearchResult datastore_search_products_by_price(DataStore* store, float min_price, float max_price) {
    SearchResult result = {NULL, 0};
    
    if (!store) return result;
    
    int max_products = 0;
    for (int i = 0; i < store->category_count; i++) {
        for (int j = 0; j < store->categories[i].subgroup_count; j++) {
            max_products += store->categories[i].subgroups[j].product_count;
        }
    }
    
    if (max_products == 0) return result;
    
    result.products = (Product*)malloc(max_products * sizeof(Product));
    if (!result.products) {
        fprintf(stderr, "Error: Failed to allocate memory for search results\n");
        return result;
    }
    
    int count = 0;
    for (int i = 0; i < store->category_count; i++) {
        for (int j = 0; j < store->categories[i].subgroup_count; j++) {
            for (int k = 0; k < store->categories[i].subgroups[j].product_count; k++) {
                Product* p = &store->categories[i].subgroups[j].products[k];
                if (p->price >= min_price && p->price <= max_price) {
                    result.products[count++] = *p;
                }
            }
        }
    }
    
    if (count < max_products && count > 0) {
        Product* resized = (Product*)realloc(result.products, count * sizeof(Product));
        if (resized) {
            result.products = resized;
        }
    } else if (count == 0) {
        free(result.products);
        result.products = NULL;
    }
    
    result.count = count;
    return result;
}

SearchResult datastore_search_products_by_quantity(DataStore* store, int min_qty, int max_qty) {
    SearchResult result = {NULL, 0};
    
    if (!store) return result;
    
    int max_products = 0;
    for (int i = 0; i < store->category_count; i++) {
        for (int j = 0; j < store->categories[i].subgroup_count; j++) {
            max_products += store->categories[i].subgroups[j].product_count;
        }
    }
    
    if (max_products == 0) return result;
    
    result.products = (Product*)malloc(max_products * sizeof(Product));
    if (!result.products) {
        fprintf(stderr, "Error: Failed to allocate memory for search results\n");
        return result;
    }
    
    int count = 0;
    for (int i = 0; i < store->category_count; i++) {
        for (int j = 0; j < store->categories[i].subgroup_count; j++) {
            for (int k = 0; k < store->categories[i].subgroups[j].product_count; k++) {
                Product* p = &store->categories[i].subgroups[j].products[k];
                if (p->quantity >= min_qty && p->quantity <= max_qty) {
                    result.products[count++] = *p;
                }
            }
        }
    }
    
    if (count < max_products && count > 0) {
        Product* resized = (Product*)realloc(result.products, count * sizeof(Product));
        if (resized) {
            result.products = resized;
        }
    } else if (count == 0) {
        free(result.products);
        result.products = NULL;
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
    printf("\n╔══════════════════════════════════════════════════════════╗\n");
    printf("║  ALL DATA - HIERARCHICAL VIEW                              ║\n");
    printf("╚══════════════════════════════════════════════════════════╝\n\n");
    
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
// File I/O Functions (CRITICAL FIXES)
// ============================================================================

bool datastore_save(DataStore* store, const char* filename) {
    if (!store || !filename) {
        fprintf(stderr, "Error: Invalid parameters for save\n");
        return false;
    }
    
    // Create temp file name
    char temp_file[512];
    snprintf(temp_file, sizeof(temp_file), "%s.tmp", filename);
    
    // Write to temp file first
    FILE* file = fopen(temp_file, "wb");
    if (!file) {
        fprintf(stderr, "Error: Cannot create temporary file %s\n", temp_file);
        return false;
    }
    
    // Write and validate header
    if (fwrite(&store->category_count, sizeof(int), 1, file) != 1 ||
        fwrite(&store->next_category_id, sizeof(int), 1, file) != 1 ||
        fwrite(&store->next_subgroup_id, sizeof(int), 1, file) != 1 ||
        fwrite(&store->next_product_id, sizeof(int), 1, file) != 1) {
        fprintf(stderr, "Error: Failed to write header\n");
        fclose(file);
        remove(temp_file);
        return false;
    }
    
    // Write each category with validation
    for (int i = 0; i < store->category_count; i++) {
        Category* cat = &store->categories[i];
        
        if (fwrite(&cat->id, sizeof(int), 1, file) != 1 ||
            fwrite(cat->name, sizeof(char), 50, file) != 50 ||
            fwrite(cat->description, sizeof(char), 200, file) != 200 ||
            fwrite(&cat->subgroup_count, sizeof(int), 1, file) != 1) {
            fprintf(stderr, "Error: Failed to write category %d\n", cat->id);
            fclose(file);
            remove(temp_file);
            return false;
        }
        
        // Write each subgroup
        for (int j = 0; j < cat->subgroup_count; j++) {
            Subgroup* sub = &cat->subgroups[j];
            
            if (fwrite(&sub->id, sizeof(int), 1, file) != 1 ||
                fwrite(&sub->category_id, sizeof(int), 1, file) != 1 ||
                fwrite(sub->name, sizeof(char), 50, file) != 50 ||
                fwrite(sub->description, sizeof(char), 200, file) != 200 ||
                fwrite(&sub->product_count, sizeof(int), 1, file) != 1) {
                fprintf(stderr, "Error: Failed to write subgroup %d\n", sub->id);
                fclose(file);
                remove(temp_file);
                return false;
            }
            
            // Write each product
            for (int k = 0; k < sub->product_count; k++) {
                Product* prod = &sub->products[k];
                if (fwrite(prod, sizeof(Product), 1, file) != 1) {
                    fprintf(stderr, "Error: Failed to write product %d\n", prod->id);
                    fclose(file);
                    remove(temp_file);
                    return false;
                }
            }
        }
    }
    
    fclose(file);
    
    // Atomic file replacement
    remove(BACKUP_FILE);
    rename(filename, BACKUP_FILE);
    
    if (rename(temp_file, filename) != 0) {
        fprintf(stderr, "Error: Failed to finalize save\n");
        rename(BACKUP_FILE, filename);
        return false;
    }
    
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
        return true;
    }
    
    // Free existing data
    datastore_free(store);
    *store = datastore_init();
    
    // Read and validate header
    if (fread(&store->category_count, sizeof(int), 1, file) != 1 ||
        fread(&store->next_category_id, sizeof(int), 1, file) != 1 ||
        fread(&store->next_subgroup_id, sizeof(int), 1, file) != 1 ||
        fread(&store->next_product_id, sizeof(int), 1, file) != 1) {
        fprintf(stderr, "Error: Corrupted file header\n");
        fclose(file);
        return false;
    }
    
    // Validate header data
    if (store->category_count < 0 || store->category_count > 10000 ||
        store->next_category_id <= 0 || store->next_subgroup_id <= 0 || 
        store->next_product_id <= 0) {
        fprintf(stderr, "Error: Invalid data in file header\n");
        fclose(file);
        return false;
    }
    
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
        
        if (fread(&cat->id, sizeof(int), 1, file) != 1 ||
            fread(cat->name, sizeof(char), 50, file) != 50 ||
            fread(cat->description, sizeof(char), 200, file) != 200 ||
            fread(&cat->subgroup_count, sizeof(int), 1, file) != 1) {
            fprintf(stderr, "Error: Failed to read category %d\n", i);
            fclose(file);
            datastore_free(store);
            return false;
        }
        
        if (cat->subgroup_count < 0 || cat->subgroup_count > 1000) {
            fprintf(stderr, "Error: Invalid subgroup count in category\n");
            fclose(file);
            datastore_free(store);
            return false;
        }
        
        // Allocate subgroup array
        cat->subgroup_capacity = cat->subgroup_count > 10 ? cat->subgroup_count : 10;
        cat->subgroups = (Subgroup*)malloc(cat->subgroup_capacity * sizeof(Subgroup));
        
        if (!cat->subgroups) {
            fprintf(stderr, "Error: Failed to allocate memory for subgroups\n");
            fclose(file);
            datastore_free(store);
            return false;
        }
        
        // Read each subgroup
        for (int j = 0; j < cat->subgroup_count; j++) {
            Subgroup* sub = &cat->subgroups[j];
            
            if (fread(&sub->id, sizeof(int), 1, file) != 1 ||
                fread(&sub->category_id, sizeof(int), 1, file) != 1 ||
                fread(sub->name, sizeof(char), 50, file) != 50 ||
                fread(sub->description, sizeof(char), 200, file) != 200 ||
                fread(&sub->product_count, sizeof(int), 1, file) != 1) {
                fprintf(stderr, "Error: Failed to read subgroup\n");
                fclose(file);
                datastore_free(store);
                return false;
            }
            
            if (sub->product_count < 0 || sub->product_count > 10000) {
                fprintf(stderr, "Error: Invalid product count in subgroup\n");
                fclose(file);
                datastore_free(store);
                return false;
            }
            
            // Allocate product array
            sub->product_capacity = sub->product_count > 10 ? sub->product_count : 10;
            sub->products = (Product*)malloc(sub->product_capacity * sizeof(Product));
            
            if (!sub->products) {
                fprintf(stderr, "Error: Failed to allocate memory for products\n");
                fclose(file);
                datastore_free(store);
                return false;
            }
            
            // Read each product
            for (int k = 0; k < sub->product_count; k++) {
                Product* prod = &sub->products[k];
                if (fread(prod, sizeof(Product), 1, file) != 1) {
                    fprintf(stderr, "Error: Failed to read product\n");
                    fclose(file);
                    datastore_free(store);
                    return false;
                }
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