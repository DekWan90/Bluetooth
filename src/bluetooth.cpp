#include <unistd.h>
#include <cstring>
#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>
#include <bluetooth/sdp.h>
#include <bluetooth/sdp_lib.h>
#include <bluetooth/rfcomm.h>
#include <sys/ioctl.h>

extern "C"
{
#include <fcntl.h>
}

#include "bluetooth.h"

BlueTooth::BlueTooth( std::string address, int channels )
{
    this->address = address;
    this->channels = channels;
    this->data = BlueTooth::Data();

    data.status = 0;
}

BlueTooth::~BlueTooth()
{
    Close();
}

void BlueTooth::Close()
{
    if( data.status != 0 )
    {
        close( data.status );
        write( data.rep[1], "close", ( strlen( "close" ) + 1 ) );
        data.status = 0;
    }
}

void BlueTooth::Connect()
{
    Close();

    // allocate an error pipe
    if( pipe( data.rep ) == -1 )
    {
        std::cerr << "Cannot create pipe for reading - " << strerror( errno ) << std::endl;
    }

    int flags = fcntl( data.rep[0], F_GETFL, 0 );

    fcntl( data.rep[0], F_SETFL, flags | O_NONBLOCK );

    struct sockaddr_rc addr = { 0x00, { { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } }, 0x00 };

    // allocate a socket
    data.status = socket( AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM );

    // set the connection parameters (who to connect to)
    addr.rc_family = AF_BLUETOOTH;
    addr.rc_channel = uint8_t( channels );
    str2ba( address.c_str(), &addr.rc_bdaddr );

    // connect to server
    int status = connect( data.status, ( struct sockaddr* )& addr, sizeof( addr ) );
    int sock_flags = fcntl( data.status, F_GETFL, 0);
    fcntl( data.status, F_SETFL, sock_flags | O_NONBLOCK );

    if( status != 0 )
        std::cerr << "Cannot connect " << address << std::endl;
}

void BlueTooth::Write( std::vector<unsigned char> bytes )
{
    if( !bytes.size() )
    {
        std::cerr << "bytes cannot be null" << std::endl;
        return;
    }

    if( data.status == 0 ) std::cerr << "Attempting to write to a closed connection" << std::endl;
    if( write( data.status, reinterpret_cast<char*>( bytes.data() ), bytes.size() ) != int( bytes.size() ) ) std::cerr << "Writing attempt was unsuccessful" << std::endl;
}

bool BlueTooth::isConnect()
{
    if( !data.status ) return false;
    else return true;
}

int BlueTooth::Read( std::vector<unsigned char> bytes )
{
    if( data.status == 0 )
        std::cerr << "connection has been closed" << std::endl;

    if ( !bytes.size() )
    {
        std::cout << "buffer cannot be null" << std::endl;
        return 0;
    }

    fd_set set;
    FD_ZERO(&set);
    FD_SET( data.status, &set );
    FD_SET( data.status, &set );

    int nfds = ( data.status > data.rep[0]) ? data.status : data.rep[0];
    int size = -1;

    if( pselect( nfds + 1, &set, NULL, NULL, NULL, NULL ) >= 0 )
    {
        if( FD_ISSET( data.status, &set ) )
            size = read( data.status, reinterpret_cast<char*>( bytes.data() ), bytes.size() );
        else // when no data is read from rfcomm the connection has been closed.
            size = 0;
    }

    if( size < 0 )
        std::cerr << "Error reading from connection" << std::endl;

    return size;
}

bool BlueTooth::isDataAvailable()
{
	int count;
	ioctl( data.status, FIONREAD, &count );
	return count > 0;
}
