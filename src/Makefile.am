sbin_PROGRAMS=devsn2mac
devsn2mac_SOURCES=devsn2mac.c

systemdsysdir = $(libdir)/systemd/system
systemdsys_DATA = devsn2mac.service

SERVICE_SUBS = \
	s,[@]sbindir[@],$(sbindir),g

devsn2mac.service: devsn2mac.service.in
	sed -e '$(SERVICE_SUBS)' < $< > $@
