��    <      �  S   �      (  �   )  3        R  "   Y  ,   |  /   �  +   �  #     '   )  '   Q  &   y     �  '   �  [   �     :  -   P  )   ~  5   �  )   �  ,   	  '   5	  +   ]	  ;   �	  0   �	  ,   �	  ;   #
  0   _
  ,   �
  �  �
  �  S  �   �  $   �  <   �  A   �     +  �   G  (   $  �  M  k  �     :  �  L  �  �  G   g  C   �  >   �  .   2  �   a  �   +  �    b   $     w$  0   �$  H  �$     +     +  0   +  D   Q-  9   �-  $   �-  �  �-  �   �/  3   �0     �0  !   �0  ,   �0  0   1  ,   J1  #   w1  '   �1  '   �1  &   �1     2  '   )2  `   Q2     �2  -   �2  )   �2  5   !3  )   W3  ,   �3  '   �3  +   �3  ;   4  0   >4  ,   o4  ;   �4  0   �4  ,   	5  �  65  �  �7  �   l:  $   ;  <   ';  A   d;     �;  �   �;  +   �<  �  �<  l  P>     �@  �  �@  �  WB  H   �C  D   8D  9   }D  .   �D  �   �D  �   �E  �  �F  +   �N     �N  0   �N  G  O     TU     lU  4  qU  >   �W  3   �W  $   X            '       "                 5   #      %           1          3         	   *            )      9      ,                   7   <              4            
       -      $          2       8      ;   :   6          0                        /   +          !      &       .             (           A list of pairwise encryption algorithms which prevents connections to Wi-Fi networks that do not utilize one of the algorithms in the list. For maximum compatibility leave this property empty.  Each list element may be one of "tkip" or "ccmp". Active connection removed before it was initialized Add... Can not change the connection type Connection removed before it was initialized Connection sharing via a protected WiFi network Connection sharing via an open WiFi network Could not daemonize: %s [error %u]
 Couldn't initialize PKCS#12 decoder: %d Couldn't initialize PKCS#12 decoder: %s Couldn't initialize PKCS#8 decoder: %s Device LLDP neighbors Error initializing certificate data: %s Error: Device '%s' was not recognized as a Wi-Fi device, check NetworkManager Wi-Fi plugin. Error: bad color: %s
 Error: polkit agent initialization failed: %s Error: secret agent initialization failed Failed to finalize decryption of the private key: %d. Failed to initialize the MD5 context: %d. Failed to initialize the MD5 engine: %s (%s) Failed to initialize the crypto engine. Failed to initialize the crypto engine: %d. Failed to initialize the decryption cipher context: %s (%s) Failed to initialize the decryption cipher slot. Failed to initialize the decryption context. Failed to initialize the encryption cipher context: %s (%s) Failed to initialize the encryption cipher slot. Failed to initialize the encryption context. IP configuration method. NMSettingIP4Config and NMSettingIP6Config both support "auto", "manual", and "link-local". See the subclass-specific documentation for other values. In general, for the "auto" method, properties such as "dns" and "routes" specify information that is added on to the information returned from automatic configuration.  The "ignore-auto-routes" and "ignore-auto-dns" properties modify this behavior. For methods that imply no upstream network, such as "shared" or "link-local", these properties must be empty. For IPv4 method "shared", the IP subnet can be configured by adding one manual IPv4 address or otherwise 10.42.x.0/24 is chosen. If TRUE, indicates that the network is a non-broadcasting network that hides its SSID. This works both in infrastructure and AP mode. In infrastructure mode, various workarounds are used for a more reliable discovery of hidden networks, such as probe-scanning the SSID.  However, these workarounds expose inherent insecurities with hidden SSID networks, and thus hidden SSID networks should be used with caution. In AP mode, the created network does not broadcast its SSID. Note that marking the network as hidden may be a privacy issue for you (in infrastructure mode) or client stations (in AP mode), as the explicit probe-scans are distinctly recognizable on the air. If you are creating a VPN, and the VPN connection you wish to create does not appear in the list, you may not have the correct VPN plugin installed. Ignore automatically obtained routes Ignoring unrecognized log domain(s) '%s' from config files.
 Ignoring unrecognized log domain(s) '%s' passed on command line.
 Modem initialization failed NetworkManager monitors all network connections and automatically
chooses the best connection to use.  It also allows the user to
specify wireless access points which wireless cards in the computer
should associate with. Never use this network for default route One of NM_SETTING_MAC_RANDOMIZATION_DEFAULT (0) (never randomize unless the user has set a global default to randomize and the supplicant supports randomization),  NM_SETTING_MAC_RANDOMIZATION_NEVER (1) (never randomize the MAC address), or NM_SETTING_MAC_RANDOMIZATION_ALWAYS (2) (always randomize the MAC address). This property is deprecated for 'cloned-mac-address'. Deprecated: 1 One or more flags which control the behavior and features of the VLAN interface.  Flags include NM_VLAN_FLAG_REORDER_HEADERS (0x1) (reordering of output packet headers), NM_VLAN_FLAG_GVRP (0x2) (use of the GVRP protocol), and NM_VLAN_FLAG_LOOSE_BINDING (0x4) (loose binding of the interface to its master device's operating state). NM_VLAN_FLAG_MVRP (0x8) (use of the MVRP protocol). The default value of this property is NM_VLAN_FLAG_REORDER_HEADERS, but it used to be 0. To preserve backward compatibility, the default-value in the D-Bus API continues to be 0 and a missing property on D-Bus is still considered as 0. PIN code required Specifies the allowed "phase 2" inner EAP-based authentication methods when an EAP method that uses an inner TLS tunnel is specified in the "eap" property.  Recognized EAP-based "phase 2" methods are "md5", "mschapv2", "otp", "gtc", and "tls". Each "phase 2" inner method requires specific parameters for successful authentication; see the wpa_supplicant documentation for more details. Specifies the allowed "phase 2" inner non-EAP authentication methods when an EAP method that uses an inner TLS tunnel is specified in the "eap" property.  Recognized non-EAP "phase 2" methods are "pap", "chap", "mschap", "mschapv2", "gtc", "otp", "md5", and "tls". Each "phase 2" inner method requires specific parameters for successful authentication; see the wpa_supplicant documentation for more details. System policy prevents sharing connections via a protected WiFi network System policy prevents sharing connections via an open WiFi network The device is lacking capabilities required by the connection. Unexpected failure to normalize the connection Usage: nmcli agent polkit { help }

Registers nmcli as a polkit action for the user session.
When a polkit daemon requires an authorization, nmcli asks the user and gives
the response back to polkit.

 Usage: nmcli agent secret { help }

Runs nmcli as NetworkManager secret agent. When NetworkManager requires
a password it asks registered agents for it. This command keeps nmcli running
and if a password is required asks the user for it.

 Usage: nmcli device wifi { ARGUMENTS | help }

Perform operation on Wi-Fi devices.

ARGUMENTS := [list [ifname <ifname>] [bssid <BSSID>]]

List available Wi-Fi access points. The 'ifname' and 'bssid' options can be
used to list APs for a particular interface, or with a specific BSSID.

ARGUMENTS := connect <(B)SSID> [password <password>] [wep-key-type key|phrase] [ifname <ifname>]
                     [bssid <BSSID>] [name <name>] [private yes|no] [hidden yes|no]

Connect to a Wi-Fi network specified by SSID or BSSID. The command creates
a new connection and then activates it on a device. This is a command-line
counterpart of clicking an SSID in a GUI client. The command always creates
a new connection and thus it is mainly useful for connecting to new Wi-Fi
networks. If a connection for the network already exists, it is better to
bring up the existing profile as follows: nmcli con up id <name>. Note that
only open, WEP and WPA-PSK networks are supported at the moment. It is also
assumed that IP configuration is obtained via DHCP.

ARGUMENTS := hotspot [ifname <ifname>] [con-name <name>] [ssid <SSID>]
                     [band a|bg] [channel <channel>] [password <password>]

Create a Wi-Fi hotspot. Use 'connection down' or 'device disconnect'
to stop the hotspot.
Parameters of the hotspot can be influenced by the optional parameters:
ifname - Wi-Fi device to use
con-name - name of the created hotspot connection profile
ssid - SSID of the hotspot
band - Wi-Fi band to use
channel - Wi-Fi channel to use
password - password to use for the hotspot

ARGUMENTS := rescan [ifname <ifname>] [[ssid <SSID to scan>] ...]

Request that NetworkManager immediately re-scan for available access points.
NetworkManager scans Wi-Fi networks periodically, but in some cases it might
be useful to start scanning manually. 'ssid' allows scanning for a specific
SSID, which is useful for APs with hidden SSIDs. More 'ssid' parameters can be
given. Note that this command does not show the APs,
use 'nmcli device wifi list' for that.

 Usage: nmcli general permissions { help }

Show caller permissions for authenticated operations.

 VPN disconnected Warning: polkit agent initialization failed: %s
 With "cloned-mac-address" setting "random" or "stable", by default all bits of the MAC address are scrambled and a locally-administered, unicast MAC address is created. This property allows to specify that certain bits are fixed. Note that the least significant bit of the first MAC address will always be unset to create a unicast MAC address. If the property is NULL, it is eligible to be overwritten by a default connection setting. If the value is still NULL or an empty string, the default is to create a locally-administered, unicast MAC address. If the value contains one MAC address, this address is used as mask. The set bits of the mask are to be filled with the current MAC address of the device, while the unset bits are subject to randomization. Setting "FE:FF:FF:00:00:00" means to preserve the OUI of the current MAC address and only randomize the lower 3 bytes using the "random" or "stable" algorithm. If the value contains one additional MAC address after the mask, this address is used instead of the current MAC address to fill the bits that shall not be randomized. For example, a value of "FE:FF:FF:00:00:00 68:F7:28:00:00:00" will set the OUI of the MAC address to 68:F7:28, while the lower bits are randomized. A value of "02:00:00:00:00:00 00:00:00:00:00:00" will create a fully scrambled globally-administered, burned-in MAC address. If the value contains more than one additional MAC addresses, one of them is chosen randomly. For example, "02:00:00:00:00:00 00:00:00:00:00:00 02:00:00:00:00:00" will create a fully scrambled MAC address, randomly locally or globally administered. can not set property: %s name remove [<value>|<index>|<option name>]  :: delete the value

Removes the property value. For single-valued properties, this sets the
property back to its default value. For container-type properties, this removes
all the values of that property, or you can specify an argument to remove just
a single item or option. The argument is either a value or index of the item to
remove, or an option name (for properties with named options).

Examples: nmcli ipv4.dns> remove 8.8.8.8
          nmcli ipv4.dns> remove 2
          nmcli bond.options> remove downdelay

 there exists a conflicting plugin (%s) that has the same %s.%s value there exists a conflicting plugin with the same name (%s) unknown error initializing plugin %s Project-Id-Version: PACKAGE VERSION
Report-Msgid-Bugs-To: 
PO-Revision-Date: 2018-12-22 17:10+0000
Last-Translator: Andi Chandler <Unknown>
Language-Team: Sugar Labs
MIME-Version: 1.0
Content-Type: text/plain; charset=UTF-8
Content-Transfer-Encoding: 8bit
Plural-Forms: nplurals=2; plural=n != 1;
X-Launchpad-Export-Date: 2019-07-18 11:13+0000
X-Generator: Launchpad (build 19010)
X-Project-Style: gnome
Language: en-GB
 A list of pairwise encryption algorithms which prevents connections to Wi-Fi networks that do not utilise one of the algorithms in the list. For maximum compatibility leave this property empty.  Each list element may be one of "tkip" or "ccmp". Active connection removed before it was initialised Add… Cannot change the connection type Connection removed before it was initialised Connection sharing via a protected Wi-Fi network Connection sharing via an open Wi-Fi network Could not daemonise: %s [error %u]
 Couldn't initialise PKCS#12 decoder: %d Couldn't initialise PKCS#12 decoder: %s Couldn't initialise PKCS#8 decoder: %s Device LLDP neighbours Error initialising certificate data: %s Error: Device '%s' was not recognised as a Wi-Fi device, check the NetworkManager Wi-Fi plug-in. Error: bad colour: %s
 Error: polkit agent initialisation failed: %s Error: secret agent initialisation failed Failed to finalise decryption of the private key: %d. Failed to initialise the MD5 context: %d. Failed to initialise the MD5 engine: %s (%s) Failed to initialise the crypto engine. Failed to initialise the crypto engine: %d. Failed to initialise the decryption cipher context: %s (%s) Failed to initialise the decryption cipher slot. Failed to initialise the decryption context. Failed to initialise the encryption cipher context: %s (%s) Failed to initialise the encryption cipher slot. Failed to initialise the encryption context. IP configuration method. NMSettingIP4Config and NMSettingIP6Config both support "auto", "manual", and "link-local". See the subclass-specific documentation for other values. In general, for the "auto" method, properties such as "dns" and "routes" specify information that is added on to the information returned from automatic configuration.  The "ignore-auto-routes" and "ignore-auto-dns" properties modify this behaviour. For methods that imply no upstream network, such as "shared" or "link-local", these properties must be empty. For IPv4 method "shared", the IP subnet can be configured by adding one manual IPv4 address or otherwise 10.42.x.0/24 is chosen. If TRUE, indicates that the network is a non-broadcasting network that hides its SSID. This works both in infrastructure and AP mode. In infrastructure mode, various workarounds are used for a more reliable discovery of hidden networks, such as probe-scanning the SSID.  However, these workarounds expose inherent insecurities with hidden SSID networks, and thus hidden SSID networks should be used with caution. In AP mode, the created network does not broadcast its SSID. Note that marking the network as hidden may be a privacy issue for you (in infrastructure mode) or client stations (in AP mode), as the explicit probe-scans are distinctly recognisable on the air. If you are creating a VPN, and the VPN connection you wish to create does not appear in the list, you may not have the correct VPN plug-in installed. Ignore automatically-obtained routes Ignoring unrecognised log domain(s) '%s' from config files.
 Ignoring unrecognised log domain(s) '%s' passed on command line.
 Modem initialisation failed NetworkManager monitors all network connections and automatically
chooses the best connection to use.  It also allows the user to
specify with which wireless access points the wireless cards in the computer
should associate. Never use this network as the default route One of NM_SETTING_MAC_RANDOMIZATION_DEFAULT (0) (never randomise unless the user has set a global default to randomise and the supplicant supports randomisation),  NM_SETTING_MAC_RANDOMIZATION_NEVER (1) (never randomise the MAC address), or NM_SETTING_MAC_RANDOMIZATION_ALWAYS (2) (always randomise the MAC address). This property is deprecated for 'cloned-mac-address'. Deprecated: 1 One or more flags which control the behaviour and features of the VLAN interface.  Flags include NM_VLAN_FLAG_REORDER_HEADERS (0x1) (reordering of output packet headers), NM_VLAN_FLAG_GVRP (0x2) (use of the GVRP protocol), and NM_VLAN_FLAG_LOOSE_BINDING (0x4) (loose binding of the interface to its master device's operating state). NM_VLAN_FLAG_MVRP (0x8) (use of the MVRP protocol). The default value of this property is NM_VLAN_FLAG_REORDER_HEADERS, but it used to be 0. To preserve backward compatibility, the default-value in the D-Bus API continues to be 0 and a missing property on D-Bus is still considered as 0. A PIN code is required Specifies the allowed "phase 2" inner EAP-based authentication methods when an EAP method that uses an inner TLS tunnel is specified in the "eap" property.  Recognised EAP-based "phase 2" methods are "md5", "mschapv2", "otp", "gtc", and "tls". Each "phase 2" inner method requires specific parameters for successful authentication; see the wpa_supplicant documentation for more details. Specifies the allowed "phase 2" inner non-EAP authentication methods when an EAP method that uses an inner TLS tunnel is specified in the "eap" property.  Recognised non-EAP "phase 2" methods are "pap", "chap", "mschap", "mschapv2", "gtc", "otp", "md5", and "tls". Each "phase 2" inner method requires specific parameters for successful authentication; see the wpa_supplicant documentation for more details. System policy prevents sharing connections via a protected Wi-Fi network System policy prevents sharing connections via an open Wi-Fi network The device lacks capabilities required by the connection. Unexpected failure to normalise the connection Usage: nmcli agent polkit { help }

Registers nmcli as a polkit action for the user session.
When a polkit daemon requires an authorisation, nmcli asks the user and gives
the response back to polkit.

 Usage: nmcli agent secret { help }

Runs nmcli as NetworkManager secret agent. When NetworkManager requires
a password, it asks registered agents for it. This command keeps nmcli running
and if a password is required, asks the user for it.

 Usage: nmcli device wifi { ARGUMENTS | help }

Perform operation on Wi-Fi devices.

ARGUMENTS := [list [ifname <ifname>] [bssid <BSSID>]]

List available Wi-Fi access points. The 'ifname' and 'bssid' options can be
used to list APs for a particular interface, or with a specific BSSID.

ARGUMENTS := connect <(B)SSID> [password <password>] [wep-key-type key|phrase] [ifname <ifname>]
                     [bssid <BSSID>] [name <name>] [private yes|no] [hidden yes|no]

Connect to a Wi-Fi network specified by SSID or BSSID. The command creates
a new connection and then activates it on a device. This is a command-line
counterpart of clicking an SSID in a GUI client. The command always creates
a new connection, and thus it is mainly useful for connecting to new Wi-Fi
networks. If a connection for the network already exists, it is better to
bring up the existing profile as follows: nmcli con up id <name>. Note that
only 'open', 'WEP' and 'WPA-PSK' networks are supported at the moment. It is also
assumed that IP configuration is obtained via DHCP.

ARGUMENTS := hotspot [ifname <ifname>] [con-name <name>] [ssid <SSID>]
                     [band a|bg] [channel <channel>] [password <password>]

Create a Wi-Fi hotspot. Use 'connection down' or 'device disconnect'
to stop the hotspot.
Parameters of the hotspot can be influenced by the optional parameters:
ifname - Wi-Fi device to use
con-name - name of the created hotspot connection profile
ssid - SSID of the hotspot
band - Wi-Fi band to use
channel - Wi-Fi channel to use
password - password to use for the hotspot

ARGUMENTS := rescan [ifname <ifname>] [[ssid <SSID to scan>] ...]

Request that NetworkManager immediately re-scan for available access points.
NetworkManager scans Wi-Fi networks periodically, but in some cases it might
be useful to start scanning manually. 'ssid' allows scanning for a specific
SSID, which is useful for APs with hidden SSIDs. More 'ssid' parameters can be
given. Note that this command does not show the APs,
use 'nmcli device wifi list' for that.

 Usage: nmcli general permissions { help }

 Disconnected Warning: polkit agent initialisation failed: %s
 With "cloned-mac-address" setting "random" or "stable", by default all bits of the MAC address are scrambled and a locally-administered, unicast MAC address is created. This property allows to specify that certain bits are fixed. Note that the least significant bit of the first MAC address will always be unset to create a unicast MAC address. If the property is NULL, it is eligible to be overwritten by a default connection setting. If the value is still NULL or an empty string, the default is to create a locally-administered, unicast MAC address. If the value contains one MAC address, this address is used as mask. The set bits of the mask are to be filled with the current MAC address of the device, while the unset bits are subject to randomisation. Setting "FE:FF:FF:00:00:00" means to preserve the OUI of the current MAC address and only randomize the lower 3 bytes using the "random" or "stable" algorithm. If the value contains one additional MAC address after the mask, this address is used instead of the current MAC address to fill the bits that shall not be randomised. For example, a value of "FE:FF:FF:00:00:00 68:F7:28:00:00:00" will set the OUI of the MAC address to 68:F7:28, while the lower bits are randomised. A value of "02:00:00:00:00:00 00:00:00:00:00:00" will create a fully scrambled globally-administered, burnt-in MAC address. If the value contains more than one additional MAC addresses, one of them is chosen randomly. For example, "02:00:00:00:00:00 00:00:00:00:00:00 02:00:00:00:00:00" will create a fully scrambled MAC address, randomly locally or globally administered. cannot set property: %s Name remove [<value>|<index>|<option name>]  :: delete the value

Removes the property value. For single-valued properties, this sets the
property back to its default value. For container-type properties, this removes
all the values of that property; or you can specify an argument to remove just
a single item or option. The argument is either a value or index of the item to
be removed, or an option name (for properties with named options).

Examples: nmcli ipv4.dns> remove 8.8.8.8
          nmcli ipv4.dns> remove 2
          nmcli bond.options> remove downdelay

 a conflicting plugin exists (%s) that has the same %s.%s value a conflicting plugin with the same name exists (%s) unknown error initialising plugin %s 