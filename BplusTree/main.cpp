#include"function.h"
#include<iostream>
#include<fstream>
#include <windows.h>
#include<ctime> 
#include<iomanip>
using namespace std; 

//root是一个全局变量，因此根节点不用以引用的方式传入插入和删除函数;
extern Node *root;
//M是阶数，L是叶子的容量 
extern int M;
extern int L;
extern bool found;

int main(){                       
	root=new Node();  
	clock_t start,end;
	cout<<"We think the demand of BB system is M=5 and L=5."<<endl
		<<"You can enter 5 and 5 to test..."<<endl<<endl 
		<<"If you have other ideas about the value of M and L,"<<endl
		<<"you can enter other values between 3 and "<<MAX-2<<"..."<<endl<<endl;
	while(1){ 
		cout<<"Please enter M:";
	    cin>>M;
		cout<<"Please enter L:"; 
	    cin>>L;
	    if(M>=MAX||L>=MAX||M<=2||L<=2){
	    	cout<<"M and L must be less than "<<MAX<<" and more than 2"<<endl;
	    	continue;
	    }
	    break;
	}
	cout<<"Now generate the default tree:"<<endl;
    
    char choice;
    int amount=0;//记录树中的值的总数 
    int temp;//记录要操作的值  
	ifstream fin("input1.txt");
	while(!fin.eof()){
		fin>>choice;
		if(choice=='1'){
			fin>>temp;
			start=clock();
			insert(root,temp);
			end=clock();
			amount++;
			cout<<"-----After inserting "<<temp<<":-----"<<endl;
			print();
			cout<<"Time for inserting "<<temp<<": "<<fixed<<setprecision(3)
			<<(double)(end-start)/CLOCKS_PER_SEC<<" s\n"<<endl; 
		} 
		else if(choice=='2'){     
			found = false;
			fin>>temp;
			if(amount==0){
				cout<<"There is nothing to erase."<<endl;
				continue;
			}
			if(!search(root,temp)){
				cout<<temp<<" is not in this tree."<<endl;
				continue;
			}
			start=clock();
			deleteValue(root, temp, 0);
			end=clock();
			amount--;
			cout<<"-----After erasing "<<temp<<":-----"<<endl;
			print();
			cout<<"Time for erasing "<<temp<<": "<<fixed<<setprecision(3)
			<<(double)(end-start)/CLOCKS_PER_SEC<<" s\n"<<endl; 
		}
	}
	fin.close();
	
	//手动操作：
	cout<<"The default tree has been generated"<<endl;
	cout<<"Now you can choose from the menu:"<<endl;
	cout<<"-----------menu-----------"<<endl
		<<" 1-----------------insert"<<endl
		<<" 2------------------earse"<<endl
		<<" 3-----------------search"<<endl 
		<<" 4------------------print"<<endl
		<<" 5------clear and rebuild"<<endl
		<<"------Other to exit-------"<<endl;
	while(1){
		cout<<endl<<"Enter your choice:";
		cin>>choice;
		if(choice=='1'){
			cout<<"Now insert! Enter the value:";
			cin>>temp;
			start=clock();
			insert(root,temp);
			end=clock();
			amount++;
			cout<<"-----After inserting "<<temp<<":-----"<<endl;
			print();
			cout<<"Time for inserting "<<temp<<": "<<fixed<<setprecision(3)
			<<(double)(end-start)/CLOCKS_PER_SEC<<" s\n"<<endl; 
		}        
		else if(choice=='2'){
			if(amount==0){
				cout<<"There is nothing to erase."<<endl;
				continue;
			}
			cout<<"Now erase! Enter the value:";
			found = false;
			cin>>temp;
			if(!search(root,temp)){
				cout<<temp<<" is not in this tree."<<endl;
				continue;
			}
			start=clock();
			deleteValue(root, temp, 0);
			end=clock();
			amount--;
			cout<<"-----After erasing "<<temp<<":-----"<<endl;
			print();
			cout<<"Time for erasing "<<temp<<": "<<fixed<<setprecision(3)
			<<(double)(end-start)/CLOCKS_PER_SEC<<" s\n"<<endl; 
		}
		else if(choice=='3'){
			cout<<"Now search! Enter the value:";
			cin>>temp;
			start=clock();
			int p=search(root,temp);
			end=clock(); 
			if(p){
				cout<<"Find it!"<<endl;
    				cout<<"Time for searching "<<temp<<": "<<fixed<<setprecision(3)
				<<(double)(end-start)/CLOCKS_PER_SEC<<" s\n"<<endl; 
			}
			else{
				cout<<"Can't find it!"<<endl;
				cout<<"Time for searching "<<temp<<": "<<fixed<<setprecision(3)
				<<(double)(end-start)/CLOCKS_PER_SEC<<" s\n"<<endl; 
			}
		}     
		else if(choice=='4'){
			start=clock();
			print();
			end=clock();
			cout<<"Time for printing: "<<fixed<<setprecision(3)
			<<(double)(end-start)/CLOCKS_PER_SEC<<" s\n"<<endl; 
		}
		else if(choice=='5'){
			clear(root);
			cout<<"The tree has been destroyed!"<<endl
				<<"Now you can build your new tree!"<<endl;
			root=new Node();
		}
		else return 0;
	} 
    //释放内存
	 clear(root);
    return 0;
}


