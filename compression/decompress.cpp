#include<iostream>
#include<fstream>
#include<math.h>
#include<cstring>
#include<string>
#include<sstream>
using namespace std;

int capacity = 0;

struct user{
  char ch;
  int freq;
  user* leftChild;
  user* rightChild;
};


class minHeap{
private:
	int size;
	user** Arr;
public:
	minHeap():size(capacity){}
	void setSize(){
	    string capacityN;
		fstream fin;
		fin.open("table.csv",ios::in);
		string line;
		getline(fin,line);
		stringstream ss(line);
		getline(ss,capacityN);
		capacity = stoi(capacityN);
		Arr = new user*[capacity];
		string ch;
		string freq;
		for(int i=0;i<capacity;i++){
			getline(fin,line);
			stringstream ss(line);
			getline(ss,ch,',');
			getline(ss,freq);
            user* temp = new user;
            temp -> ch = ch[0];
            temp -> freq = stoi(freq);
            temp -> leftChild = NULL;
            temp -> rightChild = NULL;
            Arr[i] = temp;
            cout<<Arr[i] -> ch<<" -> "<<Arr[i] -> freq<<endl;
		}
		fin.close();
		size = capacity;
		cout<<size<<endl;
    }

	void minHeapify(int fIndex){
		if(fIndex > size/2){
			return;
		}
		int fLeft = 2*fIndex;
		int fRight = fLeft + 1;
		int fMin = fIndex;
        if(fRight <= size){
        	if(Arr[fRight-1]->freq < Arr[fIndex -1]->freq){
        		fMin = fRight;
        	}
        }
        if(fLeft <= size){
        	if(Arr[fLeft -1]->freq < Arr[fMin -1]->freq){
        		fMin = fLeft;
        	}
        }
		if(fMin != fIndex){
			user* temp = Arr[fMin - 1];
			Arr[fMin - 1] = Arr[fIndex -1];
			Arr[fIndex -1] = temp;
			minHeapify(fMin);
		}
	}

	void buildMinHeap(){
		for(int i = size/2;i>=1;i--){
			minHeapify(i);
		}
	}

	user* extractMin(){
		buildMinHeap();
		user* temp = Arr[0];
		Arr[0] = Arr[size -1];
		Arr[size - 1] = temp;
		size--;
		return temp;
	}

	void insert(){
		user* leftChild = extractMin();
		user* rightChild = extractMin();
		user* ptr = createNode(leftChild,rightChild);
		size++;
		Arr[size-1] = ptr;
	}


	user* createNode(user* leftChild , user* rightChild){
		user* temp = new user;
		temp -> freq = leftChild -> freq + rightChild -> freq;
		temp -> leftChild = leftChild;
		temp -> rightChild = rightChild;
		return temp;
	}

	void createHuffmanTree(){
		while(size > 1){
			cout<<"creating"<<endl;
			insert();
		}
		cout<<"finally ,created"<<endl;
	}

	void makeDecompressedFile(){
		ofstream fout;
		char i;
		fout.open("decompressedFile.txt",ios::out);
		ifstream fin;
		fin.open("compressedNewFile.txt",ios::in);
		user* temp;
		while(!fin.eof()){
          temp = Arr[0];
		  while(1){
		  	  if(temp -> leftChild == NULL && temp -> rightChild == NULL){
		  	  	cout<<temp -> ch<<endl;
		  	  	fout.put(temp -> ch);
                break;
		  	  }
		  	  fin.get(i);
			  if(i == '1'){
			     if(temp -> rightChild != NULL){
					  temp =temp -> rightChild;
				 }
			  }
			  else{
				  if(temp -> leftChild != NULL){
					  temp = temp -> leftChild;
				  }
			  }
		   }
	    }
	    fin.close();
	    fout.close();
	}

};


int main(){
    minHeap mh;
    //it copies table data to an array to create huffman tree to gat codes to decompress compresssed file
    //takes O(n)
    mh.setSize();
    //O(n)
    mh.createHuffmanTree();
    //O(N) but with big fat constant ignore for simplicity
    mh.makeDecompressedFile();
}


//total time = O(n) + O(n) + O(N) --> O(N) is predominant
