=/*PlotRtlib
 * 
 * Autor: francisco thiago - franciscothiago@alu.ufc.br
 */
#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

int sizex = 10;
int sizey = 3;
int space = 2;
int gridSize = 2;


int tInicial,tFinal = -1; //Intervalos

#define arriveColor  "blue"
#define missColor  "black"
#define schdColor  "red"
#define gridColor  "green"






//Protipos
vector<string> parserJason (string in);
string generate (vector<string> in);


int show = 0;
int main(int argc,char **argv)
{
	string fileIn{};
	string fileOut{};

	for(int i = 1 ; i < argc ; i++)
	{
			string tmp{argv[i]};
			
			if(tmp.find("last=") != string::npos)
			{
					tFinal = stoi(tmp.substr(5,tmp.length() - 5 ));
			}
			else if(tmp.find("first=") != string::npos)
			{
					tInicial = stoi(tmp.substr(6,tmp.length() - 6 ));
			}
			else if(tmp.find("in=") != string::npos)
			{
					fileIn =tmp.substr(3,tmp.length() - 3 );
			}
			else if(tmp.find("out=") != string::npos)
			{
					fileOut =tmp.substr(4,tmp.length() - 4 );
			}
			else if(tmp.find("show_miss=") != string::npos)
			{
					show = stoi(tmp.substr(10,tmp.length() - 10));
			}
			else if(tmp.find("sizex=") != string::npos)
			{
					sizex = stoi(tmp.substr(6,tmp.length() - 6));
			}
			else if(tmp.find("sizey=") != string::npos)
			{
					sizey = stoi(tmp.substr(6,tmp.length() - 6));
			}
			else if(tmp.find("--help") != string::npos)
			{
                    cout<<"PlotRtlib - franciscothiago@alu.ufc.br"<<endl;
					cout<<"in= arquivo de entrada"<<endl;
					cout<<"out= arquivo de saida"<<endl;
					cout<<"first= limite inferior do tempo ou tempo inicial"<<endl;
					cout<<"last= limite superior do tempo ou tempo final"<<endl;
					cout<<"show_miss= mostrar na tela onde ocorreu miss de deadline !!! Apenas valores numericos 0,1"<<endl;
					cout<<endl<<"Lembre-se que o valor deve estar junto ao comando, exemplo, show_miss=1"<<endl;
			}
			else
			{
					cout << "Comando desconhecido :" + tmp  + " use --help"<< endl;
					return 0;
			}
	}
	
	if(tFinal < tInicial)
	{
		cout << "Tempo final deve ser maior que o inicial"<< endl;
		return 0;
	}
	
	if(fileIn.length() <= 0)
	{
		cout << "Digite um arquivo de entrada"<< endl;
	}
	
	if(fileOut.length() <= 0)
	{
		fileOut = "graph.tex";
	}
	
	ifstream in;
	ofstream out;
	
	in.open(fileIn);
	
	if(!in)
	{
			cout <<"Error ao abrir o arquivo de entrada"<<endl;
			
			return 0;
	}
	
	string data{};
	while(!in.eof())
	{
			string tmp;
			
			getline(in,tmp);
			
			data += tmp + "\n";
	}
	in.close();
	
	out.open(fileOut);
	out << generate (parserJason (data));

	out.close();

	
	return 0;
}


vector<string> parserJason (string in)
{
		int i,f = 0 ;
		
		i = f = 0;
		vector<string> r{};
		
		int higher = 0;
		do
		{	
			
			
			i = in.find(34,i);
			f = in.find(34,i+1);	
			
			string tmp{} ;
			tmp = in.substr(i+1,f-i-1);
			//cout << tmp << endl;
			
			r.push_back(tmp);
		}while( (i=in.find(34,f+1))   !=   string::npos);
		
		return r;
		

}


typedef struct
{
		vector<int> schdTimes{};
		vector<int> uSchdTimes{};
		vector<int> arvTimes{};
		vector<int> missTimes{};
		string name{};
}task;

