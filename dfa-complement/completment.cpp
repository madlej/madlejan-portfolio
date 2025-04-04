#include <algorithm>
#include <cassert>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <deque>
#include <functional>
#include <iostream>
#include <limits>
#include <list>
#include <map>
#include <optional>
#include <queue>
#include <set>
#include <sstream>
#include <stack>
#include <vector>

using State = unsigned;
using Symbol = uint8_t;
using Word = std::vector<Symbol>;

struct MISNFA {
    std::set<State> states;
    std::set<Symbol> alphabet;
    std::map<std::pair<State, Symbol>, std::set<State>> transitions;
    std::set<State> initialStates;
    std::set<State> finalStates;
};

struct DFA {
    std::set<State> states;
    std::set<Symbol> alphabet;
    std::map<std::pair<State, Symbol>, State> transitions;
    State initialState;
    std::set<State> finalStates;

    bool operator==(const DFA& x) const = default;
};

/* ----------------------------------------------------------------------------------------*/

using namespace std;



struct Transition{
    set<State> curr_state;
    Symbol symbol;
    set<State> to_state;
};

struct My_DFA {
    set<State> states;
    set<Symbol> alphabet;
    vector<Transition> transitions; 
    set<State> initialState;
    set<set<State>> finalStates;
};

class Queue{
    public:
        bool isEmpty() {
            return m_queue.empty();
        }
        void addQueue(set<State> state){
            m_queue.push_back(state);
        }

        void removeQueue(){
            if(isEmpty())
                return;
            
            m_queue.erase(m_queue.begin());
        }

        set<State> front(){
            if(isEmpty()){
                set<State> s = {NULL};
                return s;
            }
            return m_queue.front();
        }

        size_t size(){
            return m_queue.size();
        }

    private:
        vector<set<State>> m_queue;
};


// ..................... Print .....................
void printSetState(set<State> m_set){
    cout << "{";
    for(set<State>::iterator it = m_set.begin(); it != m_set.end(); ++it){
        cout << *it;
        if(next(it) != m_set.end())
            cout << ", ";
    }
    cout << "}";
}

void printSetSymbol(set<Symbol> m_set){
    cout << "{";
    for(set<Symbol>::iterator it = m_set.begin(); it != m_set.end(); ++it){
        cout << *it;
        if(next(it) != m_set.end())
            cout << ", ";
    }
    cout << "}";
}

void printTransitions(map<pair<State, Symbol>, State> transitions){
    for(auto it = transitions.begin(); it != transitions.end(); ++it){
        State currentState = it->first.first;  
        Symbol inputSymbol = it->first.second; 
        State nextState = it->second;          
        cout << "(" << currentState << ", " << inputSymbol << ") -> " << nextState;
        if(next(it) != transitions.end())
            cout << endl;
    }
}

void printAutomat(DFA ressDfa){
    cout << "States: ";
    printSetState(ressDfa.states);
    cout <<endl;

    cout << "Alphabet: ";
    printSetSymbol(ressDfa.alphabet);
    cout << endl;

    cout << "Transitions: " << endl;
    printTransitions(ressDfa.transitions);
    cout << endl;

    cout << "Initial state: " << ressDfa.initialState << endl;

    cout << "Final states: {";
    printSetState(ressDfa.finalStates);
    cout << "}"<< endl;

    cout << endl;
    cout << endl;
}

void printSingleTransition(struct Transition transition){
        cout << "(";
        printSetState(transition.curr_state);
        cout << ", " << transition.symbol << ") -> ";
        printSetState(transition.to_state);
        cout << endl;
}

void printTransitions(vector<Transition> transitions){
    for(size_t i = 0; i < transitions.size(); i++){
        printSingleTransition(transitions[i]);
    }
    cout << endl;
}
// ..................... Print .....................

bool itemMapFinder(State state, Symbol symbol, const MISNFA& nfa, set<State> &target){
    auto it = nfa.transitions.find(std::make_pair(state, symbol));

    if (it != nfa.transitions.end()) {
        target = it->second;
        return true;
    }
    return false;
}

struct TransitTmp{
    unsigned int state;
    uint8_t symbol;
    set<State> m_Transit;
};

bool finderTransitionElement(unsigned int element, const MISNFA& nfa, vector<TransitTmp>& vectTransitTmp){

    set<State> target;
    bool finded = false;

    for(auto itS = nfa.states.begin(); itS != nfa.states.end(); itS++){     // ... for first part of key map in transition

        if(*itS == element){                                                       // ... if element from queueHead == key, find transition using symbol (alphabet)
            for(auto itA = nfa.alphabet.begin(); itA != nfa.alphabet.end(); itA++){
                bool targetFinded = itemMapFinder(*itS, *itA, nfa, target);
                if(targetFinded){
                    TransitTmp t;
                    t.state = element;
                    t.symbol = *itA;
                    t.m_Transit = target;
                    vectTransitTmp.push_back(t);
                    finded = true;
                }
            }
        }
    }

    if(finded)
        return true;
    return false;
};

