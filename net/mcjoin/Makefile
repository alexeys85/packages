#
# Copyright (C) 2023 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=mcjoin
PKG_VERSION:=2.11
PKG_RELEASE:=1
PKG_LICENSE:=GPL-2.0-or-later
PKG_LICENSE_FILES:=COPYING

PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.gz
PKG_SOURCE_URL:=https://github.com/troglobit/mcjoin/releases/download/v$(PKG_VERSION)
PKG_HASH:=0ec72cb333d43c26d947b38059580166f131eafb7b6dccbb6ee619abd4e98670

include $(INCLUDE_DIR)/package.mk

define Package/mcjoin
	SECTION:=net
	CATEGORY:=Network
	SUBMENU:=Routing and Redirection
	TITLE:=Simple multicast testing application
	MAINTAINER:=Alexey Smirnov <s.alexey@gmail.com>
endef

define Package/mcjoin/description
	mcjoin is a simple and easy-to-use tool to test IPv4 and IPv6 multicast.
endef

define Package/mcjoin/install
	$(INSTALL_DIR) $(1)/usr/sbin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/src/mcjoin $(1)/usr/sbin/
endef

$(eval $(call BuildPackage,mcjoin))
