/**
 * @file product.c
 * @brief Product management implementation (FIXED UI)
 * @author PMS Team
 * @date 2025
 * @compatible Dev-C++ 6.3, TDM-GCC 9.2.0, C11
 */

#include "../include/product.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

/**
 * @brief Trim whitespace from string (helper function)
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

/**
 * @brief Get current timestamp (helper function)
 */
static void get_current_timestamp(char* buffer, size_t size) {
    if (!buffer || size < 20) return;
    
    time_t now = time(NULL);
    struct tm* tm_info = localtime(&now);
    
    strftime(buffer, size, "%Y-%m-%d %H:%M:%S", tm_info);
}

Product product_create(int id, int subgroup_id, const char* code, const char* name, 
                      const char* description, float price, int quantity) {
    Product product;
    
    product.id = id;
    product.subgroup_id = subgroup_id;
    product.price = price;
    product.quantity = quantity;
    
    // Copy and trim code
    strncpy(product.code, code ? code : "", sizeof(product.code) - 1);
    product.code[sizeof(product.code) - 1] = '\0';
    trim_string(product.code);
    
    // Copy and trim name
    strncpy(product.name, name ? name : "", sizeof(product.name) - 1);
    product.name[sizeof(product.name) - 1] = '\0';
    trim_string(product.name);
    
    // Copy and trim description
    strncpy(product.description, description ? description : "", sizeof(product.description) - 1);
    product.description[sizeof(product.description) - 1] = '\0';
    trim_string(product.description);
    
    // Set timestamps
    get_current_timestamp(product.created_at, sizeof(product.created_at));
    strcpy(product.updated_at, product.created_at);
    
    return product;
}

void product_display(const Product* product) {
    if (!product) {
        printf("Error: Product pointer is NULL\n");
        return;
    }
    
    printf("\n╔══════════════════════════════════════════════════════════╗\n");
    printf(  "║   Product Information                                    ║\n");
    printf(  "╚══════════════════════════════════════════════════════════╝\n");
    printf("  ID:          %d\n", product->id);
    printf("  Subgroup ID: %d\n", product->subgroup_id);
    printf("  Code:        %s\n", product->code);
    printf("  Name:        %s\n", product->name);
    printf("  Description: %s\n", product->description);
    printf("  Price:       $%.2f\n", product->price);
    printf("  Quantity:    %d\n", product->quantity);
    printf("  Total Value: $%.2f\n", product->price * product->quantity);
    printf("  Created:     %s\n", product->created_at);
    printf("  Updated:     %s\n", product->updated_at);
    printf("\n");
}

void product_display_table_header(void) {
    printf("  ┌────────┬──────────┬────────────┬──────────────────────┬──────────────┬──────────┐\n");
    printf("  │   ID   │ Sub ID   │ Code       │ Name                 │    Price     │ Quantity │\n");
    printf("  ├────────┼──────────┼────────────┼──────────────────────┼──────────────┼──────────┤\n");
}

void product_display_table_row(const Product* product) {
    if (!product) return;
    
    // Truncate name if too long for display
    char display_name[22];
    strncpy(display_name, product->name, 20);
    display_name[20] = '\0';
    if (strlen(product->name) > 20) {
        display_name[17] = '.';
        display_name[18] = '.';
        display_name[19] = '.';
        display_name[20] = '\0';
    }
    
    printf("  │ %-6d │ %-8d │ %-10s │ %-20s │ $%11.2f │ %-8d │\n",
           product->id,
           product->subgroup_id,
           product->code,
           display_name,
           product->price,
           product->quantity);
}

void product_display_table_footer(void) {
    printf("  └────────┴──────────┴────────────┴──────────────────────┴──────────────┴──────────┘\n");
}

bool product_update_code(Product* product, const char* code) {
    if (!product) {
        fprintf(stderr, "Error: Product pointer is NULL\n");
        return false;
    }
    
    if (!code || strlen(code) == 0) {
        fprintf(stderr, "Error: Code cannot be empty\n");
        return false;
    }
    
    strncpy(product->code, code, sizeof(product->code) - 1);
    product->code[sizeof(product->code) - 1] = '\0';
    trim_string(product->code);
    
    if (strlen(product->code) == 0) {
        fprintf(stderr, "Error: Code cannot be empty after trimming\n");
        return false;
    }
    
    product_update_timestamp(product);
    return true;
}

bool product_update_name(Product* product, const char* name) {
    if (!product) {
        fprintf(stderr, "Error: Product pointer is NULL\n");
        return false;
    }
    
    if (!name || strlen(name) == 0) {
        fprintf(stderr, "Error: Name cannot be empty\n");
        return false;
    }
    
    strncpy(product->name, name, sizeof(product->name) - 1);
    product->name[sizeof(product->name) - 1] = '\0';
    trim_string(product->name);
    
    if (strlen(product->name) == 0) {
        fprintf(stderr, "Error: Name cannot be empty after trimming\n");
        return false;
    }
    
    product_update_timestamp(product);
    return true;
}

bool product_update_description(Product* product, const char* description) {
    if (!product) {
        fprintf(stderr, "Error: Product pointer is NULL\n");
        return false;
    }
    
    if (!description) {
        description = "";
    }
    
    strncpy(product->description, description, sizeof(product->description) - 1);
    product->description[sizeof(product->description) - 1] = '\0';
    trim_string(product->description);
    
    product_update_timestamp(product);
    return true;
}

bool product_update_price(Product* product, float price) {
    if (!product) {
        fprintf(stderr, "Error: Product pointer is NULL\n");
        return false;
    }
    
    if (price < 0) {
        fprintf(stderr, "Error: Price cannot be negative\n");
        return false;
    }
    
    product->price = price;
    product_update_timestamp(product);
    return true;
}

bool product_update_quantity(Product* product, int quantity) {
    if (!product) {
        fprintf(stderr, "Error: Product pointer is NULL\n");
        return false;
    }
    
    if (quantity < 0) {
        fprintf(stderr, "Error: Quantity cannot be negative\n");
        return false;
    }
    
    product->quantity = quantity;
    product_update_timestamp(product);
    return true;
}

void product_update_timestamp(Product* product) {
    if (!product) return;
    
    get_current_timestamp(product->updated_at, sizeof(product->updated_at));
}

bool product_is_valid(const Product* product) {
    if (!product) {
        return false;
    }
    
    if (product->id <= 0) {
        return false;
    }
    
    if (product->subgroup_id <= 0) {
        return false;
    }
    
    if (strlen(product->code) == 0) {
        return false;
    }
    
    if (strlen(product->name) == 0) {
        return false;
    }
    
    if (product->price < 0) {
        return false;
    }
    
    if (product->quantity < 0) {
        return false;
    }
    
    return true;
}