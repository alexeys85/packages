include $(TOPDIR)/rules.mk

PKG_NAME:=bupd-server
PKG_VERSION:=0.1.1
#PKG_SOURCE:=mbt-$(PKG_VERSION).zip
#PKG_SOURCE_URL:=file://
#PKG_HASH:=
PKG_RELEASE:=1
CMAKE_INSTALL:=1
#do not strip backward symbols
#RSTRIP:=:

PKG_MAINTAINER:=Alexey Smirnov <s.alexey@gmail.com>

include $(INCLUDE_DIR)/package.mk
include $(INCLUDE_DIR)/cmake.mk

CMAKE_OPTIONS += \
	-DCMAKE_BUILD_TYPE=Release \
	-DBUILD_TESTS=OFF \
	-DCMAKE_CXX_FLAGS="-DLOG_TO_FILE=1"

define Package/bupd-server
	SECTION:=net
	CATEGORY:=Network
	DEPENDS:=+libstdcpp +libmnl
	TITLE:=BUPD server implementation
endef

define Build/Configure
	echo "install(TARGETS bserver-app RUNTIME DESTINATION .)" >> $(PKG_BUILD_DIR)/CMakeLists.txt
	$(call Build/Configure/Default)
endef

define Package/bupd-server/install
	$(INSTALL_DIR) $(1)/usr/sbin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/bserver/bserver-app $(1)/usr/sbin/
	$(INSTALL_DIR) $(1)/etc
	$(INSTALL_CONF) $(PKG_BUILD_DIR)/bserver/bserver-zlog.cfg $(1)/etc
endef

$(eval $(call BuildPackage,bupd-server))
