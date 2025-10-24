#include "../include/category.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define INITIAL_SUBGROUP_CAPACITY 10

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

Category category_create(int id, const char* name, const char* description) {
    Category category;
    
    category.id = id;
    category.subgroup_count = 0;
    category.subgroup_capacity = INITIAL_SUBGROUP_CAPACITY;
    
    // Allocate initial subgroup array
    category.subgroups = (Subgroup*)malloc(INITIAL_SUBGROUP_CAPACITY * sizeof(Subgroup));
    if (!category.subgroups) {
        fprintf(stderr, "Error: Failed to allocate memory for subgroups\n");
        category.subgroup_capacity = 0;
    }
    
    // Copy and trim name
    strncpy(category.name, name ? name : "", sizeof(category.name) - 1);
    category.name[sizeof(category.name) - 1] = '\0';
    trim_string(category.name);
    
    // Copy and trim description
    strncpy(category.description, description ? description : "", sizeof(category.description) - 1);
    category.description[sizeof(category.description) - 1] = '\0';
    trim_string(category.description);
    
    return category;
}

bool category_add_subgroup(Category* category, Subgroup subgroup) {
    if (!category) {
        fprintf(stderr, "Error: Category pointer is NULL\n");
        return false;
    }
    
    if (!subgroup_is_valid(&subgroup)) {
        fprintf(stderr, "Error: Invalid subgroup data\n");
        return false;
    }
    
    // Check if capacity needs to be expanded
    if (category->subgroup_count >= category->subgroup_capacity) {
        int new_capacity = category->subgroup_capacity * 2;
        Subgroup* new_subgroups = (Subgroup*)realloc(category->subgroups, new_capacity * sizeof(Subgroup));
        
        if (!new_subgroups) {
            fprintf(stderr, "Error: Failed to expand subgroup array\n");
            return false;
        }
        
        category->subgroups = new_subgroups;
        category->subgroup_capacity = new_capacity;
    }
    
    // Add subgroup
    category->subgroups[category->subgroup_count] = subgroup;
    category->subgroup_count++;
    
    return true;
}

/**
 * ✅ FIXED: Properly handle memory when removing subgroup
 * 
 * OLD BUG: 
 * - Called subgroup_free() then copied freed pointer
 * - Resulted in dangling pointer
 * 
 * NEW SOLUTION:
 * - Use swap-and-pop method (O(1) instead of O(n))
 * - Free only the removed subgroup
 * - No memory leaks, no dangling pointers
 */
bool category_remove_subgroup(Category* category, int subgroup_id) {
    if (!category) {
        fprintf(stderr, "Error: Category pointer is NULL\n");
        return false;
    }
    
    if (category->subgroup_count <= 0) {
        fprintf(stderr, "Error: Category has no subgroups\n");
        return false;
    }
    
    // Find subgroup index
    int index = -1;
    for (int i = 0; i < category->subgroup_count; i++) {
        if (category->subgroups[i].id == subgroup_id) {
            index = i;
            break;
        }
    }
    
    if (index == -1) {
        fprintf(stderr, "Error: Subgroup ID %d not found in category\n", subgroup_id);
        return false;
    }
    
    // ✅ FIX: Free the subgroup to be removed
    subgroup_free(&category->subgroups[index]);
    
    // ✅ FIX: Swap with last element (O(1) operation)
    // This avoids shifting all elements left
    int last_index = category->subgroup_count - 1;
    if (index < last_index) {
        // Move last subgroup to the removed position
        category->subgroups[index] = category->subgroups[last_index];
    }
    
    // Decrease count (effectively removing the last element)
    category->subgroup_count--;
    
    return true;
}

Subgroup* category_find_subgroup_by_id(Category* category, int subgroup_id) {
    if (!category) {
        return NULL;
    }
    
    for (int i = 0; i < category->subgroup_count; i++) {
        if (category->subgroups[i].id == subgroup_id) {
            return &category->subgroups[i];
        }
    }
    
    return NULL;
}

