//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
//

package multi_client;
import ned.DatarateChannel;

channel EthernetChannel extends DatarateChannel
{
    datarate = 100Mbps;
    delay = 0.1ms;
}

simple MultiClient
{
    parameters:
        string filename = default("test.txt");
        int fileSize @unit(B) = default(1024B);
        double requestDelay @unit(s) = default(1s);
        string mode = default("download");  // "download" or "upload"
        int chunkSize @unit(B) = default(512B);
    gates:
        output out;
        input in;
}

simple MultiServer
{
    parameters:
        int chunkSize @unit(B) = default(512B);
        double processingDelay @unit(s) = default(0.1s);
        bool enableEncryption = default(true);
    gates:
        output out[];
        input in[];
}

network MultiClientNet
{
    parameters:
        int numClients = default(3);

        @display("bgb=800,600");
    submodules:
        client[numClients]: MultiClient {
            parameters:
                @display[0]("p=100,100");  // Client 0 position
                @display[1]("p=100,300");  // Client 1 position
                @display[2]("p=100,500");  // Client 2 position
        }
        server: MultiServer {
            @display("p=500,300;is=vl");
        }
    connections:
        for i=0..numClients-1 {
            client[i].out --> EthernetChannel --> server.in++;
            server.out++ --> EthernetChannel --> client[i].in;
        }
}






////
//// This program is free software: you can redistribute it and/or modify
//// it under the terms of the GNU Lesser General Public License as published by
//// the Free Software Foundation, either version 3 of the License, or
//// (at your option) any later version.
////
//// This program is distributed in the hope that it will be useful,
//// but WITHOUT ANY WARRANTY; without even the implied warranty of
//// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//// GNU Lesser General Public License for more details.
////
//// You should have received a copy of the GNU Lesser General Public License
//// along with this program.  If not, see http://www.gnu.org/licenses/.
////
//
//package multi_client;
//import ned.DatarateChannel;
//
//channel EthernetChannel extends DatarateChannel
//{
//    datarate = 100Mbps;
//    delay = 0.1ms;
//}
//
//simple MultiClient
//{
//    parameters:
//        string filename = default("test.txt");
//        int fileSize @unit(B) = default(1024B);
//        double requestDelay @unit(s) = default(1s);
//        string mode = default("download");  // "download" or "upload"
//        int chunkSize @unit(B) = default(512B);
//    gates:
//        output out;
//        input in;
//}
//
//simple MultiServer
//{
//    parameters:
//        int chunkSize @unit(B) = default(512B);
//        double processingDelay @unit(s) = default(0.1s);
//        bool enableEncryption = default(true);
//    gates:
//        output out[];
//        input in[];
//}
//
//network MultiClientNet
//{
//    parameters:
//        int numClients = default(3);
//
//        @display("bgb=577,212");
//    submodules:
//        client[numClients]: MultiClient {
//            @display("p=36,122");
//        }
//        server: MultiServer {
//
//            @display("p=284,122;is=vl");
//        }
//    connections:
//        for i=0..numClients-1 {
//            client[i].out --> EthernetChannel --> server.in++;
//            server.out++ --> EthernetChannel --> client[i].in;
//        }
//}
