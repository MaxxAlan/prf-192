/**
 * @file category.c
 * @brief Category module implementation
 * @author PMS Development Team - Member 3
 * 
 * Complete implementation of category operations including hierarchical
 * management of subgroups, deep memory cleanup, and aggregate statistics.
 */

#include "category.h"
#include <string.h>
#include <ctype.h>

/* ========== HELPER FUNCTIONS ========== */

/**
 * @brief Trim leading and trailing whitespace from a string
 * @param str String to trim (modified in place)
 */
static void trim_string(char* str) {
    if (!str) return;
    
    char* start = str;
    while (*start && isspace((unsigned char)*start)) {
        start++;
    }
    
    char* end = start + strlen(start) - 1;
    while (end > start && isspace((unsigned char)*end)) {
        end--;
    }
    *(end + 1) = '\0';
    
    if (start != str) {
        memmove(str, start, strlen(start) + 1);
    }
}

/**
 * @brief Check if a string is empty or contains only whitespace
 * @param str String to check
 * @return 1 if empty/whitespace only, 0 otherwise
 */
static int is_empty_string(const char* str) {
    if (!str) return 1;
    
    while (*str) {
        if (!isspace((unsigned char)*str)) {
            return 0;
        }
        str++;
    }
    return 1;
}

/**
 * @brief Case-insensitive string comparison
 * @param s1 First string
 * @param s2 Second string
 * @return 0 if equal (case-insensitive), non-zero otherwise
 */
static int strcasecmp_custom(const char* s1, const char* s2) {
    if (!s1 || !s2) return -1;
    
    while (*s1 && *s2) {
        int diff = tolower((unsigned char)*s1) - tolower((unsigned char)*s2);
        if (diff != 0) return diff;
        s1++;
        s2++;
    }
    return tolower((unsigned char)*s1) - tolower((unsigned char)*s2);
}

/* ========== PUBLIC FUNCTIONS IMPLEMENTATION ========== */

Category Category_create(const char* name) {
    Category category;
    
    // Initialize with invalid state
    category.name[0] = '\0';
    category.subgroups = NULL;
    category.subgroup_count = -1;
    
    // Validate name
    if (!name || is_empty_string(name)) {
        fprintf(stderr, "Error: Category name cannot be empty\n");
        return category;
    }
    
    // Copy name with bounds checking
    strncpy(category.name, name, sizeof(category.name) - 1);
    category.name[sizeof(category.name) - 1] = '\0';
    trim_string(category.name);
    
    // Initialize empty subgroups array
    category.subgroups = NULL;
    category.subgroup_count = 0;
    
    return category;
}

void Category_free(Category* category) {
    if (!category) {
        fprintf(stderr, "Warning: NULL category pointer in Category_free\n");
        return;
    }
    
    // Free all subgroups (which frees their products)
    if (category->subgroups) {
        for (int i = 0; i < category->subgroup_count; i++) {
            Subgroup_free(&category->subgroups[i]);
        }
        free(category->subgroups);
        category->subgroups = NULL;
    }
    
    // Reset state
    category->subgroup_count = 0;
    category->name[0] = '\0';
}

int Category_addSubgroup(Category* category, Subgroup subgroup) {
    // Validate category pointer
    if (!category) {
        fprintf(stderr, "Error: NULL category pointer in Category_addSubgroup\n");
        return 0;
    }
    
    // Validate category is initialized
    if (category->subgroup_count < 0) {
        fprintf(stderr, "Error: Cannot add subgroup to uninitialized category\n");
        return 0;
    }
    
    // Validate subgroup
    if (!Subgroup_isValid(&subgroup)) {
        fprintf(stderr, "Error: Cannot add invalid subgroup\n");
        return 0;
    }
    
    // Check for duplicate name (case-insensitive)
    if (Category_subgroupExists(category, subgroup.name)) {
        fprintf(stderr, "Error: Subgroup '%s' already exists in category '%s'\n", 
                subgroup.name, category->name);
        return 0;
    }
    
    // Allocate or resize subgroups array
    Subgroup* temp = (Subgroup*)realloc(category->subgroups, 
                                        (category->subgroup_count + 1) * sizeof(Subgroup));
    
    if (!temp) {
        fprintf(stderr, "Error: Memory allocation failed in Category_addSubgroup\n");
        return 0;
    }
    
    category->subgroups = temp;
    
    // Add subgroup to array
    category->subgroups[category->subgroup_count] = subgroup;
    category->subgroup_count++;
    
    return 1;
}

