/**
 * @file product.c
 * @brief Product module implementation
 * @author PMS Development Team - Member 1
 * 
 * Complete implementation of product operations with comprehensive
 * input validation, error handling, and bounds checking.
 */

#include "product.h"
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

/* ========== PUBLIC FUNCTIONS IMPLEMENTATION ========== */

Product Product_create(int id, const char* name, double price, int quantity, const char* description) {
    Product product;
    
    // Initialize with invalid values
    product.id = -1;
    product.name[0] = '\0';
    product.price = 0.0;
    product.quantity = 0;
    product.description[0] = '\0';
    
    // Validate ID (must be positive)
    if (id <= 0) {
        fprintf(stderr, "Error: Product ID must be positive (got %d)\n", id);
        return product;
    }
    
    // Validate name
    if (!name || is_empty_string(name)) {
        fprintf(stderr, "Error: Product name cannot be empty\n");
        return product;
    }
    
    // Validate price
    if (price < 0) {
        fprintf(stderr, "Error: Product price cannot be negative (got %.2f)\n", price);
        return product;
    }
    
    // Validate quantity
    if (quantity < 0) {
        fprintf(stderr, "Error: Product quantity cannot be negative (got %d)\n", quantity);
        return product;
    }
    
    // All validations passed, create product
    product.id = id;
    
    // Copy name safely with bounds checking
    strncpy(product.name, name, sizeof(product.name) - 1);
    product.name[sizeof(product.name) - 1] = '\0';
    trim_string(product.name);
    
    product.price = price;
    product.quantity = quantity;
    
    // Copy description (can be NULL)
    if (description && !is_empty_string(description)) {
        strncpy(product.description, description, sizeof(product.description) - 1);
        product.description[sizeof(product.description) - 1] = '\0';
        trim_string(product.description);
    } else {
        strcpy(product.description, "No description");
    }
    
    return product;
}

int Product_updatePrice(Product* product, double new_price) {
    // Validate product pointer
    if (!product) {
        fprintf(stderr, "Error: NULL product pointer in Product_updatePrice\n");
        return 0;
    }
    
    // Validate product is initialized
    if (product->id <= 0) {
        fprintf(stderr, "Error: Cannot update uninitialized product\n");
        return 0;
    }
    
    // Validate new price
    if (new_price < 0) {
        fprintf(stderr, "Error: Price cannot be negative (got %.2f)\n", new_price);
        return 0;
    }
    
    product->price = new_price;
    return 1;
}

int Product_updateQuantity(Product* product, int new_quantity) {
    // Validate product pointer
    if (!product) {
        fprintf(stderr, "Error: NULL product pointer in Product_updateQuantity\n");
        return 0;
    }
    
    // Validate product is initialized
    if (product->id <= 0) {
        fprintf(stderr, "Error: Cannot update uninitialized product\n");
        return 0;
    }
    
    // Validate new quantity
    if (new_quantity < 0) {
        fprintf(stderr, "Error: Quantity cannot be negative (got %d)\n", new_quantity);
        return 0;
    }
    
    product->quantity = new_quantity;
    return 1;
}

int Product_updateName(Product* product, const char* new_name) {
    // Validate product pointer
    if (!product) {
        fprintf(stderr, "Error: NULL product pointer in Product_updateName\n");
        return 0;
    }
    
    // Validate product is initialized
    if (product->id <= 0) {
        fprintf(stderr, "Error: Cannot update uninitialized product\n");
        return 0;
    }
    
    // Validate new name
    if (!new_name || is_empty_string(new_name)) {
        fprintf(stderr, "Error: Product name cannot be empty\n");
        return 0;
    }
    
    // Update name with bounds checking
    strncpy(product->name, new_name, sizeof(product->name) - 1);
    product->name[sizeof(product->name) - 1] = '\0';
    trim_string(product->name);
    
    return 1;
}

int Product_updateDescription(Product* product, const char* new_description) {
    // Validate product pointer
    if (!product) {
        fprintf(stderr, "Error: NULL product pointer in Product_updateDescription\n");
        return 0;
    }
    
    // Validate product is initialized
    if (product->id <= 0) {
        fprintf(stderr, "Error: Cannot update uninitialized product\n");
        return 0;
    }
    
    // Update description (can be NULL or empty)
    if (new_description && !is_empty_string(new_description)) {
        strncpy(product->description, new_description, sizeof(product->description) - 1);
        product->description[sizeof(product->description) - 1] = '\0';
        trim_string(product->description);
    } else {
        strcpy(product->description, "No description");
    }
    
    return 1;
}

void Product_print(const Product* product) {
    // Validate product pointer
    if (!product) {
        fprintf(stderr, "Error: NULL product pointer in Product_print\n");
        return;
    }
    
    // Validate product is initialized
    if (product->id <= 0) {
        printf("Invalid/Uninitialized Product\n");
        return;
    }
    
    printf("\n");
    printf("Product Details:\n");
    printf("================\n");
    printf("SKU:         %d\n", product->id);
    printf("Name:        %s\n", product->name);
    printf("Price:       $%.2f\n", product->price);
    printf("Quantity:    %d\n", product->quantity);
    printf("Total Value: $%.2f\n", product->price * product->quantity);
    printf("Description: %s\n", product->description);
    printf("================\n");
}

void Product_printTableRow(const Product* product) {
    // Validate product pointer
    if (!product) {
        fprintf(stderr, "Error: NULL product pointer in Product_printTableRow\n");
        return;
    }
    
    // Validate product is initialized
    if (product->id <= 0) {
        printf("%-6s %-30s %12s %8s\n", "N/A", "Invalid Product", "N/A", "N/A");
        return;
    }
    
    // Print in fixed-width columns for alignment
    printf("%-6d %-30.30s $%11.2f %8d\n", 
           product->id, 
           product->name, 
           product->price, 
           product->quantity);
}

void Product_printTableHeader(void) {
    printf("\n");
    printf("%-6s %-30s %12s %8s\n", "SKU", "Name", "Price", "Qty");
    printf("------------------------------------------------------------\n");
}

/* ========== END OF PRODUCT MODULE ========== */