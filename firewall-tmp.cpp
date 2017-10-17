
void MainWindow::on_iptablesapply_clicked()
{
    if(ui->czyrouter->isChecked() == true)
    {
        QString localinterface = ui->localinterface->currentText();
        QString internet = ui->internet->currentText();
        QString commandroutersave = "iptables -t nat -A POSTROUTING -o "+internet+" -j MASQUERADE; sysctl -w net.ipv4.ip_forward=1; ";
        commandroutersave += "iptables -A FORWARD -o "+localinterface+" -m state --state ESTABLISHED,RELATED -j ACCEPT;";//dodane -o localinterface
        //ponizej dodane sprawdzic czy dziala
        commandroutersave += "iptables -A FORWARD -o "+internet+" -m state --state NEW,ESTABLISHED,RELATED -j ACCEPT;";
//
//SPRAWDZIĆ CO SIĘ TUTAJ ODWALA Z TYM PUBLIC IP BO JAKIEŚ WTF JEST
//SPRAWDZIĆ CO SIĘ TUTAJ ODWALA Z TYM PUBLIC IP BO JAKIEŚ WTF JEST
//SPRAWDZIĆ CO SIĘ TUTAJ ODWALA Z TYM PUBLIC IP BO JAKIEŚ WTF JEST
//OGULNIE TO W INNEJ SEKCJI TRZA OKREŚLAĆ PUBLICZNOŚĆ INTERFEJSÓW, TUTAJ TYLKO RELATED I ESTABLISHED
//

        if(ui->czypublicip->isChecked() == true)
        {
            commandroutersave += "iptables -A INPUT -i "+internet+" -m state --state NEW -j ACCEPT; ";//jesli ma być dostęp z zewnątrz, ale określić trzeba by interfejs i co to w sumie ma do maskarady, bez sensu zupełnie
            //dodać interfejs wejściowy
        }
        else
        {
            // ta pozwala wszystkim innym, zło commandroutersave += "iptables -A INPUT -m state --state NEW -i ! "+internet+" -j ACCEPT; ";//jesli nie z zewnoncz;
            commandroutersave += "iptables -A INPUT -m state --state NEW -i "+internet+" -j DROP; ";
            //to też bez sensu reguła, dosyć niebezpieczna na górze stosu reguł
        }
        //commandroutersave += "iptables -P INPUT DROP; ";
        //commandroutersave += "iptables -A FORWARD -i "+internet+" -o "+internet+" -m state --state NEW -j REJECT; ";
        //=================================PROCES
        QProcess routersave;
        routersave.start("/bin/bash", QStringList()<< "-c" << commandroutersave);
        routersave.waitForFinished();
        routersave.close();
    }

    if(ui->czydenat->isChecked() == true)
    {
        //czydziala?
        QString sip = ui->dsourceip->text();//Public Interface IP
        QString sport = ui->dsourceport->text();//Ports To Forward
        QString sinterface = ui->dsourceinterface->currentText();//Public Interface
        QString dip = ui->ddestinationip->text();//Local Server IP (destination)
        QString dport = ui->ddestinationport->text();//Local Port (destination)
        QString dinterface = ui->ddestinationinterface->currentText();//Local Interface (connecting to LAN)
        //QString commanddenatsave = "iptables -t nat -A PREROUTING -i "+sinterface+" -p tcp --sport "+sport+" -d "+sip+" --dport "+dport+" -j DNAT --to-destination "+dip;
        QString commanddenatsave = "sysctl -w net.ipv4.ip_forward=1; ";
        //commanddenatsave += ";<br />";
        commanddenatsave += "iptables -t nat -A PREROUTING ";
        if(sip != "")
        {
            commanddenatsave += "-d "+sip+" ";
        }
        if(sinterface != "")
        {
            commanddenatsave += "-i "+sinterface+" ";
        }
        if(sport != "")
        {
            commanddenatsave += "-p tcp --dport "+sport+" ";
        }

        commanddenatsave += "-j DNAT --to-destination "+dip+":"+dport+"; ";
       /*
        if(sinterface != "")
        {
            commanddenatsave += "iptables -t nat -A PREROUTING -i "+sinterface+" -p tcp --dport "+sport+" -j DNAT --to-destination "+dip+":"+dport;//main DNAT rule
        }
        else
        {
            commanddenatsave += "iptables -t nat -A PREROUTING -p tcp --dport "+sport+" -j DNAT --to-destination "+dip+":"+dport;//main DNAT rule
        }
        */
        commanddenatsave += "iptables -A FORWARD ";
        if(sinterface != "")
        {
            commanddenatsave += "-i "+sinterface+" ";
        }
        if(dinterface != "")
        {
            commanddenatsave += "-o "+dinterface+" ";
        }
        commanddenatsave += "-p tcp --sport "+sport+" -d "+dip+" --dport "+dport+" -m state --state NEW -j ACCEPT; ";
        commanddenatsave += "iptables -A FORWARD ";
        if(dinterface != "")
        {
            commanddenatsave += "-i "+dinterface+" ";
        }
        if(sinterface != "")
        {
            commanddenatsave += "-o "+sinterface+" ";
        }
        commanddenatsave += " -m state --state ESTABLISHED,RELATED -j ACCEPT; ";

        commanddenatsave += "iptables -A FORWARD ";
        if(dinterface != "")
        {
            commanddenatsave += "-o "+dinterface+" ";
        }
        if(sinterface != "")
        {
            commanddenatsave += "-i "+sinterface+" ";
        }
        commanddenatsave += " -m state --state ESTABLISHED,RELATED -j ACCEPT; ";

        //commanddenatsave += "iptables -A FORWARD -i "+dinterface+" -o "+sinterface+" -m state --state ESTABLISHED,RELATED -j ACCEPT";
        //commanddenatsave += "iptables -A FORWARD -i "+sinterface+" -o "+dinterface+" -m state --state ESTABLISHED,RELATED -j ACCEPT";



        //XXX add: checking if rule exists, or remove duplicates
        //iptables-save | awk '!x[$0]++'
        consolefeed += consolebegin+commanddenatsave;
        //=================================PROCES
        QProcess denatsave;
        denatsave.start("/bin/bash", QStringList()<< "-c" << "echo \""+commanddenatsave+"\" > denat.txt && chmod 777 denat.txt");
        denatsave.waitForFinished();
        denatsave.close();
    }
    iptablesmanager->close();
}

void MainWindow::iptablesAllowedServices()
{

    //foricz zaznaczona usługa
    QString port = "22";
    QProcess addAllowedService;
    addAllowedService.start("/bin/bash", QStringList()<< "-c" << "iptables -A INPUT -p tcp -m tcp --dport "+port+" -j ACCEPT");
    addAllowedService.waitForFinished();
    addAllowedService.close();


    //konec foricz
}
