if(NOT BUILD_KF5)
    add_subdirectory(kde4)
else()
    ecm_install_icons(
        ICONS 16-apps-umbrello.png
              16-mimetypes-application-x-uml.png
              22-apps-umbrello.png
              32-apps-umbrello.png
              32-mimetypes-application-x-uml.png
              48-apps-umbrello.png
              64-apps-umbrello.png
              128-apps-umbrello.png
              sc-apps-umbrello.svgz
        THEME hicolor
        DESTINATION ${ICON_INSTALL_DIR}
    )
endif()
