/**
 * @file category.h
 * @brief Category module header - Manages collections of subgroups
 * @author PMS Development Team - Member 3
 * @date 2025
 * 
 * This module provides the Category data structure and operations for managing
 * collections of subgroups. Categories serve as top-level containers in the
 * system hierarchy, organizing subgroups which in turn contain products.
 * 
 * Hierarchy: Category → Subgroup → Product
 */

#ifndef CATEGORY_H
#define CATEGORY_H

#include "subgroup.h"

/**
 * @struct Category
 * @brief Top-level container for organizing subgroups
 * 
 * Manages a dynamic array of subgroups with automatic memory management.
 * Subgroups within a category must have unique names (case-insensitive).
 */
typedef struct {
    char name[50];              /**< Category name (max 49 chars + null terminator) */
    Subgroup* subgroups;        /**< Dynamic array of subgroups */
    int subgroup_count;         /**< Current number of subgroups in the array */
} Category;

/* ========== CATEGORY LIFECYCLE ========== */

/**
 * @brief Create and initialize a new category
 * 
 * Creates an empty category with the given name.
 * Subgroups array is initialized to NULL and will be allocated on first add.
 * 
 * @param name Category name (must not be NULL or empty)
 * @return Category Initialized category structure
 * 
 * @note Returns category with subgroup_count = -1 on failure
 * 
 * Example:
 * @code
 * Category cat = Category_create("Electronics");
 * if (cat.subgroup_count == -1) {
 *     printf("Failed to create category\n");
 * }
 * @endcode
 */
Category Category_create(const char* name);

/**
 * @brief Free all memory associated with a category
 * 
 * Deallocates all subgroups (which in turn frees their products),
 * then frees the subgroups array and resets the category structure.
 * Safe to call multiple times on the same category.
 * 
 * @param category Pointer to category structure (must not be NULL)
 * 
 * @note This performs deep cleanup of the entire hierarchy
 */
void Category_free(Category* category);

/* ========== SUBGROUP MANAGEMENT ========== */

/**
 * @brief Add a subgroup to the category
 * 
 * Adds a subgroup to the dynamic array with duplicate name checking
 * (case-insensitive). Automatically resizes the array using realloc.
 * 
 * @param category Pointer to category structure (must not be NULL)
 * @param subgroup Subgroup to add (must be valid/initialized)
 * @return int 1 on success, 0 on failure
 * 
 * @note Fails if:
 *       - category is NULL
 *       - subgroup is invalid/uninitialized
 *       - subgroup with same name already exists (case-insensitive)
 *       - memory allocation fails
 * 
 * Example:
 * @code
 * Subgroup sg = Subgroup_create("Laptops");
 * if (!Category_addSubgroup(&cat, sg)) {
 *     printf("Failed to add subgroup\n");
 * }
 * @endcode
 */
int Category_addSubgroup(Category* category, Subgroup subgroup);

/**
 * @brief Remove a subgroup from the category by name
 * 
 * Removes the subgroup, frees its resources, and shifts remaining subgroups
 * to fill the gap. Reallocates array to save memory.
 * Search is case-insensitive.
 * 
 * @param category Pointer to category structure (must not be NULL)
 * @param subgroup_name Name of subgroup to remove (case-insensitive)
 * @return int 1 if subgroup was found and removed, 0 otherwise
 * 
 * @note If last subgroup is removed, frees the array and sets pointer to NULL
 * 
 * Example:
 * @code
 * if (Category_removeSubgroup(&cat, "Laptops")) {
 *     printf("Subgroup removed successfully\n");
 * }
 * @endcode
 */
int Category_removeSubgroup(Category* category, const char* subgroup_name);

/**
 * @brief Update an existing subgroup in the category
 * 
 * Finds subgroup by name (case-insensitive) and replaces it with updated version.
 * Useful for updating subgroup details while maintaining collection integrity.
 * 
 * @param category Pointer to category structure (must not be NULL)
 * @param subgroup Updated subgroup (must have same name as existing)
 * @return int 1 if subgroup was found and updated, 0 otherwise
 */
int Category_updateSubgroup(Category* category, Subgroup subgroup);

/* ========== SEARCH OPERATIONS ========== */

/**
 * @brief Find a subgroup by its name (case-insensitive)
 * 
 * Performs linear search through the subgroups array.
 * 
 * @param category Pointer to category structure (must not be NULL)
 * @param subgroup_name Name to search for (case-insensitive)
 * @return Subgroup* Pointer to subgroup if found, NULL otherwise
 * 
 * @warning Returned pointer becomes invalid after Category_removeSubgroup
 *          or Category_free is called
 * 
 * Example:
 * @code
 * Subgroup* sg = Category_findSubgroupByName(&cat, "laptops");
 * if (sg) {
 *     Subgroup_print(sg);
 * }
 * @endcode
 */
Subgroup* Category_findSubgroupByName(Category* category, const char* subgroup_name);

