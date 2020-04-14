#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/internet-module.h"
#include "ns3/applications-module.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE("Exercise2");



int main(int argc, char *argv[]) {
    LogComponentEnable("Exercise2", LOG_LEVEL_INFO);
    LogComponentEnable("UdpEchoClientApplication", LOG_LEVEL_INFO);
    LogComponentEnable("UdpEchoServerApplication", LOG_LEVEL_INFO);

    // Create Network Topology
    NS_LOG_INFO("Creating Network Topology...");

    NodeContainer nodes;
    nodes.Create(3);

    NodeContainer nodesA;
    nodesA.Add(nodes.Get(0));
    nodesA.Add(nodes.Get(1));

    NodeContainer nodesB;
    nodesB.Add(nodes.Get(1));
    nodesB.Add(nodes.Get(2));

    PointToPointHelper pointToPointA;
    pointToPointA.SetDeviceAttribute("DataRate", StringValue("10Mbps"));
    pointToPointA.SetChannelAttribute("Delay", StringValue("5ms"));

    PointToPointHelper pointToPointB;
    pointToPointB.SetDeviceAttribute("DataRate", StringValue("0.1Mbps"));
    pointToPointB.SetChannelAttribute("Delay", StringValue("10ms"));

    NetDeviceContainer devicesA;
    devicesA = pointToPointA.Install(nodesA);

    NetDeviceContainer devicesB;
    devicesB = pointToPointA.Install(nodesB);

    InternetStackHelper stack;
    stack.Install(nodes);

    Ipv4AddressHelper addressA;
    addressA.SetBase("10.1.1.0", "255.255.255.0");
    Ipv4InterfaceContainer interfacesA = addressA.Assign(devicesA);

    Ipv4AddressHelper addressB;
    addressB.SetBase("10.1.2.0", "255.255.255.0");
    Ipv4InterfaceContainer interfacesB = addressB.Assign(devicesB);

    // Create Applications
    NS_LOG_INFO("Creating Applications...");

    UdpEchoClientHelper echoClientA(interfacesA.GetAddress(1), 9);
    echoClientA.SetAttribute("MaxPackets", UintegerValue(1500));
    echoClientA.SetAttribute("Interval", TimeValue(MilliSeconds(1)));
    echoClientA.SetAttribute("PacketSize", UintegerValue(1050));

    UdpEchoClientHelper echoClientB(interfacesB.GetAddress(1), 9);
    echoClientB.SetAttribute("MaxPackets", UintegerValue(1500));
    echoClientB.SetAttribute("Interval", TimeValue(MilliSeconds(10)));
    echoClientB.SetAttribute("PacketSize", UintegerValue(1050));

    ApplicationContainer clientAppsA;
    clientAppsA.Add(echoClientA.Install(nodesA.Get(0)));
    clientAppsA.Start(Seconds(2));
    clientAppsA.Stop(Seconds(4));

    ApplicationContainer clientAppsB;
    clientAppsB.Add(echoClientB.Install(nodesB.Get(0)));
    clientAppsB.Start(Seconds(2));
    clientAppsB.Stop(Seconds(4));

    UdpEchoServerHelper echoServerA(9);

    UdpEchoServerHelper echoServerB(9);

    ApplicationContainer serverAppsA;
    serverAppsA.Add(echoServerA.Install(nodesA.Get(1)));
    serverAppsA.Start(Seconds(1));
    serverAppsA.Stop(Seconds(5));

    ApplicationContainer serverAppsB;
    serverAppsB.Add(echoServerB.Install(nodesB.Get(1)));
    serverAppsB.Start(Seconds(1));
    serverAppsB.Stop(Seconds(5));

    // Run Simulator
    NS_LOG_INFO("Running Simulator...");
    Simulator::Run();
    Simulator::Stop(Seconds(5));

    // Destory Simulator
    NS_LOG_INFO("Destroying Simulator...");
    Simulator::Destroy();

    return 0;
}