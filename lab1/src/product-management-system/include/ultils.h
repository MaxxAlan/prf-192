/**
 * @file utils.h
 * @brief Utilities and Data Store Management
 * @description Core module for data persistence, validation, and system utilities
 * 
 * Features:
 * - Centralized data store (DataStore)
 * - File I/O operations (save/load)
 * - Input validation and sanitization
 * - Search and filtering utilities
 * - Memory management helpers
 * 
 * @author Product Management System
 * @date 2025
 */

#ifndef UTILS_H
#define UTILS_H

#include "category.h"
#include <stdbool.h>

/* ========================================================================
 * DATA STORE - Centralized Storage
 * ======================================================================== */

/**
 * @struct DataStore
 * @brief Central repository for all system data
 */
typedef struct {
    Category* categories;        /**< Array of categories */
    int category_count;          /**< Number of categories */
    int category_capacity;       /**< Allocated capacity */
    
    int next_category_id;        /**< Next available category ID */
    int next_subgroup_id;        /**< Next available subgroup ID */
    int next_product_id;         /**< Next available product ID */
    
    bool is_modified;            /**< Flag for unsaved changes */
    char last_saved[20];         /**< Last save timestamp */
} DataStore;

/* ========================================================================
 * DATA STORE OPERATIONS
 * ======================================================================== */

/**
 * @brief Initialize data store
 * @return Pointer to new DataStore or NULL on failure
 */
DataStore* datastore_init(void);

/**
 * @brief Free data store and all contained data
 * @param store DataStore to free
 */
void datastore_free(DataStore* store);

/**
 * @brief Add category to data store
 * @param store DataStore instance
 * @param category Category to add
 * @return true on success, false on failure
 */
bool datastore_add_category(DataStore* store, Category category);

/**
 * @brief Remove category from data store
 * @param store DataStore instance
 * @param category_id ID of category to remove
 * @return true on success, false if not found
 */
bool datastore_remove_category(DataStore* store, int category_id);

/**
 * @brief Find category by ID
 * @param store DataStore instance
 * @param category_id Category ID to search
 * @return Pointer to Category or NULL if not found
 */
Category* datastore_find_category(DataStore* store, int category_id);

/**
 * @brief Get next available category ID
 * @param store DataStore instance
 * @return Next category ID
 */
int datastore_next_category_id(DataStore* store);

/**
 * @brief Get next available subgroup ID
 * @param store DataStore instance
 * @return Next subgroup ID
 */
int datastore_next_subgroup_id(DataStore* store);

/**
 * @brief Get next available product ID
 * @param store DataStore instance
 * @return Next product ID
 */
int datastore_next_product_id(DataStore* store);

/* ========================================================================
 * FILE I/O OPERATIONS
 * ======================================================================== */

/**
 * @brief Save data store to file
 * @param store DataStore to save
 * @param filename Target file path
 * @return true on success, false on failure
 */
bool datastore_save(DataStore* store, const char* filename);

/**
 * @brief Load data store from file
 * @param filename Source file path
 * @return Loaded DataStore or NULL on failure
 */
DataStore* datastore_load(const char* filename);

/**
 * @brief Check if file exists
 * @param filename File path to check
 * @return true if exists, false otherwise
 */
bool file_exists(const char* filename);

/**
 * @brief Create backup of data file
 * @param filename File to backup
 * @return true on success, false on failure
 */
bool create_backup(const char* filename);

/* ========================================================================
 * INPUT VALIDATION
 * ======================================================================== */

/**
 * @brief Read and validate integer input
 * @param prompt Message to display
 * @param min Minimum allowed value
 * @param max Maximum allowed value
 * @return Validated integer
 */
int read_int(const char* prompt, int min, int max);

/**
 * @brief Read and validate float input
 * @param prompt Message to display
 * @param min Minimum allowed value
 * @param max Maximum allowed value
 * @return Validated float
 */
float read_float(const char* prompt, float min, float max);

/**
 * @brief Read and sanitize string input
 * @param prompt Message to display
 * @param buffer Output buffer
 * @param size Buffer size
 * @return Number of characters read
 */
int read_string(const char* prompt, char* buffer, int size);

/**
 * @brief Read yes/no confirmation
 * @param prompt Message to display
 * @return true for yes, false for no
 */
bool read_confirmation(const char* prompt);

/**
 * @brief Validate product code format
 * @param code Product code to validate
 * @return true if valid format, false otherwise
 */
bool validate_product_code(const char* code);