void myInsertSet(set<State> from, set<State> &to){
    for(auto it = from.begin(); it != from.end(); it++){
        to.insert(*it);
    }
}

vector<Transition> finderTransitionSet(set<State> queueHead, Queue &queue, const MISNFA& nfa, My_DFA &my_DFA){

    vector<TransitTmp> vectTransitTmp;
    vector<Transition> finalTransitionTmp;
    
    // ... for each element from queueHead
    for(auto itQ = queueHead.begin(); itQ != queueHead.end(); itQ++){       
        finderTransitionElement(*itQ, nfa, vectTransitTmp);
    }

    // ... each symbol find all posible states (compress values from each element)
    size_t possitionVect = 0;
    for(auto itA = nfa.alphabet.begin(); itA != nfa.alphabet.end(); itA++){

        Transition t;
        finalTransitionTmp.push_back(t);
        finalTransitionTmp[possitionVect].curr_state = queueHead;
        finalTransitionTmp[possitionVect].symbol = *itA;

        for(size_t i = 0; i < vectTransitTmp.size(); i++){
            if(*itA == vectTransitTmp[i].symbol){
                myInsertSet(vectTransitTmp[i].m_Transit, finalTransitionTmp[possitionVect].to_state);
            }
        }
        possitionVect++;
    }

    return finalTransitionTmp;
}


void stepDeterminize(Queue &queue, const MISNFA& nfa, My_DFA &my_DFA){

    set<State> queueHead = queue.front();

    //... control, if state (queueHead) isnt already save in ressult (my_DFA)
    for(size_t i = 0; i < my_DFA.transitions.size(); i++){
        if(my_DFA.transitions[i].curr_state == queueHead){
            queue.removeQueue();
            return;
        }
    }

    vector<Transition> transCurrElem = finderTransitionSet(queueHead, queue, nfa, my_DFA);


    //... save transition to final result
    for(size_t i = 0; i < transCurrElem.size(); i++){
        my_DFA.transitions.push_back(transCurrElem[i]);
    }

    //... save transitions elements to queue
    for(size_t i = 0; i < transCurrElem.size(); i++){
        if(!transCurrElem[i].to_state.empty()){
            queue.addQueue(transCurrElem[i].to_state);          
        }
    }

    queue.removeQueue();
};

void setFinalStates(const MISNFA& nfa, My_DFA &my_DFA){

    for(auto itNfa = nfa.finalStates.begin(); itNfa != nfa.finalStates.end(); itNfa++){
        for(size_t i = 0; i < my_DFA.transitions.size(); i++){
            for(auto itDfa = my_DFA.transitions[i].curr_state.begin(); itDfa != my_DFA.transitions[i].curr_state.end(); itDfa++){
                if(*itDfa == *itNfa){
                    my_DFA.finalStates.insert(my_DFA.transitions[i].curr_state);
                }
            }
        }
    }

};

void revBFS(const My_DFA &dfa, set<set<State>> &ressStates, set<State> startingPoint){

    Queue queue;

    //... check, if starting point isnt already added
    for(auto it = ressStates.begin(); it != ressStates.end(); it++){
        if(*it == startingPoint){
            return;
        }
    }

    for(size_t i = 0; i < dfa.transitions.size(); i++){
        if(dfa.transitions[i].curr_state == startingPoint){
            ressStates.insert(dfa.transitions[i].curr_state);
        }
    }

    queue.addQueue(startingPoint);

    while(!queue.isEmpty()){

        set<State> currState = queue.front();
        queue.removeQueue();

        bool alreadyVisited = false;

        //... find if curr element has some transitions (own transitions)
        for(size_t i = 0; i < dfa.transitions.size(); i++){
            
            if(dfa.transitions[i].to_state == currState){

                alreadyVisited = false;

                for(auto it = ressStates.begin(); it != ressStates.end(); it++){
                    if(*it == dfa.transitions[i].curr_state){
                        alreadyVisited = true;
                    }
                }

                if(alreadyVisited != true){
                    queue.addQueue(dfa.transitions[i].curr_state);
                    Transition t;
                    t.curr_state = dfa.transitions[i].curr_state;
                    ressStates.insert(dfa.transitions[i].curr_state);
                }
            }
        }
    }
};


