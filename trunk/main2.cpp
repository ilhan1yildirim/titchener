/*
Author: İlhan Yıldırım
Description: The program gets a random sequence and returns its T-complexity value 
defined by Mark Titchener at "M.R.Titchener:A Measure of Information, IEEE Data 
Compression Conference, Snowbird, Utah, March 2000.
*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

	int LastPAddress=-1;
	int LastKAddress=-1;
	int previousLastSymbol;
	int currentLength;
	int newSymbolNumber=2;
	int lastSymbol=2;
	int locationOfP, sequenceSize;
	int endFlag=0;
	int* newArray;
	int** symbolMatrix;
	int* newSymbolPosition1;
	int* newSymbolPosition2;
	int kLimit;
	int maxK=0;
	#define FREE(P) ((void)(free((P)), (P) = NULL))
	

void FindP(int *currentArray, int *k, int *p){
	int i, positiveFlag;
	LastPAddress++;
	positiveFlag=0;
	for(i=currentLength;i>=0;i--){
		if(currentArray[i]!=-1){
			if(positiveFlag==0){
				positiveFlag=1;
			}
			else{
				p[LastPAddress]=currentArray[i];
				locationOfP=i;
				break;
			}
		}
	}
	return;
}
int FindK(int *currentArray, int *k, int *p){
	int i,lastP;
	lastP=p[LastPAddress];
	LastKAddress++;
	k[LastKAddress]=1;
	for(i=1;i<=locationOfP;i++){
		if(currentArray[locationOfP-i] == lastP){
			k[LastKAddress]++;
		}
		else if(currentArray[locationOfP-i] == -1){
			;
		}
		else{
			break;
		}
	}
	if(k[LastKAddress]>kLimit){
		return 1;
	}
	if(k[LastKAddress]>maxK){
		maxK=k[LastKAddress];
	}
	return 0;
}
void SearchForLastP(int *currentArray, int *k, int *p){
	int lastP = p[LastPAddress],i,j,z,x=0,m,n;
	int numberOfRepetitions=0, counter=0, decreasedLength =0;
	int newSymbolCount=0;
	int currentIndex=0;
	newArray=(int *)malloc((currentLength+1)*sizeof(int));
	for(x=0;x<=currentLength;x++){
		newArray[x]=-1;
	}
	z=lastSymbol;
	for(i=0;i<=locationOfP;i++){
		x=0;
		j=-1;
		if(currentArray[i]==lastP){
			if(k[LastKAddress]>1){	
				numberOfRepetitions=0;
				for(j=0;j<k[LastKAddress]-1;j++){
					if(i+j+x+1>=currentLength){
						break;
					}
					if(currentArray[i+x+j+1]==-1){
						x++;
						j--;
						counter++;
					}
					else if(currentArray[i+j+x+1]==lastP){
						numberOfRepetitions++;
						currentArray[i+j+x+1]=-1;
						decreasedLength++;
					}
					else{
						break;
					}
				}
			}//Number Of Repetitions is found
			else{
				for(j=0;j<currentLength-i;j++){
					if(currentArray[i+j+1]==-1){
					}
					else{
						break;//any symbol different than -1 and current P
					}
				}
			}
			while(1){
				if(currentArray[i+j+x+1]==-1){
					j++;
				}
				else{
					break;
				}
			}
			if(symbolMatrix[currentArray[i+j+x+1]][numberOfRepetitions]<=previousLastSymbol){
				symbolMatrix[currentArray[i+j+x+1]][numberOfRepetitions]=lastSymbol;
				newSymbolPosition1[newSymbolCount]=currentArray[i+j+x+1];
				newSymbolPosition2[newSymbolCount]=numberOfRepetitions;
				newSymbolCount++;
				currentArray[i]=lastSymbol;
				lastSymbol++;
			}
			else{
				currentArray[i]=symbolMatrix[currentArray[i+j+x+1]][numberOfRepetitions];
			}//Symbol is changed
			currentArray[i+j+x+1]=-1;
			decreasedLength++;
		}
		i=i+j+x+1;
	}
	/*
	for(i=0;i<newSymbolCount;i++){
		symbolMatrix[newSymbolPosition1[i]][newSymbolPosition2[i]] = 0;
	}*/
	for(i=0;i<=currentLength;i++){
		if(currentArray[i]==-1){
			;
		}
		else{
			newArray[currentIndex]=currentArray[i];
			currentArray[currentIndex]=currentArray[i];
			currentIndex++;
		}
	}
	currentArray=newArray;
	currentLength = currentIndex-1;
	previousLastSymbol = lastSymbol-1;
	free(newArray);
	return;
}
long Factorial(long k){
	if (k>1){
		return k*Factorial(k-1);
	}
	else{
		return 1;
	}
}