/**
 * @brief Validate price value
 * @param price Price to validate
 * @return true if valid, false otherwise
 */
bool validate_price(float price);

/**
 * @brief Validate quantity value
 * @param quantity Quantity to validate
 * @return true if valid, false otherwise
 */
bool validate_quantity(int quantity);

/* ========================================================================
 * STRING UTILITIES
 * ======================================================================== */

/**
 * @brief Trim whitespace from string
 * @param str String to trim (modified in-place)
 */
void trim_string(char* str);

/**
 * @brief Convert string to uppercase
 * @param str String to convert (modified in-place)
 */
void to_uppercase(char* str);

/**
 * @brief Convert string to lowercase
 * @param str String to convert (modified in-place)
 */
void to_lowercase(char* str);

/**
 * @brief Check if string contains substring (case-insensitive)
 * @param haystack String to search in
 * @param needle Substring to find
 * @return true if found, false otherwise
 */
bool string_contains(const char* haystack, const char* needle);

/**
 * @brief Safe string copy with truncation
 * @param dest Destination buffer
 * @param src Source string
 * @param size Destination buffer size
 */
void safe_strcpy(char* dest, const char* src, int size);

/* ========================================================================
 * SEARCH & FILTER
 * ======================================================================== */

/**
 * @brief Search products across all categories
 * @param store DataStore to search
 * @param keyword Search keyword
 * @param results Output array of matching products
 * @param max_results Maximum number of results
 * @return Number of products found
 */
int search_products(DataStore* store, const char* keyword, 
                   Product** results, int max_results);

/**
 * @brief Filter products by price range
 * @param store DataStore to search
 * @param min_price Minimum price
 * @param max_price Maximum price
 * @param results Output array
 * @param max_results Maximum results
 * @return Number of products found
 */
int filter_by_price(DataStore* store, float min_price, float max_price,
                   Product** results, int max_results);

/**
 * @brief Filter products by quantity threshold
 * @param store DataStore to search
 * @param threshold Quantity threshold
 * @param below_threshold If true, find products below threshold
 * @param results Output array
 * @param max_results Maximum results
 * @return Number of products found
 */
int filter_by_quantity(DataStore* store, int threshold, bool below_threshold,
                      Product** results, int max_results);

/* ========================================================================
 * DISPLAY UTILITIES
 * ======================================================================== */

/**
 * @brief Clear screen (cross-platform)
 */
void clear_screen(void);

/**
 * @brief Pause and wait for user input
 */
void pause_screen(void);

/**
 * @brief Display horizontal separator line
 * @param width Line width
 * @param char Character to use
 */
void print_separator(int width, char character);

/**
 * @brief Print centered text
 * @param text Text to center
 * @param width Total width
 */
void print_centered(const char* text, int width);

/**
 * @brief Display data store statistics
 * @param store DataStore to analyze
 */
void print_statistics(DataStore* store);

/**
 * @brief Format currency for display
 * @param value Amount to format
 * @param buffer Output buffer
 * @param size Buffer size
 */
void format_currency(float value, char* buffer, int size);

/**
 * @brief Format date/time for display
 * @param buffer Output buffer
 * @param size Buffer size
 */
void format_datetime(char* buffer, int size);

/* ========================================================================
 * SYSTEM UTILITIES
 * ======================================================================== */

/**
 * @brief Get current timestamp
 * @param buffer Output buffer
 * @param size Buffer size
 */
void get_timestamp(char* buffer, int size);

/**
 * @brief Calculate total inventory value
 * @param store DataStore to calculate
 * @return Total value of all products
 */
float calculate_total_value(DataStore* store);

/**
 * @brief Get total product count
 * @param store DataStore to count
 * @return Total number of products
 */
int get_total_product_count(DataStore* store);

/**
 * @brief Check for low stock products
 * @param store DataStore to check
 * @param threshold Stock threshold
 * @return Number of low-stock products
 */
int check_low_stock(DataStore* store, int threshold);

/**
 * @brief Generate system report
 * @param store DataStore to report
 * @param filename Output file path
 * @return true on success, false on failure
 */
bool generate_report(DataStore* store, const char* filename);

/* ========================================================================
 * CONSTANTS
 * ======================================================================== */

#define DATA_FILE "data/products.dat"
#define BACKUP_FILE "data/products.bak"
#define REPORT_FILE "data/report.txt"

#define MAX_SEARCH_RESULTS 100
#define LOW_STOCK_THRESHOLD 10

#define SCREEN_WIDTH 80

#endif /* UTILS_H */