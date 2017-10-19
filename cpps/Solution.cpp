#include <cassert>
#include <list>
#include <queue>
#include <algorithm>
#include <numeric>

#include "Solution.h"

using namespace std;

Solution::Solution(){}

int Solution::solve(){
    auto initNode   =   new Node;
    for(int i=0;i<NINJA_COUNT;++i) 
        initNode->data[i]=1;
    for(int i=0;i<WARRIOR_COUNT;++i) 
        initNode->data[SPACE_SIZE-1-i]=2;
    
    cout<<"initial state:\n";
    initNode->print();
    cout<<"\n";

    // first step
    initNode    =   initNode->move(initNode->idxSpace-1);

    // searchBruteForce(initNode);
    // cout<<"==========\n";
    searchAStar(initNode);

    return 0;
}

void Solution::searchAStar(Node* initNode){
    cout<<"---a star---\n";

    list<Node* > openList, closeList;
    openList.push_back(initNode);
    Node* lastNode  =   nullptr;
    int iterations  =   0;
    while(!openList.empty()){
        ++iterations;

        Node* curNode    =   nullptr;
        // select
        {
            float minScore  =   numeric_limits<float>::max();
            decltype(openList.begin()) iterToErase;
            for(auto iter=openList.begin(); iter!=openList.end();++iter){
                Node* n =   *iter;
                if(n->getScore()<minScore){
                    minScore    =   n->getScore();
                    curNode =   n;
                    iterToErase =   iter;
                }
            }
            openList.erase(iterToErase);
        }
        closeList.push_back(curNode);

        if(isSolved(curNode)){
            lastNode    =   curNode;
            break;
        }

        // expand
        auto vecNodes   =   getPossibleMoves(curNode);
        for(auto n: vecNodes){
            if(find_if(closeList.begin(), closeList.end(), [&n](Node* node)->bool{
                for(int i=0;i<SPACE_SIZE;++i)
                    if(n->data[i]!=node->data[i])
                        return false;
                return true;
            })!=closeList.end()){
                // cout<<"walked: ";n->print();
                continue;
            }

            n->calcDistance();
            n->depth    =  curNode->depth+1;
            openList.push_back(n);
            // cout<<"\t"; n->print();
        }
    }

    if(lastNode){
        cout<<"solved after "<<iterations<<" iterations.\n";
        // reverse
        vector<Node* > vecLine;
        vecLine.push_back(lastNode);
        while(vecLine.back()->ptrParent){
            vecLine.push_back(vecLine.back()->ptrParent);
        }

        cout<<"total steps: "<<vecLine.size()<<"\n";
        for(size_t i=vecLine.size()-1;i>=0;--i){
            cout<<vecLine.size()-i<<"\t";
            vecLine[i]->print();            
        }

    }else{
        cout<<"ehhh...\n";
    }
}

void Solution::searchBruteForce(Node* initNode){
    cout<<"---brute force---\n";
    queue<Node* > theQueue;
    vector<Node* > vecWalked;
    theQueue.push(initNode);
    Node* lastNode  =   nullptr;
    // expand
    int iterations  =   0;
    while(!theQueue.empty()){
        ++iterations;

        auto curNode    =   theQueue.front();
        theQueue.pop();
        vecWalked.push_back(curNode);

        if(isSolved(curNode)){
            lastNode    =   curNode;
            break;
        }

        // cout<<"> ";curNode->print();
        auto vecNodes   =   getPossibleMoves(curNode);
        for(auto n: vecNodes){
            if(find_if(vecWalked.begin(), vecWalked.end(), [&n](Node* node)->bool{
                for(int i=0;i<SPACE_SIZE;++i)
                    if(n->data[i]!=node->data[i])
                        return false;
                return true;
            })!=vecWalked.end()){
                // cout<<"walked: ";n->print();
                continue;
            }

            theQueue.push(n);
            // cout<<"\t"; n->print();
        }
        // cout<<"----------\n";
    }

    if(lastNode){
        cout<<"solved after "<<iterations<<" iterations.\n";
        // reverse
        vector<Node* > vecLine;
        vecLine.push_back(lastNode);
        while(vecLine.back()->ptrParent){
            vecLine.push_back(vecLine.back()->ptrParent);
        }

        cout<<"total steps: "<<vecLine.size()<<"\n";
        for(size_t i=vecLine.size()-1;i>=0;--i){
            cout<<vecLine.size()-i<<"\t";
            vecLine[i]->print();            
        }

    }else{
        cout<<"ehhh...\n";
    }
}

bool Solution::isSolved(Node* n){
    for(int i=0;i<WARRIOR_COUNT;++i)
    if(n->data[i]!=2)
        return false;
    for(int i=0;i<NINJA_COUNT;++i) 
        if(n->data[SPACE_SIZE-1-i]!=1)
            return false;

    return true;
}

vector<Solution::Node* > Solution::getPossibleMoves(Node* n){
    vector<Node* > vecNodes;

    // -2
    if(n->idxSpace>=2){
        if(n->data[n->idxSpace-2]==1){
            vecNodes.push_back(n->move(n->idxSpace-2));
        }
    }
    // -1
    if(n->idxSpace>=1){
        if(n->data[n->idxSpace-1]==1){
            vecNodes.push_back(n->move(n->idxSpace-1));
        }
    }

    // +1
    if(n->idxSpace<SPACE_SIZE-1){
        if(n->data[n->idxSpace+1]==2){
            vecNodes.push_back(n->move(n->idxSpace+1));
        }
    }
    // +2
    if(n->idxSpace<SPACE_SIZE-2){
        if(n->data[n->idxSpace+2]==2){
            vecNodes.push_back(n->move(n->idxSpace+2));
        }
    }

    return vecNodes;
}
