#!/bin/bash
# /etc/init.d/xdm: start or stop XDM.

test -x /usr/bin/X11/xdm || exit 0

test -f /etc/X11/config || exit 0

grep -q ^xbase-not-configured /etc/X11/config && exit 0

case "$1" in
  start)
    grep -q ^start-kdm /etc/X11/config || exit 0
    if grep -q ^start-xdm /etc/X11/config
      then
        echo "WARNING : can only start kdm or xdm, but not both !"
    fi
    echo -n "s/WINDOWMANAGERS/" > /etc/kde/kdmrc.tmp
    cat /etc/X11/window-managers |while read A; 
    do
        if test -x "$A"
        then
    	    echo -n "`basename $A`;" >> /etc/kde/kdmrc.tmp
        fi
    done
    echo "/" >> /etc/kde/kdmrc.tmp
    sed "`cat /etc/kde/kdmrc.tmp`" < /etc/kde/kdmrc.in > /etc/kde/kdmrc
    rm /etc/kde/kdmrc.tmp
    echo -n "Starting kde display manager: kdm"    
    start-stop-daemon --start --quiet --exec /opt/kde/bin/kdm
    echo "."
    ;;
  stop)
      echo -n "Stopping kde display manager: kdm"    
      start-stop-daemon --stop --quiet --exec /opt/kde/bin/kdm \
		--pidfile /var/run/xdm-pid
      echo "."
    ;;
  *)
    echo "Usage: /etc/init.d/kdm {start|stop}"
    exit 1
esac

exit 0
