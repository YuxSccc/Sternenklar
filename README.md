# Sternenklar
Sternenklar is a code duplication tool based on graph similarity and LLVM.

Note that this tool is still under development, it have various problems.

## Requirements
The requirements is clang and support `c++17`, it can not be replaced by gcc or other compilers in `-c`. But you can still compile this tool by gcc>=9.3.0.
- clang>=9.0.0
- LLVM>=9.0.0
- Eigen>=3.3.7

Temporarily unavailable on Windows.
## Quick start
+ Compile with cmake with the following command

```shell script
mkdir build && cd build && cmake .. && make    
```
You will get a binary file in the `build` folder

+ Use this tool simply by the following command:

```shell script
./CodeSimilarity -f <code folder> -c <clang path>
```
If you don't have `-c` param, it will use `clang-9` in the environment variable by default.

The code extension in folder can only be one of the following
+ .cpp
+ .c
+ .cxx
+ .cc

Note that -j is unavailable now.

## Sample
```shell script
./CodeSimilarity -f /codes -c /usr/bin/clang

I20200501 21:22:11.840698 14500 CSAdmin.cpp:46] Compile "2018112742_C.cpp" Success.
I20200501 21:22:12.203356 14500 CSAdmin.cpp:46] Compile "2018112559_C.cpp" Success.
I20200501 21:22:12.575737 14500 CSAdmin.cpp:46] Compile "2018111070_C.cpp" Success.
I20200501 21:22:12.605813 14500 CSAdmin.cpp:61] Gen graph for code "/tempCSLL/2018112742_C.CSLL" successful.
I20200501 21:22:12.645006 14500 CSAdmin.cpp:61] Gen graph for code "/tempCSLL/2018112559_C.CSLL" successful.
I20200501 21:22:12.675953 14500 CSAdmin.cpp:61] Gen graph for code "/tempCSLL/2018111070_C.CSLL" successful.
I20200501 21:22:12.784891 14500 CSAdmin.cpp:82] Output result
I20200501 21:22:12.784898 14500 CSAdmin.cpp:84] { CodeA:2018112559_C.CSLL}  { CodeB:2018111070_C.CSLL}  Similarity = 84.0542%.
I20200501 21:22:12.784924 14500 CSAdmin.cpp:84] { CodeA:2018112742_C.CSLL}  { CodeB:2018111070_C.CSLL}  Similarity = 25.9643%.
I20200501 21:22:12.784930 14500 CSAdmin.cpp:84] { CodeA:2018112742_C.CSLL}  { CodeB:2018112559_C.CSLL}  Similarity = 17.891%.
I20200501 21:22:12.784936 14500 CSAdmin.cpp:87] Compile Failed 0 files
I20200501 21:22:12.784943 14500 CSAdmin.cpp:91] 
I20200501 21:22:12.784948 14500 CSAdmin.cpp:94] Runtime = 1926ms
```