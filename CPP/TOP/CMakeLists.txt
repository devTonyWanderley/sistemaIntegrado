cmake_minimum_required(VERSION 3.19)
project(TOP LANGUAGES CXX) # Mudei o nome para evitar confusão com a pasta TOP

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets)
qt_standard_project_setup()

# 1. Adicionar as bibliotecas ANTES do executável
# O segundo parâmetro (ex: "bin_com") é obrigatório para pastas externas
add_subdirectory("../LIB/COM" "bin_com")
add_subdirectory("../LIB/TIP" "bin_tip")
add_subdirectory("../LIB/QTW" "bin_qtw")

# 2. Configurar o executável
qt_add_executable(TOP
    WIN32 MACOSX_BUNDLE
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    # Se o motor não for uma biblioteca estática, inclua o .cpp aqui:
    ../LIB/COM/PortaSerialWin.cpp
)

# 3. Linkagem (Ordem de dependência)
target_link_libraries(TOP
    PRIVATE
        Qt6::Core
        Qt6::Widgets
        LibQtWidgets   # Nome definido no CMake da QTW
        LibTipos       # Nome definido no CMake da TIP
        MotorComunicacao # Nome definido no CMake da COM
        kernel32       # Necessário para WinAPI no Windows
)

# 4. Instalação e Deploy (Mantido conforme seu original)
install(TARGETS TOP
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET TOP
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
