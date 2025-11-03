/**
 * @file utils.h
 * @brief Utility functions and DataStore for Product Management System
 * @author PMS Team
 * @date 2025
 * @compatible Dev-C++ 6.3, TDM-GCC 9.2.0, C11
 */

#ifndef UTILS_H
#define UTILS_H

#include <stddef.h>
#include <stdbool.h>
#include "category.h"

// ============================================================================
// Color codes for Windows console
// ============================================================================

typedef enum {
    COLOR_RESET = 7,      // Light Gray (default)
    COLOR_HEADER = 11,    // Cyan
    COLOR_SUCCESS = 10,   // Green
    COLOR_ERROR = 12,     // Red
    COLOR_WARNING = 14,   // Yellow
    COLOR_INPUT = 15,     // White
    COLOR_INFO = 9        // Blue
} ConsoleColor;

/**
 * @brief Set console text color (Windows)
 * @param color Color code from ConsoleColor enum
 */
void set_color(ConsoleColor color);

// ============================================================================
// Core types
// ============================================================================

typedef struct {
    Category* categories;
    int category_count;
    int category_capacity;     // Initial 10, double on resize
    
    int next_category_id;      // Start from 1
    int next_subgroup_id;
    int next_product_id;
    
    bool is_modified;
    char last_saved[20];
} DataStore;

typedef struct {
    int total_categories;
    int total_subgroups;
    int total_products;
    float total_value;
    float average_price;
    int total_quantity;
} Statistics;

typedef struct {
    Product* products;
    int count;
} SearchResult;

// ============================================================================
// Helper / I/O functions (public)
// ============================================================================

void trim_string(char* str);
void get_current_timestamp(char* buffer, size_t size);
void clear_screen(void);
void pause_screen(void);

bool safe_input_string(const char* prompt, char* buffer, size_t size);
bool safe_input_int(const char* prompt, int* value);
bool safe_input_float(const char* prompt, float* value);

// ============================================================================
// Datastore lookup / search / reporting
// ============================================================================

Category* datastore_find_category_by_id(DataStore* store, int category_id);
Subgroup* datastore_find_subgroup_by_id(DataStore* store, int subgroup_id);
Product* datastore_find_product_by_id(DataStore* store, int product_id);

SearchResult datastore_search_products_by_name(DataStore* store, const char* name);
SearchResult datastore_search_products_by_price(DataStore* store, float min_price, float max_price);
SearchResult datastore_search_products_by_quantity(DataStore* store, int min_qty, int max_qty);
void search_result_free(SearchResult* result);

Statistics datastore_get_statistics(DataStore* store);
void datastore_display_all(DataStore* store);

// ============================================================================
// DataStore Management
// ============================================================================

/**
 * @brief Initialize the data store
 * @return Initialized DataStore
 */
DataStore datastore_init(void);

/**
 * @brief Free all data store resources
 * @param store Pointer to DataStore
 */
void datastore_free(DataStore* store);

/**
 * @brief Load data from file
 * @param store Pointer to DataStore
 * @param filename Filename to load from
 * @return true if successful, false otherwise
 */
bool datastore_load(DataStore* store, const char* filename);

/**
 * @brief Save data to file with backup
 * @param store Pointer to DataStore
 * @param filename Filename to save to
 * @return true if successful, false otherwise
 */
bool datastore_save(DataStore* store, const char* filename);

/**
 * @brief Add a category to the data store
 * @param store Pointer to DataStore
 * @param category Category to add
 * @return true if successful, false otherwise
 */
bool datastore_add_category(DataStore* store, Category category);

/**
 * @brief Remove a category from the data store
 * @param store Pointer to DataStore
 * @param category_id Category ID to remove
 * @return true if successful, false otherwise
 */
bool datastore_remove_category(DataStore* store, int category_id);

#endif // UTILS_H