int Category_removeSubgroup(Category* category, const char* subgroup_name) {
    // Validate category pointer
    if (!category) {
        fprintf(stderr, "Error: NULL category pointer in Category_removeSubgroup\n");
        return 0;
    }
    
    // Validate category is initialized
    if (category->subgroup_count < 0) {
        fprintf(stderr, "Error: Cannot remove from uninitialized category\n");
        return 0;
    }
    
    // Validate subgroup name
    if (!subgroup_name || is_empty_string(subgroup_name)) {
        fprintf(stderr, "Error: Subgroup name cannot be empty\n");
        return 0;
    }
    
    // Find subgroup index (case-insensitive)
    int index = -1;
    for (int i = 0; i < category->subgroup_count; i++) {
        if (strcasecmp_custom(category->subgroups[i].name, subgroup_name) == 0) {
            index = i;
            break;
        }
    }
    
    // Subgroup not found
    if (index == -1) {
        return 0;
    }
    
    // Free the subgroup's resources
    Subgroup_free(&category->subgroups[index]);
    
    // Shift remaining subgroups to fill the gap
    for (int i = index; i < category->subgroup_count - 1; i++) {
        category->subgroups[i] = category->subgroups[i + 1];
    }
    
    category->subgroup_count--;
    
    // If last subgroup removed, free array
    if (category->subgroup_count == 0) {
        free(category->subgroups);
        category->subgroups = NULL;
    } else {
        // Resize array to save memory
        Subgroup* temp = (Subgroup*)realloc(category->subgroups, 
                                            category->subgroup_count * sizeof(Subgroup));
        if (temp) {
            category->subgroups = temp;
        }
    }
    
    return 1;
}

int Category_updateSubgroup(Category* category, Subgroup subgroup) {
    // Validate category pointer
    if (!category) {
        fprintf(stderr, "Error: NULL category pointer in Category_updateSubgroup\n");
        return 0;
    }
    
    // Find subgroup by name
    Subgroup* existing = Category_findSubgroupByName(category, subgroup.name);
    if (!existing) {
        fprintf(stderr, "Error: Subgroup '%s' not found in category '%s'\n",
                subgroup.name, category->name);
        return 0;
    }
    
    // Free old subgroup's products and update
    Subgroup_free(existing);
    *existing = subgroup;
    return 1;
}

Subgroup* Category_findSubgroupByName(Category* category, const char* subgroup_name) {
    // Validate inputs
    if (!category || category->subgroup_count < 0 || !subgroup_name) {
        return NULL;
    }
    
    // Linear search (case-insensitive)
    for (int i = 0; i < category->subgroup_count; i++) {
        if (strcasecmp_custom(category->subgroups[i].name, subgroup_name) == 0) {
            return &category->subgroups[i];
        }
    }
    
    return NULL;
}

Product* Category_findProductById(Category* category, int product_id) {
    // Validate category
    if (!category || category->subgroup_count < 0) {
        return NULL;
    }
    
    // Search through all subgroups
    for (int i = 0; i < category->subgroup_count; i++) {
        Product* product = Subgroup_findProductById(&category->subgroups[i], product_id);
        if (product) {
            return product;
        }
    }
    
    return NULL;
}

int Category_searchProductsByName(Category* category, const char* search_name) {
    // Validate inputs
    if (!category || !search_name || category->subgroup_count < 0) {
        return 0;
    }
    
    if (is_empty_string(search_name)) {
        fprintf(stderr, "Error: Search name cannot be empty\n");
        return 0;
    }
    
    int total_found = 0;
    
    printf("\nSearching in category '%s'...\n", category->name);
    
    // Search through all subgroups
    for (int i = 0; i < category->subgroup_count; i++) {
        int found = Subgroup_searchByName(&category->subgroups[i], search_name);
        total_found += found;
    }
    
    if (total_found == 0) {
        printf("No products found matching '%s' in category '%s'\n", 
               search_name, category->name);
    } else {
        printf("\nTotal: %d product(s) found in category '%s'\n", 
               total_found, category->name);
    }
    
    return total_found;
}

int Category_subgroupExists(Category* category, const char* subgroup_name) {
    return Category_findSubgroupByName(category, subgroup_name) != NULL;
}

void Category_print(const Category* category) {
    // Validate category pointer
    if (!category) {
        fprintf(stderr, "Error: NULL category pointer in Category_print\n");
        return;
    }
    
    // Check if initialized
    if (category->subgroup_count < 0) {
        printf("Uninitialized Category\n");
        return;
    }
    
    printf("\n");
    printf("============================================================\n");
    printf("Category: %s (%d subgroup%s, %d product%s)\n", 
           category->name,
           category->subgroup_count,
           category->subgroup_count == 1 ? "" : "s",
           Category_getTotalProductCount(category),
           Category_getTotalProductCount(category) == 1 ? "" : "s");
    printf("============================================================\n");
    
    if (category->subgroup_count == 0) {
        printf("  (No subgroups in this category)\n");
        printf("============================================================\n");
        return;
    }
    
    // Print all subgroups with their products
    for (int i = 0; i < category->subgroup_count; i++) {
        Subgroup_print(&category->subgroups[i]);
    }
    
    printf("\n");
    printf("============================================================\n");
    printf("Category Total Value: $%.2f\n", Category_getTotalValue(category));
    printf("============================================================\n");
}

