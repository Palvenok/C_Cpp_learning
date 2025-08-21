# quick guide
For add folder with library in project
```cmake
add_subdirectory(libs/<lib_name>)
```

For link library to build 
```cmake
target_link_libraries(${PROJECT_NAME} PRIVATE <lib_name>)
```
