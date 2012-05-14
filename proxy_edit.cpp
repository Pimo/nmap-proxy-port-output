/*
 * Creato da Simone Locci
 * il 14/05/2012
 * prima bozza: prende un file in input dall'output di nmap
 * e lo trasforma in ip:porta dove la porta è aperta.
 * da nmap: sudo nmap -sS -p 8080,3128,80 --script http-open-proxy IP >> prova.txt
 */

#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int main() {
    string s=""; //stringa usata da getline

    //nome del file da aprire
    string nome_file_proxy="prova.txt"; //da editare
    ifstream f(nome_file_proxy.c_str());

    //nome del file d'appoggio
    string nome_file_appoggio="proxyproxyproxy.txt"; //da editare (opzionale)
    ofstream g(nome_file_appoggio.c_str());

    if(!f) {
        cout<<"Il file non esiste!";
        return -1;
    }

    //levo latency perchè mi causa interferenze con l'ip
    while(f.good()) {
        getline(f, s);

        size_t k=s.find("latency");

        if (k!=string::npos)
            s.replace(0,s.length(),"latency_replaced");

        g << s << endl;
    }
    g.close();
    f.close();


    //ora utlizzo il file senza latency e risalvo tutto sul file originale
    ifstream m(nome_file_appoggio.c_str());
    ofstream n(nome_file_proxy.c_str());


    string ip_stringa="";
    while(m.good()) //fino a quando c'è qualcosa da leggere ..
    {
        //legge tutta la riga dal file e la mette nella variabile s
        getline(m, s);

        //creo una stringa per contenere l'ip
        string ip_array="";

        //individuo l'ip che è tra ()
        size_t i=s.find_first_of('(');
        size_t j=s.find_last_of(')');

        //vuoto la stringa dell'ip e salvo la nuova compresa di ()
        if (i!=string::npos && j!=string::npos) {
            ip_stringa="";
            ip_array=s.substr(i, j); //salvo l'ip
        }

        //trasformo l'ip in array e lo salvo su una variabile di tipo string
        ip_array.c_str();
        for(int i=0; i<ip_array.length(); i++){
            if((ip_array.at(i) >='0' && ip_array.at(i) <='9') || ip_array.at(i) =='.')
            ip_stringa+=ip_array.at(i);
        }

        //cerco le porte aperte
        string port_array="";

        size_t k=s.find("open");

        //se è aperta stampo l'ip
        if (k!=string::npos){
            n<<endl;
            n<<ip_stringa<<":";
            port_array=s.substr(0, k);
        }

        //stampo la porta senza protocollo
        port_array.c_str();
        for(int i=0; i<port_array.length(); i++){
            if(port_array.at(i) >='0' && port_array.at(i) <='9')
            n<<port_array.at(i);
        }

    }

    m.close(); //chiude il file
    n.close(); //chiude il file

    return 0;
}
