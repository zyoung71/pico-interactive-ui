if (NOT TARGET pico-interactive-ui)

    add_library(pico-interactive-ui STATIC
        src/interactive-ui/Screen.cpp
        src/interactive-ui/Component.cpp
        src/interactive-ui/ScreenManager.cpp
    )

    add_subdirectory(lib/pico-util)
    add_subdirectory(lib/pico-math)

    target_include_directories(pico-interactive-ui PUBLIC
        include
    )

    target_link_libraries(pico-interactive-ui
        hardware_rtc
        pico-util
        pico-math
    )

endif()