/**
 * @file product.h
 * @brief Product module header - Core data structure and operations for products
 * @author PMS Development Team - Member 1
 * @date 2025
 * 
 * This module provides the fundamental Product data structure and all operations
 * for managing individual products including creation, updates, and display.
 */

#ifndef PRODUCT_H
#define PRODUCT_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/**
 * @struct Product
 * @brief Core product data structure
 * 
 * Represents a single product with all necessary attributes for inventory management.
 * SKU (id) serves as unique identifier across the entire system.
 */
typedef struct {
    int id;                     /**< SKU - Unique product identifier */
    char name[100];             /**< Product name (max 99 chars + null terminator) */
    double price;               /**< Unit price (must be >= 0) */
    int quantity;               /**< Stock quantity (must be >= 0) */
    char description[256];      /**< Product description (max 255 chars) */
} Product;

/* ========== PRODUCT CREATION & INITIALIZATION ========== */

/**
 * @brief Create and initialize a new product
 * 
 * Creates a product with full validation of input parameters.
 * All string inputs are safely copied with bounds checking.
 * 
 * @param id Product SKU (must be positive)
 * @param name Product name (must not be NULL or empty)
 * @param price Unit price (must be >= 0)
 * @param quantity Stock quantity (must be >= 0)
 * @param description Product description (can be NULL for empty description)
 * @return Product Initialized product structure
 * 
 * @note If validation fails, returns a product with id = -1
 * 
 * Example:
 * @code
 * Product p = Product_create(1001, "Laptop Dell XPS", 1299.99, 10, "High-end laptop");
 * if (p.id == -1) {
 *     printf("Failed to create product\n");
 * }
 * @endcode
 */
Product Product_create(int id, const char* name, double price, int quantity, const char* description);

/* ========== UPDATE OPERATIONS ========== */

/**
 * @brief Update product price
 * 
 * @param product Pointer to product structure (must not be NULL)
 * @param new_price New price value (must be >= 0)
 * @return int 1 on success, 0 on failure
 * 
 * @note Validates that price is non-negative before updating
 */
int Product_updatePrice(Product* product, double new_price);

/**
 * @brief Update product quantity
 * 
 * @param product Pointer to product structure (must not be NULL)
 * @param new_quantity New quantity value (must be >= 0)
 * @return int 1 on success, 0 on failure
 * 
 * @note Validates that quantity is non-negative before updating
 */
int Product_updateQuantity(Product* product, int new_quantity);

/**
 * @brief Update product name
 * 
 * Safely updates product name with bounds checking and validation.
 * 
 * @param product Pointer to product structure (must not be NULL)
 * @param new_name New name string (must not be NULL or empty)
 * @return int 1 on success, 0 on failure
 * 
 * @note Name is truncated if longer than 99 characters
 */
int Product_updateName(Product* product, const char* new_name);

/**
 * @brief Update product description
 * 
 * Safely updates product description with bounds checking.
 * 
 * @param product Pointer to product structure (must not be NULL)
 * @param new_description New description string (can be NULL for empty)
 * @return int 1 on success, 0 on failure
 * 
 * @note Description is truncated if longer than 255 characters
 */
int Product_updateDescription(Product* product, const char* new_description);

/* ========== DISPLAY OPERATIONS ========== */

/**
 * @brief Print detailed product information
 * 
 * Displays all product attributes in a formatted, readable layout.
 * Suitable for detailed product views.
 * 
 * @param product Pointer to product structure (must not be NULL)
 * 
 * Example output:
 * @code
 * Product Details:
 * ================
 * SKU: 1001
 * Name: Laptop Dell XPS
 * Price: $1299.99
 * Quantity: 10
 * Description: High-end laptop
 * @endcode
 */
void Product_print(const Product* product);

/**
 * @brief Print product as a table row
 * 
 * Displays product in a compact table format for listing multiple products.
 * Uses fixed-width columns for alignment.
 * 
 * @param product Pointer to product structure (must not be NULL)
 * 
 * Example output:
 * @code
 * 1001  Laptop Dell XPS             $1299.99      10
 * @endcode
 */
void Product_printTableRow(const Product* product);

/**
 * @brief Print table header for product listings
 * 
 * Prints the header row matching the format of Product_printTableRow().
 * Call this before printing multiple products in table format.
 * 
 * Example output:
 * @code
 * SKU   Name                        Price         Qty
 * --------------------------------------------------------
 * @endcode
 */
void Product_printTableHeader(void);

#endif /* PRODUCT_H */