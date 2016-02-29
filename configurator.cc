#include<iostream>
#include<fstream>
#include<cstdlib>
#include<string>
#include "configurator.h"

using namespace std;

string themy[500], kolory[2][500][500]; //[0][a][b] - nazwa koloru; [1][a][b] - kod koloru
int colors[500]; //ilosc kolorow w danym themie
int ilosc_themow;
bool dual[500];
extern int wyswietl(string, int, int);
extern int wys, szer;
int rozpoznajthem(string nazwa)
{
  for(int i=0; i<ilosc_themow; i++)
  {
    if(themy[i]==nazwa)
    {
      return i;
    }
  }
  return -1;
}
int rozpoznajcolor(string nazwa)
{
  ifstream config;
  ofstream config2;
  string tmp="./config.cfg", tmp2;
  config.open(tmp.c_str());
  string them;
  int numerthemu=-1;
  while(!config.eof())
  {
    getline(config, tmp2);
    if(tmp2.size()>0)
    {
      if(tmp2.find(":theme")!=string::npos)
      {
        int pocz=tmp2.find('"');
        string tmp3=tmp2.substr(pocz+1);
        int dlu=tmp3.find('"');
        them=tmp2.substr(pocz+1, dlu);
        for(int i=0; i<500; i++)
        {
          if(themy[i]==them)
          {
            numerthemu=i;
            break;
          }
        }
      }
    }
  }
  if(numerthemu==-1)
  {
    return -2;
  }
  for(int i=0; i<colors[numerthemu]; i++)
  {
    if(kolory[0][numerthemu][i]==nazwa)
    {
      return i;
    }
  }
  return -1;
}
string instantc(string name)
{
  string out;
  if(rozpoznajcolor(name)<0)
  {
    if(rozpoznajcolor(name)==-1)
    {
      out=name+" not found";
      return out;
    }
    else
    {
      out="Theme path not found.\nFirst change theme to correct";
      return out;
    }
  }
  ifstream config;
  ofstream config2;
  string tmp="./config.cfg", tmp2, rm="rm ./config.tmp", cp="cp ./config.tmp ./config.cfg";
  config.open(tmp.c_str());
  int licznik=0, pozycja;
  while(!config.eof())
  {
    getline(config, tmp2);
    if(tmp2.size()>0)
    {
      licznik++;
      if(tmp2.find(":color")!=string::npos)
      {
        pozycja=licznik;
      }
    }
  }
  int licznik2=0;
  tmp2="./config.tmp";
  config.close();
  config.open(tmp.c_str());
  config2.open(tmp2.c_str());
  string tmp3;
  while(licznik2!=licznik)
  {
    getline(config, tmp3);
    if(tmp3.size()>0)
    {
      licznik2++;
      if(licznik2==pozycja)
      {
        config2<<" \""<<name<<"\" :color\n";
      }
      else
      {
        config2<<tmp3<<"\n";
      }
    }
  }
  config.close();
  config2.close();
  system(cp.c_str());
  system(rm.c_str());
  out="\nColor changed to "+name+"\n";
  return out;
}
string instantt(string name)
{
  ifstream config;
  ofstream config2;
  if(rozpoznajthem(name)==-1)
  {
    string out="\n"+name+" theme not exist\n";
    return out;
  }
  string tmp="./config.cfg", tmp2, rm="rm ./config.tmp", cp="cp ./config.tmp ./config.cfg";
  config.open(tmp.c_str());
  string them;
  string newtheme;
  int numerthemu;
  int licznik=0, pozycja;
  while(!config.eof())
  {
    getline(config, tmp2);
    if(tmp2.size()>0)
    {
      licznik++;
      if(tmp2.find(":theme")!=string::npos)
      {
        pozycja=licznik;
        int pocz=tmp2.find('"');
        string tmp3=tmp2.substr(pocz+1);
        int dlu=tmp3.find('"');
        them=tmp2.substr(pocz+1, dlu);
        for(int i=0; i<500; i++)
        {
          if(themy[i]==them)
          {
            numerthemu=i;
            break;
          }
        }
      }
    }
  }
  int licznik2=0;
  tmp2="./config.tmp";
  config.close();
  config.open(tmp.c_str());
  config2.open(tmp2.c_str());
  string tmp3;
  while(licznik2!=licznik)
  {
    getline(config, tmp3);
    if(tmp3.size()>0)
    {
      licznik2++;
      if(licznik2==pozycja)
      {
        config2<<" \""<<name<<"\" :theme\n";
      }
      else
      {
        config2<<tmp3<<"\n";
      }
    }
  }
  config.close();
  config2.close();
  system(cp.c_str());
  system(rm.c_str());
  string out="\nTheme changed to "+name+"\n\n";
  return out;
}
void znajdz_themy()
{
  string polecenie="find -L ./themes -name \"theme.cfg\" > ./themy.tmp";
  system(polecenie.c_str());
  ifstream tmp;
  tmp.open("themy.tmp");
  int licznik=0;
  string tmp2;
  while(!tmp.eof())
  {
    getline(tmp, tmp2);
    if(tmp2.size()>0){
    int tmpsize=tmp2.size();
    themy[licznik]=tmp2.substr(9, tmpsize-19);
    licznik++;
   }
  }
  //cout<<licznik<<" themes found\n";
  ilosc_themow=licznik;
}
void skanuj_themy()
{
  ifstream tmp;
  for(int i=0; i<ilosc_themow; i++)
  {
    string them="./themes/"+themy[i]+"/theme.cfg";
    tmp.open(them.c_str());
    int licznik=0;
    string tmp2;
    while(!tmp.eof())
    {
      getline(tmp, tmp2);
      if(tmp2.size()>0)
      {
        if(tmp2.find(":dual")!=string::npos)
        {
          if(tmp2.find("true")!=string::npos)
          {
            dual[i]=true;
          }
          if(tmp2.find("false")!=string::npos)
          {
            dual[i]=false;
          }
        }
        if(tmp2.find(":list")!=string::npos)
        {
          while(!tmp.eof())
          {
            getline(tmp, tmp2);
            if(tmp2.size()>0)
            {
              if(tmp2.find(')')!=string::npos)
              {
                break;
              }
              if(tmp2.find('"')!=string::npos && tmp2.find(':')!=string::npos)
              {
                kolory[0][i][licznik]=tmp2.substr((tmp2.find(':'))+1);
                kolory[1][i][licznik]=tmp2.substr((tmp2.find('"'))+1, ((tmp2.rfind('"'))-(tmp2.find('"'))-1));
                licznik++;
              }
            }
          }
        }
      }
    }
    colors[i]=licznik;
    tmp.close();
  }
}
void zmienthem()
{
  system("clear");
  ifstream config;
  ofstream config2;
  string tmp="./config.cfg", tmp2, rm="rm ./config.tmp", cp="cp ./config.tmp ./config.cfg";
  config.open(tmp.c_str());
  string them;
  string newtheme;
  int numerthemu;
  int pozycjalisty=0;
  int licznik=0, pozycja;
  while(!config.eof())
  {
    getline(config, tmp2);
    if(tmp2.size()>0)
    {
      licznik++;
      if(tmp2.find(":theme")!=string::npos)
      {
        pozycja=licznik;
        int pocz=tmp2.find('"');
        string tmp3=tmp2.substr(pocz+1);
        int dlu=tmp3.find('"');
        them=tmp2.substr(pocz+1, dlu);
        for(int i=0; i<500; i++)
        {
          if(themy[i]==them)
          {
            numerthemu=i;
            break;
          }
        }
      }
    }
  }
  int licznik2=0;
  tmp2="./config.tmp";
  config.close();
  config.open(tmp.c_str());
  config2.open(tmp2.c_str());
  string tmp3;
  while(licznik2!=licznik)
  {
    getline(config, tmp3);
    if(tmp3.size()>0)
    {
      licznik2++;
      if(licznik2==pozycja)
      {
        cout<<"Current theme: "<<them<<"\n\nW prawo podglad, enter aby wybrac:\n";
        int lol=0;
        bool czy=1;

        while(lol!=10)
        {
          cout<<"\033[0;0f\033[3B";
          for(int i=0; i<ilosc_themow and czy; i++)
          {
            if(i==pozycjalisty)
            {
              cout<<"\033[1m*";
            }
            else
            {
              cout<<"\033[0m-";
            }
            cout<<themy[i]<<"\n";
          }
          cout<<"\033[0m";
          lol=getch();
          czy=0;
          if(lol==65 or lol==66 or lol==67)
          {
            czy=1;
            if(lol==65)
            {
              pozycjalisty--;
              if(pozycjalisty<0)
              {
                pozycjalisty=ilosc_themow-1;
              }
            }
            else if(lol==66)
            {
              pozycjalisty++;
              if(pozycjalisty>ilosc_themow-1)
              {
                pozycjalisty=0;
              }
            }
            else
            {
              string tpath="./themes/"+themy[pozycjalisty]+"/bul_03_o.png";
              rozdzialka(tpath);
              wyswietl(tpath, szer, wys);
            }
          }
        }
        newtheme=themy[pozycjalisty];
        config2<<" \""<<newtheme<<"\" :theme\n";
      }
      else
      {
        config2<<tmp3<<"\n";
      }
    }
  }
  config.close();
  config2.close();
  system(cp.c_str());
  system(rm.c_str());
  cout<<"\nTheme changed to "<<newtheme<<"\n";
}
void zmienkolor()
{
  system("clear");
  ifstream config;
  ofstream config2;
  string tmp="./config.cfg", tmp2, rm="rm ./config.tmp", cp="cp ./config.tmp ./config.cfg";
  config.open(tmp.c_str());
  string them, curcolor;
  int numerthemu;
  int pozycjalisty=0;
  int licznik=0, pozycja;
  while(!config.eof())
  {
    getline(config, tmp2);
    if(tmp2.size()>0)
    {
      licznik++;
      if(tmp2.find(":theme")!=string::npos)
      {
        int pocz=tmp2.find('"');
        string tmp3=tmp2.substr(pocz+1);
        int dlu=tmp3.find('"');
        them=tmp2.substr(pocz+1, dlu);
        for(int i=0; i<500; i++)
        {
          if(themy[i]==them)
          {
            numerthemu=i;
            break;
          }
        }
      }
      if(tmp2.find(":color")!=string::npos)
      {
        pozycja=licznik;
        int pocz=tmp2.find('"');
        string tmp3=tmp2.substr(pocz+1);
        int dlu=tmp3.find('"');
        curcolor=tmp2.substr(pocz+1, dlu);
      }
    }
  }
  int licznik2=0;
  tmp2="./config.tmp";
  config.close();
  config.open(tmp.c_str());
  config2.open(tmp2.c_str());
  string tmp3;
  while(licznik2!=licznik)
  {
    getline(config, tmp3);
    if(tmp3.size()>0)
    {
      licznik2++;
      if(licznik2==pozycja)
      {
        cout<<"Current theme: "<<them<<"\nCurrent color: "<<curcolor<<"\n\nW prawo podglad, enter aby wybrac:\n";
        int lol=0;
        bool czy=1;
        while(lol!=10)
        {
          cout<<"\033[0;0f\033[4B";
          for(int i=0; i<colors[numerthemu] and czy; i++)
          {
            if(i==pozycjalisty)
            {
              cout<<"\033[1m*";
            }
            else
            {
              cout<<"\033[0m-";
            }
            cout<<kolory[0][numerthemu][i]<<"\n";
          }
          cout<<"\033[0m";
          lol=getch();
          czy=0;
          if(lol==65 or lol==66 or lol==67)
          {
            czy=1;
            if(lol==65)
            {
              pozycjalisty--;
              if(pozycjalisty<0)
              {
                pozycjalisty=colors[numerthemu]-1;
              }
            }
            else if(lol==66)
            {
              pozycjalisty++;
              if(pozycjalisty>colors[numerthemu]-1)
              {
                pozycjalisty=0;
              }
            }
            else
            {
              if(dual[numerthemu])
              {
                string tpath="./themes/"+themy[numerthemu]+"/eye_03_L"+kolory[1][numerthemu][pozycjalisty]+".png";
                rozdzialka(tpath);
                wyswietl(tpath, szer, wys);
              }
              else
              {
                string tpath="./themes/"+themy[numerthemu]+"/eye_03"+kolory[1][numerthemu][pozycjalisty]+".png";
                rozdzialka(tpath);
                wyswietl(tpath, szer, wys);
              }
            }
          }
        }
        string color=kolory[0][numerthemu][pozycjalisty];
        config2<<" \""<<color<<"\" :color\n";
      }
      else
      {
        config2<<tmp3<<"\n";
      }
    }
  }
  config.close();
  config2.close();
  system(cp.c_str());
  system(rm.c_str());
  cout<<"\nColor changed to "<<kolory[0][numerthemu][pozycjalisty]<<"\n";
}
int main(int argc, char * argv[])
{
//↑-65 ↓-66 ENTER-10
  znajdz_themy();
  skanuj_themy();
  bool czyc=0, czyt=0;
  string c="-c", t="-t";
  for(int i=0; i<argc; i++)
  {
   // cout<<argv[i]<<endl;
    string tmp=argv[i];
    if(tmp==c)
    {
      cout<<instantc(argv[i+1]);
      czyc=i+1;
    }
    if(tmp==t)
    {
      cout<<instantt(argv[i+1]);
      czyt=i+1;
    }
  }
  //zafsze najpierw theme
  if(czyt)
  {
    instantt(argv[czyt]);
  }
  if(czyc)
  {
    instantc(argv[czyc]);
  }
  if(czyc+czyt)
  {
    return 0;
  }
  //system("sleep 10");

  system("clear");
  string message="Welcome in configurator";
  for(int i=0; i<=message.size(); i++)
  {
    for(int j=message.size(); j>i; j--)
    {
      cout<<" "<<flush;
    }
    for(int j=0; j<i; j++)
    {
      cout<<message[j]<<flush;
    }
    system("sleep 0.06");
    cout<<"\r";
  }
  system("sleep 0.7");
  cout<<"\n\nPamietaj aby najpierw zamknac oczka!!!\n\nNacisnij\n1 aby zmienic color\n2 aby zmienic them\n3 aby zmienic oba naraz\n4 aby wyjsc\n";
  int tmp=0;
  while(tmp!=52)
  {
    tmp=getch();
    if(tmp==49)
    {
      zmienkolor();
      tmp=52;
    }
    if(tmp==50)
    {
      zmienthem();
      tmp=52;
    }
    if(tmp==51)
    {
      zmienthem();
      system("sleep 1");
      system("clear");
      cout<<"Teraz nastapi zmiana coloru."<<endl;
      system("sleep 1.5");
      zmienkolor();
      tmp=52;
    }
  }
}
