TEMPLATE = subdirs
SUBDIRS = libeyes desktop plasmoid

OTHER_FILES += \
  README.markdown\
  color.cfg\
  .gitignore \
  config.cfg \
  scripts/checklibs \
  scripts/libcfg \
  themes/default/theme.cfg

plasmoid.path = /usr
plasmoid.extra = cp plasmoid/plasma-applet-eyes.desktop /usr/share/kde4/services/ ; cp plasmoid/libeyes_plasmoid.so /usr/lib/kde4/plasma_applet_eyes.so
INSTALLS += plasmoid

system ( echo "Eyes configured." )
system ( echo "  type 'make' to build Eyes." )
