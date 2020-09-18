# Kontroller

A kodi remote control app for SailfishOS.

# Features

Current features include :

* music library browsing
* video library browsing
* player control
* basic library management
* basic remote
* discovery of servers on the local network
* multiple servers
* volume management, via kodi or external device
* shutdown / wakeup management

# WakeOnLan

WakeOnLan usually requires some configuration at the server level.

## Raspberry pi / LibreElec

LibreElec should already enable wake on lan. Note that, however, wake on lan
will not work for the wifi interface (kontroller does not detect that).

If not using LibreElec, the steps for Linux will usually work.

## PC

Usually, WakeOnLan option must be enabled in the bios/uefi settings. Sometimes
the name is unclear, but usually look for *wake by pci device*. It must then
be activated at the system level

### Linux

You need to install the *ethtool* package to help with configuring wake on
line. Then, identify your network card with ```ip link```. It should show a line with either something like ```ethX``` or ```enpXsY```, where X and Y are digits.

Create the file ```/etc/systemd/system/wol.service``` (as root) with the following content :

```
[Unit]
Description=Configure Wake On LAN

[Service]
Type=oneshot
ExecStart=/sbin/ethtool -s enp2s0 wol g

[Install]
WantedBy=basic.target
```

Replace ```enp2s0``` with the actual value for your network interface.

Then, type (as root) the following commands :

```
# systemctl daemon-reload
# systemctl enable wol.service
```

### Windows

There are lots of tutorials online to find information on how to enable wake
on lan on windows. Check https://duckduckgo.com/?q=windows+enable+wake+on+lan
for details.
