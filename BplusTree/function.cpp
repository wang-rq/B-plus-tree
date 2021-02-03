#include<queue>
#include<iostream>
#include <windows.h>
#include<cmath>
#include"function.h"

using namespace std;

int M,L; 
Node *root;
bool found;
//层次遍历法打印 
void print(){
	cout<<endl;
	queue<Node*> q;
	q.push(root);
	int width=1;
	int i=0;
	while(!q.empty()){
		Sleep(20);
		Node *current = q.front();
		q.pop();
		i++;
		cout <<"[ ";
		int j;
		for(j=0; j<current->count; j++){
			cout << current->value[j] << " ";
			if(current->child[j]!=NULL)
				q.push(current->child[j]);
		}
		if(current->value[j]==INT_MAX && current->child[j]!=NULL)
			q.push(current->child[j]);
		cout << "]  ";
		if(i==width){
			cout<<endl;
			i=0;
			width=q.size();
		}
	}
	cout<<endl;
}

//查找 
bool search(Node* current, int val){
	Sleep(20);
	for(int i=0;i<=current->count;i++){
		if(val==current->value[i]) return true;
		else if(val < current->value[i] && current->child[i]!=NULL){
			return search(current->child[i], val);
		}
		else if(val < current->value[i] && current->child[i]==NULL)
			return false; 
	}
	return false; 
}

void splitLeaf(Node *current){ //分裂叶节点 	
	//分成左和右，其中原节点改成左节点，右节点新建 
    Node *right = new Node();
    current->count = ceil((float)L/2); 
    right->count = L+1-current->count;
    right->parent = current->parent;
    
	memcpy(&right->value[0], &current->value[current->count], sizeof(int)*(right->count));
	for(int i=right->count; i<=L+1; i++)
    	current->value[i] = INT_MAX;
	Sleep(40);//延时40ms; 
    int temp = right->value[0];
    
    if(current->parent!=NULL){ //不是根 
    	Sleep(20);//延时20ms; 
    	Node *newChildNode = right;
        current = current->parent;
		//将temp值放入合适的位置 
		for(int i=0; i<=current->count; i++){
			if(temp<current->value[i]){
				memcpy(&current->value[i+1], &current->value[i], sizeof(int)*(current->count-i));
				current->value[i]=temp;	
				break;	
			}
		}
		current->count++;
        
		int i=0;
        for(i=0; i<current->count; i++){
			if(newChildNode->value[0] < current->child[i]->value[0]){
				memcpy(&current->child[i+1],&current->child[i],sizeof(Node*)*(current->count-i));
				current->child[i]=newChildNode;
				break;
			}
        }
        current->child[i] = newChildNode;
		//找到现在的父母 
        int j=0;
        while(current->child[j]!=NULL){
        	current->child[i]->parent = current;
        	j++;
        }
    }
    
    else{ //是根 
    	Sleep(20);
        Node *parent = new Node(1, temp, current, right);
        current->parent = right->parent = parent;
        root = parent;
        return;
    }
}

void splitNonLeaf(Node *current){ //分裂非叶节点 
	Node *right = new Node();
	current->count=M/2;
	right->count = M-current->count-1;
    //找到父母 
    right->parent = current->parent;
    memcpy(&right->value[0], &current->value[current->count], sizeof(int)*(M-M/2+1));
    memcpy(&right->child[0], &current->child[current->count], sizeof(Node*)*(M-M/2+1));
    for(int i=current->count; i<=M; i++)
    	current->value[i] = INT_MAX;
    for(int i=current->count+1; i<=M; i++)
    	current->child[i] = NULL;
	Sleep(40);
    int temp = right->value[0];
	//右节点内容和指针都往前移，因为原来的第一个要被移到上面了 
    memcpy(&right->value[0], &right->value[1], sizeof(int)*(right->count+1));
    memcpy(&right->child[0], &right->child[1], sizeof(Node*)*(right->count+1));
	//找父母 
    for(int i=0;i<current->count+1;i++){
        current->child[i]->parent = current;
    }
    
    for(int i=0;i<right->count+1;i++){
        right->child[i]->parent = right;
    }

    if(current->parent!=NULL){ //如果不是根，将temp放入父母节点 
    	Sleep(20);
        current = current->parent;
        Node *newChildNode = right;
		//找到插入的合适位置 ，插入数值 
		for(int i=0; i<=current->count; i++){
			if(temp<current->value[i]){
				memcpy(&current->value[i+1], &current->value[i], sizeof(int)*(current->count-i));
				current->value[i]=temp;	
				break;	
			}
		}
        current->count++;
		//找到合适的指针位置，插入指针 
		int i=0;
        for(i=0; i<current->count; i++){
			if(newChildNode->value[0] < current->child[i]->value[0]){
				memcpy(&current->child[i+1],&current->child[i],sizeof(Node*)*(current->count-i));
				current->child[i]=newChildNode;
				break;
			}
        }
        current->child[i] = newChildNode;
		//找父母 
         for(int i=0;i<current->count+1;i++){
            current->child[i]->parent = current;
        }
    }
    else{ //如果是根 
    	Sleep(20);
        Node *parent = new Node(1, temp, current, right);
		current->parent = right->parent = parent;
        root = parent;
        return;
    }
}

