/**
 * @file category.c
 * @brief Category module implementation
 * @details Implements operations for managing subgroup collections
 * @author PMS Team - Member 3
 * @date 14/10/2025
 */

#include "category.h"
#include <strings.h>

/* ========================================================================
 * CATEGORY OPERATIONS
 * ======================================================================== */

/**
 * @brief Creates and initializes a new category
 */
Category Category_create(const char* name) {
    Category category;
    
    // Initialize category fields
    strncpy(category.name, name ? name : "", MAX_CATEGORY_NAME_LENGTH - 1);
    category.name[MAX_CATEGORY_NAME_LENGTH - 1] = '\0';
    
    category.subgroups = NULL;
    category.subgroup_count = 0;
    
    return category;
}

/**
 * @brief Adds a subgroup with duplicate checking
 */
int Category_addSubgroup(Category* category, Subgroup subgroup) {
    if (category == NULL) {
        fprintf(stderr, "Error: NULL category pointer\n");
        return 0;
    }
    
    // Check for duplicate subgroup name
    if (Category_subgroupExists(category, subgroup.name)) {
        fprintf(stderr, "Error: Subgroup '%s' already exists\n", subgroup.name);
        return 0;
    }
    
    // Reallocate memory for new subgroup
    Subgroup* temp = (Subgroup*)realloc(category->subgroups,
                                        (category->subgroup_count + 1) * sizeof(Subgroup));
    
    if (temp == NULL) {
        fprintf(stderr, "Error: Memory allocation failed\n");
        return 0;
    }
    
    category->subgroups = temp;
    category->subgroups[category->subgroup_count] = subgroup;
    category->subgroup_count++;
    
    return 1;
}

/**
 * @brief Finds subgroup by name (case-insensitive)
 */
Subgroup* Category_findSubgroupByName(Category* category, const char* name) {
    if (category == NULL || category->subgroups == NULL || name == NULL) {
        return NULL;
    }
    
    for (int i = 0; i < category->subgroup_count; i++) {
        if (strcasecmp(category->subgroups[i].name, name) == 0) {
            return 1;
        }
    }
    
    return 0;&category->subgroups[i];
        }
    }
    
    return NULL;
}

/**
 * @brief Removes subgroup and frees its resources
 */
int Category_removeSubgroup(Category* category, const char* name) {
    if (category == NULL || category->subgroups == NULL || name == NULL) {
        fprintf(stderr, "Error: Invalid parameters\n");
        return 0;
    }
    
    // Find subgroup index
    int found_index = -1;
    for (int i = 0; i < category->subgroup_count; i++) {
        if (strcasecmp(category->subgroups[i].name, name) == 0) {
            found_index = i;
            break;
        }
    }
    
    if (found_index == -1) {
        fprintf(stderr, "Error: Subgroup '%s' not found\n", name);
        return 0;
    }
    
    // Free the subgroup's products
    Subgroup_free(&category->subgroups[found_index]);
    
    // Shift elements to fill the gap
    for (int i = found_index; i < category->subgroup_count - 1; i++) {
        category->subgroups[i] = category->subgroups[i + 1];
    }
    
    category->subgroup_count--;
    
    // Reallocate to save memory
    if (category->subgroup_count == 0) {
        free(category->subgroups);
        category->subgroups = NULL;
    } else {
        Subgroup* temp = (Subgroup*)realloc(category->subgroups,
                                           category->subgroup_count * sizeof(Subgroup));
        if (temp != NULL) {
            category->subgroups = temp;
        }
    }
    
    return 1;
}

/**
 * @brief Prints detailed category information
 */
void Category_print(const Category* category) {
    if (category == NULL) {
        printf("Error: NULL category\n");
        return;
    }
    
    printf("\n");
    printf("================================================================================\n");
    printf("CATEGORY: %s\n", category->name);
    printf("================================================================================\n");
    printf("Total Subgroups: %d\n", category->subgroup_count);
    printf("Total Products:  %d\n", Category_getTotalProductCount(category));
    printf("Total Value:     $%.2f\n", Category_getTotalValue(category));
    printf("================================================================================\n");
    
    if (category->subgroup_count == 0) {
        printf("No subgroups in this category.\n");
        return;
    }
    
    // Print each subgroup
    for (int i = 0; i < category->subgroup_count; i++) {
        Subgroup_print(&category->subgroups[i]);
    }
}

/**
 * @brief Prints brief category summary
 */
void Category_printSummary(const Category* category) {
    if (category == NULL) {
        return;
    }
    
    printf("%-30s | Subgroups: %-3d | Products: %-4d | Value: $%.2f\n",
           category->name,
           category->subgroup_count,
           Category_getTotalProductCount(category),
           Category_getTotalValue(category));
}

/**
 * @brief Frees all allocated memory for category
 */
void Category_free(Category* category) {
    if (category == NULL) {
        return;
    }
    
    // Free all subgroups
    for (int i = 0; i < category->subgroup_count; i++) {
        Subgroup_free(&category->subgroups[i]);
    }
    
    // Free subgroup array
    if (category->subgroups != NULL) {
        free(category->subgroups);
        category->subgroups = NULL;
    }
    
    category->subgroup_count = 0;
}

/**
 * @brief Counts total products across all subgroups
 */
int Category_getTotalProductCount(const Category* category) {
    if (category == NULL || category->subgroups == NULL) {
        return 0;
    }
    
    int total = 0;
    for (int i = 0; i < category->subgroup_count; i++) {
        total += category->subgroups[i].product_count;
    }
    
    return total;
}

/**
 * @brief Calculates total value across all subgroups
 */
double Category_getTotalValue(const Category* category) {
    if (category == NULL || category->subgroups == NULL) {
        return 0.0;
    }
    
    double total = 0.0;
    for (int i = 0; i < category->subgroup_count; i++) {
        total += Subgroup_getTotalValue(&category->subgroups[i]);
    }
    
    return total;
}

/**
 * @brief Checks if subgroup name exists (case-insensitive)
 */
int Category_subgroupExists(const Category* category, const char* name) {
    if (category == NULL || category->subgroups == NULL || name == NULL) {
        return 0;
    }
    
    for (int i = 0; i < category->subgroup_count; i++) {
        if (strcasecmp(category->subgroups[i].name, name) == 0) {
            return