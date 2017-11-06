#include "backup_window.h"
#include "ui_backup_window.h"

backup_window::backup_window(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::backup_window)
{
    ui->setupUi(this);
}

backup_window::~backup_window()
{
    delete ui;
}

/*
GUI for backup-manager https://github.com/sukria/Backup-Manager
-load backup directories from /etc/backup-manager.conf
-interpret backup directories, change all to array if space exists in at least one directory
-load destination dir from /etc/backup-manager.conf





# Where to store the archives
export BM_REPOSITORY_ROOT="/var/archives"

# At which frequency will you build your archives?
# You can choose either "daily" or "hourly".
# This should match your CRON configuration.
export BM_ARCHIVE_FREQUENCY="daily"

# Prefix of every archive on that box (default is HOSTNAME)
export BM_ARCHIVE_PREFIX="$HOSTNAME"

# Archive filename format
# 	long  : host-full-path-to-folder.tar.gz
# 	short : parentfolder.tar.gz
export BM_TARBALL_NAMEFORMAT="long"


# Type of archives
# Available types are:
#     tar, tar.gz, tar.bz2, tar.xz, tar.lzma, dar, zip.
# Make sure to satisfy the appropriate dependencies
# (bzip2, dar, xz, lzma, ...).
export BM_TARBALL_FILETYPE="tar.gz"

# Paths without spaces in their name:
# export BM_TARBALL_DIRECTORIES="/etc /boot"

# If one or more of the targets contain a space, use the array:
declare -a BM_TARBALL_TARGETS

BM_TARBALL_TARGETS[0]="/etc"
BM_TARBALL_TARGETS[1]="/boot"

export BM_TARBALL_TARGETS

# Files to exclude when generating tarballs, you can put absolute
# or relative paths, Bash wildcards are possible.
export BM_TARBALL_BLACKLIST="/dev /sys /proc /tmp"

*/
