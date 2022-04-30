This project helps me ...
The directories are lib, test and main.

//-----------------
2019-05-26
//-----------------
git clone https://github.com/FlorinChertes/ex603_mcp_001

This will set the HEAD of your master to point to commit_sha.
git clone -n <repo_name>
git checkout <commit_sha>

https://coderwall.com/p/xyuoza/git-cloning-specific-commits

use CMake for C++17
https://cmake.org/cmake/help/latest/prop_tgt/CXX_STANDARD.html

//-----------------
2019-12-21 10:30
//-----------------
CMake 3.14.2
E:/users/chfl/ms160_app/dev/ex603_mcp_001
E:/users/chfl/ms160_app/build/ex603_mcp_001

//-----------------
2020-01-03 17:00
//-----------------

florin@Lenovo-19-PC:~$ cd prj_appl/dev/ex603_mcp_001/
florin@Lenovo-19-PC:~/prj_appl/dev/ex603_mcp_001$ git pull

florin@Lenovo-19-PC:~$ cd build/dev/ex603_mcp_001/
florin@Lenovo-19-PC:~/build/dev/ex603_mcp_001$ rm -fr *
florin@Lenovo-19-PC:~/build/dev/ex603_mcp_001$ cmake ~/prj_appl/dev/ex603_mcp_001/
florin@Lenovo-19-PC:~/build/dev/ex603_mcp_001$ make

//-----------------
2021-12-08 17:00
//-----------------


florin@Lenovo-19-PC:~/build/dev/ex603_mcp_001$ rm -fr *
florin@Lenovo-19-PC:~/build/dev/ex603_mcp_001$ cmake -DCMAKE_BUILD_TYPE=Release ../../../prj_appl/dev/ex603_mcp_001/
florin@Lenovo-19-PC:~/build/dev/ex603_mcp_001$ cmake --build .

//-----------------
2022-04-30 23:00
//-----------------
 git branch work_b_001
 git checkout work_b_001
  