//插入 
void insert(Node *current, int val){
	Sleep(20);//延时20ms; 
    for(int i=0; i<=current->count; i++){
    	if(val==current->value[i]) return;
        if(val<current->value[i]){
        	if(current->child[i]==NULL){
        		swap(current->value[i], val);
	            if(i==current->count){
	                current->count++;
	                break;
	            }
        	}
        	else{
        		insert(current->child[i], val);
            	if(current->count==M)
                	splitNonLeaf(current);
            	return;
        	}
        }
    }
    if(current->count==L+1){
            splitLeaf(current);
    }
}

void borrowLeaf(Node *left, Node *right, int leftNumber, bool isRight){
    int PrevRightFirstVal = right->value[0];
    if(!isRight){ //左节点是current 
        //可以直接借兄弟的 
        left->value[left->count] = right->value[0];
        left->count++;
        memcpy(&right->value[0], &right->value[1], sizeof(int)*(right->count+1));
        right->count--;
        left->parent->value[leftNumber] = right->value[0];
    }
    else{ //右节点是current 
        //可以直接借左兄弟的 
        memcpy(&right->value[1], &right->value[0], sizeof(int)*(right->count+1));
        right->value[0] = left->value[left->count-1];
        right->count++;
        left->value[left->count-1] = INT_MAX;
        left->count--;
        left->parent->value[leftNumber] = right->value[0];
    }
}

void borrowNonLeaf(Node *left, Node *right, int leftNumber, bool isRight){
    int PrevRightFirstVal = right->value[0];
    if(!isRight){ //左节点是current 
        //父母的值下移 
        left->value[left->count] = left->parent->value[leftNumber];
        //孩子指针向右兄弟借 
        left->child[left->count+1] = right->child[0];
        left->count++;
        //右节点值上移 
        left->parent->value[leftNumber] = right->value[0];
        //将右节点内容和指针整体左移 
        memcpy(&right->value[0], &right->value[1], sizeof(int)*(right->count+1));
        memcpy(&right->child[0], &right->child[1], sizeof(Node*)*(right->count+1));
        right->count--;
    }
    else{ //右节点是current 
        //整体右移，给借来的值和指针空出位置 
        memcpy(&right->value[1], &right->value[0], sizeof(int)*(right->count+1));
        memcpy(&right->child[1], &right->child[0], sizeof(Node*)*(right->count+1));
        right->value[0] = left->parent->value[leftNumber];     //父母值下移 
        right->child[0] = left->child[left->count]; // 孩子指针向左兄弟借 
        left->child[left->count]->parent=right;
        right->count++;
        
        //左节点指针上移 
        left->parent->value[leftNumber] = left->value[left->count-1];
        //左节点最后一个值被借走 
        left->value[left->count-1] = INT_MAX;
        left->child[left->count] = NULL;
        left->count--;

    }
}

//合并非叶节点 
void mergeNonLeaf(Node *left, Node *right, int rightNumber){
	Sleep(20);
    left->value[left->count] = left->parent->value[rightNumber-1];
    left->count++;
    memcpy(&left->value[left->count], &right->value[0], sizeof(int)*(right->count+1));
    memcpy(&left->child[left->count], &right->child[0], sizeof(root)*(right->count+1));
    left->count += right->count;
    memcpy(&left->parent->value[rightNumber-1], &left->parent->value[rightNumber], sizeof(int)*(left->parent->count+1));
    memcpy(&left->parent->child[rightNumber], &left->parent->child[rightNumber+1], sizeof(root)*(left->parent->count+1));
    left->parent->count--;
    for(int i=0;left->child[i]!=NULL;i++){//找到新父母 
    	left->child[i]->parent = left;
    }
}
//合并叶节点 
void mergeLeaf(Node *left, Node *right, int rightNumber){
	Sleep(20);
    memcpy(&left->value[left->count], &right->value[0], sizeof(int)*(right->count+1));
//    memcpy(&left->child[left->count], &right->child[0], sizeof(root)*(right->count+1));
    left->count += right->count;
    memcpy(&left->parent->value[rightNumber-1], &left->parent->value[rightNumber], sizeof(int)*(left->parent->count+1));
    memcpy(&left->parent->child[rightNumber], &left->parent->child[rightNumber+1], sizeof(root)*(left->parent->count+1));
    left->parent->count--;
    for(int i=0;left->child[i]!=NULL;i++){//找到新父母 
    	left->child[i]->parent = left;
    }
}

