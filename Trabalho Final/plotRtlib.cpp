/*************************************
**************************************
*
      Equipe: Rubens, Gabriel, Imário, Paraíba
      Disciplina: Sistemas Operacionais II
      
      FINALMENTE!!!!!!
*
**************************************
**************************************/      



#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

typedef struct{
		vector<int> schdTimes{}; //  tempo preemptada
		vector<int> uSchdTimes{}; //?
		vector<int> arvTimes{}; // tempo de chegada
		vector<int> missTimes{};
		string name{};
}task;

//Protipos
vector<string> parserJason (string in);
void generate (vector<string> in);


int show = 0;
int main(int argc,char **argv)
{
	string fileIn{};
	
	fileIn = "trace.json"; // Nome do .json de saida RTLIB
	
	ifstream in;
	
	in.open(fileIn);
	
	if(!in){
			cout <<"Error ao abrir o arquivo de entrada"<<endl;
			
			return 0;
	}
	
	string data{};
	while(!in.eof()){
		
			string tmp;
			
			getline(in,tmp);
			
			data += tmp + "\n";
	}
	in.close();
	
	generate (parserJason (data));
	
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

/********************* escopos *******************************/
void printMiss(task t); 
void printArrival(task t);
void printSchd(task t);
void showMenu(task t);
/*************************************************************/

void generate (vector<string> in){ // Faz a leitura do codigo


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
				return;
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
				return;
			}
			if(show == true)
			{
				cout << tasks[j].name + " em " + in[i-2] << endl;
			}
			tasks[j].missTimes.push_back(stoi(in[i-2]));
			
			//cout << tasks[j].missTimes.back()<<endl;
			
		}


	}
	r = "xxxx";
	
	/*	tasks:::::
		schdTimes //  tempo preemptada
		uSchdTimes //
		arvTimes // tempo de chegada
		missTimes // vetor de tempos onde deram MISS
		name // nome da task
	*/
	int op = 0;
	bool escalonavel = 1; // eh escalonavel
	bool sair = 0;
	
	
	int qTasks = tasks.size();
	int totalMiss = 0;
	
	cout << "Quantidade de taks na simulacao: " << qTasks << endl;
	
	int misstmp = 0;
	
	for(int i=0; i < qTasks; i++){
		misstmp = tasks[i].missTimes.size();
		cout << "\n" << tasks[i].name << " : " << endl;
		showMenu(tasks[i]);
		
		if(misstmp != 0){
			escalonavel = 0;
			totalMiss += misstmp;
		}
		
		sair = 0;
		
		while(!sair){
			
			cin >> op;
			switch(op){
				case 1: // tempo de chegada
					printArrival(tasks[i]);
					cout << "->";
					break;
				case 2: // tempo de preempcao
					printSchd(tasks[i]);
					cout << "->";
					break;
				case 3: // tempo de miss
					printMiss(tasks[i]);
					cout << "->";
					break;
				case 0: //sair
					sair = 1;
					break;
				default:
					sair = 0;
					cout << "Invalido" <<endl;
					break;
			}
		}
	}// fim do for
	
	cout << "\n\n	A escala de tasks analisada";
	if(escalonavel){
		cout << " e escalonavel" << endl;
	}else{
		cout << " nao escalonavel. (miss: " << totalMiss << ")"<<endl;
	}
	return;
}


void printMiss(task t){ // printa miss da task passada
	for(int it = 0; it < t.missTimes.size(); it++){ // misses counter
			cout << "	miss em: " << t.missTimes[it] << endl;
	}
}

void printArrival(task t){ // printa os tempos de chegada da task 
	for(int it = 0; it < t.arvTimes.size(); it++){ // misses counter
			cout << "	Chegada em: " << t.arvTimes[it] << endl;
	}
}
void printSchd(task t){ // printa os tempos preemptados da task 
	for(int it = 0; it < t.schdTimes.size(); it++){ // misses counter
			cout << "	Preemptada em: " << t.schdTimes[it] << endl;
	}
}

void showMenu(task t){
		cout << "	1 - Exibir log de tempo de chegada (" << t.arvTimes.size() << ")\n" <<
				"	2 - Exibir log de tempo de preemptacao (" << t.schdTimes.size() << ")\n" <<
				"	3 - Exibir log de tempo de miss (" << t.missTimes.size() << ")\n" <<
				"	0 - Cancelar\n"<< endl; // fim do menu
}









