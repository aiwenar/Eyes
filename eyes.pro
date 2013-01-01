TEMPLATE = subdirs
SUBDIRS = libeyes desktop

OTHER_FILES += \
  README.markdown\
  color.cfg\
  .gitignore \
  config.cfg \
  scripts/checklibs \
  scripts/libcfg \
  themes/default/theme.cfg

system ( echo "Eyes configured." )
system ( echo "  type 'make' to build Eyes." )
