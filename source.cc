#include<stdio.h>
#include<string.h>
#include<cmath>
#include<string>
#include<omnetpp.h>

using namespace omnetpp;

class Source: public cSimpleModule
{
private:
    cMessage *event;
    simtime_t timeout;
    int pConstant;
    double pLambda;
protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
};

Define_Module(Source);



void Source:: initialize() {
    event = new cMessage("event");
    pConstant = 5;
    pLambda = 0.1;
    
    scheduleAt(simTime(), event);
}

//Handle the event timeout
//Send the packet out at a variable bit rate
void Source:: handleMessage(cMessage *msg) {
    if (msg == event) {

        //Calculate random time to send next job based on poisson distribution.
                double pRandomTime = exp(pConstant * log(pLambda) - lgamma(pConstant + 1.0) - pLambda);
                //EV << "poissonRandomTime - " + std::to_string(pRandomTime);
                //scheduleAt(simTime() + pRandomTime, event);

        scheduleAt(simTime()+pRandomTime, event);

        if(pConstant == 0)
                {
                    pConstant = 10;
                } else
                {
                pConstant = pConstant -1;
                }

        cMessage *msg = new cMessage("source_msg");
        send(msg,"out");
    }
}
