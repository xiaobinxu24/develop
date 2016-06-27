file(REMOVE_RECURSE
  "sample1_unittest.pdb"
  "sample1_unittest"
)

# Per-language clean rules from dependency scanning.
foreach(lang )
  include(CMakeFiles/sample1_unittest.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