My_DFA removeUnnecessaryStates(My_DFA &dfa, set<set<State>> &allStatesForMinimalization){
    set<set<State>> ressStates;
    set<set<State>> tmpStatesDel;

    for(auto it = dfa.finalStates.begin(); it != dfa.finalStates.end(); it++){
        revBFS(dfa, ressStates, *it);
    }

    allStatesForMinimalization = ressStates;

    for(size_t i = 0; i < dfa.transitions.size(); i++){
        tmpStatesDel.insert(dfa.transitions[i].curr_state);
    }
    for(auto it = ressStates.begin(); it != ressStates.end(); it++){
        tmpStatesDel.erase(*it);
    }

    for (auto itD = tmpStatesDel.begin(); itD != tmpStatesDel.end(); itD++) {
        for (size_t i = 0; i < dfa.transitions.size(); ) {
            if (dfa.transitions[i].curr_state == *itD) {
                dfa.transitions.erase(dfa.transitions.begin() + i);
            } else {
                i++; 
            }
        }
    }

    return dfa;
};


void minimalization(My_DFA &my_DFA, const set<set<State>> &allStatesForMinimalization, DFA &ressDfa){

    map<set<State>, size_t> minimalValues;

    //... map for each set (state) its int value
    size_t stateInt = 1;
    for(auto it = allStatesForMinimalization.begin(); it != allStatesForMinimalization.end(); it++){    
        minimalValues.insert(make_pair(*it, stateInt));

        //... initialization all states
        ressDfa.states.insert(stateInt);
        stateInt++;
    }

    for(size_t i = 0; i < my_DFA.transitions.size(); i++){
        if(my_DFA.transitions[i].to_state.empty())
            continue;
        size_t currState = minimalValues[my_DFA.transitions[i].curr_state];
        size_t toState = minimalValues[my_DFA.transitions[i].to_state];

        if(currState == 0 || toState == 0){
            continue;
        }

        ressDfa.transitions.insert(make_pair(make_pair(currState, my_DFA.transitions[i].symbol), toState));
    }

    //... initialization of alphabet
    ressDfa.alphabet = my_DFA.alphabet;

    //... inialization of initial states
    ressDfa.initialState = minimalValues[my_DFA.initialState];


    //... inicialization of final states
    for(auto it = my_DFA.finalStates.begin(); it != my_DFA.finalStates.end(); it++){
        ressDfa.finalStates.insert(minimalValues[*it]);
    }
};

DFA determinize(const MISNFA& nfa){
    My_DFA my_DFA;
    DFA ressDfa;
    Queue queue;

    my_DFA.alphabet = nfa.alphabet;
    my_DFA.initialState = nfa.initialStates;

    set<State> curr_state;
    curr_state = nfa.initialStates;
    queue.addQueue(nfa.initialStates);

    while(true){
        stepDeterminize(queue, nfa, my_DFA);

        if(queue.isEmpty())
            break;
    }

    printTransitions(my_DFA.transitions);
    setFinalStates(nfa, my_DFA);

    set<set<State>> allStatesForMinimalization;
    my_DFA = removeUnnecessaryStates(my_DFA, allStatesForMinimalization);

    minimalization(my_DFA, allStatesForMinimalization, ressDfa);

    //... if automat is empty
    if(ressDfa.states.empty())
        ressDfa.states.insert(0);

    return ressDfa; 
};

DFA completeDFA(DFA mDfa){
    unsigned errorState = *mDfa.states.rbegin() + 1;
    
    for(auto it_state = mDfa.states.begin(); it_state != mDfa.states.end(); ++it_state){
        for(auto it_alphabet = mDfa.alphabet.begin(); it_alphabet != mDfa.alphabet.end(); ++it_alphabet){

            auto it_trans = mDfa.transitions.find({*it_state, *it_alphabet});

            //... transition from current state using current alphabet symbol doesnt exists
            if(it_trans == mDfa.transitions.end()){
                //... for edgecase with empty alphabet
                if(*mDfa.alphabet.begin() == '\x00' && mDfa.alphabet.size() == 1){
                    mDfa.transitions[{*it_state, *it_alphabet}] = *it_state;
                }else{
                    mDfa.transitions[{*it_state, *it_alphabet}] = errorState;

                    //... add new created state to states set
                    if(*mDfa.states.rbegin() != errorState)
                        mDfa.states.insert(errorState);
                }
            }
        }
    }

    return mDfa;
}

DFA swappAcceptAndNonaccept(DFA mDfa){

    set<State> newFinalStates;
    newFinalStates = mDfa.states;

    for(auto it_oldF = mDfa.finalStates.begin(); it_oldF != mDfa.finalStates.end(); ++it_oldF)
        newFinalStates.erase(*it_oldF);
    
    mDfa.finalStates = newFinalStates;

    return mDfa;
}

DFA deleteUselessStates(DFA mDfa, set<State> usefulStates){

    vector<pair<State, Symbol>> transToDelete;

    set<State> uselessStates = mDfa.states;
    for(auto it = usefulStates.begin(); it != usefulStates.end(); ++it){
        uselessStates.erase(*it);
    }

    for(auto it_useless = uselessStates.begin(); it_useless != uselessStates.end(); ++it_useless){
        for(auto it_trans = mDfa.transitions.begin(); it_trans != mDfa.transitions.end(); ++it_trans){
            if((it_trans->first.first == *it_useless) || (it_trans->second == *it_useless) ){
                transToDelete.push_back({it_trans->first.first , it_trans->first.second});
            }
        }
    }

    for(size_t i = 0; i < transToDelete.size(); i++){
        mDfa.transitions.erase(transToDelete[i]);
    }    

    mDfa.states = usefulStates;

    return mDfa;
};

