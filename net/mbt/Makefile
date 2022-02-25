include $(TOPDIR)/rules.mk

PKG_NAME:=mbt
PKG_VERSION:=0.2.0
PKG_RELEASE:=$(AUTORELEASE)
CMAKE_INSTALL:=1
#do not strip backward symbols
RSTRIP:=:

PKG_MAINTAINER:=Alexey Smirnov <s.alexey@gmail.com>

include $(INCLUDE_DIR)/package.mk
include $(INCLUDE_DIR)/cmake.mk

CMAKE_OPTIONS += -DCMAKE_BUILD_TYPE=Release

define Package/mbt
	SECTION:=net
	CATEGORY:=Network
	DEPENDS:=+libstdcpp +libconfig +libdw +libbfd +libelf
	TITLE:=Failover tunnel implementation
endef

define Package/mbt/install
	$(INSTALL_DIR) $(1)/usr/sbin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/mbt $(1)/usr/sbin/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/mbtctl $(1)/usr/sbin/
endef

$(eval $(call BuildPackage,mbt))
