/**
 * @file product.h
 * @brief Product management module for Product Management System
 * @author PMS Team
 * @date 2025
 * @compatible Dev-C++ 6.3, TDM-GCC 9.2.0, C11
 */

#ifndef PRODUCT_H
#define PRODUCT_H

#include <stdbool.h>

/**
 * @brief Product structure
 */
typedef struct {
    int id;
    int subgroup_id;
    char code[20];
    char name[100];
    char description[200];
    float price;
    int quantity;
    char created_at[20];
    char updated_at[20];
} Product;

/**
 * @brief Create a new product
 * @param id Product ID
 * @param subgroup_id Parent subgroup ID
 * @param code Product code
 * @param name Product name
 * @param description Product description
 * @param price Product price
 * @param quantity Product quantity
 * @return Initialized product
 */
Product product_create(int id, int subgroup_id, const char* code, const char* name, 
                      const char* description, float price, int quantity);

/**
 * @brief Display product information
 * @param product Pointer to product
 */
void product_display(const Product* product);

/**
 * @brief Display product table header
 */
void product_display_table_header(void);

/**
 * @brief Display product as a table row
 * @param product Pointer to product
 */
void product_display_table_row(const Product* product);

/**
 * @brief Update product code
 * @param product Pointer to product
 * @param code New code
 * @return true if successful, false otherwise
 */
bool product_update_code(Product* product, const char* code);

/**
 * @brief Update product name
 * @param product Pointer to product
 * @param name New name
 * @return true if successful, false otherwise
 */
bool product_update_name(Product* product, const char* name);

/**
 * @brief Update product description
 * @param product Pointer to product
 * @param description New description
 * @return true if successful, false otherwise
 */
bool product_update_description(Product* product, const char* description);

/**
 * @brief Update product price
 * @param product Pointer to product
 * @param price New price
 * @return true if successful, false otherwise
 */
bool product_update_price(Product* product, float price);

/**
 * @brief Update product quantity
 * @param product Pointer to product
 * @param quantity New quantity
 * @return true if successful, false otherwise
 */
bool product_update_quantity(Product* product, int quantity);

/**
 * @brief Update product timestamp
 * @param product Pointer to product
 */
void product_update_timestamp(Product* product);

/**
 * @brief Validate product data
 * @param product Pointer to product
 * @return true if valid, false otherwise
 */
bool product_is_valid(const Product* product);

#endif // PRODUCT_H