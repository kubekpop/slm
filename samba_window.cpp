#include "samba_window.h"
#include "ui_samba_window.h"

samba_window::samba_window(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::samba_window)
{
    ui->setupUi(this);
}

samba_window::~samba_window()
{
    delete ui;
}

/*

#!/bin/bash
mkdir /tmp/samba-shares
grep -n -e '\[' /etc/samba/smb.conf |  grep -v global | grep -v homes | grep -v printers | grep -v 'print\$' | cut -d: -f1 > /tmp/shares
ilosc=$(wc -l /tmp/shares | cut -d" " -f1)
for (( i=1; $i<=$ilosc; i++ ));do
line1=$(sed -n "$i"p /tmp/shares) #1st line
helper=$(($i + 1))
line2=$(sed -n "$helper"p /tmp/shares) #last line
line2=$(($line2 - 1));
if [[ $i == $ilosc ]];then line2=$(wc -l /etc/samba/smb.conf | cut -d" " -f1); fi
sed -n "$line1","$line2"p /etc/samba/smb.conf > /tmp/samba-shares/share_$line1-$line2
done

Na tym etapie mamy w katalogu /tmp/samba-shares/ pliki share_pierwszalinia-ostatnialinia, wyglądające coś koło tego:
[test3]
        path = /gumolit2
        comment = "so"
        browseable = yes
        
Z nich trzeba wziąć pierwszą linijkę i to jest nazwa udziału, na tym pliku dokonujemy zmian i potem wstawić ten plik do /etc/samba/smb.conf w miejsce linijek określonych w nazwie
  */



/*
 FULL-DEBUG:

#!/bin/bash

grep -n -e '\[' /etc/samba/smb.conf |  grep -v global | grep -v homes | grep -v printers | grep -v 'print\$' | cut -d: -f1 > /tmp/shares
#wc -l /tmp/shares -> eg. 5 shares
#wc -l /etc/samba/smb.conf -> end of last share
ilosc=$(wc -l /tmp/shares | cut -d" " -f1)
echo "Ilość: "$ilosc;
  for (( i=1; $i<=$ilosc; i++ ));do
  line1=$(sed -n "$i"p /tmp/shares) #1st line
  echo "Line1: $line1"
  helper=$(($i + 1))
  echo "Helper: $helper"
  line2=$(sed -n "$helper"p /tmp/shares) #last line
  line2=$(($line2 - 1));
  if [[ $i == $ilosc ]];then line2=$(wc -l /etc/samba/smb.conf | cut -d" " -f1); fi
   echo "Line2: $line2"

  echo "I: $i"

  sed -n "$line1","$line2"p /etc/samba/smb.conf > /tmp/share-$i
  done



  */