float CalculateLi(float x, int numberOfSteps){
	float sum=0,delta;
	int i;
	delta=(x-2)/numberOfSteps;
	for(i=0;i<numberOfSteps;i++){
		sum+=delta/log(2+i*delta);
	}
	return (sum+1.045);
}
float CalculateInvLi(float x, float precision){
	int i;
	float y,upper, lower, mid;
	upper=sequenceSize;
	lower=0;
	mid=(upper+lower)/2;
	while(1){
		y=CalculateLi(mid,12000);
		if(y<x+precision && y>x-precision){
			lower=mid;
			upper=upper;
			break;
		}
		else if(y>x){
			upper=mid;
			mid=(lower+upper)/2;
		}
		else{
			lower=mid;
			mid=(lower+upper)/2;
		}
	}
	return mid;
}
int main(int argc, char *argv[]){
	int i=0,m;
	int *currentArray;
	int *k;
	int *p;
	double sum=0;
	char chnew;
	clock_t t;
	FILE *infile;
	t = clock();
	sequenceSize=atof(argv[1]);
	kLimit=(int)(log10(float(sequenceSize))/0.301);
	if(sequenceSize<100000){
		currentArray=(int *)malloc(sequenceSize*sizeof(int));
		symbolMatrix=(int **)malloc((sequenceSize/4)*sizeof(int*));
		newSymbolPosition1=(int*)malloc((sequenceSize/4)*sizeof(int));
		newSymbolPosition2=(int*)malloc((sequenceSize/4)*sizeof(int));
		for(m=0; m<(sequenceSize/4); m++){
			newSymbolPosition1[m]=0;
			newSymbolPosition2[m]=0;
			symbolMatrix[m]=(int*)malloc(kLimit*sizeof(int));
		}
		for(i=0;i<(sequenceSize/4);i++){
			for(m=0;m<kLimit;m++){
				symbolMatrix[i][m]=0;
			}
		}
	}
	else if(sequenceSize<1200000){
		currentArray=(int *)malloc(sequenceSize*sizeof(int));
		symbolMatrix=(int **)malloc((sequenceSize/8)*sizeof(int*));
		newSymbolPosition1=(int*)malloc((sequenceSize/8)*sizeof(int));
		newSymbolPosition2=(int*)malloc((sequenceSize/8)*sizeof(int));
		for(m=0; m<(sequenceSize/8); m++){
			newSymbolPosition1[m]=0;
			newSymbolPosition2[m]=0;
			symbolMatrix[m]=(int*)malloc(kLimit*sizeof(int));
		}
		for(i=0;i<(sequenceSize/8);i++){
			for(m=0;m<kLimit;m++){
				symbolMatrix[i][m]=0;
			}
		}
	}
	else{
		currentArray=(int *)malloc(sequenceSize*sizeof(int));
		symbolMatrix=(int **)malloc((sequenceSize/10)*sizeof(int*));
		newSymbolPosition1=(int*)malloc((sequenceSize/10)*sizeof(int));
		newSymbolPosition2=(int*)malloc((sequenceSize/10)*sizeof(int));
		for(m=0; m<(sequenceSize/10); m++){
			newSymbolPosition1[m]=0;
			newSymbolPosition2[m]=0;
			symbolMatrix[m]=(int*)malloc(kLimit*sizeof(int));
		}
		for(i=0;i<(sequenceSize/10);i++){
			for(m=0;m<kLimit;m++){
				symbolMatrix[i][m]=0;
			}
		}
	}
	k=(int *)malloc(sequenceSize*sizeof(int));
	p=(int *)malloc(sequenceSize*sizeof(int));
	for(i=0;i<sequenceSize;i++){
		k[i]=0;
		p[i]=0;
	}
	i=0;
	currentLength=sequenceSize-1;
	infile=fopen(argv[2],"r");
	while(1){
		if(fscanf(infile, "%c", &chnew) != EOF){
			currentArray[i]=int(chnew-48);
			i++;
		}
		else{
			break;
		}
	}
	while(1){
		FindP(currentArray, k, p);
		if(FindK(currentArray, k, p)==1){
			printf("Hata");
			break;
		}
		SearchForLastP(currentArray, k, p);
		if(locationOfP==0 || currentLength ==0){
			for(i=0;i<=LastKAddress;i++){
				printf("%d\n",k[i]);
				sum+=log10((double)(k[i])+1);
			}
			sum=sum/log10(2.0);
			break;
		}
	}
	printf("%d\t%d\n",lastSymbol, k[LastKAddress]);
	printf("Complexity : %f\n", sum);
	printf("Entropy per bit : %f\n", CalculateInvLi(sum, 0.001)/sequenceSize);
	printf("Maximum of k is %d\n", maxK);
	t = clock() - t;
	printf ("It took me %d clicks (%f seconds).\n",t,((float)t)/CLOCKS_PER_SEC);	
}