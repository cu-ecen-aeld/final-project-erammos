
##############################################################
#
# AESD-ASSIGNMENTS
#
##############################################################

PIR_SENSOR_VERSION = '0.0.1'
PIR_SENSOR_SITE = $(TOPDIR)/../pir-sensor
PIR_SENSOR_SITE_METHOD = local
PIR_SENSOR_MODULE_SUBDIRS += driver
PIR_SENSOR_LICENSE = GPL-2.0
PIR_SENSOR_LICENSE_FILES = COPYING
define PIR_SENSOR_BUILD_CMDS
	$(MAKE) $(TARGET_CONFIGURE_OPTS) -C $(@D)
endef

# TODO add your writer, finder and finder-test utilities/scripts to the in#stallation steps below
define PIR_SENSOR_INSTALL_TARGET_CMDS
  $(INSTALL) -m 0755 $(@D)/pir-demo $(TARGET_DIR)/usr/bin/
endef

$(eval $(kernel-module))
$(eval $(generic-package))
