#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>

typedef struct point
{
	double value;
	int timestamp;
	struct point *next;
}point;

typedef point* Point;

void freeMem(Point* lst) 
{ 
   Point  aux=*lst; 
   Point aux1; 
   while (aux!=NULL)  
   { 
       aux1=aux->next; 
       free(aux); 
       aux=aux1; 
   } 
   *lst=NULL; 
} 

Point addLast(Point lst,double val,int time)
{
	Point nou=(point *)malloc(sizeof(point));
	nou->timestamp=time;
	nou->value=val;
	if(lst==NULL) //lista este vida
	{
		nou->next=NULL;
		lst=nou;
	}
	else
	{
		Point aux=lst; // aux parcurge nodurile listei pana la ultimel element al acesteia
		while(aux->next!=NULL)
			aux=aux->next;
		aux->next=nou;
		nou->next=NULL;
	}
	return lst;
}

Point read(Point lst,int *n) //citeste un nr n si o lista de n valori
{
	int i,time;
	double x;
	scanf("%d",n);
	for(i=0;i<*n;i++)
	{
		scanf("%d%lf",&time,&x);
		lst=addLast(lst,x,time);
	}
	return lst;
}

int sizeOfList(Point lst) //returneaza nr de noduri ale listei lst
{
	int n=0;
	Point aux=lst;
	while(aux!=NULL)
	{
		n++;
		aux=aux->next;
	}
	return n;
}

void printVal(Point lst) //afiseaza lista lst
{
	Point aux=lst;
	printf("%d\n",sizeOfList(lst));
	while(aux!=NULL)
	{
		printf("%d %.2f\n",aux->timestamp,aux->value);
		aux=aux->next;
	}
}

Point middle(Point lst) //intoarce adresa elementului din mijlocul listei lst cu nr impar de noduri
{
	Point aux=lst;
	int k=0;
	int l=sizeOfList(lst);
	while(aux!=NULL && k!=l/2)
	{
		aux=aux->next;
		k++;
	}
	return aux;
}

double media(Point lst) //calculeaza media aritmetica a valorilor din fereastra de 5 elemente spre care pointeaza lst
{
	double s=0;
	Point aux=lst;
	while(aux!=NULL)
	{
		s=s+aux->value;
		aux=aux->next;
	}
	return s/5;
}

double deviation(Point lst) //retureaza deviatia valorilor din fereastra
{
	Point aux=lst;
	Point l=lst;
	double s=0;
	while(aux!=NULL)
	{
		s=s+pow(aux->value-media(l),2);
		aux=aux->next;
	}
	s=s/5;
	return sqrt(s);
}

int validation(double x,double media,double deviation)
{
	if(x<media-deviation || x>media+deviation)
		return 1;
	return 0;
}

Point deleteItem(Point lst,double x)//sterge primul element cu valoarea x din lista
{
	Point aux1,aux2;
	aux1=lst;
	aux2=aux1;
	while(aux1->next!=NULL && aux1->value!=x)
	{
		aux2=aux1;
		aux1=aux1->next;
	}
	aux2->next=aux1->next;
	free(aux1);
	return lst;
}

Point subList(Point lst) //returneaza o sublista de 5 elemente pornind de la adresa lst
{
	int i;
	Point lst_aux=NULL;
	for(i=0;i<5;i++)
	{
		lst_aux=addLast(lst_aux,lst->value,lst->timestamp);
		lst=lst->next;
	}
	return lst_aux;
}

Point modifyList(Point lst,Point rez)//sterge elementele din lst care alcatuiesc lista rez
{
	Point aux=rez;
	double val;
	while(aux!=NULL)
	{
		val=aux->value;
		lst=deleteItem(lst,val);
		aux=aux->next;
	}
	freeMem(&rez);
	rez=NULL;
	return lst;
}

Point memExceptions(Point lst)//memoreaza in rez o lista cu valorile care trebuie sterse
{
	Point rez=NULL;
	Point aux=lst; //aux retine adresa de inceput a sublistelor de lungime 5
	while(aux->next->next->next->next!=NULL)
	{
		Point lst_aux=NULL; //memoreaza o lista de lungime 5
		lst_aux=subList(aux);
		double med=media(lst_aux); //med memoreza media aritmetica a elementelor din lst_aux
		double mj=middle(lst_aux)->value; //mj este valoarea elementului din mijlocul listei
		double dev=deviation(lst_aux); //dev este deviatia standard a valorilor din fereastra
		if(validation(mj,med,dev)==1) 
			rez=addLast(rez,mj,middle(lst_aux)->timestamp);
		aux=aux->next;
		freeMem(&lst_aux);
		lst_aux=NULL;
	}
	lst=modifyList(lst,rez);
	return lst;
}

