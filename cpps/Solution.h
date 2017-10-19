#pragma once

#include <iostream>
#include <vector>

/*
NINJA_WARRIOR
1111102222222
*/

constexpr int SPACE_SIZE    =   31;
constexpr int NINJA_COUNT   =   SPACE_SIZE/2;
constexpr int WARRIOR_COUNT =   SPACE_SIZE/2;

//TODO: memory leak

class Solution{
public:
    struct Node{
        Node(){}
        Node(const Node& rhs){
            for(int i=0;i<SPACE_SIZE;++i)
                this->data[i]   =   rhs.data[i];
            this->idxSpace  =   rhs.idxSpace;
            this->ptrParent =   nullptr;
        }
        Node* move(int i){
            Node* newNode   =   new Node(*this);
            newNode->ptrParent  =   this;

            newNode->data[newNode->idxSpace]    =   newNode->data[i];
            newNode->data[i]    =   0;
            newNode->idxSpace   =   i;

            return newNode;
        }
        void print(std::ostream& os=std::cout){
            for(int i=0;i<SPACE_SIZE;++i)
                os<<data[i];
            os<<"\t"<<idxSpace<<"\t"<<getScore();
            os<<"\n";
        }
        void calcDistance(){
            std::vector<int> ninPos, warPos;
            for(int i=0;i<SPACE_SIZE;++i){
                if(data[i]==1) ninPos.push_back(i);
                else if(data[i]==2) warPos.push_back(i);
            }

            distance    =   0;
            for(size_t i=0;i<NINJA_COUNT;++i){
                distance    +=  (SPACE_SIZE-NINJA_COUNT+i)-ninPos[i];
            }
            for(size_t i=0;i<WARRIOR_COUNT;++i){
                distance    +=  warPos[i]-i;
            }
        }
        float getScore(){
            return depth*0.5f+distance;
        }

        int data[SPACE_SIZE]{0};
        int idxSpace{NINJA_COUNT};
        Node* ptrParent{nullptr};
        int distance{0};
        int depth{0};
    };

    Solution();

    int solve();

private:

    bool isSolved(Node* n);
    std::vector<Node* > getPossibleMoves(Node* n);

    void searchBruteForce(Node* n);
    void searchAStar(Node* n);
};
