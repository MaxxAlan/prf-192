# Hệ thống Quản lý Sản phẩm

# (Product Management System)

### 1\. Mục tiêu dự án

Hệ thống quản lý sản phẩm đa dạng, dễ mở rộng, cho phép:

Quản lý thông tin sản phẩm theo mã hàng (SKU), phân hàng, nhóm hàng.

CRUD: Thêm, Xóa, Sửa, Xem sản phẩm.

Lọc, tìm kiếm, thống kê sản phẩm theo nhóm, phân hàng, giá cả, số lượng.

Lưu dữ liệu vào file nhị phân .dat để đảm bảo tốc độ, ổn định và dễ dùng cho nhiều người.

Cấu trúc code modular, cleancode, dễ bảo trì và phát triển nhóm.

### 2\. Cấu trúc dự án

ProductManagement/

│

├─ include/ # Header files

│ ├─ product.h

│ ├─ subgroup.h

│ ├─ category.h

│ └─ utils.h

│

├─ src/ # Source files

│ ├─ product.c

│ ├─ subgroup.c

│ ├─ category.c

│ └─ main.c

│

├─ data/ # File dữ liệu nhị phân

│ └─ products.dat

│

├─ build/ # Binary output

│

└─ README.md

### 3\. Nguyên tắc code

Cleancode \& Modular

Mỗi module có file .h và .c.

Hàm module luôn dùng prefix để tránh xung đột (Product\_, Subgroup\_, Category\_).

Tránh biến toàn cục

Dữ liệu được load từ file .dat vào struct và mảng/linked list.

Tương tác với file .dat qua hàm wrapper

Không đọc hoặc ghi trực tiếp ngoài module utils.

### 4\. Module \& Hàm chính

#### 4.1 Product Module

Product_create(Product \*p, int id, char\* name, double price, int quantity, char\* description)

Product_update(Product \*p, double price, int quantity)

Product_print(const Product \*p)

#### 4.2 Subgroup Module

Subgroup_addProduct(Subgroup \*sg, Product p)

Subgroup_removeProduct(Subgroup \*sg, int productId)

Subgroup_findProductById(const Subgroup \*sg, int id)

#### 4.3 Category Module

Category_addSubgroup(Category \*cat, Subgroup sg)

Category_findSubgroupByName(const Category \*cat, const char\* name)

Category_print(const Category \*cat)

#### 4.4 Utils Module (File .dat)

loadData(const char\* filepath, Category \*categories, int \*category_count) → đọc dữ liệu nhị phân

saveData(const char\* filepath, const Category \*categories, int category_count) → ghi dữ liệu nhị phân

validateInput(...) → kiểm tra input người dùng

### 5\. Phân nhiệm vụ cho 5 thành viên

Thành viên Module / Task chính

Mạnh 1 Product Module: CRUD, validate, print

Phương Anh 2 Subgroup Module: add/remove/find product

Khoa 3 Category Module: add/find subgroup, print

Tuấn Anh 4 Utils Module: load/save .dat, input validation

Phương 5 Main \& Integration: menu, console, kết nối module

### 6\. Quy trình làm việc

Mỗi thành viên làm trên branch riêng.

Commit chỉ module riêng, tránh merge trực tiếp main.

Trước khi merge: compile + test unit module.

Dữ liệu test dùng data/products.dat để đồng bộ.

### 7\. Hướng dẫn build \& chạy

\# Biên dịch

gcc src/\*.c -Iinclude -o build/ProductManagement

\# Chạy

./build/ProductManagement

Lưu ý: products.dat sẽ được tạo tự động nếu chưa có.

###

### 8\. Lợi ích khi dùng .dat

Nhanh và ổn định: đọc/ghi trực tiếp struct, không parse text.

Dễ sử dụng nhiều người: tránh lỗi format, giữ dữ liệu đồng bộ.

Sẵn sàng mở rộng: thêm nhóm, phân hàng, sản phẩm mới mà không phá cấu trúc.
