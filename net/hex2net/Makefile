include $(TOPDIR)/rules.mk

PKG_NAME:=hex2net
PKG_VERSION:=0.1
PKG_RELEASE:=1
PKG_MAINTAINER:=Alexey Smirnov <s.alexey@gmail.com>
PKG_BUILD_PARALLEL:=1
CMAKE_INSTALL:=1

include $(INCLUDE_DIR)/package.mk
include $(INCLUDE_DIR)/cmake.mk

CMAKE_OPTIONS += -DCMAKE_BUILD_TYPE=Release

define Package/hex2net
	SECTION:=net
	CATEGORY:=Network
	TITLE:=Utility to convert HEX string and send it over network
endef

define Package/hex2net/description
	Utility to convert HEX string to bytes and send it over network and optionally
	wait for an answer.
endef

define Package/hex2net/install
	$(INSTALL_DIR) $(1)/usr/sbin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/hex2net $(1)/usr/sbin/
endef

$(eval $(call BuildPackage,hex2net))
