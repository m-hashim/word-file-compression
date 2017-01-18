
#include<iostream>
#include<fstream>
#include<string>
#include<map>
#include<vector>

#define INPUT_FILE "data.txt"
#define COMP_FILE "compress.txt"
#define DECOMP_FILE "decompress.txt"

using namespace std;

typedef map<string,int> temp;
temp temporary,Mapdict;
temp::iterator p;

vector<string> vec;

int count=0;

/*
/temporary stores single words and its frequency
/Mapdict stores words with its position in file
*/

struct Freq
{
	string str;
	int count;
};
struct Freq Frtemp;


void collect()
{

ifstream fi;
int n;	
char x;
string s;
fi.open(INPUT_FILE);

if(! fi)
{
cout<<"file not found";
 exit(4);
}
while(!fi.eof())
	{		
	x = fi.get();
	
	if(!isalpha(x)) 
	continue;
	
    while 	((x!=' ')&&	( isalpha(x) ))           //s contains words
			{
			if(fi.eof()) break;
			s = s + x;
    		x = fi.get();
			}
	
	if(s.size()<2)
	continue;
	
	if(x==' ')
	s=s+x;
	
	p=temporary.find(s);
	if(p==temporary.end())				  		   //not find
	{
	temporary.insert(pair<string,int>(s,1));	//insert new  word in temporary map
	}
	else
	{
	n=temporary[s];
	n++;										//add 1 to the frequency of existing elements
	temporary[s]=n;
	}
	s.clear();
}

fi.close();

}


void make(int freq)
{	
	cout<<"Data Compression :- ";					
 	 
	int count=0;
	string temp="*-*-";
	
	for(p=temporary.begin();p!=temporary.end();p++)
	{	
		if((*p).second>=freq) count++;// total number of data;
	}
	struct Freq Fr[count];
	count=0;
	
	for(p=temporary.begin();p!=temporary.end();p++)
	{	
		if((*p).second>=freq)
		{
			Fr[count].str=(*p).first;
			Fr[count++].count=(*p).second;
		}
	}
	
	for(int i=0;i<count-1;i++)
	for(int j=0;j<count-i-1;j++)
	{
		if(Fr[j].count<Fr[j+1].count)      //bubble sorting
		{
			Frtemp = Fr[j];
			Fr[j] = Fr[j+1];
			Fr[j+1] = Frtemp; 
		}
	}
	//structure Fr is sorted
	int tc=count;
	count=0;
	for(int i=0;i<tc;i++)
	{
		if(count%256==26||count%256==0||count%256==10)	//temp is passed in vector vec when special chracter appears
			{ 
				vec.push_back(temp);
				count++;
			}
			
		vec.push_back(Fr[i].str);	//str is passed in vector vec
		count++;
	}
	Mapdict.clear();
	for(int i=0;i<vec.size();i++)
	{
		p=Mapdict.find(vec[i]);
		if(p==Mapdict.end())  		   //not find
		{
			Mapdict.insert(pair<string,int>(vec[i],i));	//insert new  word in dictionary
		}
		
	}
	
	ofstream fo;
	fo.open(COMP_FILE);
	if(!fo)
	{
	cout<<"file not found";
	exit(4);
	}
	for(int i = 0; i < vec.size(); i++)
	{	//cout<<vec[i]<<" "<<i<<endl;
   		fo<<vec[i] << endl;						//vector vec elements is pass into the compressed file 
	}
	fo<<"**end dict**\n";						//to mark the finish of dictionary
	
	fo.close();
	
}


void compress()

{	
	string code;
	string s;
	char x;
	
	int offset=128;
	int range1 = offset+int(vec.size()/256) + 5; //padding;
	int range2 = 256 - range1;
	
	int flag=0;
	
	ifstream fi;
	ofstream fo;
	fi.open(INPUT_FILE);
	fo.open(COMP_FILE,ios::app);
	
	if(fi.eof() || fo.eof())
	exit(4);

	while(!fi.eof())
	{		
		x=fi.get();
		
		if(!isalpha(x))				//single aphabet is passed in compressed file
		{
			fo<<x;
			continue;
		}
		while((isalpha(x) ) && (x!=' '))
		{
			if(fi.eof()) 				//s contain a word
			break;
			s=s+x;
			x = fi.get();
		}
		if(s.size()<2)
		{
			fo<<s;					//single alphabet is forward in compressed file
			s.clear();				
		
			if(!isalpha(x))
				fo<<x;					//single non-alphabet is forward in compressed file
			continue;
		}
		
		if(x==' ')					//s contain a single word
			s=s+x;
		
		int flag=0;
		//s contain a word
		p=Mapdict.find(s);				//p stores address of map(s)
		if(p!=Mapdict.end())  			//find
		{								
		    int i= (*p).second;			//i contain value by line
		
			if(i>=range2)				//i greater than range hence 2 aplhabet is passed
			{
				unsigned int a=32768;
				a=a+i;
				int b=a/256;
				unsigned char ch= b;
				unsigned char ch2=a%256;
		
			    if(ch2!=26)
				{
					fo<<ch;
					fo<<ch2;
				}
				else
				cout<<"------";
			}
			else
			{
				unsigned char ch = range1+i; //single character is passed
				//	cout<<ch<<"  ";
				fo<<ch;
			}
				flag=1;
 		}
		if(flag==0)
		fo<<s;
		if(!isalpha(x)&&x!=' ')
		fo<<x;
		s.clear();
	}
	fo<<"\n****end****";
	cout<<"Completed";
	fo.close();
	fi.close();
}

void decompress()
{
	int n;	
	char x;
	string line;
	string s;
	s.clear();
	
	ifstream fi;
	ofstream fo;
	fo.open(DECOMP_FILE);
	fi.open(COMP_FILE);
	if(! fi && !fo)
	{
	cout<<"file not found";
	exit(4);
 	}
	 vector<string> vec; 
	while(getline(fi,line))
		{
		if(line=="**end dict**")
			break;
		else
			vec.push_back(line);

		}
	//cout<<vec.size();
	int offset=128;
	int range1 = offset+int(vec.size()/256) + 5; //padding;
	int range2 = 256 - range1;
	//cout<<" range1 "<<range1<<" range2 "<<range2<<"--";
	
	unsigned char x1;
	
	while(1)
	{
	getline(fi,line);		
	int ci=0;
	if(line=="****end****")
	{	
		cout<<"\nDecompression :- ";
		break;
	}
	
	while(ci<line.size())			//single line is read
	{
		
		x1 = line[ci++];
		int l=int(x1);
		if (l >= range1)
		fo<<vec[l-range1];      			//single alphabet - words is printed
		else if( l > 126 && l<range1  )
		{	int j= int(x1);
			x1=line[ci++];
			unsigned int total= (j*256)+int(x1);
			unsigned int rem=total-32768;	
			if(rem<0||rem>vec.size())		
			cout<<"\n ERROR";
			else
			{
				fo<<vec[rem];				//double alphabet - words is printed 
				continue;
			}
		}
		else
		fo<<x1;
	}
fo<<endl;

}
cout<<"Completed";
fo.close();
}


int main()
{
int freq=2;

collect();
make(freq);
compress();
decompress();

return 0;
}