Point merge(Point l1,Point l2) //returneaza rezultatul "interclasarii" a 2 liste deja sortate(l1 si l2)
{
	Point rez=NULL;
	if(l1==NULL)
		return (l2);
	else 
		if(l2==NULL)
			return (l1);
	if(l1->value<=l2->value)
	{
		rez=l1;
		rez->next=merge(l1->next,l2);
	}
	else
	{
		rez=l2;
		rez->next=merge(l1,l2->next);
	}
	return rez;
}


void part(Point first, Point *first1, Point *first2) //first1 este pointer catre inceputul listei first 
{                                                    //first2 este pointer catre primul nod din a doua jumatate a listei first
    Point aux1=first; 
    Point aux2=first->next; 
    while(aux2!=NULL) 
    { 
    	aux2=aux2->next; 
    	if(aux2!=NULL) 
    	{ 
        	aux1=aux1->next; 
        	aux2=aux2->next; 
    	} 
    } 
    *first1=first; 
    *first2=aux1->next; 
    aux1->next=NULL; 
} 

void MergeSort(Point *lst)
{
	Point first=*lst;
	Point aux1=NULL,aux2=NULL;
	if(first->next==NULL)
		return; //s-a ajuns la situatia de punct fix
	part(first,&aux1,&aux2);
	MergeSort(&aux1);
	MergeSort(&aux2);
	*lst=merge(aux1,aux2);
}

Point mediana(Point lst)
{
	Point rez=NULL;
	Point aux=lst;
	while(aux->next->next->next->next!=NULL)
	{
		Point sublist=NULL;
		sublist=subList(aux);
		MergeSort(&sublist);
		rez=addLast(rez,middle(sublist)->value,aux->next->next->timestamp);
		freeMem(&sublist);
		sublist=NULL;
		aux=aux->next;
	}
	return rez;
}

Point medie(Point lst)
{
	Point rez=NULL;
	Point aux=lst;
	double m=0;
	while(aux->next->next->next->next!=NULL)
	{
		Point sublist=NULL;
		sublist=subList(aux); //sublist memoreaza adresa de inceput a ferestrei de 5 elemente 
		m=media(sublist); //m memoreaza media aritmetica a elementelor sublistei cu 5 noduri
		rez=addLast(rez,m,aux->next->next->timestamp);
		freeMem(&sublist);
		sublist=NULL;
		aux=aux->next;
	}
	return rez;
}

int interval(int a,int b)
{
	if(a-b>=100 && a-b<=1000)
		return 1;
	return 0;
}

Point uniformisation(Point lst)
{
	Point aux=lst;
	Point pre=lst;
	while(aux!=NULL)
	{
		if(interval(aux->timestamp,pre->timestamp)==1) 
		{
			aux->timestamp=(aux->timestamp+pre->timestamp)/2;
			aux->value=(aux->value+pre->value)/2;
		}
		pre=aux;
		aux=aux->next;
	}
	return lst;
}
  
double function_w(int i,int k)
{
	double fr=(double)(i)/(k-1);
	double num=fr*fr*0.9+0.1;
	double s=0;
	for(int j=0;j<=k-1;j++)
	{
		double el=(double)j/(k-1);
		s=s+el*el*0.9+0.1;
	}
	double rez=num/s;
	return rez;
}

double suma(Point lst,int k)
{
	double s=0;
	int i=0;
	Point aux=lst;
	while(aux!=NULL&&i<=k)
	{
		s=s+(aux->value)*function_w(i,k);
		aux=aux->next;
		i++;
	}
	return s;
}

double function_c(int left,int right,int current)
{
	double C;
	C=(double)(current-left)/(right-left);
	return C;
}

double function_f(Point left,Point right,int current)
{
	double C=function_c(left->next->next->timestamp,right->next->next->timestamp,current);
	double s1=suma(left,3);
	double s2=suma(right,3);
	double rez=(1-C)*s1+C*s2;
	return rez;
}

Point addFirst(Point lst,double val,int t)
{
	Point nou=(point *)calloc(1,sizeof(point));
	nou->value=val;
	nou->timestamp=t;
	if(lst==NULL)
	{
		nou->next=NULL;
		lst=nou;
	}
	else
	{
		nou->next=lst;
		lst=nou;
	}
	return lst;
}