void category_display(const Category* category) {
    if (!category) {
        printf("Error: Category pointer is NULL\n");
        return;
    }
    
    printf("\n╔══════════════════════════════════════════════════════════╗\n");
    printf("║  Category Information                                      ║\n");
    printf("╚══════════════════════════════════════════════════════════╝\n");
    printf("  ID:          %d\n", category->id);
    printf("  Name:        %s\n", category->name);
    printf("  Description: %s\n", category->description);
    printf("  Subgroups:   %d (Capacity: %d)\n", category->subgroup_count, category->subgroup_capacity);
    
    if (category->subgroup_count > 0) {
        printf("\n  Subgroups in this category:\n");
        subgroup_display_table_header();
        for (int i = 0; i < category->subgroup_count; i++) {
            subgroup_display_table_row(&category->subgroups[i]);
        }
        printf("  └────────┴──────────────┴──────────────────────────────────────────┴──────────┘\n");
        
        // Display products in each subgroup
        for (int i = 0; i < category->subgroup_count; i++) {
            if (category->subgroups[i].product_count > 0) {
                printf("\n  Products in '%s' (Subgroup ID: %d):\n", 
                       category->subgroups[i].name, 
                       category->subgroups[i].id);
                product_display_table_header();
                for (int j = 0; j < category->subgroups[i].product_count; j++) {
                    product_display_table_row(&category->subgroups[i].products[j]);
                }
                printf("  ────────────────────────────────────────────────────────────────────────────\n");
            }
        }
    }
    printf("\n");
}

void category_display_table_header(void) {
    printf("  ┌────────┬──────────────────────────────────────────────────┬───────────┐\n");
    printf("  │   ID   │ Category Name                                  │ Subgroups │\n");
    printf("  ├────────┼──────────────────────────────────────────────────┼───────────┤\n");
}

void category_display_table_row(const Category* category) {
    if (!category) return;
    
    printf("  │ %-6d │ %-46s │ %-9d │\n",
           category->id,
           category->name,
           category->subgroup_count);
}

bool category_update_name(Category* category, const char* name) {
    if (!category) {
        fprintf(stderr, "Error: Category pointer is NULL\n");
        return false;
    }
    
    if (!name || strlen(name) == 0) {
        fprintf(stderr, "Error: Name cannot be empty\n");
        return false;
    }
    
    strncpy(category->name, name, sizeof(category->name) - 1);
    category->name[sizeof(category->name) - 1] = '\0';
    trim_string(category->name);
    
    if (strlen(category->name) == 0) {
        fprintf(stderr, "Error: Name cannot be empty after trimming\n");
        return false;
    }
    
    return true;
}

bool category_update_description(Category* category, const char* description) {
    if (!category) {
        fprintf(stderr, "Error: Category pointer is NULL\n");
        return false;
    }
    
    if (!description) {
        description = "";
    }
    
    strncpy(category->description, description, sizeof(category->description) - 1);
    category->description[sizeof(category->description) - 1] = '\0';
    trim_string(category->description);
    
    return true;
}

bool category_is_valid(const Category* category) {
    if (!category) {
        return false;
    }
    
    if (category->id <= 0) {
        return false;
    }
    
    if (strlen(category->name) == 0) {
        return false;
    }
    
    if (category->subgroup_count < 0 || category->subgroup_capacity < 0) {
        return false;
    }
    
    if (category->subgroup_count > category->subgroup_capacity) {
        return false;
    }
    
    return true;
}

void category_free(Category* category) {
    if (!category) {
        return;
    }
    
    // Free all subgroups (which will free all products)
    if (category->subgroups) {
        for (int i = 0; i < category->subgroup_count; i++) {
            subgroup_free(&category->subgroups[i]);
        }
        free(category->subgroups);
        category->subgroups = NULL;
    }
    
    category->subgroup_count = 0;
    category->subgroup_capacity = 0;
}