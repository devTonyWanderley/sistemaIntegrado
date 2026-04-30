add_executable(AppTopografia
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
)

# A mágica do CMake: ele resolve todas as dependências recursivamente
target_link_libraries(AppTopografia PRIVATE 
    lib_com 
    #lib_pro
    lib_geo 
    Qt6::Widgets
)