Point genLeft(Point lst,Point adr) //genereaza sublista cu 3 noduri situata inaintea lui adr
{
	Point aux=lst;
	while(aux->next->next!=adr)
		aux=aux->next;
	Point adr1=NULL;
	do
	{
		adr1=addLast(adr1,aux->value,aux->timestamp);
		aux=aux->next;
	}
	while(aux!=adr);
	adr1=addLast(adr1,aux->value,aux->timestamp); //se adauga si nodul de la adresa adr==aux
	return adr1;
}

Point genRight(Point lst) //returneaza adresa de inceput a listei cu 3 noduri situata dupa lst
{
	Point r=NULL;
	Point aux=lst;
	int i=1;
	while(i<=3)
	{
		r=addFirst(r,aux->value,aux->timestamp);
		aux=aux->next;
		i++;
	}
	return r;
}

Point insertList(Point l1,Point l2,Point nod) ///insereaza pe l2 in l1 dupa nodul nod
{
	Point aux2=l2;
	Point aux1=l1;
	while(aux1!=nod)
		aux1=aux1->next;
	while(aux2->next!=NULL)
		aux2=aux2->next;
	aux2->next=nod->next;
	aux1->next=l2;
	return l1;
}

Point fill(Point lst)
{
	int nr_val; //numarul de valori care urmeaza a fi introduse in lista initiala
	Point aux=lst;
	Point aux1=aux;
	Point newList=NULL;
	Point right=NULL;
	Point left=NULL;
	while(aux1->next!=NULL)
	{
		int i=1;
		nr_val=0;
		if(aux->timestamp-aux1->timestamp>1000)
		{
			newList=NULL;
			int val1=aux1->timestamp;
			right=genRight(aux); ///right este sublista de 3 elemente situata dupa aux
			left=genLeft(lst,aux1); //left este sublista de 3 elemente situata inaintea lui aux1
			nr_val=(aux->timestamp-aux1->timestamp)/200;
			while(i<=nr_val && val1+200!=aux->timestamp)
			{
				double ad=function_f(left,right,val1+200);
				newList=addLast(newList,ad,val1+200);
				val1=val1+200;
				i++;
			}
			lst=insertList(lst,newList,aux1);
			freeMem(&right);
			freeMem(&left);
		}
		aux1=aux;
		aux=aux->next;	
		newList=NULL;
		freeMem(&newList);
	}
	return lst;
}

int frecv(float x,float a,float b) //cauta pe x in intervalul [a,b]
{
	if(x>=a && x<=b)
		return 1;
	return 0;
}

int apparitions(Point lst,float a,float b) //returneaza numarul de elemente ale listei care se gasesc in intervalul [a,b]
{
	int f=0;
	Point aux=lst;
	while(aux!=NULL)
	{
		if(frecv((aux->value),a,b)==1)
			f++;
		aux=aux->next;
	}
	return f;
}

void statistica(Point lst,int delta)
{
	float p=-350;
	float p1=350;
	for(int i=p;i<=p1;i=i+delta)
	{
		int f=apparitions(lst,i,i+delta);
		if(f!=0)
			printf("[%d, %d] %d\n",i,i+delta,f);
	}
}

int main(int argc, char **argv)
{
	int st=0,i;
	Point lst=NULL; 
	Point rez=NULL;
	int nr=0;
	lst=read(lst,&nr);
	char *com;
	com=(char *)malloc(10);
	for(i=1;i<=argc-1;i++)
	{
		strcpy(com,argv[i]);
		if(strstr(com,"e1")!=NULL)
		{
			lst=memExceptions(lst);
			continue;
		}
		if(strstr(com,"e2")!=NULL)
		{
			rez=mediana(lst);
			freeMem(&lst);
			lst=rez;
			continue;
		}
		if(strstr(com,"e3")!=NULL)
		{
			rez=medie(lst);
			freeMem(&lst);
			lst=rez;
			continue;
		}
		if(strstr(com,"u")!=NULL)
		{
			lst=uniformisation(lst);
			continue;
		}
		if(strstr(com,"c")!=NULL)
			lst=fill(lst);
		if(strstr(com,"st")!=NULL)
		{
			int delta=atoi(argv[i]+4);
			statistica(lst,delta);
			st=1;
		}
	}
	if(st==0)
		printVal(lst);
	freeMem(&lst);
	free(com);
	return 0;
}