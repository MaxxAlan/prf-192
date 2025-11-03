/**
 * @file subgroup.h
 * @brief Subgroup management module for Product Management System
 * @author PMS Team
 * @date 2025
 * @compatible Dev-C++ 6.3, TDM-GCC 9.2.0, C11
 */

#ifndef SUBGROUP_H
#define SUBGROUP_H

#include "product.h"
#include <stdbool.h>
/**
 * @brief Display subgroup table footer
 */
void subgroup_display_table_footer(void);
/**
 * @brief Subgroup structure containing products
 */
typedef struct {
    int id;
    int category_id;
    char name[50];
    char description[200];
    Product* products;         // Dynamic array
    int product_count;
    int product_capacity;      // Initial 10, double on resize
} Subgroup;

/**
 * @brief Create a new subgroup
 * @param id Subgroup ID
 * @param category_id Parent category ID
 * @param name Subgroup name
 * @param description Subgroup description
 * @return Initialized subgroup
 */
Subgroup subgroup_create(int id, int category_id, const char* name, const char* description);

/**
 * @brief Add a product to the subgroup
 * @param subgroup Pointer to subgroup
 * @param product Product to add
 * @return true if successful, false otherwise
 */
bool subgroup_add_product(Subgroup* subgroup, Product product);

/**
 * @brief Remove a product from the subgroup by ID
 * @param subgroup Pointer to subgroup
 * @param product_id Product ID to remove
 * @return true if successful, false otherwise
 */
bool subgroup_remove_product(Subgroup* subgroup, int product_id);

/**
 * @brief Find a product in the subgroup by ID
 * @param subgroup Pointer to subgroup
 * @param product_id Product ID to find
 * @return Pointer to product if found, NULL otherwise
 */
Product* subgroup_find_product_by_id(Subgroup* subgroup, int product_id);

/**
 * @brief Display subgroup information
 * @param subgroup Pointer to subgroup
 */
void subgroup_display(const Subgroup* subgroup);

/**
 * @brief Display subgroup table header
 */
void subgroup_display_table_header(void);

/**
 * @brief Display subgroup as a table row
 * @param subgroup Pointer to subgroup
 */
void subgroup_display_table_row(const Subgroup* subgroup);

/**
 * @brief Update subgroup name
 * @param subgroup Pointer to subgroup
 * @param name New name
 * @return true if successful, false otherwise
 */
bool subgroup_update_name(Subgroup* subgroup, const char* name);

/**
 * @brief Update subgroup description
 * @param subgroup Pointer to subgroup
 * @param description New description
 * @return true if successful, false otherwise
 */
bool subgroup_update_description(Subgroup* subgroup, const char* description);

/**
 * @brief Validate subgroup data
 * @param subgroup Pointer to subgroup
 * @return true if valid, false otherwise
 */
bool subgroup_is_valid(const Subgroup* subgroup);

/**
 * @brief Free subgroup resources
 * @param subgroup Pointer to subgroup
 */
void subgroup_free(Subgroup* subgroup);

#endif // SUBGROUP_H