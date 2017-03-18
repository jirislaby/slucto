prefix=/usr
bindir=$(prefix)/bin
datadir=$(prefix)/share
INSTALL=install
INSTALL_DIR=$(INSTALL) -d
INSTALL_DATA=$(INSTALL) -m 644

all:

install:
	$(INSTALL_DIR) $(DESTDIR)/$(bindir)
	$(INSTALL) slucto $(DESTDIR)/$(bindir)
	$(INSTALL) slucto_create $(DESTDIR)/$(bindir)
	$(INSTALL_DIR) $(DESTDIR)/$(datadir)/slucto
	$(INSTALL_DATA) faktura/faktura.tex $(DESTDIR)/$(datadir)/slucto
