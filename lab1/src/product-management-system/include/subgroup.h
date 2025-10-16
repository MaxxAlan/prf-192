/**
 * @file subgroup.h
 * @brief Subgroup module header - Manages collections of products
 * @author PMS Development Team - Member 2
 * @date 2025
 * 
 * This module provides the Subgroup data structure and operations for managing
 * collections of products. Subgroups serve as intermediate containers between
 * categories and individual products in the system hierarchy.
 * 
 * Hierarchy: Category → Subgroup → Product
 */

#ifndef SUBGROUP_H
#define SUBGROUP_H

#include "product.h"

/**
 * @struct Subgroup
 * @brief Container for a collection of related products
 * 
 * Manages a dynamic array of products with automatic memory management.
 * Products within a subgroup must have unique IDs.
 */
typedef struct {
    char name[50];              /**< Subgroup name (max 49 chars + null terminator) */
    Product* products;          /**< Dynamic array of products */
    int product_count;          /**< Current number of products in the array */
} Subgroup;

/* ========== SUBGROUP LIFECYCLE ========== */

/**
 * @brief Create and initialize a new subgroup
 * 
 * Creates an empty subgroup with the given name.
 * Products array is initialized to NULL and will be allocated on first add.
 * 
 * @param name Subgroup name (must not be NULL or empty)
 * @return Subgroup Initialized subgroup structure
 * 
 * @note Returns subgroup with product_count = -1 on failure
 * 
 * Example:
 * @code
 * Subgroup sg = Subgroup_create("Laptops");
 * if (sg.product_count == -1) {
 *     printf("Failed to create subgroup\n");
 * }
 * @endcode
 */
Subgroup Subgroup_create(const char* name);

/**
 * @brief Free all memory associated with a subgroup
 * 
 * Deallocates the products array and resets the subgroup structure.
 * Safe to call multiple times on the same subgroup.
 * 
 * @param subgroup Pointer to subgroup structure (must not be NULL)
 * 
 * @note After calling this, the subgroup should not be used until re-initialized
 */
void Subgroup_free(Subgroup* subgroup);

/* ========== PRODUCT MANAGEMENT ========== */

/**
 * @brief Add a product to the subgroup
 * 
 * Adds a product to the dynamic array with duplicate ID checking.
 * Automatically resizes the array using realloc.
 * 
 * @param subgroup Pointer to subgroup structure (must not be NULL)
 * @param product Product to add (must have valid ID)
 * @return int 1 on success, 0 on failure
 * 
 * @note Fails if:
 *       - subgroup is NULL
 *       - product has invalid ID (id <= 0)
 *       - product with same ID already exists
 *       - memory allocation fails
 * 
 * Example:
 * @code
 * Product p = Product_create(1001, "Laptop", 1299.99, 10, "Gaming");
 * if (!Subgroup_addProduct(&sg, p)) {
 *     printf("Failed to add product\n");
 * }
 * @endcode
 */
int Subgroup_addProduct(Subgroup* subgroup, Product product);

/**
 * @brief Remove a product from the subgroup by ID
 * 
 * Removes the product and shifts remaining products to fill the gap.
 * Reallocates array to save memory if product is found.
 * 
 * @param subgroup Pointer to subgroup structure (must not be NULL)
 * @param product_id ID of product to remove
 * @return int 1 if product was found and removed, 0 otherwise
 * 
 * @note If last product is removed, frees the array and sets pointer to NULL
 * 
 * Example:
 * @code
 * if (Subgroup_removeProduct(&sg, 1001)) {
 *     printf("Product removed successfully\n");
 * }
 * @endcode
 */
int Subgroup_removeProduct(Subgroup* subgroup, int product_id);

/**
 * @brief Update an existing product in the subgroup
 * 
 * Finds product by ID and replaces it with updated version.
 * Useful for updating product details while maintaining collection integrity.
 * 
 * @param subgroup Pointer to subgroup structure (must not be NULL)
 * @param product Updated product (must have same ID as existing product)
 * @return int 1 if product was found and updated, 0 otherwise
 */
int Subgroup_updateProduct(Subgroup* subgroup, Product product);

/* ========== SEARCH OPERATIONS ========== */

