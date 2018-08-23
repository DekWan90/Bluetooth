#ifndef BLUETOOTH_H
#define BLUETOOTH_H

#include <iostream>
#include <vector>

class BlueTooth
{
public:
    BlueTooth( std::string address, int channels );
    virtual ~BlueTooth();

    void Connect();
    void Write( std::vector<unsigned char> bytes );

    bool isConnect();
    bool isDataAvailable();
    int Read( std::vector<unsigned char> bytes );

protected:

private:
    struct Data
    {
        int status;
        int rep[2];
    };

    struct Data data;
    std::string address;
    int channels;

    void Close();
};

#endif // BLUETOOTH_H