void Category_printSummary(const Category* category) {
    // Validate category pointer
    if (!category) {
        fprintf(stderr, "Error: NULL category pointer in Category_printSummary\n");
        return;
    }
    
    // Check if initialized
    if (category->subgroup_count < 0) {
        printf("Uninitialized Category\n");
        return;
    }
    
    printf("\n");
    printf("Category: %s (%d subgroup%s, %d product%s, Total: $%.2f)\n",
           category->name,
           category->subgroup_count,
           category->subgroup_count == 1 ? "" : "s",
           Category_getTotalProductCount(category),
           Category_getTotalProductCount(category) == 1 ? "" : "s",
           Category_getTotalValue(category));
    
    if (category->subgroup_count > 0) {
        for (int i = 0; i < category->subgroup_count; i++) {
            Subgroup_printSummary(&category->subgroups[i]);
        }
    } else {
        printf("  (No subgroups)\n");
    }
}

void Category_printSubgroupList(const Category* category) {
    // Validate category pointer
    if (!category || category->subgroup_count < 0) {
        return;
    }
    
    if (category->subgroup_count == 0) {
        printf("  (No subgroups in this category)\n");
        return;
    }
    
    printf("\nSubgroups in category '%s':\n", category->name);
    for (int i = 0; i < category->subgroup_count; i++) {
        printf("  %d. %s\n", i + 1, category->subgroups[i].name);
    }
}

double Category_getTotalValue(const Category* category) {
    // Validate category
    if (!category || category->subgroup_count < 0 || category->subgroup_count == 0) {
        return 0.0;
    }
    
    double total = 0.0;
    for (int i = 0; i < category->subgroup_count; i++) {
        total += Subgroup_getTotalValue(&category->subgroups[i]);
    }
    
    return total;
}

int Category_getTotalProductCount(const Category* category) {
    // Validate category
    if (!category || category->subgroup_count < 0 || category->subgroup_count == 0) {
        return 0;
    }
    
    int total = 0;
    for (int i = 0; i < category->subgroup_count; i++) {
        total += Subgroup_getProductCount(&category->subgroups[i]);
    }
    
    return total;
}

int Category_getTotalQuantity(const Category* category) {
    // Validate category
    if (!category || category->subgroup_count < 0 || category->subgroup_count == 0) {
        return 0;
    }
    
    int total = 0;
    for (int i = 0; i < category->subgroup_count; i++) {
        total += Subgroup_getTotalQuantity(&category->subgroups[i]);
    }
    
    return total;
}

int Category_getSubgroupCount(const Category* category) {
    if (!category || category->subgroup_count < 0) {
        return 0;
    }
    return category->subgroup_count;
}

int Category_isEmpty(const Category* category) {
    return !category || category->subgroup_count <= 0;
}

int Category_isValid(const Category* category) {
    return category && category->subgroup_count >= 0;
}

void Category_printStatistics(const Category* category) {
    // Validate category
    if (!category) {
        fprintf(stderr, "Error: NULL category pointer in Category_printStatistics\n");
        return;
    }
    
    if (category->subgroup_count < 0) {
        printf("Cannot print statistics for uninitialized category\n");
        return;
    }
    
    printf("\n");
    printf("============================================================\n");
    printf("Statistics for Category: %s\n", category->name);
    printf("============================================================\n");
    
    int total_products = Category_getTotalProductCount(category);
    int total_quantity = Category_getTotalQuantity(category);
    double total_value = Category_getTotalValue(category);
    
    printf("Total Subgroups:     %d\n", category->subgroup_count);
    printf("Total Products:      %d\n", total_products);
    printf("Total Quantity:      %d units\n", total_quantity);
    printf("Total Value:         $%.2f\n", total_value);
    
    if (total_products > 0) {
        printf("Average Value/Product: $%.2f\n", total_value / total_products);
        printf("Average Qty/Product:   %.1f units\n", (double)total_quantity / total_products);
    }
    
    if (category->subgroup_count > 0) {
        printf("\nPer-Subgroup Breakdown:\n");
        printf("------------------------------------------------------------\n");
        printf("%-20s %10s %10s %15s\n", "Subgroup", "Products", "Quantity", "Value");
        printf("------------------------------------------------------------\n");
        
        for (int i = 0; i < category->subgroup_count; i++) {
            Subgroup* sg = &category->subgroups[i];
            printf("%-20.20s %10d %10d $%14.2f\n",
                   sg->name,
                   Subgroup_getProductCount(sg),
                   Subgroup_getTotalQuantity(sg),
                   Subgroup_getTotalValue(sg));
        }
        printf("------------------------------------------------------------\n");
    }
    
    printf("============================================================\n");
}

/* ========== END OF CATEGORY MODULE ========== */