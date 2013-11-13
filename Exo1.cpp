#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <time.h>  
#include <stdlib.h> 

using namespace std;


class Data
{
private:
	
	string data;

public:

	Data()
	{
	}

	Data(char * input)
	{
		data ="";
		ifstream fichier(input, ios::in);  // on ouvre le fichier en lecture

        if(fichier)  // si l'ouverture a réussi
        {      
            // instructions

        	string ligne;

        	while(getline(fichier, ligne))  // tant que l'on peut mettre la ligne dans "contenu"
        	{
        		data += ligne;
        	}
            fichier.close();  // on ferme le fichier
        }
        else  // sinon
        	 cout << "Impossible d'ouvrir le fichier !" << endl;
    }


    string toString()
    {
    	// cout << &data ;
    	return data;
    }

    inline bool operator== (const Data &D2) const
    {
    	//cout << "Je compare derp derp" << endl;
    	//sleep(1);
    	for (int i = 0; i < 1000000; ++i)
    	{
    		/* code */
    	}
    	//cout << "Data source : " << data << " == " << D2.data << endl;
    	return (data == D2.data);

    }

    inline bool operator!= (const Data &D2) const
    {
    	//sleep(500);
    	return (data != D2.data);
    }



};

typedef Data* Element;

vector<Element> vector_element;
vector<Data> my_vector;
int N = 10;

struct Resultat
{
	bool b;
	Element e;
	int n;
};


int occ(Element e,int i,int j)
{
	int result = 0;
	for(int k = i;k<j;k++)
	{
		if (vector_element[k] == e)
		{
			result++;
		}
	}
	return result;
}

void initSame()
{
	for(int i =0; i<N ;i++)
 	{
 		int valeur = rand() % my_vector.size() ;
 		vector_element.push_back(&my_vector[1]);
 	}
}

void initMaj()
{
	for(int i =0; i<N/2 ;i++)
 	{
 		int valeur = rand() % my_vector.size() ;
 		vector_element.push_back(&my_vector[valeur]);
 	}
 	for(int i =N/2; i<N ;i++)
 	{
 		int valeur = rand() % my_vector.size() ;
 		vector_element.push_back(&my_vector[1]);
 	}
}

void initRand()
{
	for(int i =0; i<N ;i++)
 	{
 		int valeur = rand() % my_vector.size() ;
 		vector_element.push_back(&my_vector[valeur]);
 	}
}

struct Resultat naiveMaj()
{
	struct Resultat result;
	result.b = false;
	result.e = NULL;

	for (int i = 0; i < N/2; ++i)
 	{
 		if (occ(vector_element[i],i,N)>N/2)
 		{
 			//cout << "We got a winner here " << vector_element[i]->toString() << endl;
 			result.e = vector_element[i];
 			result.b = true;
 			return result;
 		}
 	}
}

struct Resultat RecurrenceMaj(int i, int j)
{
	struct Resultat result;
	result.b = false;
	result.e = NULL;

	if(i == j)
	{
		result.b = true ;
		result.e = vector_element[i];
		return result;
	}

	struct Resultat gauche = RecurrenceMaj(i,(j+i)/2);
	struct Resultat droite = RecurrenceMaj((j+i+1)/2,j);

	if (!gauche.b && !droite.b)
	{
		return result;
	}
	if (gauche.b && droite.b)
	{
		if(droite.e != NULL && gauche.e != NULL && *gauche.e == *droite.e)
		{
			result.b = true;
			result.e = droite.e;
			return result;
		}

		int occD = occ(droite.e,i,j);
		int occG = occ(gauche.e,i,j);

		if (occG > (j-i+1)/2)
		{
			result.e = gauche.e;
			result.b = true;
			return result;
		}

		if (occD > (j-i+1)/2)
		{
			result.e = droite.e;
			result.b = true;
			return result;
		}
	}
	if (gauche.b && !droite.b)
	{
		int occG = occ(gauche.e,i,j);
		if (occG > (j-i+1)/2)
		{
			result.e = gauche.e;
			result.b = true;
			return result;
		}
	}

	if (!gauche.b && droite.b)
	{
		int occD = occ(droite.e,i,j);
		if (occD > (j-i+1)/2)
		{
			result.e = droite.e;
			result.b = true;
			return result;
		}
	}
}

struct Resultat PseudoMajoritaire(int i, int j)
{
	struct Resultat result;
	result.b = false;
	result.e = NULL;
	result.n = 0;

	if(i == j)
	{
		result.b = true ;
		result.e = vector_element[i];
		result.n = 1;
		return result;
	}

	struct Resultat gauche = RecurrenceMaj(i,(j+i)/2);
	struct Resultat droite = RecurrenceMaj((j+i+1)/2,j);

	int tmpValue = j-i+1;

	if (!gauche.b && !droite.b)
	{
		return result;
	}
	if (gauche.b && droite.b)
	{
		if (droite.e != NULL && gauche.e != NULL && *gauche.e == *droite.e)
		{
			result.b = true;
			result.e = gauche.e;
			result.n = gauche.n + droite.n;
			return result;
		}

		if (gauche.n == droite.n)
		{
			return result;
		}

		if (gauche.n > droite.n)
		{
			result.b = true;
			result.e = gauche.e;
			result.n = tmpValue/2 + gauche.n - droite.n;
			return result;
		}
		else
		{
			result.b = true;
			result.e = droite.e;
			result.n = tmpValue/2 + droite.n - gauche.n;
			return result;
		}
	}

	if (gauche.b && !droite.b)
	{
		result.e = gauche.e;
		result.b = true;
		result.n = gauche.n + tmpValue;
		return result;
	}

	if (!gauche.b && droite.b)
	{
		result.e = droite.e;
		result.b = true;
		result.n = droite.n + tmpValue;
		return result;
	}
}

struct Resultat PseudoMajoritaireLaunch(int i, int j)
{
	struct Resultat result = PseudoMajoritaire(i,j);
	if(result.b)
	{
		if (occ(result.e,i,j)>j/2)
		{
			return result;
		}
		else
		{
			result.b = false;
			result.e = NULL;
			return result;
		}
	}
}


int main(int argc, char * argv[])
{
	//
	// INIT 
	// Change N to increase the vector size

	srand (time(NULL));
	Data d1("DATA1");
	Data d2("DATA2");
	Data d3("void.txt");
	Data d4("DATA4");
	Data d5("DATA3");

	my_vector.push_back(d1);
	my_vector.push_back(d2);
	my_vector.push_back(d3);
	my_vector.push_back(d4);
	my_vector.push_back(d5);

	initMaj();

 	

 	//
 	// END INIT
 	//

 	struct Resultat result;



 	cout << "START" << endl;
 	clock_t startTime = clock();
	// some code here

	result = naiveMaj();

	// to compute its execution duration in runtime
	cout << "naiveMaj = " <<  double( clock() - startTime ) / (double)CLOCKS_PER_SEC<< " seconds." << endl;

	startTime = clock();
	// some code here

	result = RecurrenceMaj(0,N);

	// to compute its execution duration in runtime
	cout << "RecurrenceMaj = " << double( clock() - startTime ) / (double)CLOCKS_PER_SEC<< " seconds." << endl;

	startTime = clock();
	// some code here

	result = PseudoMajoritaireLaunch(0,N);

	// to compute its execution duration in runtime
	cout << "PseudoMajoritaireLaunch = " <<  double( clock() - startTime ) / (double)CLOCKS_PER_SEC<< " seconds." << endl;


 	if(result.b)
 	{
 		cout << "MAJ = " << result.e->toString() << endl;
 	}

}
