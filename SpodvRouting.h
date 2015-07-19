/*
  SpodvRouting.h : Esqueleto que hereda de la clase aodv::RoutingProtocol
  para su modificación y la implementación del protocolo SPODV.

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
  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

  Autor: Jon Senra Dearle <jonsenra.93@gmail.com>
	Copyright (C) 2015  Universidad de Sevilla

*/

#include "ns3/aodv-module.h"
#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/mobility-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/wifi-module.h"
#include "ns3/v4ping-helper.h"

// Añadir aquí la definición de métodos y atributos que sobreescribirá de la clase padre
// además de aquellos propios que implemente.
namespace ns3{

	class SpodvRouting : public aodv::RoutingProtocol {
		public:

		private:

	};
}
