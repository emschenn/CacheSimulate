#include <iostream>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include<cmath>
#include <vector>
#define SIZE 100000
using namespace std;
struct cache{
	int valid;
	int tag;
	unsigned data;
};
bool searchSet(int size,struct cache c[],int tag){
        for(int i=0;i<size;i++){
                if(c[i].tag==tag)
                        return true;
        }
        return false;
}
int main(int argc,char* argv[]){
	int cacheSize,blockSize,assoc,replace,num=0;
	int m=0,h=0,x,y,i,index,tag;
	unsigned mem[SIZE];
	int miss[SIZE];
	int hit[SIZE];
	//read file
	ifstream infile(argv[2]);
	infile>>cacheSize; cacheSize=cacheSize*1024;
	infile>>blockSize;
      	infile>>assoc;
        infile>>replace;
	while(!infile.eof()){
		infile>>hex>>mem[num];
		num++;
	}
	num--;
	int blockNum = cacheSize/blockSize;
	unsigned cache[blockNum][2];
	cout<<blockNum;
	//write file
	fstream outfile;
        outfile.open(argv[4],ios::out);	

	//directed map
	if(assoc==0){
		for(x=0;x<blockNum;x++)
			for(y=0;y<2;y++)
				cache[x][y]=-1;
		for(i=0;i<num;i++){
			index = (mem[i]/blockSize)%blockNum;
			tag = (mem[i]/blockSize)/index;
//			cout<<mem[i]<<endl<<index<<endl<<tag<<endl;
			if(cache[index][0]==-1||cache[index][1]!=tag){
				cache[index][0]=1;
				cache[index][1]=tag;
				cache[index][2]=mem[i];
				miss[m]=i+1;
				m++;
			}
			else{
				cache[index][0]=1;
                                cache[index][1]=tag;
                                cache[index][2]=mem[i];
				hit[h]=i+1;
				h++;			
			}
		}
	     	outfile<<"Hits instructions:";
		for(x=0;x<h-1;x++)	outfile<<hit[x]<<",";		
		outfile<<hit[h-1];
        	outfile<<endl<<"Misses instructions:";
		for(y=0;y<m-1;y++)	outfile<<miss[y]<<",";
		outfile<<miss[m-1];
        	outfile<<endl<<"Miss rate:"<<double(m)/double(num)<<endl;
	}
	//four set associative
	else if(assoc==1){
		struct cache set0[blockNum/4];
		struct cache set1[blockNum/4];
		struct cache set2[blockNum/4];
		struct cache set3[blockNum/4];
		int n0=0,n1=0,n2=0,n3=0;
		for(i=0;i<blockNum/4;i++){
			set0[i].valid=-1;
			set1[i].valid=-1;
			set2[i].valid=-1;
			set3[i].valid=-1;
		}
		for(i=0;i<num;i++){
			index = (mem[i]/blockSize)%blockNum;
			tag = (mem[i]/blockSize)/index;
			if(index%4==0){
				if(searchSet(blockNum/4,set0,tag)==false){
					if(set0[blockNum/4-1].valid!=-1){	//full
					}
					else{
						set0[n0].valid = 1;
						set0[n0].tag = tag;
						set0[n0].data = mem[i];
						miss[m]=i+1; m++;
					}
				}
				else{
                                	set0[n0].valid = 1;
                                        set0[n0].tag = tag;
                                        set0[n0].data = mem[i];
                                        hit[h]=i+1; h++;
				}
				n0++;
			}
                        else if(index%4==1){
				if(searchSet(blockNum/4,set1,tag)==false){
                                        if(set1[blockNum/4-1].valid!=-1){
                                        }
                                        else{
                                                set1[n1].valid = 1;
                                                set1[n1].tag = tag;
                                                set1[n1].data = mem[i];
                                                miss[m]=i+1; m++;
                                        }
                                }
                                else{
                                        set1[n1].valid = 1;
                                        set1[n1].tag = tag;
                                        set1[n1].data = mem[i];
                                        hit[h]=i+1; h++;
                                }
				n1++;
                        }
                        else if(index%4==2){
				if(searchSet(blockNum/4,set2,tag)==false){
                                        if(set2[blockNum/4-1].valid!=-1){
                                        }
                                        else{
                                                set2[n2].valid = 1;
                                                set2[n2].tag = tag;
                                                set2[n2].data = mem[i];
                                                miss[m]=i+1; m++;
                                        }
                                }
                                else{
                                        set0[n2].valid = 1;
                                        set0[n2].tag = tag;
                                        set0[n2].data = mem[i];
                                        hit[h]=i+1; h++;
                                }
				n2++;
                        }
                        else if(index%4==3){
				if(searchSet(blockNum/4,set3,tag)==false){
                                        if(set3[blockNum/4-1].valid!=-1){ //full
                                        }
                                        else{
                                                set3[n3].valid = 1;
                                                set3[n3].tag = tag;
                                                set3[n3].data = mem[i];
                                                miss[m]=i+1; m++;
                                        }
                                }
                                else{
                                        set3[n3].valid = 1;
                                        set3[n3].tag = tag;
                                        set3[n3].data = mem[i];
                                        hit[h]=i+1; h++;
                                }
				n3++;
                        }

		}
		outfile<<"Hits instructions:";
                for(x=0;x<h-1;x++)      outfile<<hit[x]<<",";
                outfile<<hit[h-1];
                outfile<<endl<<"Misses instructions:";
                for(y=0;y<m-1;y++)      outfile<<miss[y]<<",";
                outfile<<miss[m-1];
                outfile<<endl<<"Miss rate:"<<double(m)/double(num)<<endl;			
	}
	else if(assoc==2){
		struct cache set[blockNum];
		int n=0;
		for(i=0;i<blockNum;i++)
			set[i].valid=-1;
		for(i=0;i<num;i++){
			index = (mem[i]/blockSize)%blockNum;
                        tag = (mem[i]/blockSize)/index;
			if(searchSet(blockSize,set,tag)==false){
				set[n].valid = 1;
                                set[n].tag = tag;
                                set[n].data = mem[i];
                                miss[m]=i+1; m++;
                       	}
			else{
				if(set[blockNum-1].valid!=-1){
				}
				else{
 					set[n].valid = 1;
                                	set[n].tag = tag;
                                	set[n].data = mem[i];
                                	hit[h]=i+1; h++;
				}	
			}
			n++;
		}
		outfile<<"Hits instructions:";
                for(x=0;x<h-1;x++)      outfile<<hit[x]<<",";
                outfile<<hit[h-1];
                outfile<<endl<<"Misses instructions:";
                for(y=0;y<m-1;y++)      outfile<<miss[y]<<",";
                outfile<<miss[m-1];
                outfile<<endl<<"Miss rate:"<<double(m)/double(num)<<endl;
	}
}

