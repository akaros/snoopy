# Snoopy from harvey (from Plan 9)
# Rules: keep this simple. Make sure the gcc is in your path and nobody gets hurt.

###
# This allows snoopy to be built from within the Akaros distribution (maybe).
# The Makefrag exports a few things like CROSS_COMPILE and KFS_ROOT, which
# we'll use.  We'll provide defaults if we couldn't find them.
-include ../../Makefrag
ifndef CROSS_COMPILE
CROSS_COMPILE := x86_64-ucb-akaros-
endif
ifndef KFS_ROOT
KFS_ROOT := $(AKAROS_ROOT)/kern/kfs
endif
ifndef Q
Q ?= @
endif


### Build flags for all targets
#
CFLAGS          = -O2 -std=gnu99 -fno-stack-protector -fgnu89-inline -fPIC -static -fno-omit-frame-pointer -g -Iinclude -Wall -Werror
LDFLAGS         =
LDLIBS          = -lpthread -lbenchutil -lm -liplib -lndblib -lvmm -lbenchutil
DEST            = $(KFS_ROOT)/bin/


### Build tools
# 
CC=$(CROSS_COMPILE)gcc
AR=$(CROSS_COMPILE)ar
ALL=snoopy
FILES= \
aoeata.c \
aoe.c \
aoecmd.c \
aoemask.c \
aoemd.c \
aoerr.c \
arp.c \
bootp.c \
cec.c \
dhcp.c \
dump.c \
eap.c \
eap_identity.c \
eapol.c \
eapol_key.c \
ether.c \
gre.c \
hdlc.c \
icmp6.c \
icmp.c \
il.c \
ip6.c \
ip.c \
main.c \
protos.c \
rarp.c \
rc4keydesc.c \
rtcp.c \
rtp.c \
tcp.c \
ttls.c \
udp.c \
readn.c util.c \
y.tab.c

NO=\
ospf.c \
ppp.c \
ppp_ccp.c \
ppp_chap.c \
ppp_comp.c \
ppp_ipcp.c \
ppp_lcp.c \
pppoe_disc.c \
pppoe_sess.c \
dns.c \
ninep.c \

all: $(ALL)

y.tab.c: y.tab.h 

y.tab.h: filter.y
	yacc --defines filter.y

install: all
	$(Q)echo "Installing $(ALL) in $(DEST)"
	$(Q)cp $(ALL) $(DEST)

# compilers are fast. Just rebuild it each time.
snoopy: $(FILES)
	$(CC) $(CFLAGS) $(LDFLAGS) -o snoopy $(FILES) $(LDLIBS)
clean:
	rm -f $(ALL) *.o
