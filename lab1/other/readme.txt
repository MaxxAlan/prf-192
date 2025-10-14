Mục tiêu chính của Lab 1 là áp dụng kỹ thuật Phân rã (Decomposition) để phân tích các yêu cầu của dự án.

1. Mục Tiêu Cốt Lõi (Core Objective)
Trong Lab 1, bạn cần thực hiện hai nhiệm vụ chính để tạo ra Tài liệu Phân tích (Analysis Document):


Phân rã (Decomposition): Phân tách dự án lớn (Quản lý Sinh viên) thành các chức năng nhỏ hơn (Ví dụ: Thêm sinh viên, Tính GPA, Tìm kiếm, Sắp xếp, Lưu tệp).



Nhận dạng Mẫu (Pattern Recognition): Xác định các mẫu logic sẽ lặp lại trong mã code (Ví dụ: kiểm tra tính hợp lệ của đầu vào, logic tìm kiếm, cấu trúc menu).

2. Các Bước Cần Làm Trong Lab 1
A. Nhiệm Vụ 1: Phân rã Chức năng (Decomposition)
Dự án Quản lý Sinh viên bao gồm các yêu cầu lớn. Bạn cần chia chúng thành các hàm (function) cụ thể:

Chức Năng Chính (Main Function)	Các Nhiệm Vụ Con (Subtasks/Hàm) Cần Phân rã
Quản Lý Sinh Viên	
1. Thêm sinh viên mới (add_student). 2. Sửa thông tin sinh viên (update_student). 3. Xóa sinh viên (delete_student). 4. Hiển thị danh sách sinh viên (display_all_students).

Tính Toán Điểm	
1. Tính điểm trung bình (GPA) cho từng sinh viên (calculate_gpa).

Tìm Kiếm/Sắp Xếp	
1. Tìm kiếm sinh viên theo ID/Tên (search_student). 2. Sắp xếp danh sách theo GPA/Tên (sort_students).

Quản Lý Tệp	
1. Lưu dữ liệu sinh viên vào tệp (save_to_file). 2. Tải dữ liệu từ tệp (load_from_file).


Xuất sang Trang tính
B. Nhiệm Vụ 2: Nhận Dạng Mẫu (Pattern Recognition)
Xác định các khối logic sẽ được tái sử dụng để chuẩn bị cho việc sử dụng Hàm (Functions) và Trừu tượng hóa (Abstraction) trong Lab 3:

Mẫu Logic (Pattern)	Mô Tả	Áp Dụng
Input Validation	
Kiểm tra đầu vào của người dùng có hợp lệ không (ví dụ: điểm phải từ 0-10, ID phải là chuỗi/số,...).

Dùng trong add_student, update_student,...
Menu Structure	
Cấu trúc vòng lặp do-while và switch-case để hiển thị và xử lý các lựa chọn của menu.

Dùng trong hàm main() hoặc hàm show_menu().
Array Iteration	
Logic lặp qua mảng sinh viên để tìm kiếm, hiển thị hoặc sắp xếp.

Dùng trong search_student, sort_students,...

Xuất sang Trang tính
3. Sản Phẩm Cần Nộp (Deliverable)
Sản phẩm của Lab 1 là Tài liệu Phân tích:

Danh sách các hàm chức năng (như bảng trên).

Mỗi chức năng được phân rã thành các bước thực hiện (ví dụ: Chức năng Thêm Sinh Viên -> 1. Yêu cầu nhập ID -> 2. Kiểm tra ID đã tồn tại chưa -> 3. Yêu cầu nhập Tên -> 4. Lưu vào mảng).


Danh sách các mẫu logic đã nhận dạng (Input Validation, Menu Structure,...).


Lời khuyên: Giảng viên khuyến khích bạn sử dụng sơ đồ hoặc bảng để trình bày phân tích này một cách trực quan.