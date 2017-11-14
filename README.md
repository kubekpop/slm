# Simple Linux Manager
Manager for most common services. Project website: https://slm.tuxdev.com

## README
### 0. WARNING
Tuxdev team puts much effort into the code to have it working across all distributions, but some errors might occur and might damage your service/system. We take no responsibility for such situation. Please use sections marked as (testing) in a safe environment before working on production system.

### 1. DEPENDENCIES
SLM uses following commands: netstat, systemctl, ps, iptables, awk, sed, grep. Make sure to have these available. Here is a brief list of packages not always installed by default with their distro-specific names:
* SUSE: net-tools-deprecated sshpass
* ARCH: sshpass
* FEDORA: sshpass
* DEBIAN/UBUNTU: sshpass

### 2. USAGE
SLM is a GUI binary file either installed from rpm/deb package or run directly from an executable. It's supposed to run as a standard user. Advanced options appear after authenticating in a program.

#### Main window
This section shows manageable services, their status, functional buttons to start/stop/restart and configure basic options.

#### FTP Configurator
This panel is used for vsftpd server configuration. Available options are:
* ftpd_banner 
* anonymous_enable
* local_enable
* chroot_local_user
* 
 
#### NFS Configurator
This panel is used for nfs server configuration. It allows to manage most common export options:
* squash
* rw/ro permissions
* access permissions
* other options in plain text edit field

#### Firewall Configurator
This panel is used for firewall configuration. It uses iptables command. Available features (only adding and deleting, no edit available yet
* DNAT port forwarding
* Masquerade LAN to WAN
* Open ports
* List rules in chain and delete them

#### Apache Configurator
This panel is used for Apache configuration. Available features:
* Add new vhost
  * Root directory
  * Server address
  * Log file
* Manage existing vhosts
  * Change root directory
  * Change server address

#### Samba Configurator

#### DHCP Configurator

#### Backup-Manager
This panel is used for backup-manager configuration. It allows to modify following options in config file:
* Incremental frequency
* Master tarball frequency
* File format
* Filename prefix
* Directories to backup
* Backup directory
On first launch SLM checks what method of directory specification your config file uses. If it detects old method (BM_TARBALL_DIRECTORIES) it asks for permission to convert it to array method (BM_TARBALL_TARGETS). Not allowing it to do so will result in closing the window and denying functionality.

### 3. LOG FILES

### 4. MISC

### 5. COMMON PROBLEMS

