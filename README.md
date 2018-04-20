# Simple Linux Manager
Manager for most common services. Project website: https://slm.tuxdev.com

## README
### 0. WARNING
Tuxdev team puts much effort into the code to have it working across all distributions, but some errors might occur and might damage your service/system. We take no responsibility for such situation. Please use sections marked as (testing) in a safe environment before working on production system. Sections marked as (Preview) are yet to be implemented, but only basic functionality (mostly GUI) is ready at the moment.

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
* write_enable
* anon_mkdir_write_enable
* anon_upload_enable
* anonymous_root
* chown_uploads
* chown_username
 
#### NFS Configurator
This panel is used for nfs server configuration. It allows to manage most common export options:
* path
* squash
* rw/ro permissions
* access permissions
* other options in plain text edit field
Unfortuately, there is no support for multi-access shares (multiple permissions in one line)

#### Firewall Configurator (preview, WIP)
This panel is used for firewall configuration. It uses iptables command. Available features (only adding and deleting, no edit available yet
* DNAT port forwarding
* Masquerade LAN to WAN
* Open ports
* List rules in chain and delete them (preview)

#### Apache Configurator (preview)
This panel is used for Apache configuration. Available features:
* Add new vhost
  * Root directory
  * Server address
  * Log file
* Manage existing vhosts
  * Change root directory
  * Change server address

#### Samba Configurator (preview)

#### DHCP Configurator (preview)

#### Backup-Manager
This panel is used for backup-manager configuration. It allows to modify following options in config file:
* Incremental frequency
* Master tarball frequency
* File format
* Filename prefix
* Directories to backup
* Backup directory
On first launch SLM checks what method of directory specification your config file uses. If it detects old method (BM_TARBALL_DIRECTORIES) it asks for permission to convert it to array method (BM_TARBALL_TARGETS). Not allowing it to do so will result in closing the window and denying functionality.

#### DNS Configurator (preview)
This panel is used to manage DNS server (bind). It allows to create and manage zones:
* Create forward lookup zone
* Create reverse lookup zone
* Manage records in zones
* Show records in forward lookup zones (done)

#### QEMU/KVM
This panel is used to view and manage existing virtual machines (doesn't support creating new domains).
* List machines
* Show machine's status
* Start machine
* Stop machine
* Restart machine
* Show domain's XML (not yet)

#### Docker
This panel is used to manage Docker containers
* Show status
* List existing containers
* List port mapping
* List directory mapping
* Create new container on existing image
* Start/Stop/Restart(not yet)/Delete(not yet)

### 3. LOG FILES (preview)
SLM stores most effective commands in a log file located in /var/logs/slm.log. 

### 4. MISC

### 5. COMMON PROBLEMS

