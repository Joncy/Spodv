/*
  SpodvHelper.h : Clase helper para la creación e instalación en nodos
  del protocolo de encaminamiento SPODV. Esta clase es una modificación
  sobre aodv-helper.h, distribuida como parte de la librería oficial
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

#ifndef SPODV_HELPER_H_
#define SPODV_HELPER_H_

#include "ns3/object-factory.h"
#include "ns3/node.h"
#include "ns3/node-container.h"
#include "ns3/ipv4-routing-helper.h"

namespace ns3 {
	class SpodvHelper : public Ipv4RoutingHelper		 {
		public:
			SpodvHelper();

			// This method is mainly for internal use by the other helpers;
			// clients are expected to free the dynamic memory allocated by this method
			// Return pointer to clone of this OlsrHelper
			SpodvHelper* Copy (void) const;

			// Parameters
  			//  node	the node on which the routing protocol will run
			// Returns a newly-created routing protocol
			// This method will be called by ns3::InternetStackHelper::Install
			virtual Ptr<Ipv4RoutingProtocol> Create (Ptr<Node> node) const;

			// Parameters
 			// name 	the name of the attribute to set
    		// value	the value of the attribute to set.
			// This method controls the attributes of SpodvRouting
			void Set (std::string name, const AttributeValue &value);

			// Assign a fixed random variable stream number to the random variables used by this model.
			// Return the number of streams (possibly zero) that have been assigned.
			// The Install() method of the InternetStackHelper should have previously been called by the user.
			int64_t AssignStreams (NodeContainer c, int64_t stream);

		private:
			ObjectFactory m_agentFactory;
	};
}
#endif // SPODV_HELPER_H_
