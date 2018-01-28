#define CC 1
#ifndef CC
struct Elm{
    int elm;
    int priority;
};

class Queues{
    private:
        int priority = INT_MAX;
        priorityQueues Q;
    public:
        void push_back(Elm) {Elm.priority = priority; Q.insert(Elm.priority, Elm); priority--;}
        Elm pop_front(){Q.extractMax(Elm.priority, Elm); priority++;}
};

class Stack{
    private:
        int priority = INT_MIN;
        priorityQueues Q;
    public:
        void push(Elm) {Elm.priority = priority; Q.insert(Elm.priority, Elm); priority++;}
        Elm pop(){Q.extractMax(Elm.priority, Elm); priority--;}
}
#endif

int main(){
    return 0;
}