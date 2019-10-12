#include<stdio.h>
#include<string.h>
#include<omnetpp.h>

using namespace omnetpp;

class Sink: public cSimpleModule
{
protected:
    virtual void handleMessage(cMessage *msg);
};

Define_Module(Sink);

//Handle the packets received from source via channel
void Sink:: handleMessage(cMessage *msg)
{
    EV <<"Message received from channel\n";
    EV << msg << "\n";
    delete msg;
   // EV <<"discarded msg\n";
}