//删除关键字 
void deleteValue(Node* current, int val, int curNodePosition){
	 Sleep(20);
	bool isLeaf=(current->child[0]==NULL);

    //记录当前节点的value[0] 
    int curValue0 = current->value[0];
    for(int i=0;found==false &&  i<=current->count; i++){
        if(val < current->value[i] && current->child[i] != NULL){
            deleteValue(current->child[i], val, i);
        }

        if(val == current->value[i] && current->child[i] == NULL){
			//通过将叶节点的value和pointers从下标为i+1的位置起左移一位达到删除该关键字的目的 
            memcpy(&current->value[i], &current->value[i+1], sizeof(int)*(current->count+1));
            current->count--;
            found = true;
        }
     }     
  	//当前的根节点只有一个指向孩子节点的指针，没有关键字 
    if(current->parent==NULL && current->child[0] != NULL && current->count == 0){
        root = current->child[0];//孩子节点成为新的根节点 
        root->parent = NULL;
        delete current; 
        current=NULL;
        return;
    }
    //若删除的关键字在叶节点 
    if(isLeaf && current->parent!=NULL){
		//叶节点只有右兄弟节点 
        if(curNodePosition==0){
            Node *right; 
            right = current->parent->child[1]; //right指向叶节点的右兄弟节点 
			if(current->count < ceil((float)L/2)){
	            //若右兄弟节点不为空，且右兄弟节点的关键字数目＞ceil((float)L/2),则向右兄弟节点借关键字
				Sleep(20); 
				if(right!=NULL){
					if(right->count > ceil((float)L/2)) borrowLeaf(current, right, 0, false);
					else if(current->count+right->count <= L) mergeLeaf(current, right, 1);
				}
        	}
        }
		//否则 
        else{
			Node* left, * right;
			//left 指向叶节点的左兄弟节点 
            left = current->parent->child[curNodePosition-1];
			//right 指向叶节点的右兄弟节点 
            right = current->parent->child[curNodePosition+1];
	
			if(current->count<ceil((float)L/2)){
				Sleep(20);
	            //若可以向左兄弟节点借关键字 
	            if(left!=NULL && left->count > ceil((float)L/2)) borrowLeaf(left, current, curNodePosition-1, true);
	            //若可以向右兄弟节点借关键字 
	            else if(right!=NULL && right->count > ceil((float)L/2)) borrowLeaf(current, right, curNodePosition, false);	            
	            //当左右兄弟节点均不可以借，若可以与左兄弟节点合并 
	            else if (left!=NULL && current->count+left->count <= L) mergeLeaf(left, current, curNodePosition);
	            //当左右兄弟节点均不可以借，若可以与右兄弟节点合并 
	            else if (right!=NULL && current->count+right->count <= L) mergeLeaf(current, right, curNodePosition+1);
	        }
        }
    }
    //若在非叶节点删除关键字 
    else if(!isLeaf && current->parent!=NULL){
		//若该非叶节点是其父节点的第一个孩子
        if(curNodePosition==0){
            Node* right; 
            right = current->parent->child[1];//right指向非叶节点的右兄弟 
            
            //若删除关键字后节点的关键字数目小于最小关键字数目 
			if(current->count<ceil((float)M/2)-1){
				Sleep(20);
				//若可以向右兄弟节点借关键字 
				if(right!=NULL){
					if(right->count >ceil(((float)M/2)-1)) borrowNonLeaf(current, right, 0, false);
					else if(current->count+right->count<=M-1) mergeNonLeaf(current, right, 1);
				}
        	}
        }
        //否则
        else{
			Node* left, *right; 
			//left 指向左兄弟节点 
            left = current->parent->child[curNodePosition-1];
			//right 指向右兄弟节点 
            right = current->parent->child[curNodePosition+1]; 
			if(current->count<ceil((float)M/2)-1){
				Sleep(20);
	            if( left!=NULL && left->count >ceil((float)M/2)-1) borrowNonLeaf(left, current, curNodePosition-1, true);
	            else if(right!=NULL && right->count >ceil((float)M/2)-1) borrowNonLeaf(current, right, curNodePosition, false);
	            else if ( left!=NULL && current->count+left->count <= M-1) mergeNonLeaf(left, current, curNodePosition);
	            else if ( right!=NULL && current->count+right->count <= M-1) mergeNonLeaf(current, right, curNodePosition+1);    
        	}
        }
    }
    //对非叶节点的索引值进行调整 
    Node *tempNode = current->parent;
    while(tempNode!=NULL){
    		Sleep(20);
            for(int i=0; i<tempNode->count;i++){
                if(tempNode->value[i]==curValue0){
                    tempNode->value[i] = current->value[0];
                    break;
                }
        }
        tempNode = tempNode->parent;
    }
}
//释放内存 
void clear(Node* root){
	if(root!=NULL){
		int temp=root->count;
		for(int i=0;i<temp;i++){
			clear(root->child[i]);
		}
		if(root->child[temp]!=NULL) clear(root->child[temp]);
		delete root;
		root=NULL;
	}
} 



