#include <iostream>
#include <memory>
#include <vector>
#include "include/device.h"
#include "include/bluetooth.h"

int main()
{
	std::unique_ptr<class Device> device( new Device() );
	std::vector<struct Device::Data> data = device->Mengintip();
	std::vector<class BlueTooth> bluetooth;

	std::cout << data.size() << std::endl;

	for( auto i : data )
	{
		//if( i.name == "BT-110v1.0.1" )
		{
			std::cout << "name: " << i.name << std::endl;
			std::cout << "address: " << i.address << std::endl;
			std::cout << "class: " << i.getDeviceClass() << std::endl;
			std::cout << "major class: " << i.getMajorDeviceClass() << std::endl;
			std::cout << "service class: " << i.getServiceClass() << std::endl;
			std::cout << "last seen: " << i.FormatDate( "%c", i.lastSeen ) << std::endl;
			std::cout << "last used: " << i.FormatDate( "%c", i.lastUsed ) << std::endl;
			std::cout << "channel ID: " << i.Channels() << std::endl;
			std::cout << std::endl;

			class BlueTooth current_bluetooth( i.address, i.Channels() );
			bluetooth.push_back( current_bluetooth );
		}
	}

	for( size_t i = 0; i < bluetooth.size(); i++ )
	{
		bluetooth[i].Connect();

		if( bluetooth[i].isConnect() )
		{
			std::vector<unsigned char> bytes = { 0xff, 0x55, 0x00, 0xff, 0x00, 0xff };
			int value = 22;

			if (value <= 255)
			{
				bytes[2] = value;
				bytes[3] = 255 - value;
			}
			else
			{
				bytes[2] = value % 256;
				bytes[3] = 255 - bytes[2];
				bytes[4] = value / 256;
				bytes[5] = 255 - bytes[4];
			}

			bluetooth[i].Write( bytes );
		}
	}

	return 0;
}