DFA removeUselessStates(DFA mDfa){

    queue<State> queueUsefulStates;
    set<State> usefulStates;

    //... for edgecase with empty alphabet 
    usefulStates.insert(mDfa.initialState);

    for(auto it = mDfa.finalStates.begin(); it != mDfa.finalStates.end(); ++ it){
        queueUsefulStates.push(*it);
    }

    while(!queueUsefulStates.empty()){
        State currState = queueUsefulStates.front();
        queueUsefulStates.pop();
        usefulStates.insert(currState);

        //... find incoming states for currState and add them to queue
        for(auto it = mDfa.transitions.begin(); it != mDfa.transitions.end(); ++it){
            if(it->second == currState){
                if(usefulStates.find(it->first.first) == usefulStates.end()){
                    queueUsefulStates.push(it->first.first);
                }
            }
        }
    }

    cout << "Usefull states: ";
    printSetState(usefulStates);
    cout << endl;

    mDfa = deleteUselessStates(mDfa, usefulStates);

    return mDfa;
}



DFA complement(const MISNFA& nfa){
    DFA ressDFA;

    ressDFA = determinize(nfa);
    cout << "+++++ DETERMINISTIC AUTOMATON +++++" << endl;
    printAutomat(ressDFA);

    ressDFA = completeDFA(ressDFA);
    cout << "+++++ COMPLETE DETERMINISTIC AUTOMATON +++++" << endl;
    printAutomat(ressDFA);

    ressDFA = swappAcceptAndNonaccept(ressDFA);
    cout << "+++++ SWAPPING ACCEPTING AND NON-ACCEPTING STATES +++++" << endl;
    printAutomat(ressDFA);

    ressDFA = removeUselessStates(ressDFA);
    cout << "+++++ REMOVING USELESS STATES +++++" << endl;
    printAutomat(ressDFA);

    cout << endl;
    cout << "---------------------------------------------------------------------------------" <<endl;
    cout << endl;
    return ressDFA;
};
bool run(const DFA& dfa, const Word& word){
    //... check if all symbols are in dfa alphabet
    for(size_t i = 0; i < word.size(); i++){
        if(dfa.alphabet.find(word[i]) == dfa.alphabet.end())
            return false;
    }

    State currState = dfa.initialState;

    for(size_t i = 0; i < word.size(); i++){
        auto it = dfa.transitions.find({currState, word[i]});
        if (it != dfa.transitions.end()) {
            currState = it->second;  
        } else {
            return false;
        }
    }
    
    if(dfa.finalStates.find(currState) != dfa.finalStates.end())
        return true;

    return false;
};


