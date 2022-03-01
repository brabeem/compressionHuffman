#include<iostream>
#include<fstream>
#include<math.h>
#include<cstring>
#include<string>
using namespace std;

int capacity = 0;

struct user{
  char ch;
  unsigned int freq;
  user* nxt;
  user* prev;
  user* leftChild;
  user* rightChild;
  string code;
};

class hashTab{
protected:
int nKeys,mSlots;
user** mainTable;
user** Arr;
public:
  //initializing the maintable with all initial values to NULL
  //initializing the number of initial slots to 5
  hashTab():nKeys(0),mSlots(5){
    mainTable = new user*[5];
    for(int i = 0; i< 5; i++){
      mainTable[i] = NULL;
    }
  }
  
  void createList(){
  	 Arr = new user*[capacity];
  	 int j =0;
  	 for(int i = 0;i<mSlots;i++){
  	 	user* node = mainTable[i];
  	 	while(node != NULL){
  	 		Arr[j] = node;
  	 		j++;	
  	 		node = node -> nxt;
  	 	}
  	 }
  }

  user* putIntoDict(char ch){
     user* temp = new user;
     capacity++;
     temp -> ch = ch;
     temp -> freq = 1;
     temp -> nxt = NULL;//??prev NULL nagreko birsera??
     temp -> prev =NULL;
     temp -> leftChild = NULL;
     temp -> rightChild = NULL;
     return temp;
  }
  
  int hashValueOf(char ch){
    return ch % mSlots;
  }


  void insertH(char ch){
    nKeys++;
    if(nKeys > mSlots){
      //table size exceeds the loading factor//table doubling
      //first iterate over the maintable copy all the data to the next table of double size
      //then insert the value you want to insert by recursively calling the insertH function
      user** nxtMainTable = new user*[2*mSlots]; 
      user* mainPtr = NULL;
      mSlots = 2* mSlots;
      int hashVal = 0;
      user* collision = NULL;
      user* cMainPtr;
      for(int i = 0;i<(mSlots/2);i++){
        mainPtr = mainTable[i];
        
        while(mainPtr != NULL){
          hashVal = hashValueOf(mainPtr -> ch);
          if(nxtMainTable[hashVal] == NULL){
            //put the data directly
            cMainPtr = mainPtr;
            mainPtr = mainPtr -> nxt;
            nxtMainTable[hashVal] = cMainPtr;
            cMainPtr -> nxt = NULL;
            cMainPtr -> prev = NULL;
          }
          else{
            //collision occurs
            cMainPtr = mainPtr;
            mainPtr = mainPtr -> nxt;
            collision = nxtMainTable[hashVal];
            cMainPtr -> nxt = collision;
            collision -> prev = cMainPtr;
            nxtMainTable[hashVal] = cMainPtr;
            cMainPtr -> prev = NULL;
          }
        }


      }
      delete [] mainTable;
      mainTable = nxtMainTable;
      nxtMainTable = NULL;
      nKeys--;
      insertH(ch);
    }
    else{
       int hashVal = hashValueOf(ch);
       //there is nothing in the hashed value
       if(mainTable[hashVal] == NULL){
       	user* temp = putIntoDict(ch);
       	mainTable[hashVal] = temp;
       }
       else{
       	user* node = mainTable[hashVal];
       	bool found = false; 
       	//there is sth in hashed value and you find the character
       	while(node != NULL && !found){
       		if(node -> ch == ch){
       			found = true;
       			node -> freq = node -> freq + 1;
       		}
       		else{
       			node = node -> nxt;
       		}
       	}
        ///there is sth in the table but you dont find the character
       	if(!found){
       		user* temp = putIntoDict(ch);
       		//collision occurs//solved by chaining
            user* collision = mainTable[hashVal];
            temp -> nxt = collision;
            collision -> prev = temp;
            mainTable[hashVal] = temp;
       	}
       }
    }
  }