/**
 * @brief Find a product by ID across all subgroups in the category
 * 
 * Searches through all subgroups in the category for a product with
 * the specified ID.
 * 
 * @param category Pointer to category structure (must not be NULL)
 * @param product_id Product ID to search for
 * @return Product* Pointer to product if found, NULL otherwise
 * 
 * @note Returns first match found (product IDs should be globally unique)
 */
Product* Category_findProductById(Category* category, int product_id);

/**
 * @brief Search for products by name across all subgroups (partial match)
 * 
 * Searches through all subgroups for products whose names contain
 * the search string (case-insensitive).
 * Prints results directly to stdout, organized by subgroup.
 * 
 * @param category Pointer to category structure (must not be NULL)
 * @param search_name Search string (case-insensitive)
 * @return int Total number of products found across all subgroups
 * 
 * Example:
 * @code
 * int count = Category_searchProductsByName(&cat, "dell");
 * printf("Found %d matching products\n", count);
 * @endcode
 */
int Category_searchProductsByName(Category* category, const char* search_name);

/**
 * @brief Check if a subgroup with given name exists in category
 * 
 * @param category Pointer to category structure (must not be NULL)
 * @param subgroup_name Name to check (case-insensitive)
 * @return int 1 if subgroup exists, 0 otherwise
 */
int Category_subgroupExists(Category* category, const char* subgroup_name);

/* ========== DISPLAY OPERATIONS ========== */

/**
 * @brief Print entire category hierarchy (all subgroups and products)
 * 
 * Displays the complete category structure in a hierarchical format,
 * showing all subgroups and their products.
 * 
 * @param category Pointer to category structure (must not be NULL)
 * 
 * Example output:
 * @code
 * ============================================================
 * Category: Electronics (2 subgroups, 5 products)
 * ============================================================
 * 
 * Subgroup: Laptops (3 products)
 * ============================================================
 * SKU   Name                        Price         Qty
 * ------------------------------------------------------------
 * 1001  Laptop Dell XPS             $1299.99      10
 * ...
 * @endcode
 */
void Category_print(const Category* category);

/**
 * @brief Print category summary (name, counts, and subgroup list)
 * 
 * Compact display showing category overview with subgroup summaries.
 * Does not show individual products.
 * 
 * @param category Pointer to category structure (must not be NULL)
 * 
 * Example output:
 * @code
 * Category: Electronics (2 subgroups, 5 products, $15,234.50)
 *   - Laptops (3 products, Total: $12,499.70)
 *   - Tablets (2 products, Total: $2,734.80)
 * @endcode
 */
void Category_printSummary(const Category* category);

/**
 * @brief Print only subgroup names in the category
 * 
 * Simple list of subgroup names without additional details.
 * 
 * @param category Pointer to category structure (must not be NULL)
 */
void Category_printSubgroupList(const Category* category);

/* ========== STATISTICS & CALCULATIONS ========== */

/**
 * @brief Calculate total inventory value of all products in category
 * 
 * Sums up the total value across all subgroups in the category.
 * 
 * @param category Pointer to category structure (must not be NULL)
 * @return double Total value, or 0.0 if category is NULL or empty
 * 
 * Example:
 * @code
 * double total = Category_getTotalValue(&cat);
 * printf("Total category value: $%.2f\n", total);
 * @endcode
 */
double Category_getTotalValue(const Category* category);

/**
 * @brief Get total number of products across all subgroups
 * 
 * Counts products in all subgroups in the category.
 * 
 * @param category Pointer to category structure (must not be NULL)
 * @return int Total product count, or 0 if category is NULL or empty
 */
int Category_getTotalProductCount(const Category* category);

/**
 * @brief Get total quantity of all products across all subgroups
 * 
 * Sums up quantity field for all products in all subgroups.
 * 
 * @param category Pointer to category structure (must not be NULL)
 * @return int Total quantity, or 0 if category is NULL or empty
 */
int Category_getTotalQuantity(const Category* category);

/**
 * @brief Get number of subgroups in category
 * 
 * @param category Pointer to category structure (must not be NULL)
 * @return int Number of subgroups, or 0 if category is NULL
 */
int Category_getSubgroupCount(const Category* category);

/* ========== UTILITY FUNCTIONS ========== */

/**
 * @brief Check if category is empty (no subgroups)
 * 
 * @param category Pointer to category structure (must not be NULL)
 * @return int 1 if empty (no subgroups), 0 otherwise
 */
int Category_isEmpty(const Category* category);

/**
 * @brief Check if category is valid/initialized
 * 
 * @param category Pointer to category structure (must not be NULL)
 * @return int 1 if valid, 0 if invalid or uninitialized
 */
int Category_isValid(const Category* category);

/**
 * @brief Get detailed statistics for the category
 * 
 * Prints comprehensive statistics including:
 * - Total subgroups, products, quantity
 * - Total inventory value
 * - Per-subgroup breakdown
 * - Average values
 * 
 * @param category Pointer to category structure (must not be NULL)
 */
void Category_printStatistics(const Category* category);

#endif /* CATEGORY_H */