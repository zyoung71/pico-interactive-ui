if (NOT TARGET pico-interactive-ui)

    add_library(pico-interactive-ui STATIC
        src/interactive-ui/Screen.cpp
        src/interactive-ui/ScreenManager.cpp
        src/interactive-ui/Component.cpp
        src/interactive-ui/SelectableComponent.cpp
        src/interactive-ui/components/BitmapComponent.cpp
        src/interactive-ui/components/PixelBufferComponent.cpp
        src/interactive-ui/components/PaddingComponent.cpp
        src/interactive-ui/components/TextComponent.cpp
        src/interactive-ui/components/DropDownBoxComponent.cpp
        src/interactive-ui/components/FunctionComponent.cpp
        src/interactive-ui/components/CircleComponent.cpp
    )

    add_subdirectory(lib/pico-math)
    add_subdirectory(lib/pico-event-hardware)

    target_include_directories(pico-interactive-ui PUBLIC
        include
    )

    target_link_libraries(pico-interactive-ui
        pico-math
        pico-event-hardware
    )

endif()