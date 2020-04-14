#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/internet-module.h"
#include "ns3/applications-module.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE("Exercise2");

NetDeviceContainer CreateNetDeviceContainer(NodeContainer nodes, std::string dataRate, std::string delay) {
    PointToPointHelper *pointToPoint_p =  new PointToPointHelper;
    pointToPoint_p->SetDeviceAttribute("DataRate", StringValue(dataRate));
    pointToPoint_p->SetChannelAttribute("Delay", StringValue(delay));
    return pointToPoint_p->Install(nodes);
}

Ipv4InterfaceContainer CreateIpv4InterfaceContainer(NetDeviceContainer devices, Ipv4Address network, Ipv4Mask mask) {
    Ipv4AddressHelper *address_p = new Ipv4AddressHelper;
    address_p->SetBase(network, mask);
    return address_p->Assign(devices);
}

UdpEchoClientHelper* CreateUdpEchoClientHelper(Address destIp, uint16_t port, uint64_t maxPackets, uint64_t packetSize, Time interval) {
    UdpEchoClientHelper *echoClient_p = new UdpEchoClientHelper(destIp, port);
    echoClient_p->SetAttribute("MaxPackets", UintegerValue(maxPackets));
    echoClient_p->SetAttribute("PacketSize", UintegerValue(packetSize));
    echoClient_p->SetAttribute("Interval", TimeValue(interval));
    return echoClient_p;
}

UdpEchoServerHelper* CreateUdpEchoServerHelper(uint16_t port) {
    return new UdpEchoServerHelper(port);
}

int main(int argc, char *argv[]) {
    LogComponentEnable("Exercise2", LOG_LEVEL_INFO);
    LogComponentEnable("UdpEchoClientApplication", LOG_LEVEL_INFO);
    LogComponentEnable("UdpEchoServerApplication", LOG_LEVEL_INFO);

    // Create Network Topology
    NS_LOG_INFO("Creating Network Topology...");

    NodeContainer nodes;
    nodes.Create(3);
    NodeContainer nodesA(nodes.Get(0), nodes.Get(1));
    NodeContainer nodesB(nodes.Get(1), nodes.Get(2));

    NetDeviceContainer devicesA = CreateNetDeviceContainer(nodesA, "10Mbps", "5ms");
    NetDeviceContainer devicesB = CreateNetDeviceContainer(nodesB, "0.1Mbps", "10ms");

    InternetStackHelper stack;
    stack.Install(nodes);

    Ipv4InterfaceContainer interfacesA = CreateIpv4InterfaceContainer(devicesA, "10.1.1.0", "255.255.255.0");
    Ipv4InterfaceContainer interfacesB = CreateIpv4InterfaceContainer(devicesB, "10.1.2.0", "255.255.255.0");

    // Create Applications
    NS_LOG_INFO("Creating Applications...");

    ApplicationContainer clientApps;
    clientApps.Add(CreateUdpEchoClientHelper(interfacesA.GetAddress(1), 9, 1500, 1050, MilliSeconds(1))->Install(nodesA.Get(0)));
    clientApps.Add(CreateUdpEchoClientHelper(interfacesB.GetAddress(1), 9, 1500, 1050, MilliSeconds(10))->Install(nodesB.Get(0)));
    clientApps.Start(Seconds(2));
    clientApps.Stop(Seconds(4));

    ApplicationContainer serverApps;
    serverApps.Add(CreateUdpEchoServerHelper(9)->Install(nodesA.Get(1)));
    serverApps.Add(CreateUdpEchoServerHelper(9)->Install(nodesB.Get(1)));
    serverApps.Start(Seconds(1));
    serverApps.Stop(Seconds(5));

    // Run Simulator
    NS_LOG_INFO("Running Simulator...");
    Simulator::Run();
    Simulator::Stop(Seconds(5));

    // Destory Simulator
    NS_LOG_INFO("Destroying Simulator...");
    Simulator::Destroy();

    return 0;
}