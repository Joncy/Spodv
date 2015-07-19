/*
 spodv.cc : Escenario de pruebas de simulación para el protocolo en desarrollo
 SPODV. Esta clase es una modificación
 sobre aodv.cc, distribuida como parte de la librería oficial
 de ns3 y Copyright (c) 2009 IITP RAS y cuya autoría según textualmente
 en el archivo citado es:
 "Authors: Pavel Boyko <boyko@iitp.ru>, written after OlsrHelper by
 Mathieu Lacage <mathieu.lacage@sophia.inria.fr>"

 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License version 2 as
 published by the Free Software Foundation;

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

 This is an example script for AODV manet routing protocol."

 Autor: Jon Senra Dearle <jonsenra.93@gmail.com>
 Copyright (C) 2015  Universidad de Sevilla
 */

 // Includes

#include "ns3/aodv-module.h"
#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/mobility-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/wifi-module.h"
#include "ns3/v4ping-helper.h"
#include <iostream>
#include <cmath>


#include "ns3/vector.h"
#include "ns3/string.h"
#include "ns3/socket.h"
#include "ns3/double.h"
#include "ns3/config.h"
#include "ns3/log.h"
#include "ns3/command-line.h"
#include "ns3/mobility-model.h"
#include "ns3/yans-wifi-helper.h"
#include "ns3/position-allocator.h"
#include "ns3/mobility-helper.h"
#include "ns3/internet-stack-helper.h"
#include "ns3/ipv4-address-helper.h"
#include "ns3/ipv4-interface-container.h"
#include <iostream>

#include "ns3/ocb-wifi-mac.h"
#include "ns3/wifi-80211p-helper.h"
#include "ns3/wave-mac-helper.h"
#include "ns3/on-off-helper.h"
#include "SpodvHelper.h"
#include "SpodvRouting.h"

using namespace ns3;

class SpodvExample
{
public:
  SpodvExample ();
  /// Configure script parameters, \return true on successful configuration
  bool Configure (int argc, char **argv);
  /// Run simulation
  void Run ();
  /// Report results
  void Report (std::ostream & os);

private:

  // parameters
  /// Number of nodes
  uint32_t size;
  /// Simulation time, seconds
  double totalTime;
  /// Start time, seconds
  double initTime;
  /// Write per-device PCAP traces if true
  bool pcap;
  /// Print routes if true
  bool printRoutes;
  // Route to movement tracefile
  std::string traceFile;

  // network
  NodeContainer nodes;
  NetDeviceContainer devices;
  Ipv4InterfaceContainer interfaces;

private:
  void CreateNodes ();
  void CreateDevices ();
  void InstallInternetStack ();
  void InstallApplications ();
};

int main (int argc, char **argv)
{
  SpodvExample test;
  if (!test.Configure (argc, argv))
    NS_FATAL_ERROR ("Configuration failed. Aborted.");

  test.Run ();
  test.Report (std::cout);
  return 0;
}

//-----------------------------------------------------------------------------
SpodvExample::SpodvExample () :
  size (10),
  totalTime (110),
  initTime (10),
  pcap (false),
  printRoutes (true),
  traceFile("/Users/joncy/Documents/Universidad/TFG/ns3/ns-allinone-3.22/ns-3.22/scratch/tfg/Psodv.ns_movements")
{
}

bool
SpodvExample::Configure (int argc, char **argv)
{
  // Enable AODV logs by default. Comment this if too noisy
  // LogComponentEnable("AodvRoutingProtocol", LOG_LEVEL_ALL);

  SeedManager::SetSeed (12345);
  CommandLine cmd;

  cmd.AddValue ("pcap", "Write PCAP traces.", pcap);
  cmd.AddValue ("printRoutes", "Print routing table dumps.", printRoutes);
  cmd.AddValue ("size", "Number of nodes.", size);
  cmd.AddValue ("time", "Simulation time, s.", totalTime);
  cmd.AddValue ("time", "Init time, s.", initTime);
  cmd.AddValue ("tracefile", "Route to ns2 movement tracefile", traceFile);

  cmd.Parse (argc, argv);
  return true;
}

void
SpodvExample::Run ()
{
//  Config::SetDefault ("ns3::WifiRemoteStationManager::RtsCtsThreshold", UintegerValue (1)); // enable rts cts all the time.
  CreateNodes ();
  CreateDevices ();
  InstallInternetStack ();
  InstallApplications ();

  std::cout << "Starting simulation for " << totalTime -initTime << " s ...\n";

  Simulator::Stop (Seconds (totalTime));
  Simulator::Run ();
  Simulator::Destroy ();
}

void
SpodvExample::Report (std::ostream &)
{
}

void
SpodvExample::CreateNodes ()
{
  std::cout << "Creating " << (unsigned)size << " nodes.\n";
  nodes.Create (size);
  // Name nodes
  for (uint32_t i = 0; i < size; ++i)
    {
      std::ostringstream os;
      os << "node-" << i;
      Names::Add (os.str (), nodes.Get (i));
    }

  // Create Manhattan Grid
  Ns2MobilityHelper ns2mobility = Ns2MobilityHelper(traceFile);
  ns2mobility.Install();

}

void
SpodvExample::CreateDevices ()
{
  std::string phyMode ("OfdmRate6MbpsBW10MHz");
  NqosWaveMacHelper wifi80211pMac = NqosWaveMacHelper::Default ();
  YansWifiPhyHelper wifiPhy = YansWifiPhyHelper::Default ();
  YansWifiChannelHelper wifiChannel = YansWifiChannelHelper::Default ();
  wifiPhy.SetChannel (wifiChannel.Create ());
  wifiPhy.SetPcapDataLinkType (YansWifiPhyHelper::DLT_IEEE802_11);
  Wifi80211pHelper wifi80211p = Wifi80211pHelper::Default ();
  wifi80211p.SetRemoteStationManager ("ns3::ConstantRateWifiManager",
                                      "DataMode",StringValue (phyMode),
                                      "ControlMode",StringValue (phyMode));
  devices = wifi80211p.Install (wifiPhy, wifi80211pMac, nodes);

  if (pcap)
    {
      wifiPhy.EnablePcapAll (std::string ("spodv"));
    }
}

void
SpodvExample::InstallInternetStack ()
{
  SpodvHelper spodv;
  // you can configure SPODV attributes here using spodv.Set(name, value)
  InternetStackHelper stack;
  stack.SetRoutingHelper (spodv); // has effect on the next Install ()
  stack.Install (nodes);
  Ipv4AddressHelper address;
  address.SetBase ("10.0.0.0", "255.0.0.0");
  interfaces = address.Assign (devices);

  if (printRoutes)
    {
      Ptr<OutputStreamWrapper> routingStream = Create<OutputStreamWrapper> ("spodv.routes", std::ios::out);
      spodv.PrintRoutingTableAllAt (Seconds (90), routingStream);
    }
}

void
SpodvExample::InstallApplications ()
{
  OnOffHelper onOff ("ns3::UdpSocketFactory", interfaces.GetAddress (size -1));

  ApplicationContainer p = onOff.Install (nodes.Get (0));
  p.Start (Seconds (initTime));
  p.Stop (Seconds (totalTime) - Seconds (0.001));
}
