/**
 * @file category.h
 * @brief Category management module for Product Management System
 * @author PMS Team
 * @date 2025
 * @compatible Dev-C++ 6.3, TDM-GCC 9.2.0, C11
 */

#ifndef CATEGORY_H
#define CATEGORY_H

#include "subgroup.h"
#include <stdbool.h>

/**
 * @brief Category structure containing subgroups
 */
typedef struct {
    int id;
    char name[50];
    char description[200];
    Subgroup* subgroups;       // Dynamic array
    int subgroup_count;
    int subgroup_capacity;     // Initial 10, double on resize
} Category;

/**
 * @brief Create a new category
 * @param id Category ID
 * @param name Category name
 * @param description Category description
 * @return Initialized category
 */
Category category_create(int id, const char* name, const char* description);

/**
 * @brief Add a subgroup to the category
 * @param category Pointer to category
 * @param subgroup Subgroup to add
 * @return true if successful, false otherwise
 */
bool category_add_subgroup(Category* category, Subgroup subgroup);

/**
 * @brief Remove a subgroup from the category by ID
 * @param category Pointer to category
 * @param subgroup_id Subgroup ID to remove
 * @return true if successful, false otherwise
 */
bool category_remove_subgroup(Category* category, int subgroup_id);

/**
 * @brief Find a subgroup in the category by ID
 * @param category Pointer to category
 * @param subgroup_id Subgroup ID to find
 * @return Pointer to subgroup if found, NULL otherwise
 */
Subgroup* category_find_subgroup_by_id(Category* category, int subgroup_id);

/**
 * @brief Display category information with all subgroups
 * @param category Pointer to category
 */
void category_display(const Category* category);

/**
 * @brief Display category table header
 */
void category_display_table_header(void);

/**
 * @brief Display category as a table row
 * @param category Pointer to category
 */
void category_display_table_row(const Category* category);

/**
 * @brief Display category table footer
 */
void category_display_table_footer(void);

/**
 * @brief Update category name
 * @param category Pointer to category
 * @param name New name
 * @return true if successful, false otherwise
 */
bool category_update_name(Category* category, const char* name);

/**
 * @brief Update category description
 * @param category Pointer to category
 * @param description New description
 * @return true if successful, false otherwise
 */
bool category_update_description(Category* category, const char* description);

/**
 * @brief Validate category data
 * @param category Pointer to category
 * @return true if valid, false otherwise
 */
bool category_is_valid(const Category* category);

/**
 * @brief Free category resources (including all subgroups and products)
 * @param category Pointer to category
 */
void category_free(Category* category);

#endif // CATEGORY_H