include_directories(
    ${QT_INCLUDE_DIR}
    ${KDE1_INCLUDE_DIR}
)

set(KEDIT_SRCS
    kedit.cpp keditcl1.cpp keditcl2.cpp print.cpp filldlg.cpp mail.cpp  urldlg.cpp
)

set(KEDIT_MOC
    kedit.moc keditcl.moc print.moc filldlg.moc mail.moc urldlg.moc
)

QT1_WRAP_MOC(MOC_FILES ${KEDIT_MOC})

add_executable(kedit ${KEDIT_SRCS} ${MOC_FILES})
target_link_libraries(kedit
    ${QT_LIBRARIES}
    ${X11_LIBRARIES}
    ${KDE1_KDECORE}
    ${KDE1_KDEUI}
    ${KDE1_KFM}
    ${KDE1_KFILE}
    ${KDE1_KSPELL}
)

install(TARGETS kedit RUNTIME DESTINATION ${KDE1_BINDIR})

install(FILES kedit.kdelnk DESTINATION ${KDE1_DATADIR}/kedit)

install_icon(kedit.xpm "${KDE1_DATADIR}/kedit")
install_icon(redo.xpm "${KDE1_DATADIR}/kedit")
install_icon(undo.xpm "${KDE1_DATADIR}/kedit")