/* ----------------------------------------------------------------------------------------*/
int main() {
    MISNFA in0 = {
        {0, 1, 2},
        {'e', 'l'},
        {
            {{0, 'e'}, {1}},
            {{0, 'l'}, {1}},
            {{1, 'e'}, {2}},
            {{2, 'e'}, {0}},
            {{2, 'l'}, {2}},
        },
        {0},
        {1, 2},
    };
    auto out0 = complement(in0);
    assert(run(out0, {}) == true);
    assert(run(out0, {'e', 'l'}) == true);
    assert(run(out0, {'l'}) == false);
    assert(run(out0, {'l', 'e', 'l', 'e'}) == true);
    MISNFA in1 = {
        {0, 1, 2, 3},
        {'g', 'l'},
        {
            {{0, 'g'}, {1}},
            {{0, 'l'}, {2}},
            {{1, 'g'}, {3}},
            {{1, 'l'}, {3}},
            {{2, 'g'}, {1}},
            {{2, 'l'}, {0}},
            {{3, 'l'}, {1}},
        },
        {0},
        {0, 2, 3},
    };
    auto out1 = complement(in1);
    assert(run(out1, {}) == false);
    assert(run(out1, {'g'}) == true);
    assert(run(out1, {'g', 'l'}) == false);
    assert(run(out1, {'g', 'l', 'l'}) == true);
    MISNFA in2 = {
        {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11},
        {'a', 'b'},
        {
            {{0, 'a'}, {1}},
            {{0, 'b'}, {2}},
            {{1, 'a'}, {3}},
            {{1, 'b'}, {4}},
            {{2, 'a'}, {5}},
            {{2, 'b'}, {6}},
            {{3, 'a'}, {7}},
            {{3, 'b'}, {8}},
            {{4, 'a'}, {9}},
            {{5, 'a'}, {5}},
            {{5, 'b'}, {10}},
            {{6, 'a'}, {8}},
            {{6, 'b'}, {8}},
            {{7, 'a'}, {11}},
            {{8, 'a'}, {3}},
            {{8, 'b'}, {9}},
            {{9, 'a'}, {5}},
            {{9, 'b'}, {5}},
            {{10, 'a'}, {1}},
            {{10, 'b'}, {2}},
            {{11, 'a'}, {5}},
            {{11, 'b'}, {5}},
        },
        {0},
        {5, 6},
    };
    auto out2 = complement(in2);
    assert(run(out2, {}) == true);
    assert(run(out2, {'a'}) == true);
    assert(run(out2, {'a', 'a', 'a', 'a', 'a', 'b'}) == true);
    assert(run(out2, {'a', 'a', 'a', 'c', 'a', 'a'}) == false);
    MISNFA in3 = {
        {0, 1, 2, 3, 4, 5, 6, 7, 8, 9},
        {'e', 'j', 'k'},
        {
            {{0, 'e'}, {1}},
            {{0, 'j'}, {2}},
            {{0, 'k'}, {3}},
            {{1, 'e'}, {2}},
            {{1, 'j'}, {4}},
            {{1, 'k'}, {5}},
            {{2, 'e'}, {6}},
            {{2, 'j'}, {0}},
            {{2, 'k'}, {4}},
            {{3, 'e'}, {7}},
            {{3, 'j'}, {2}},
            {{3, 'k'}, {1}},
            {{4, 'e'}, {4}},
            {{4, 'j'}, {8}},
            {{4, 'k'}, {7}},
            {{5, 'e'}, {4}},
            {{5, 'j'}, {0}},
            {{5, 'k'}, {4}},
            {{6, 'e'}, {7}},
            {{6, 'j'}, {8}},
            {{6, 'k'}, {4}},
            {{7, 'e'}, {3}},
            {{7, 'j'}, {1}},
            {{7, 'k'}, {8}},
            {{8, 'e'}, {2}},
            {{8, 'j'}, {4}},
            {{8, 'k'}, {9}},
            {{9, 'e'}, {4}},
            {{9, 'j'}, {0}},
            {{9, 'k'}, {4}},
        },
        {0},
        {1, 6, 8},
    };
    auto out3 = complement(in3);
    assert(run(out3, {}) == true);
    assert(run(out3, {'b', 'e', 'e'}) == false);
    assert(run(out3, {'e', 'e', 'e'}) == false);
    assert(run(out3, {'e', 'j'}) == true);
    assert(run(out3, {'e', 'k', 'j', 'e', 'j', 'j', 'j', 'e', 'k'}) == true);
    
    MISNFA in4 = {
        {0, 1, 2, 3, 4, 5},
        {'e', 'n', 'r'},
        {
            {{0, 'e'}, {1}},
            {{0, 'n'}, {1}},
            {{0, 'r'}, {2}},
            {{1, 'e'}, {2}},
            {{1, 'n'}, {3}},
            {{1, 'r'}, {3}},
            {{2, 'e'}, {3}},
            {{2, 'n'}, {3}},
            {{2, 'r'}, {1}},
            {{3, 'e'}, {1}},
            {{3, 'n'}, {1}},
            {{3, 'r'}, {2}},
            {{4, 'r'}, {5}},
        },
        {0},
        {4, 5},
    };
    auto out4 = complement(in4);
    assert(run(out4, {}) == true);
    assert(run(out4, {'e', 'n', 'r', 'e', 'n', 'r', 'e', 'n', 'r', 'e', 'n', 'r'}) == true);
    assert(run(out4, {'n', 'e', 'r', 'n', 'r', 'r', 'r', 'n', 'e', 'n', 'n', 'm', '\x0c', '\x20'}) == false);
    assert(run(out4, {'r', 'r', 'r', 'e', 'n'}) == true);
    
    MISNFA in5 = {
        {0, 1, 2, 3, 4, 5, 6},
        {'l', 'q', 't'},
        {
            {{0, 'l'}, {2, 4, 5}},
            {{0, 'q'}, {2}},
            {{0, 't'}, {1}},
            {{1, 'l'}, {0, 2}},
            {{1, 'q'}, {1, 4}},
            {{1, 't'}, {0, 2}},
            {{2, 'l'}, {5}},
            {{2, 'q'}, {0, 4}},
            {{2, 't'}, {0}},
            {{3, 'l'}, {3, 4}},
            {{3, 'q'}, {0}},
            {{3, 't'}, {0, 2}},
            {{4, 't'}, {4}},
            {{5, 'l'}, {0, 2}},
            {{5, 'q'}, {4, 5}},
            {{5, 't'}, {0, 2}},
            {{6, 'l'}, {4, 6}},
            {{6, 'q'}, {0}},
            {{6, 't'}, {0, 2}},
        },
        {2, 4},
        {0, 1, 3, 5, 6},
    };
    auto out5 = complement(in5);
    assert(run(out5, {}) == true);
    assert(run(out5, {'q', 'q'}) == true);
    assert(run(out5, {'q', 'q', 'l'}) == false);
    assert(run(out5, {'q', 'q', 'q'}) == false);

    
    MISNFA myIn6UselessStates = {
        {0, 1, 2},
        {'a', 'b'},
        {
            {{0, 'a'}, {1}},
            {{1, 'a'}, {1}},
            {{1, 'b'}, {2}},
            {{2, 'a'}, {2}},
            {{2, 'b'}, {2}},
        },
        {0},
        {1, 2},
    };
    auto out6 = complement(myIn6UselessStates);
    MISNFA myIn7EmptyAlphabet = {
        {0, 1, 2},
        {'\x00'},
        {

        },
        {0},
        {0, 1, 2},
    };
    auto out7 = complement(myIn7EmptyAlphabet);

    MISNFA myIn8EmptyAlphabetWihFinalState = {
        {0},
        {'\x00'},
        {
            {{0, '\x00'}, {0}}
        },
        {0},
        {0},
    };
    auto out8W = complement(myIn8EmptyAlphabetWihFinalState);

    MISNFA myIn8EmptyAlphabetWithoutFinalState = {
        {0},
        {'\x00'},
        {
            {{0, '\x00'}, {0}}
        },
        {0},
        {},
    };
    auto out8Without = complement(myIn8EmptyAlphabetWithoutFinalState);
    
    MISNFA my9Error5Submision= {
        {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11},
        {'a', 'b', 'c'},
        {
            {{1, 'a'}, {2}},
            {{1, 'b'}, {3, 4}},
            // {{1, 'b'}, {4}},
            {{1, 'c'}, {5, 6}},
            // {{1, 'c'}, {6}},
            {{3, 'c'}, {4, 6, 7}},
            // {{3, 'c'}, {7}},
            // {{3, 'c'}, {6}},
            {{5, 'a'}, {1, 5}},
            // {{5, 'a'}, {5}},
            {{5, 'c'}, {1, 8}},
            // {{5, 'c'}, {8}},
            {{8, 'a'}, {5}},
            {{8, 'b'}, {2, 3, 4, 5, 7}},
            // {{8, 'b'}, {5}},
            // {{8, 'b'}, {2}},
            // {{8, 'b'}, {4}},
            // {{8, 'b'}, {7}},
            {{8, 'c'}, {6}},
            {{2, 'a'}, {5, 8}},
            // {{2, 'a'}, {8}},
            {{2, 'b'}, {5, 7, 8}},
            // {{2, 'b'}, {8}},
            // {{2, 'b'}, {7}},
            {{9, 'b'}, {2, 8}},
            // {{9, 'b'}, {2}},
            {{9, 'c'}, {1, 4}},
            // {{9, 'c'}, {4}},
            {{10, 'a'}, {3, 10}},
            // {{10, 'a'}, {10}},
            {{10, 'b'}, {2}},
            {{10, 'c'}, {5, 7, 9}},
            // {{10, 'c'}, {9}},
            // {{10, 'c'}, {7}},
            {{4, 'a'}, {7}},
            {{7, 'a'}, {7}},
            {{11, 'a'}, {5, 8}},
            // {{11, 'a'}, {8}},
            {{11, 'b'}, {5, 7, 8}},
            // {{11, 'b'}, {8}},
            // {{11, 'b'}, {7}},
            {{6, 'a'}, {5, 8}},
            // {{6, 'a'}, {8}},
            {{6, 'b'}, {5, 7, 8}},
            // {{6, 'b'}, {8}},
            // {{6, 'b'}, {7}},
        },
        {1, 5},
        {8, 2, 11, 6},
    };
    auto outMy9 = complement(my9Error5Submision);

    // MISNFA inMy14 = {
    //     {0, 1, 2, 3, 4, 5, 6},
    //     {'o', 'r'},
    //     {
    //         {{0, 'o'}, {0, 1, 4}},
    //         {{0, 'r'}, {5}},
    //         {{1, 'o'}, {4}},
    //         {{1, 'r'}, {2}},
    //         {{2, 'o'}, {0, 1}},
    //         {{2, 'r'}, {0, 5}},
    //         {{3, 'r'}, {2, 5}},
    //         {{5, 'o'}, {0, 1}},
    //         {{5, 'r'}, {0, 5}},
    //         {{6, 'r'}, {2}},
    //     },
    //     {1, 2, 5},
    //     {0},
    // };
    // auto outMy14 = complement(inMy14);
    

    MISNFA rawMy9Error5Submision = {
    {468694294, 503248505, 614341365, 641947724, 642978639, 1196147699, 1284662134, 1287545593, 1413089082, 1617844589, 1808846586},
    {(unsigned char)'\xfc', (unsigned char)'\xfd', (unsigned char)'\xfe'},
    {
        {{468694294, '\xfc'}, {642978639, 1413089082}},
        {{468694294, '\xfd'}, {1413089082, 1617844589}},
        {{503248505, '\xfc'}, {642978639, 1413089082}},
        {{503248505, '\xfd'}, {614341365, 641947724}},
        {{503248505, '\xfe'}, {641947724}},
        {{614341365, '\xfc'}, {503248505, 641947724}},
        {{614341365, '\xfd'}, {641947724, 642978639, 1287545593}},
        {{614341365, '\xfe'}, {642978639, 1617844589}},
        {{641947724, '\xfc'}, {468694294, 1287545593}},
        {{641947724, '\xfd'}, {468694294, 503248505}},
        {{641947724, '\xfe'}, {614341365}},
        {{642978639, '\xfc'}, {642978639, 1287545593}},
        {{642978639, '\xfe'}, {641947724}},
        {{1196147699, '\xfc'}, {641947724, 1284662134}},
        {{1284662134, '\xfc'}, {468694294, 1284662134}},
        {{1284662134, '\xfd'}, {503248505, 1287545593}},
        {{1284662134, '\xfe'}, {468694294, 641947724, 1413089082, 1617844589}},
        {{1413089082, '\xfc'}, {1287545593}},
        {{1413089082, '\xfe'}, {1287545593}},
        {{1617844589, '\xfc'}, {642978639, 1413089082}},
        {{1617844589, '\xfd'}, {641947724, 1808846586}},
        {{1617844589, '\xfe'}, {641947724}},
        {{1808846586, '\xfc'}, {503248505, 641947724}},
        {{1808846586, '\xfd'}, {641947724, 642978639, 1287545593}},
        {{1808846586, '\xfe'}, {642978639, 1617844589}},
    },
    {641947724, 1413089082},
    {468694294, 641947724}
    };
    auto outMy10Erroe = complement(rawMy9Error5Submision);
    MISNFA inMy15TestRef = {
        {0, 1, 2, 3},
        {(unsigned char) 'a'},{
                    {{0, 'a'}, {0, 1, 3}},
                    {{1, 'a'}, {2}},
                    {{2, 'a'}, {0, 1}},
                    {{3, 'a'}, {0, 1}},
        },
        {1, 2},
        {0},
    };
    auto outMy15TestRef = complement(inMy15TestRef);

    MISNFA inMy15Test = {
        {0, 1, 2, 3},
        {(unsigned char) '\x80'},{
                    {{0, '\x80'}, {0, 1, 3}},
                    {{1, '\x80'}, {2}},
                    {{2, '\x80'}, {0, 1}},
                    {{3, '\x80'}, {0, 1}},
        },
        {1, 2},
        {0},
    };
    auto outMy14 = complement(inMy15Test);

    MISNFA myInput16Error7Submision = {
        {79569682, 84009183, 487661050, 515711064, 555758371, 1058415240, 1241944550, 1343516386, 1754445047, 1914391322, 2020873213},
        {'\x00', '\x01', '\x02'},
        {
            {{79569682, '\x00'}, {79569682, 555758371, 1754445047}},
            {{79569682, '\x02'}, {79569682, 84009183, 515711064}},
            {{84009183, '\x02'}, {84009183, 1754445047}},
            {{487661050, '\x00'}, {515711064}},
            {{487661050, '\x01'}, {79569682, 555758371}},
            {{487661050, '\x02'}, {79569682, 515711064, 1754445047}},
            {{515711064, '\x00'}, {515711064, 555758371, 1343516386, 1754445047}},
            {{515711064, '\x01'}, {79569682, 487661050, 555758371, 1343516386}},
            {{555758371, '\x00'}, {2020873213}},
            {{555758371, '\x01'}, {84009183}},
            {{1058415240, '\x01'}, {79569682, 1754445047}},
            {{1058415240, '\x02'}, {79569682, 515711064}},
            {{1241944550, '\x00'}, {1241944550}},
            {{1241944550, '\x01'}, {487661050, 515711064, 555758371, 1241944550, 1343516386}},
            {{1343516386, '\x00'}, {1754445047}},
            {{1754445047, '\x00'}, {1343516386}},
            {{1914391322, '\x01'}, {79569682, 1754445047}},
            {{1914391322, '\x02'}, {79569682, 515711064}},
            {{2020873213, '\x00'}, {515711064}},
            {{2020873213, '\x01'}, {79569682, 555758371}},
            {{2020873213, '\x02'}, {79569682, 515711064, 1754445047}},
        },
        {79569682, 487661050, 2020873213},
        {84009183, 555758371}
    };
    auto outMyInput16Error7Submision = complement(myInput16Error7Submision);

    MISNFA inEmptyLangSimpleInOut = {
        {0},
        {'\x00'},
        {
            {{0, '\x00'}, {0}},
        },
        {0},
        {0}
    };
    auto outEmptyLangSimpleInOut = complement(inEmptyLangSimpleInOut);
    assert(run(outEmptyLangSimpleInOut, {}) == false);
    assert(run(outEmptyLangSimpleInOut, {'a', 'b'}) == false);
    assert(run(outEmptyLangSimpleInOut, {'\x00'}) == false);


    MISNFA inEmptyLangSimpleIn = {
        {0},
        {'\x00'},
        {
            {{0, '\x00'}, {0}},
        },
        {0},
        {}
    };
    auto outEmptyLangSimpleIn = complement(inEmptyLangSimpleIn);
    assert(run(outEmptyLangSimpleIn, {}) == true);
    assert(run(outEmptyLangSimpleIn, {'a', 'b'}) == false);
    assert(run(outEmptyLangSimpleIn, {'\x00'}) == true);


    MISNFA inEmptyLangComplex1 = {
        {79569682, 84009183, 487661050, 515711064, 555758371, 1058415240, 1241944550, 1343516386, 1754445047, 1914391322, 2020873213},
        {'\x00', '\x01', '\x02'},
        {
            {{79569682, '\x00'}, {79569682, 555758371, 1754445047}},
            {{79569682, '\x02'}, {79569682, 84009183, 515711064}},
            {{84009183, '\x02'}, {84009183, 1754445047}},
            {{487661050, '\x00'}, {515711064}},
            {{487661050, '\x01'}, {79569682, 555758371}},
            {{487661050, '\x02'}, {79569682, 515711064, 1754445047}},
            {{515711064, '\x00'}, {515711064, 555758371, 1343516386, 1754445047}},
            {{515711064, '\x01'}, {79569682, 487661050, 555758371, 1343516386}},
            {{555758371, '\x00'}, {2020873213}},
            {{555758371, '\x01'}, {84009183}},
            {{1058415240, '\x01'}, {79569682, 1754445047}},
            {{1058415240, '\x02'}, {79569682, 515711064}},
            {{1241944550, '\x00'}, {1241944550}},
            {{1241944550, '\x01'}, {487661050, 515711064, 555758371, 1241944550, 1343516386}},
            {{1343516386, '\x00'}, {1754445047}},
            {{1754445047, '\x00'}, {1343516386}},
            {{1914391322, '\x01'}, {79569682, 1754445047}},
            {{1914391322, '\x02'}, {79569682, 515711064}},
            {{2020873213, '\x00'}, {515711064}},
            {{2020873213, '\x01'}, {79569682, 555758371}},
            {{2020873213, '\x02'}, {79569682, 515711064, 1754445047}},
        },
        {79569682, 487661050, 2020873213},
        {84009183, 555758371}
    };
    auto outEmptyLangComplex1 = complement(inEmptyLangComplex1);


    MISNFA inEmptyLangComplex2 = {
        {105338921, 136408612, 462907924, 519178969, 809459080, 1167377785, 1241050123, 1257646554, 1338408895, 1969518331, 2090901318},
        {'\x00', (unsigned char)'\xfe', (unsigned char)'\xff'},
        {
            {{105338921, '\x00'}, {105338921, 136408612, 519178969}},
            {{105338921, '\xfe'}, {105338921, 809459080, 1338408895}},
            {{136408612, '\x00'}, {136408612, 1338408895}},
            {{462907924, '\x00'}, {105338921, 519178969, 1338408895}},
            {{462907924, '\xfe'}, {519178969}},
            {{462907924, '\xff'}, {105338921, 809459080}},
            {{519178969, '\xfe'}, {519178969, 809459080, 1257646554, 1338408895}},
            {{519178969, '\xff'}, {105338921, 462907924, 809459080, 1257646554}},
            {{809459080, '\xfe'}, {2090901318}},
            {{809459080, '\xff'}, {136408612}},
            {{1167377785, '\x00'}, {105338921, 519178969}},
            {{1167377785, '\xff'}, {105338921, 1338408895}},
            {{1241050123, '\xfe'}, {1241050123}},
            {{1241050123, '\xff'}, {462907924, 519178969, 809459080, 1241050123, 1257646554}},
            {{1257646554, '\xfe'}, {1338408895}},
            {{1338408895, '\xfe'}, {1257646554}},
            {{1969518331, '\x00'}, {105338921, 519178969}},
            {{1969518331, '\xff'}, {105338921, 1338408895}},
            {{2090901318, '\x00'}, {105338921, 519178969, 1338408895}},
            {{2090901318, '\xfe'}, {519178969}},
            {{2090901318, '\xff'}, {105338921, 809459080}},
        },
        {105338921, 462907924, 2090901318},
        {136408612, 809459080}
    };
    auto outEmptyLangComplex2 = complement(inEmptyLangComplex2);
    

    
}

