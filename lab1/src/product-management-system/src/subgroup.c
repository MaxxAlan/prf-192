/**
 * @file subgroup.c
 * @brief Subgroup module implementation
 * @author PMS Development Team - Member 2
 * 
 * Complete implementation of subgroup operations including dynamic array
 * management, product collection handling, and memory management.
 */

#include "subgroup.h"
#include <string.h>
#include <ctype.h>

/* ========== HELPER FUNCTIONS ========== */

/**
 * @brief Trim leading and trailing whitespace from a string
 * @param str String to trim (modified in place)
 */
static void trim_string(char* str) {
    if (!str) return;
    
    // Trim leading whitespace
    char* start = str;
    while (*start && isspace((unsigned char)*start)) {
        start++;
    }
    
    // Trim trailing whitespace
    char* end = start + strlen(start) - 1;
    while (end > start && isspace((unsigned char)*end)) {
        end--;
    }
    *(end + 1) = '\0';
    
    // Move trimmed string to beginning if needed
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
 * @brief Case-insensitive substring search
 * @param haystack String to search in
 * @param needle String to search for
 * @return 1 if needle found in haystack (case-insensitive), 0 otherwise
 */
static int strcasestr_custom(const char* haystack, const char* needle) {
    if (!haystack || !needle) return 0;
    if (*needle == '\0') return 1;
    
    size_t needle_len = strlen(needle);
    size_t haystack_len = strlen(haystack);
    
    if (needle_len > haystack_len) return 0;
    
    for (size_t i = 0; i <= haystack_len - needle_len; i++) {
        int match = 1;
        for (size_t j = 0; j < needle_len; j++) {
            if (tolower((unsigned char)haystack[i + j]) != tolower((unsigned char)needle[j])) {
                match = 0;
                break;
            }
        }
        if (match) return 1;
    }
    return 0;
}

/* ========== PUBLIC FUNCTIONS IMPLEMENTATION ========== */

Subgroup Subgroup_create(const char* name) {
    Subgroup subgroup;
    
    // Initialize with invalid state
    subgroup.name[0] = '\0';
    subgroup.products = NULL;
    subgroup.product_count = -1;
    
    // Validate name
    if (!name || is_empty_string(name)) {
        fprintf(stderr, "Error: Subgroup name cannot be empty\n");
        return subgroup;
    }
    
    // Copy name with bounds checking
    strncpy(subgroup.name, name, sizeof(subgroup.name) - 1);
    subgroup.name[sizeof(subgroup.name) - 1] = '\0';
    trim_string(subgroup.name);
    
    // Initialize empty products array
    subgroup.products = NULL;
    subgroup.product_count = 0;
    
    return subgroup;
}

void Subgroup_free(Subgroup* subgroup) {
    if (!subgroup) {
        fprintf(stderr, "Warning: NULL subgroup pointer in Subgroup_free\n");
        return;
    }
    
    // Free products array if allocated
    if (subgroup->products) {
        free(subgroup->products);
        subgroup->products = NULL;
    }
    
    // Reset state
    subgroup->product_count = 0;
    subgroup->name[0] = '\0';
}

int Subgroup_addProduct(Subgroup* subgroup, Product product) {
    // Validate subgroup pointer
    if (!subgroup) {
        fprintf(stderr, "Error: NULL subgroup pointer in Subgroup_addProduct\n");
        return 0;
    }
    
    // Validate subgroup is initialized
    if (subgroup->product_count < 0) {
        fprintf(stderr, "Error: Cannot add product to uninitialized subgroup\n");
        return 0;
    }
    
    // Validate product
    if (product.id <= 0) {
        fprintf(stderr, "Error: Cannot add product with invalid ID (%d)\n", product.id);
        return 0;
    }
    
    // Check for duplicate ID
    if (Subgroup_productExists(subgroup, product.id)) {
        fprintf(stderr, "Error: Product with ID %d already exists in subgroup '%s'\n", 
                product.id, subgroup->name);
        return 0;
    }
    
    // Allocate or resize products array
    Product* temp = (Product*)realloc(subgroup->products, 
                                      (subgroup->product_count + 1) * sizeof(Product));
    
    if (!temp) {
        fprintf(stderr, "Error: Memory allocation failed in Subgroup_addProduct\n");
        return 0;
    }
    
    subgroup->products = temp;
    
    // Add product to array
    subgroup->products[subgroup->product_count] = product;
    subgroup->product_count++;
    
    return 1;
}

int Subgroup_removeProduct(Subgroup* subgroup, int product_id) {
    // Validate subgroup pointer
    if (!subgroup) {
        fprintf(stderr, "Error: NULL subgroup pointer in Subgroup_removeProduct\n");
        return 0;
    }
    
    // Validate subgroup is initialized
    if (subgroup->product_count < 0) {
        fprintf(stderr, "Error: Cannot remove from uninitialized subgroup\n");
        return 0;
    }
    
    // Find product index
    int index = -1;
    for (int i = 0; i < subgroup->product_count; i++) {
        if (subgroup->products[i].id == product_id) {
            index = i;
            break;
        }
    }
    
    // Product not found
    if (index == -1) {
        return 0;
    }
    
    // Shift remaining products to fill the gap
    for (int i = index; i < subgroup->product_count - 1; i++) {
        subgroup->products[i] = subgroup->products[i + 1];
    }
    
    subgroup->product_count--;
    
    // If last product removed, free array
    if (subgroup->product_count == 0) {
        free(subgroup->products);
        subgroup->products = NULL;
    } else {
        // Resize array to save memory
        Product* temp = (Product*)realloc(subgroup->products, 
                                          subgroup->product_count * sizeof(Product));
        if (temp) {
            subgroup->products = temp;
        }
        // If realloc fails, keep old pointer (data is safe)
    }
    
    return 1;
}

int Subgroup_updateProduct(Subgroup* subgroup, Product product) {
    // Validate subgroup pointer
    if (!subgroup) {
        fprintf(stderr, "Error: NULL subgroup pointer in Subgroup_updateProduct\n");
        return 0;
    }
    
    // Find product by ID
    Product* existing = Subgroup_findProductById(subgroup, product.id);
    if (!existing) {
        fprintf(stderr, "Error: Product with ID %d not found in subgroup '%s'\n",
                product.id, subgroup->name);
        return 0;
    }
    
    // Update product
    *existing = product;
    return 1;
}

Product* Subgroup_findProductById(Subgroup* subgroup, int product_id) {
    // Validate subgroup pointer
    if (!subgroup || subgroup->product_count < 0) {
        return NULL;
    }
    
    // Linear search
    for (int i = 0; i < subgroup->product_count; i++) {
        if (subgroup->products[i].id == product_id) {
            return &subgroup->products[i];
        }
    }
    
    return NULL;
}

int Subgroup_searchByName(Subgroup* subgroup, const char* search_name) {
    // Validate inputs
    if (!subgroup || !search_name || subgroup->product_count < 0) {
        return 0;
    }
    
    if (is_empty_string(search_name)) {
        fprintf(stderr, "Error: Search name cannot be empty\n");
        return 0;
    }
    
    int found_count = 0;
    int first_match = 1;
    
    // Search through all products
    for (int i = 0; i < subgroup->product_count; i++) {
        if (strcasestr_custom(subgroup->products[i].name, search_name)) {
            // Print header on first match
            if (first_match) {
                printf("\nSearch results in subgroup '%s':\n", subgroup->name);
                Product_printTableHeader();
                first_match = 0;
            }
            Product_printTableRow(&subgroup->products[i]);
            found_count++;
        }
    }
    
    if (found_count > 0) {
        printf("------------------------------------------------------------\n");
        printf("Found %d matching product(s)\n", found_count);
    }
    
    return found_count;
}

int Subgroup_productExists(Subgroup* subgroup, int product_id) {
    return Subgroup_findProductById(subgroup, product_id) != NULL;
}

void Subgroup_print(const Subgroup* subgroup) {
    // Validate subgroup pointer
    if (!subgroup) {
        fprintf(stderr, "Error: NULL subgroup pointer in Subgroup_print\n");
        return;
    }
    
    // Check if initialized
    if (subgroup->product_count < 0) {
        printf("Uninitialized Subgroup\n");
        return;
    }
    
    printf("\n");
    printf("Subgroup: %s (%d product%s)\n", 
           subgroup->name, 
           subgroup->product_count,
           subgroup->product_count == 1 ? "" : "s");
    printf("============================================================\n");
    
    if (subgroup->product_count == 0) {
        printf("  (No products in this subgroup)\n");
        printf("============================================================\n");
        return;
    }
    
    // Print table header
    Product_printTableHeader();
    
    // Print all products
    for (int i = 0; i < subgroup->product_count; i++) {
        Product_printTableRow(&subgroup->products[i]);
    }
    
    printf("------------------------------------------------------------\n");
    printf("Total Value: $%.2f\n", Subgroup_getTotalValue(subgroup));
    printf("============================================================\n");
}

void Subgroup_printSummary(const Subgroup* subgroup) {
    // Validate subgroup pointer
    if (!subgroup) {
        fprintf(stderr, "Error: NULL subgroup pointer in Subgroup_printSummary\n");
        return;
    }
    
    // Check if initialized
    if (subgroup->product_count < 0) {
        printf("  - Uninitialized Subgroup\n");
        return;
    }
    
    printf("  - %s (%d product%s, Total: $%.2f)\n", 
           subgroup->name,
           subgroup->product_count,
           subgroup->product_count == 1 ? "" : "s",
           Subgroup_getTotalValue(subgroup));
}

double Subgroup_getTotalValue(const Subgroup* subgroup) {
    // Validate subgroup pointer
    if (!subgroup || subgroup->product_count < 0 || subgroup->product_count == 0) {
        return 0.0;
    }
    
    double total = 0.0;
    for (int i = 0; i < subgroup->product_count; i++) {
        total += subgroup->products[i].price * subgroup->products[i].quantity;
    }
    
    return total;
}

int Subgroup_getTotalQuantity(const Subgroup* subgroup) {
    // Validate subgroup pointer
    if (!subgroup || subgroup->product_count < 0 || subgroup->product_count == 0) {
        return 0;
    }
    
    int total = 0;
    for (int i = 0; i < subgroup->product_count; i++) {
        total += subgroup->products[i].quantity;
    }
    
    return total;
}

int Subgroup_getProductCount(const Subgroup* subgroup) {
    if (!subgroup || subgroup->product_count < 0) {
        return 0;
    }
    return subgroup->product_count;
}

int Subgroup_isEmpty(const Subgroup* subgroup) {
    return !subgroup || subgroup->product_count <= 0;
}

int Subgroup_isValid(const Subgroup* subgroup) {
    return subgroup && subgroup->product_count >= 0;
}

/* ========== END OF SUBGROUP MODULE ========== */