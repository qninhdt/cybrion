<p align="center">
    <img width="100" src="./docs/icon.png" alt="Cybrion">
    <h1 align="center">Cybrion</h1>
    <p align="center"><i>❝ Just a silly blocky game ❞ — <b>@qninhdt</b></i></p>
</p>

## ⚙️ Installing


## 🔧 Building
### Windows
#### Requirements
- [Git](https://git-scm.com/)
- [CMake](https://cmake.org/)
- [Visual Studio 2015 or newer](https://visualstudio.microsoft.com/)

> _Don't forget to add CMake to enviroment path_
#### Installing libraries and compiling
- __Step 1__: Open terminal and dowload source code
```powershell
> git clone https://github.com/qninhdt/cybrion
```
- __Step 2__: Install vcpkg and libraries
```powershell
> cd cybrion
> build.bat
```
- __Step 3__: Rerun build.bat to compile project
```powershell
> build.bat
```

After compiling, executable files and resource files will be put in folder `cybrion/build`

To play game, you need to run `cybrion.exe` in that folder  

### Linux
#### Requirements
- [Git](https://git-scm.com/)
#### Installing libraries and compiling
- __Step 1__: Dowload source code
```bash
$ git clone https://github.com/qninhdt/cybrion
```
- __Step 2__: Run `build.sh`. It will install tools, libraries and compile automatically
```bash
$ cd cybrion
$ ./build.sh
```

Similar to __Windows__, build folder will be `cybrion/build`


