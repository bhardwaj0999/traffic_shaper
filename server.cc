#include<stdio.h>
#include<string.h>
#include<omnetpp.h>

using namespace omnetpp;

class Channel: public cSimpleModule
{
private:
    int max_tokens;
    int token_num;
    simtime_t timeout;
    cMessage *event;
    cQueue queue;
    int max_queue_size;

public:
    Channel();
    virtual ~Channel();

protected:
    virtual void initialize();
    virtual void handleMessage(cMessage *msg);
};

Define_Module(Channel);

Channel::Channel()
{
    event = nullptr;
}

Channel::~Channel()
{
    cancelAndDelete(event);
}

//Initialize maximum token limit
//Initialize maximum size of the queue
//Schedule token event
void Channel:: initialize() {
    max_tokens = par("token_limit");
    max_queue_size = par("queue_limit");
    token_num = max_tokens;
    WATCH(token_num);
    timeout = 0.05;
    queue.setName("my_queue");
    event = new cMessage("event");
    scheduleAt(simTime()+timeout, event);
}

//Handle the packets received from source
//Handle the token event
//Send the packets stored in queue according to the token value and no. of packets
void Channel:: handleMessage(cMessage *msg) {
    if(msg == event) {
            //increment the token number and check if there are packets in the queue.
            if (token_num < max_tokens) {
            token_num = token_num + 1;
            EV<<"token count increm="<<token_num<<"\n";
            }
            while(!queue.isEmpty() && token_num) {
                cMessage *new_msg;
                new_msg = (cMessage *)queue.pop();
                send(new_msg, "out");
                token_num = token_num - 1;
                EV<<"token count after send="<<token_num<<"\n";
            }
            scheduleAt(simTime()+timeout, event);
    } else {

        EV << "Message received from source\n";

        //Store the packets received in the queue.
        if (queue.getLength() < max_queue_size) {
        EV <<"Inserting data in queue\n";
        queue.insert(msg);
        }

        while(!queue.isEmpty() && token_num) {
            cMessage *new_msg;
            new_msg = (cMessage *)queue.pop();
            send(new_msg, "out");
            token_num = token_num - 1;
            EV<<"token count after send="<<token_num<<"\n";
        }
    }
}