  //it searches for the characters code and then returns it
  //calculates hash value in constant time ,aceesses in constant amortized time for hash table searching is constant
  string searchH(char ch){
    int hashVal = hashValueOf(ch);
    user* head= mainTable[hashVal];
    bool found = false;
    while(!found && head != NULL){
      if(ch == head -> ch){
        found = true;
      }
      else{
        head = head -> nxt;
      }
    }
    if(found){
      return head -> code;
    }
    else{
      return "nothing";
    }
  }

  void makeTable(){
  	ofstream of;
  	of.open("table.csv",ios::out);
  	of<<capacity<<"\n";
  	for(int i =0; i<capacity;i++){
  		of<<Arr[i] -> ch<<","
  		  <<Arr[i] -> freq
  		  <<"\n";
  	}
  }

  void printall(){
  	for(int i = 0;i<mSlots;i++){
  		user* node = mainTable[i];
  		while(node != NULL){
  			cout<<node -> ch <<" -> "<< node -> freq<<" node -> code : "<<node -> code<<endl;
  			node = node -> nxt;
  		}
  	}
  }

};

class minHeap:public hashTab{
private:
	int size;
public:
	minHeap():size(capacity){}
	void setSize(){ size = capacity;}
	
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
    //heap is of Size 'n' i.e number of different characters log(n)
	user* extractMin(){
		buildMinHeap();
		user* temp = Arr[0];
		Arr[0] = Arr[size -1];
		Arr[size - 1] = temp;
		size--;
		return temp;
	}

	void insert(){
		//2* extractMin() so 2* log(n) i.e log(n)
		user* leftChild = extractMin();
		user* rightChild = extractMin();
		//createnode is const time
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
    //for createhuffman tree one part takes O(log(n)) and the other part takes O(n) as O(n) more dominant as n incresses O(n) is the order
	void createHuffmanTree(){
		//since the goes on decreasing time complexity be like : log(n) + log(n -1) + log(n -2) + log(n-3)+.....
		//for which upper bounnd of log(n) can be said so O(log(n))
		while(size > 1){
			insert();
		}
	    string code ="";
	    //assigncode assigns code to the charaters as per huffman tree i.e time is propertional to number of characters O(n)
		assignCode(Arr[0],code);
	}


	void assignCode(user* root,string code){
		root -> code = code;
		if(root -> leftChild!=NULL){
			assignCode(root -> leftChild,code+"0");
		}

		if(root -> rightChild!=NULL){
            assignCode(root -> rightChild,code+"1");
		}
	}

};




int main(){
	fstream f;
	minHeap mh;
	f.open("newfile.txt",ios::in);
	if(!f){
		cout<<"Failed to open file";
	}
	else{
		char ch;
		//Reads all characters by characters from the file to be compressed
		//so let the number of characters be N,then the time complexity is O(N)
		//Inserting into a hashtable takes constant time so  total --> O(N)
		while(!f.eof()){
			f.get(ch);
			mh.insertH(ch);
		}
	}
	f.close();
	//creating a list takes (let number of different characters be 'n') O(n)
	mh.createList();
	//takes constant time
	mh.setSize();
	//creates a table to print the number of charaters and their frequency.Takes O(n)
	mh.makeTable();
	//creates the huffmanTree for generating the huffman codes O(n)
    mh.createHuffmanTree();
    //mh.printall();
    ofstream bt;
    bt.open("compressedNewFile.txt",ios::out);
    f.open("newfile.txt",ios::in);
    if(!f){
		cout<<"Failed to open f file";
	}
	else{
		char ch;
		char part;
		int what;
		//O(numberOfCharacters * maximumLengthOfCode) --> maxlengthofcode is not much so can be assumed to be constant
		while(!f.eof()){
			f.get(ch);
			//constant time
			string code = mh.searchH(ch);
			int i =0;
			while(code[i] != '\0'){
				part = code[i];
                what = part;
				bt.put(what);
				i++;
			}
		}
	}
	bt.close();
	return 0;
}

// the total time complexity  O(N) + O(n) + O(n) + O(n) --> as N is more predominant we O(N) as the time complexity for compressing file