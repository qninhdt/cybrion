<p align="center">
    <img width="100" src="./docs/icon.png" alt="Cybrion">
    <h1 align="center">Cybrion</h1>
    <p align="center"><i>❝ Just a silly blocky game ❞ — <b>@qninhdt</b></i></p>
</p>


<p align="center">
    <img src="./docs/screenshots/preview.png" alt="Cybrion preview">
</p>

## 💡 Tính năng
- Thế giới mở, vô hạn
- Sử dụng đa luồng để sinh thế giới
- Nhiều loại khối, loài hoa và cây
- Có thể lưu và mở thế giới
- Giao diện bắt mắt, dễ dùng


## 🔧 Biên dịch mã nguồn
### Windows
#### Phần mềm yêu cầu:
- [Git](https://git-scm.com/)
- [CMake](https://cmake.org/)
- [Visual Studio 2015 hoặc cao hơn](https://visualstudio.microsoft.com/)

> _Thêm CMake vào biến môi trường (environment variables)_
#### Tải thư viện và biên dịch
- __Bước 1__: Mở cửa sổ cmd rồi tải mã nguồn từ Github
```batch
> git clone https://github.com/qninhdt/cybrion
```
- __Bước 2__: Tải vcpkg và thư viện
```batch
> cd cybrion
> build.bat
```
- __Step 3__: Chạy lại file `build.bat` để biên dịch
```batch
> build.bat
```

Sau khi biên dịch, các file cần thiết cho game sẽ xuất hiện trong thư mục `cybrion/build`

Chạy file `cybrion.exe` trong thư mục trên để chơi game  

### Linux
#### Phần mềm yêu cầu
- [Git](https://git-scm.com/)
#### Tải thư viện và biên dịch
- __Bước 1__: Tải mã nguồn từ Github
```bash
$ git clone https://github.com/qninhdt/cybrion
```
- __Step 2__: Chạy `build.sh` để tự động tải thư viện, phần mềm cần thiết và biên dịch mã nguồn
```bash
$ cd cybrion
$ ./build.sh
```

Tương tự như trên __Windows__  thư mục chưa game là `cybrion/build`

## 📁 Cấu trúc mã nguồn

    ├── resources           # Chứa texture của game, file cấu hình, file shader, ...
    └── src                 # Mã nguồn
    |   ├── client              # Chứa mọi thứ liên quan đến cửa sổ, điều khiển, render          
    |   |   ├── GL                  # Xử lý OpenGL (mesh, framebuffer, texture, ...)
    |   |   ├── graphic             # Xử lý render khối, thế giới, bầu trời
    |   |   ├── resource            # Đọc file cấu hình, file shader, tải texture, ...
    |   |   └── ui                  # Xử lý giao diện (inventory, menu, text, ...)
    |   ├── core                # Chứa một vài cấu trúc dữ liệu quan trọng
    |   ├── physic              # Xử lý vật lý (ÂABB)
    |   ├── uitl                # Một số hàm hỗ trợ
    |   ├── world               # Quản lý thế giới, khối và người chơi
    |   └── ...
    └── ...

## 🎮 Hướng dẫn chơi

### Hello world
- __Bước 1__: Nhập tên thế giới muốn tạo rồi ấn nút `Create`
- __Bước 2__: Nhấn nút `Play` ở thế giới vừa tạo
- __Bước 3__: Enjoy the game 🗿🍷

### Điều khiển
- __`Chuột trái`__ : phá khối
- __`Chuột phải`__ : đặt khối
- __`WASD`__ : di chuyển
- __`Space`__ : bay lên
- __`Shift`__ : hạ xuống
- __`E`__ : mở túi đồ và chọn khối
- __`Esc`__ : tạm dừng
- __`Ctrl`__ : di chuyển nhanh hơn
- __`F1`__ : ẩn GUI
- __`F3`__ : mở công cụ debug

