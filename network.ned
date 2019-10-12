simple Source
{
    parameters:
        @display("i=block/source");
    gates:
        output out;
}

simple Sink
{
    parameters:
        @display("i=block/sink");
    gates:
        input in;
}

simple Server
{
    parameters:
        @display("i=block/routing");
        int token_limit;
        int queue_limit;
        gates:
            input in;
            output out;
}

network traffic_shaper
{
    @display("bgb=298,347");
    submodules:
        source: Source {
            @display("i=,cyan;p=75,72");
        }
       sink: Sink {
            @display("i=,gold;p=208,185");
        }
        server: Server {
            @display("i=,black");
        }
    connections:
        source.out --> {datarate = 10Mbps;} --> server.in; 
        server.out --> { datarate = 10Mbps;} --> sink.in;

}
