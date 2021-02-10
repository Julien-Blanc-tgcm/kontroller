# Kontroller

Kodi remote control app for SailfishOS.

# Features

Current features include:

* Music library browsing
* Video library browsing
* Player control
* Basic library management
* Basic remote
* Discovery of servers on the local network
* Multiple servers
* Volume management, via Kodi or external device
* Shutdown / Wake-on-LAN management

# Wake-on-LAN

Wake-on-LAN usually requires some configuration at the server level.

## Raspberry Pi / LibreELEC

LibreELEC should already enable Wake-on-LAN. Note that it
will not work for the Wi-Fi interface (Kontroller does not detect that).

If not using LibreELEC, the steps for Linux will usually work.

## PC

Usually, the *Wake-on-LAN* option must be enabled in the BIOS/UEFI settings. Sometimes
the name is unclear, but look for something like *wake up from PCI device*. It must then
be activated at the system level.

### Linux with systemd

You need to install the *ethtool* package to help with configuring Wake-on-LAN.
Then, identify your network card with ```ip link```.
It should show a line with either something like ```ethX``` or ```enpXsY```, where X and Y are digits.

Create the file ```/etc/systemd/system/wol.service``` (as root) with the following content:

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

Then, type the following commands (as root):

```
# systemctl daemon-reload
# systemctl enable wol.service
```

### Windows

There are lots of tutorials online about turning on Wake-on-LAN on Windows.
Check https://duckduckgo.com/?q=windows+enable+wake+on+lan for details.
