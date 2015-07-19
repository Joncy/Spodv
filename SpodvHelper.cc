/*
  SpodvHelper.cc : Clase helper para la creación e instalación en nodos
  del protocolo de encaminamiento SPODV. Esta clase es una modificación
  sobre aodv-helper.cc, distribuida como parte de la librería oficial
  de ns3 y Copyright (c) 2009 IITP RAS y cuya autoría según textualmente
  en el archivo citado es:
  "Authors: Pavel Boyko <boyko@iitp.ru>, written after OlsrHelper by
  Mathieu Lacage <mathieu.lacage@sophia.inria.fr>"

  This program is free software; you can redistribute it and/or
  modify it under the terms of the GNU General Public License
  as published by the Free Software Foundation; either version 2
  of the License, or (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA."

  Autor: Jon Senra Dearle <jonsenra.93@gmail.com>
  Copyright (C) 2015  Universidad de Sevilla
*/

#include "SpodvHelper.h"
#include "SpodvRouting.h"
#include "ns3/node-list.h"
#include "ns3/names.h"
#include "ns3/ptr.h"
#include "ns3/ipv4-list-routing.h"

namespace ns3 {
  NS_LOG_COMPONENT_DEFINE("SpodvHelper");


  SpodvHelper::SpodvHelper() :
  Ipv4RoutingHelper ()
  {
  	m_agentFactory.SetTypeId ("ns3::aodv::RoutingProtocol");
  }

  SpodvHelper*
  SpodvHelper::Copy (void) const
  {
  	return new SpodvHelper (*this);
  }

  Ptr<Ipv4RoutingProtocol>
  SpodvHelper::Create (Ptr<Node> node) const
  {
  	Ptr<SpodvRouting> agent = m_agentFactory.Create<SpodvRouting> ();
  	node->AggregateObject (agent);
  	return agent;
  }

  void
  SpodvHelper::Set (std::string name, const AttributeValue &value)
  {
  	m_agentFactory.Set (name, value);
  }

  int64_t
  SpodvHelper::AssignStreams (NodeContainer c, int64_t stream)
  {
  	int64_t currentStream = stream;
  	Ptr<Node> node;
  	for (NodeContainer::Iterator i = c.Begin (); i != c.End (); ++i)
  	{
  		node = (*i);
  		Ptr<Ipv4> ipv4 = node->GetObject<Ipv4> ();
  		NS_ASSERT_MSG (ipv4, "Ipv4 not installed on node");
  		Ptr<Ipv4RoutingProtocol> proto = ipv4->GetRoutingProtocol ();
  		NS_ASSERT_MSG (proto, "Ipv4 routing not installed on node");
  		Ptr<SpodvRouting> spodv = DynamicCast<SpodvRouting> (proto);
  		if (spodv)
  		{
  			currentStream += spodv->AssignStreams (currentStream);
  			continue;
  		}
  		// Spodv may also be in a list
  		Ptr<Ipv4ListRouting> list = DynamicCast<Ipv4ListRouting> (proto);
  		if (list)
  		{
  			int16_t priority;
  			Ptr<Ipv4RoutingProtocol> listProto;
  			Ptr<SpodvRouting> listSpodv;
  			for (uint32_t i = 0; i < list->GetNRoutingProtocols (); i++)
  			{
  				listProto = list->GetRoutingProtocol (i, priority);
  				listSpodv = DynamicCast<SpodvRouting> (listProto);
  				if (listSpodv)
  				{
  					currentStream += listSpodv->AssignStreams (currentStream);
  					break;
  				}
  			}
  		}
  	}
 	return (currentStream - stream);
   }
}