string generate (vector<string> in) // Gera o corpo do .tex?
{


	string r{};
	
	vector <task> tasks{};
	
	int higher = 0;

	//pega todas as tasks

	for(int i = 0 ; i < in.size() ; i += 1)
	{
		//cout << 1 << endl;
		
		if(in[i] == "task_name")
		{
			
			int j ;
			
			for(j = 0; j < tasks.size();j++)
			{
				if(tasks[j].name == in[i+1])
					break;
			}
			
			if(j == tasks.size())
			{
				task tmp{};
				
				tmp.name = in[i+1];
				
				tasks.push_back(tmp);
				
			}
			
		}
		
		if(in[i] == "time")
		{
			if(higher < stoi(in[i+1]))
			{
				higher = stoi(in[i+1]);
			}
		}
	}
	
	//cout << higher <<endl;
	//------------------------------------------------------------------
	//------------------------------------------------------------------
	for(int i = 0 ; i < in.size() ; i += 1)
	{
	
		if(in[i] == "arrival")
		{
			int j;
			for(j = 0; j < tasks.size();j++)
			{
					if(in[i+2] == tasks[j].name)
					{
							break;
					} 
			}
			
			if(j == tasks.size())
			{
				cout <<in[i+2]<<" Esta task nao foi encontrada"<< endl;	
			}
			
			tasks[j].arvTimes.push_back(stoi(in[i-2]));
		}
		else if(in[i] == "scheduled")
		{
			int j;
			for(j = 0; j < tasks.size();j++)
			{
					if(in[i+2] == tasks[j].name)
					{
							break;
					} 
			}
			
			if(j == tasks.size())
			{
				cout <<in[i+2]<<" Esta task nao foi encontrada"<< endl;	
			}
			
			tasks[j].schdTimes.push_back(stoi(in[i-2]));
			
			//cout << tasks[j].schdTimes.back()<<endl;
		}
		else if(in[i] == "end_instance")
		{
			int j;
			for(j = 0; j < tasks.size();j++)
			{
					if(in[i+2] == tasks[j].name)
					{
							break;
					} 
			}
			
			if(j == tasks.size())
			{
				string nada{};
				cout <<in[i+2]<<" Esta task nao foi encontrada"<< endl;	
				return nada;
			}
			
			tasks[j].uSchdTimes.push_back(stoi(in[i-2]));
			
			//cout << tasks[j].uSchdTimes.back()<<endl;
			
		}
		else if(in[i] == "dline_miss")
		{
			int j;
			for(j = 0; j < tasks.size();j++)
			{
					if(in[i+2] == tasks[j].name)
					{
							break;
					} 
			}
			
			if(j == tasks.size())
			{
				string nada{};
				cout <<in[i+2]<<" Esta task nao foi encontrada"<< endl;	
				return nada;
			}
			if(show == true)
			{
				cout << tasks[j].name + " em " + in[i-2] << endl;
			}
			tasks[j].missTimes.push_back(stoi(in[i-2]));
			
			//cout << tasks[j].missTimes.back()<<endl;
			
		}


	}
	
	
	
	
	
	
	//cout << higher <<endl;
	//------------------------------------------------------------------
	//------------------------------------------------------------------


	 // GERA O .TEX de saída
	if(tInicial == -1 || tFinal == -1)
	{
		tInicial = 0;
		tFinal = higher;
	}
	int ky = 0;
		
	double stepx = sizex/higher;
	
	r += "\\begin{flushleft}\n";
	
	for(auto i : tasks)
	{
			r += "\\textit{"+i.name+"}\\\\\n";
			
			r += "\\begin{tikzpicture}";
			r += "\n";
			int kx = 0;
			
			for(int j = 0; j <= i.schdTimes.size() < i.uSchdTimes.size() ? i.schdTimes.size() : i.uSchdTimes.size();j++ )
			{
				
				if(i.schdTimes[j] <=  tInicial  && (i.uSchdTimes[j] >= tFinal && i.uSchdTimes[j] > tInicial))
				{
					r += "\\filldraw[red, very thick] +("+to_string(0)+"*"+to_string(sizex)+"/"+(to_string(tFinal - tInicial))+","+to_string(ky)+"*"+to_string(sizey)+"/"+(to_string(tasks.size()))+") rectangle (" +to_string(i.uSchdTimes[j]  - tInicial)+"*"+to_string(sizex)+"/"+(to_string(tFinal -tInicial))+","+to_string(ky+1)+"*"+to_string(sizey)+"/"+(to_string(tasks.size()))+");\n";					

				}
				else if(i.schdTimes[j] >=  tInicial  && i.uSchdTimes[j] <= 	tFinal && i.uSchdTimes[j] > tInicial )
				{
										r += "\\filldraw[red, very thick] +("+to_string(i.schdTimes[j] - tInicial)+"*"+to_string(sizex)+"/"+(to_string(tFinal - tInicial))+","+to_string(ky)+"*"+to_string(sizey)+"/"+(to_string(tasks.size()))+") rectangle (" +to_string(i.uSchdTimes[j]  - tInicial)+"*"+to_string(sizex)+"/"+(to_string(tFinal -tInicial))+","+to_string(ky+1)+"*"+to_string(sizey)+"/"+(to_string(tasks.size()))+");\n";					

				}
				else if((i.schdTimes[j] > tInicial  || i.uSchdTimes[j] > tFinal ))
				{
						if(i.schdTimes[j] >=  tInicial  && i.uSchdTimes[j] > tFinal && i.uSchdTimes[j] > tInicial)
						{
							//r += "\\filldraw[red, very thick] +("+to_string(i.schdTimes[j] - tInicial)+"*"+to_string(sizex)+"/"+(to_string(tFinal - tInicial))+","+to_string(ky)+"*"+to_string(sizey)+"/"+(to_string(tasks.size()))+") rectangle (" +to_string(sizex)+","+to_string(ky+1)+"*"+to_string(sizey)+"/"+(to_string(tasks.size()))+");\n";					

						}
					
						break;
				}
				
				/*
				if(i.schdTimes[j] >= tInicial && i.schdTimes[j] <= tFinal && i.uSchdTimes[j] >= tInicial && i.uSchdTimes[j] <= tFinal)
				{
					r += "\\filldraw[red, very thick] +("+to_string(i.schdTimes[j] - tInicial)+"*"+to_string(sizex)+"/"+(to_string(tFinal - tInicial))+","+to_string(ky)+"*"+to_string(sizey)+"/"+(to_string(tasks.size()))+") rectangle (" +to_string(i.uSchdTimes[j]  - tInicial)+"*"+to_string(sizex)+"/"+(to_string(tFinal -tInicial))+","+to_string(ky+1)+"*"+to_string(sizey)+"/"+(to_string(tasks.size()))+");\n";					
				}
				else if( i.schdTimes[j] > tFinal &&  i.uSchdTimes[j] > tFinal )
				{
					
						if( (i.schdTimes.size()) > (i.uSchdTimes.size() ) && j <= i.schdTimes.size() )
						{
							r += "\\filldraw[red, very thick] +("+to_string(i.schdTimes[j] - tInicial)+"*"+to_string(sizex)+"/"+(to_string(tFinal - tInicial))+","+to_string(ky)+"*"+to_string(sizey)+"/"+(to_string(tasks.size()))+") rectangle (" +to_string(tFinal-tInicial)+"*"+to_string(sizex)+"/"+(to_string(tFinal -tInicial))+","+to_string(ky+1)+"*"+to_string(sizey)+"/"+(to_string(tasks.size()))+");\n";					

						}
						break;
				}*/
			}

			for(int j = tInicial; j <= tFinal;j++ )
			{	
				r += "\\filldraw[green, very thick] +("+to_string(j - tInicial)+"*"+to_string(sizex)+"/"+(to_string(tFinal - tInicial))+","+to_string(ky)+"*"+to_string(sizey)+"/"+(to_string(tasks.size()))+") rectangle (" +to_string(j- tInicial)+"*"+to_string(sizex)+"/"+(to_string(tFinal -tInicial))+","+to_string(ky+1)+"*"+to_string(sizey)+"/"+(to_string(tasks.size()))+");\n";				

			}
			

			
			for(auto j : i.arvTimes)
			{	
				if(j >= tInicial && j <= tFinal)
				{
					r += "\\filldraw[blue, very thick] +("+to_string(j - tInicial)+"*"+to_string(sizex)+"/"+(to_string(tFinal - tInicial))+","+to_string(ky)+"*"+to_string(sizey)+"/"+(to_string(tasks.size()))+") rectangle (" +to_string(j - tInicial)+"*"+to_string(sizex)+"/"+(to_string(tFinal -tInicial))+","+to_string(ky+1)+"*"+to_string(sizey)+"/"+(to_string(tasks.size()))+");\n";				
					//r += "\\node[draw,text] at (0,0) {some text}\n";	
				}
				else if(j> tFinal )
				{
						break;
				}
			}
			
			for(auto j : i.missTimes)
			{	
				if(j >= tInicial && j <= tFinal)
				{
					r += "\\filldraw[black, very thick] +("+to_string(j - tInicial)+"*"+to_string(sizex)+"/"+(to_string(tFinal - tInicial))+","+to_string(ky)+"*"+to_string(sizey)+"/"+(to_string(tasks.size()))+") rectangle (" +to_string(j - tInicial)+"*"+to_string(sizex)+"/"+(to_string(tFinal -tInicial))+","+to_string(ky+1)+"*"+to_string(sizey)+"/"+(to_string(tasks.size()))+");\n";				
					//r += "\\node[draw,text] at (0,0) {some text}\n";	
				}
				else if(j> tFinal )
				{
						break;
				}
			}
			
			
			ky += space;
			r += "\\end{tikzpicture}\\\\\n";
	}
	r += "\\end{flushleft}\n";
	



	return r;
}