/**
 * @brief Find a product by its ID
 * 
 * Performs linear search through the products array.
 * 
 * @param subgroup Pointer to subgroup structure (must not be NULL)
 * @param product_id ID to search for
 * @return Product* Pointer to product if found, NULL otherwise
 * 
 * @warning Returned pointer becomes invalid after Subgroup_removeProduct
 *          or Subgroup_free is called
 * 
 * Example:
 * @code
 * Product* p = Subgroup_findProductById(&sg, 1001);
 * if (p) {
 *     Product_print(p);
 * }
 * @endcode
 */
Product* Subgroup_findProductById(Subgroup* subgroup, int product_id);

/**
 * @brief Search for products by name (partial match, case-insensitive)
 * 
 * Finds all products whose names contain the search string.
 * Prints results directly to stdout.
 * 
 * @param subgroup Pointer to subgroup structure (must not be NULL)
 * @param search_name Search string (case-insensitive)
 * @return int Number of products found
 * 
 * Example:
 * @code
 * int count = Subgroup_searchByName(&sg, "lap");
 * printf("Found %d matching products\n", count);
 * @endcode
 */
int Subgroup_searchByName(Subgroup* subgroup, const char* search_name);

/**
 * @brief Check if a product with given ID exists in subgroup
 * 
 * @param subgroup Pointer to subgroup structure (must not be NULL)
 * @param product_id ID to check
 * @return int 1 if product exists, 0 otherwise
 */
int Subgroup_productExists(Subgroup* subgroup, int product_id);

/* ========== DISPLAY OPERATIONS ========== */

/**
 * @brief Print all products in the subgroup
 * 
 * Displays products in a formatted table with header.
 * Shows subgroup name and product count.
 * 
 * @param subgroup Pointer to subgroup structure (must not be NULL)
 * 
 * Example output:
 * @code
 * Subgroup: Laptops (3 products)
 * --------------------------------------------------------
 * SKU   Name                        Price         Qty
 * --------------------------------------------------------
 * 1001  Laptop Dell XPS             $1299.99      10
 * 1002  Laptop HP Pavilion          $899.99       15
 * 1003  Laptop Lenovo ThinkPad      $1499.99      8
 * --------------------------------------------------------
 * @endcode
 */
void Subgroup_print(const Subgroup* subgroup);

/**
 * @brief Print subgroup summary (name and product count only)
 * 
 * Compact display suitable for listing multiple subgroups.
 * 
 * @param subgroup Pointer to subgroup structure (must not be NULL)
 * 
 * Example output:
 * @code
 * Subgroup: Laptops (3 products)
 * @endcode
 */
void Subgroup_printSummary(const Subgroup* subgroup);

/* ========== STATISTICS & CALCULATIONS ========== */

/**
 * @brief Calculate total inventory value of all products
 * 
 * Sums up (price × quantity) for all products in the subgroup.
 * 
 * @param subgroup Pointer to subgroup structure (must not be NULL)
 * @return double Total value, or 0.0 if subgroup is NULL or empty
 * 
 * Example:
 * @code
 * double total = Subgroup_getTotalValue(&sg);
 * printf("Total inventory value: $%.2f\n", total);
 * @endcode
 */
double Subgroup_getTotalValue(const Subgroup* subgroup);

/**
 * @brief Get total quantity of all products
 * 
 * Sums up quantity field for all products in the subgroup.
 * 
 * @param subgroup Pointer to subgroup structure (must not be NULL)
 * @return int Total quantity, or 0 if subgroup is NULL or empty
 */
int Subgroup_getTotalQuantity(const Subgroup* subgroup);

/**
 * @brief Get product count in subgroup
 * 
 * @param subgroup Pointer to subgroup structure (must not be NULL)
 * @return int Number of products, or 0 if subgroup is NULL
 */
int Subgroup_getProductCount(const Subgroup* subgroup);

/* ========== UTILITY FUNCTIONS ========== */

/**
 * @brief Check if subgroup is empty
 * 
 * @param subgroup Pointer to subgroup structure (must not be NULL)
 * @return int 1 if empty (no products), 0 otherwise
 */
int Subgroup_isEmpty(const Subgroup* subgroup);

/**
 * @brief Check if subgroup is valid/initialized
 * 
 * @param subgroup Pointer to subgroup structure (must not be NULL)
 * @return int 1 if valid, 0 if invalid or uninitialized
 */
int Subgroup_isValid(const Subgroup* subgroup);

#endif /* SUBGROUP_H */