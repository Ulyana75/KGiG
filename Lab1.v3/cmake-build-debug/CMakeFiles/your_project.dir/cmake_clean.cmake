file(REMOVE_RECURSE
  "CMakeFiles/your_project.dir/main.cpp.obj"
  "libyour_project.dll.a"
  "your_project.exe"
  "your_project.exe.manifest"
  "your_project.pdb"
)

# Per-language clean rules from dependency scanning.
foreach(lang CXX)
  include(CMakeFiles/your_project.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
