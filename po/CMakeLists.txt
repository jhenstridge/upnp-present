intltool_update_potfile(
  KEYWORDS "tr" "tr:1,2" "N_"
  GETTEXT_PACKAGE ${GETTEXT_PACKAGE}
  COPYRIGHT_HOLDER "James Henstridge"
  )

intltool_install_translations(
  ALL
  GETTEXT_PACKAGE ${GETTEXT_PACKAGE}
)
