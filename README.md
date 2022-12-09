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
* Shutdown /  wake-up management

# Translations

Kontroller is available in several languages, including:

* Belgian
* Dutch
* English
* French
* German
* Russian
* Swedish
* Norwegian

Translating the app is done on [weblate](http://hosted.weblate.org/projects/kontroller).

# Support kontroller

Kontroller is developed for fun, on my free time. If you wish to support its
development, then consider supporting [weblate](https://weblate.org/donate/)
as they provide the translation infrastructure for free.

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

### Linux

#### Using systemd-networkd

Identify the name of your ethernet card. Run the following command (as root):

```
networkctl status -a
```

Identify the card with the ip address, for example. If nothing is returned, it means you
are not using systemd-networkd. If ok, create the following file:

```
/etc/systemd/network/50-<devname>.link
```

With the following content

```
[Match]
Type=ether

[Link]
NamePolicy=path kernel database
MACAddressPolicy=persistent
WakeOnLan=magic
```

If you have multiple ethernet interfaces, be sure to use the correct `[Match]` parameter, if you
don't want to enable it on all interfaces.

Also create, if needed, the `/etc/systemd/network/50-<devname>.network`:

```
[Match]
Name=<devname>

[Network]
DHCP=yes
```

#### Without systemd-networkd

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
