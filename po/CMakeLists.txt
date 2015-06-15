intltool_update_potfile(
  ALL
  UBUNTU_SDK_DEFAULTS
  KEYWORDS "tr" "tr:1,2" "N_"
  GETTEXT_PACKAGE ${GETTEXT_PACKAGE}
  POTFILES_TEMPLATE "POTFILES.in.in"
  COPYRIGHT_HOLDER "James Henstridge"
  )

intltool_install_translations(
  ALL
  GETTEXT_PACKAGE ${GETTEXT_PACKAGE}
)
