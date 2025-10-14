/**
 * @file product.h
 * @brief Product structure and function declarations
 * @details Defines the Product data structure and operations for product management
 * @author PMS Team
 * @date 14/10/2025
 */

#ifndef PRODUCT_H
#define PRODUCT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ========================================================================
 * CONSTANTS AND MACROS
 * ======================================================================== */

#define MAX_NAME_LENGTH 100
#define MAX_DESCRIPTION_LENGTH 256

/* ========================================================================
 * DATA STRUCTURES
 * ======================================================================== */

/**
 * @struct Product
 * @brief Represents a product in the inventory system
 * 
 * @var Product::id
 * Unique product identifier (SKU)
 * 
 * @var Product::name
 * Product name (max 100 characters)
 * 
 * @var Product::price
 * Product price (must be non-negative)
 * 
 * @var Product::quantity
 * Stock quantity (must be non-negative)
 * 
 * @var Product::description
 * Product description (max 256 characters)
 */
typedef struct {
    int id;
    char name[MAX_NAME_LENGTH];
    double price;
    int quantity;
    char description[MAX_DESCRIPTION_LENGTH];
} Product;

/* ========================================================================
 * FUNCTION PROTOTYPES
 * ======================================================================== */

/**
 * @brief Creates and initializes a new product
 * @param id Product SKU (unique identifier)
 * @param name Product name
 * @param price Product price
 * @param quantity Stock quantity
 * @param description Product description
 * @return Initialized Product structure
 */
Product Product_create(int id, const char* name, double price, int quantity, const char* description);

/**
 * @brief Updates product price
 * @param product Pointer to the product to update
 * @param new_price New price value (must be >= 0)
 * @return 1 if successful, 0 if invalid price
 */
int Product_updatePrice(Product* product, double new_price);

/**
 * @brief Updates product quantity
 * @param product Pointer to the product to update
 * @param new_quantity New quantity value (must be >= 0)
 * @return 1 if successful, 0 if invalid quantity
 */
int Product_updateQuantity(Product* product, int new_quantity);

/**
 * @brief Updates product name
 * @param product Pointer to the product to update
 * @param new_name New product name
 */
void Product_updateName(Product* product, const char* new_name);

/**
 * @brief Updates product description
 * @param product Pointer to the product to update
 * @param new_description New description
 */
void Product_updateDescription(Product* product, const char* new_description);

/**
 * @brief Prints product details to console
 * @param product Pointer to the product to print
 */
void Product_print(const Product* product);

/**
 * @brief Prints product details in a formatted table row
 * @param product Pointer to the product to print
 */
void Product_printTableRow(const Product* product);

#endif // PRODUCT_H