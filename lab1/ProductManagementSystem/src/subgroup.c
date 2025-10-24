
#include "../include/subgroup.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define INITIAL_PRODUCT_CAPACITY 10

/**
 * @brief Trim whitespace from string (helper)
 */
static void trim_string(char* str) {
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

Subgroup subgroup_create(int id, int category_id, const char* name, const char* description) {
    Subgroup subgroup;
    
    subgroup.id = id;
    subgroup.category_id = category_id;
    subgroup.product_count = 0;
    subgroup.product_capacity = INITIAL_PRODUCT_CAPACITY;
    
    // Allocate initial product array
    subgroup.products = (Product*)malloc(INITIAL_PRODUCT_CAPACITY * sizeof(Product));
    if (!subgroup.products) {
        fprintf(stderr, "Error: Failed to allocate memory for products\n");
        subgroup.product_capacity = 0;
    }
    
    // Copy and trim name
    strncpy(subgroup.name, name ? name : "", sizeof(subgroup.name) - 1);
    subgroup.name[sizeof(subgroup.name) - 1] = '\0';
    trim_string(subgroup.name);
    
    // Copy and trim description
    strncpy(subgroup.description, description ? description : "", sizeof(subgroup.description) - 1);
    subgroup.description[sizeof(subgroup.description) - 1] = '\0';
    trim_string(subgroup.description);
    
    return subgroup;
}

bool subgroup_add_product(Subgroup* subgroup, Product product) {
    if (!subgroup) {
        fprintf(stderr, "Error: Subgroup pointer is NULL\n");
        return false;
    }
    
    if (!product_is_valid(&product)) {
        fprintf(stderr, "Error: Invalid product data\n");
        return false;
    }
    
    // Check if capacity needs to be expanded
    if (subgroup->product_count >= subgroup->product_capacity) {
        int new_capacity = subgroup->product_capacity * 2;
        Product* new_products = (Product*)realloc(subgroup->products, new_capacity * sizeof(Product));
        
        if (!new_products) {
            fprintf(stderr, "Error: Failed to expand product array\n");
            return false;
        }
        
        subgroup->products = new_products;
        subgroup->product_capacity = new_capacity;
    }
    
    // Add product
    subgroup->products[subgroup->product_count] = product;
    subgroup->product_count++;
    
    return true;
}

/**
 * ✅ FIXED: Use swap-and-pop method for O(1) removal
 * 
 * OLD METHOD:
 * - Shifted all elements left (O(n))
 * - Unnecessary work for removing one element
 * 
 * NEW METHOD:
 * - Swap with last element (O(1))
 * - Order doesn't matter for our use case
 * - Much faster for large arrays
 */
bool subgroup_remove_product(Subgroup* subgroup, int product_id) {
    if (!subgroup) {
        fprintf(stderr, "Error: Subgroup pointer is NULL\n");
        return false;
    }
    
    if (subgroup->product_count <= 0) {
        fprintf(stderr, "Error: Subgroup has no products\n");
        return false;
    }
    
    // Find product index
    int index = -1;
    for (int i = 0; i < subgroup->product_count; i++) {
        if (subgroup->products[i].id == product_id) {
            index = i;
            break;
        }
    }
    
    if (index == -1) {
        fprintf(stderr, "Error: Product ID %d not found in subgroup\n", product_id);
        return false;
    }
    
    // ✅ FIX: Swap with last element (O(1) instead of O(n))
    int last_index = subgroup->product_count - 1;
    if (index < last_index) {
        // Move last product to the removed position
        subgroup->products[index] = subgroup->products[last_index];
    }
    
    // Decrease count (effectively removing the last element)
    subgroup->product_count--;
    
    return true;
}

Product* subgroup_find_product_by_id(Subgroup* subgroup, int product_id) {
    if (!subgroup) {
        return NULL;
    }
    
    for (int i = 0; i < subgroup->product_count; i++) {
        if (subgroup->products[i].id == product_id) {
            return &subgroup->products[i];
        }
    }
    
    return NULL;
}

void subgroup_display(const Subgroup* subgroup) {
    if (!subgroup) {
        printf("Error: Subgroup pointer is NULL\n");
        return;
    }
    
    printf("\n╔══════════════════════════════════════════════════════════╗\n");
    printf("║  Subgroup Information                                      ║\n");
    printf("╚══════════════════════════════════════════════════════════╝\n");
    printf("  ID:          %d\n", subgroup->id);
    printf("  Category ID: %d\n", subgroup->category_id);
    printf("  Name:        %s\n", subgroup->name);
    printf("  Description: %s\n", subgroup->description);
    printf("  Products:    %d (Capacity: %d)\n", subgroup->product_count, subgroup->product_capacity);
    
    if (subgroup->product_count > 0) {
        printf("\n  Products in this subgroup:\n");
        product_display_table_header();
        for (int i = 0; i < subgroup->product_count; i++) {
            product_display_table_row(&subgroup->products[i]);
        }
        printf("  ────────────────────────────────────────────────────────────────────────────\n");
    }
    printf("\n");
}

void subgroup_display_table_header(void) {
    printf("  ┌────────┬──────────────┬──────────────────────────────────────┬──────────┐\n");
    printf("  │   ID   │ Category ID  │ Subgroup Name                        │ Products │\n");
    printf("  ├────────┼──────────────┼──────────────────────────────────────┼──────────┤\n");
}

void subgroup_display_table_row(const Subgroup* subgroup) {
    if (!subgroup) return;
    
    printf("  │ %-6d │ %-12d │ %-36s │ %-8d │\n",
           subgroup->id,
           subgroup->category_id,
           subgroup->name,
           subgroup->product_count);
}

bool subgroup_update_name(Subgroup* subgroup, const char* name) {
    if (!subgroup) {
        fprintf(stderr, "Error: Subgroup pointer is NULL\n");
        return false;
    }
    
    if (!name || strlen(name) == 0) {
        fprintf(stderr, "Error: Name cannot be empty\n");
        return false;
    }
    
    strncpy(subgroup->name, name, sizeof(subgroup->name) - 1);
    subgroup->name[sizeof(subgroup->name) - 1] = '\0';
    trim_string(subgroup->name);
    
    if (strlen(subgroup->name) == 0) {
        fprintf(stderr, "Error: Name cannot be empty after trimming\n");
        return false;
    }
    
    return true;
}

bool subgroup_update_description(Subgroup* subgroup, const char* description) {
    if (!subgroup) {
        fprintf(stderr, "Error: Subgroup pointer is NULL\n");
        return false;
    }
    
    if (!description) {
        description = "";
    }
    
    strncpy(subgroup->description, description, sizeof(subgroup->description) - 1);
    subgroup->description[sizeof(subgroup->description) - 1] = '\0';
    trim_string(subgroup->description);
    
    return true;
}

bool subgroup_is_valid(const Subgroup* subgroup) {
    if (!subgroup) {
        return false;
    }
    
    if (subgroup->id <= 0) {
        return false;
    }
    
    if (subgroup->category_id <= 0) {
        return false;
    }
    
    if (strlen(subgroup->name) == 0) {
        return false;
    }
    
    if (subgroup->product_count < 0 || subgroup->product_capacity < 0) {
        return false;
    }
    
    if (subgroup->product_count > subgroup->product_capacity) {
        return false;
    }
    
    return true;
}

void subgroup_free(Subgroup* subgroup) {
    if (!subgroup) {
        return;
    }
    
    if (subgroup->products) {
        free(subgroup->products);
        subgroup->products = NULL;
    }
    
    subgroup->product_count = 0;
    subgroup->product_capacity = 0;
}