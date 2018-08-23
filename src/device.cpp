#include <iostream>
#include <numeric>

extern "C"
{
#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>
#include <bluetooth/sdp.h>
#include <bluetooth/sdp_lib.h>
#include <bluetooth/rfcomm.h>
}

#include "device.h"

Device::Device()
{
    //ctor
}

Device::~Device()
{
    //dtor
}

std::vector<struct Device::Data> Device::Mengintip()
{
    char addr[19] = { 0 };
    char name[248] = { 0 };

    int dev_id = hci_get_route( nullptr );
    int sock = hci_open_dev( dev_id );

    if( dev_id < 0 || sock < 0 ) std::cerr << "error opening socket" << std::endl;

    int max_rsp = 255;
    inquiry_info *ii = (inquiry_info*)malloc(max_rsp * sizeof(inquiry_info));
    int num_rsp = hci_inquiry( dev_id, 8, max_rsp, NULL, &ii, IREQ_CACHE_FLUSH );

    std::vector<struct Device::Data> data;

    for (int i = 0; i < num_rsp; i++)
    {
        ba2str(&(ii + i)->bdaddr, addr);
        memset(name, 0, sizeof(name));

        hci_read_remote_name(sock, &(ii + i)->bdaddr, sizeof(name), name, 0);

        unsigned char *p = (ii + i)->dev_class;
        uint32_t cod = uint32_t( p[0] + ( 0x100 * ( p[1] + ( 0x100 * p[2] ) ) ) );

        Device::Data current_data;
        current_data.address = std::string( addr );
        current_data.name = std::string( name );
        current_data.connected = false;
        current_data.remembered = false;
        current_data.authenticated = false;
        current_data.lastSeen = 0;
        current_data.lastUsed = 0;
        current_data.device_class = DeviceClass( cod & 0x1ffc );
        current_data.major_device_class = DeviceClass( cod & DC_Uncategorized );
        current_data.service_class = ServiceClass( cod >> 13 );
        data.push_back( current_data );
    }


    return data;
}

std::string Device::Data::getDeviceClass( DeviceClass device_class )
{
    switch( device_class )
    {
    case DC_Miscellaneous:
        return "Miscellaneous";
    case DC_Computer:
        return "Computer";
    case DC_DesktopComputer:
        return "Desktop Computer";
    case DC_ServerComputer:
        return "Server Computer";
    case DC_LaptopComputer:
        return "Laptop Computer";
    case DC_HandheldComputer:
        return "Handheld Computer";
    case DC_PdaComputer:
        return "Pda Computer";
    case DC_WearableComputer:
        return "Wearable Computer";
    case DC_Phone:
        return "Phone";
    case DC_CellPhone:
        return "Cell Phone";
    case DC_CordlessPhone:
        return "Cordless Phone";
    case DC_SmartPhone:
        return "Smart Phone";
    case DC_WiredPhone:
        return "Wired Phone";
    case DC_IsdnAccess:
        return "Isdn Access";
    case DC_AccessPointAvailable:
        return "Access Point Available";
    case DC_AccessPoint1To17:
        return "Access Point 1 To 17";
    case DC_AccessPoint17To33:
        return "Access Point 17 To 33";
    case DC_AccessPoint33To50:
        return "Access Point 33 To 50";
    case DC_AccessPoint50To67:
        return "Access Point 50 To 67";
    case DC_AccessPoint67To83:
        return "Access Point 67 To 83";
    case DC_AccessPoint83To99:
        return "Access Point 83 To 99";
    case DC_AccessPointNoService:
        return "Access Point No Service";
    case DC_AudioVideoUnclassified:
        return "Audio Video Unclassified";
    case DC_AudioVideoHeadset:
        return "Audio Video Headset";
    case DC_AudioVideoHandsFree:
        return "Audio Video Hands Free";
    case DC_AudioVideoMicrophone:
        return "Audio Video Microphone";
    case DC_AudioVideoLoudSpeaker:
        return "Audio Video LoudSpeaker";
    case DC_AudioVideoHeadphones:
        return "Audio Video Headphones";
    case DC_AudioVideoPortable:
        return "Audio Video Portable";
    case DC_AudioVideoCar:
        return "Audio VideoCar";
    case DC_AudioVideoSetTopBox:
        return "Audio Video Set Top Box";
    case DC_AudioVideoHiFi:
        return "Audio Video HiFi";
    case DC_AudioVideoVcr:
        return "AudioVideoVcr";
    case DC_AudioVideoVideoCamera:
        return "Audio Video Video Camera";
    case DC_AudioVideoCamcorder:
        return "Audio Video Camcorder";
    case DC_AudioVideoMonitor:
        return "Audio Video Monitor";
    case DC_AudioVideoDisplayLoudSpeaker:
        return "Audio Video Display LoudSpeaker";
    case DC_AudioVideoVideoConferencing:
        return "Audio Video Video Conferencing";
    case DC_AudioVideoGaming:
        return "Audio Video Gaming";
    case DC_Peripheral:
        return "Peripheral";
    case DC_PeripheralJoystick:
        return "Peripheral Joystick";
    case DC_PeripheralGamepad:
        return "Peripheral Gamepad";
    case DC_PeripheralRemoteControl:
        return "Peripheral Remote Control";
    case DC_PeripheralSensingDevice:
        return "Peripheral Sensing Device";
    case DC_PeripheralDigitizerTablet:
        return "Peripheral Digitizer Tablet";
    case DC_PeripheralCardReader:
        return "Peripheral Card Reader";
    case DC_PeripheralKeyboard:
        return "Peripheral Keyboard";
    case DC_PeripheralPointingDevice:
        return "Peripheral Pointing Device";
    case DC_PeripheralCombinedKeyboardPointingDevice:
        return "Peripheral Combined Keyboard Pointing Device";
    case DC_Imaging:
        return "Imaging";
    case DC_ImagingDisplay:
        return "Imaging Display";
    case DC_ImagingCamera:
        return "Imaging Camera";
    case DC_ImagingScanner:
        return "Imaging Scanner";
    case DC_ImagingPrinter:
        return "Imaging Printer";
    case DC_Wearable:
        return "Wearable";
    case DC_WearableWristWatch:
        return "Wearable Wrist Watch";
    case DC_WearablePager:
        return "Wearable Pager";
    case DC_WearableJacket:
        return "Wearable Jacket";
    case DC_WearableHelmet:
        return "Wearable Helmet";
    case DC_WearableGlasses:
        return "Wearable Glasses";
    case DC_Toy:
        return "Toy";
    case DC_ToyRobot:
        return "Toy Robot";
    case DC_ToyVehicle:
        return "Toy Vehicle";
    case DC_ToyFigure:
        return "Toy Figure";
    case DC_ToyController:
        return "Toy Controller";
    case DC_ToyGame:
        return "Toy Game";
    case DC_Medical:
        return "Medical";
    case DC_MedicalBloodPressureMonitor:
        return "Medical Blood Pressure Monitor";
    case DC_MedicalThermometer:
        return "Medical Thermometer";
    case DC_MedicalWeighingScale:
        return "Medical Weighing Scale";
    case DC_MedicalGlucoseMeter:
        return "Medical Glucose Meter";
    case DC_MedicalPulseOximeter:
        return "Medical Pulse Oximeter";
    case DC_MedicalHeartPulseRateMonitor:
        return "Medical Heart Pulse Rate Monitor";
    case DC_MedicalDataDisplay:
        return "Medical Data Display";
    case DC_Uncategorized:
        return "Uncategorized";
    default:
        return "";
    }
}

std::string Device::Data::getServiceClass()
{
    if (service_class == SC_None)
        return "None";

    std::vector<std::string> list;

    if ((service_class & SC_LimitedDiscoverableMode) != 0)
        list.push_back("Limited Discoverable Mode");
    if ((service_class & SC_Positioning) != 0)
        list.push_back("Positioning");
    if ((service_class & SC_Network) != 0)
        list.push_back("Network");
    if ((service_class & SC_Rendering) != 0)
        list.push_back("Rendering");
    if ((service_class & SC_Capturing) != 0)
        list.push_back("Capturing");
    if ((service_class & SC_ObjectTransfer) != 0)
        list.push_back("Object Transfer");
    if ((service_class & SC_Audio) != 0)
        list.push_back("Audio");
    if ((service_class & SC_Telephony) != 0)
        list.push_back("Telephony");
    if ((service_class & SC_Information) != 0)
        list.push_back("Information");

    return accumulate( list.begin(), list.end(), std::string(), []( const std::string& a, const std::string& b ) -> std::string{ return a + ( a.length() > 0 ? ", " : "" ) + b; } );
}

std::string Device::Data::FormatDate( const char *format, time_t time )
{
    if (time <= 0)
        return "--";

    char buffer[256] = { 0 };
    tm *timeinfo = localtime(&time);

    if (timeinfo)
        strftime(buffer, sizeof(buffer), format, timeinfo);

    return buffer;
}

int Device::Data::Channels()
{
    int channelID = -1;
    uuid_t svc_uuid;
    bdaddr_t target;
    bdaddr_t source = { { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } };
    sdp_list_t *response_list = NULL, *search_list, *attrid_list;
    sdp_session_t *session = 0;

    str2ba(address.c_str(), &target);

    // connect to the SDP server running on the remote machine
    // session = sdp_connect(BDADDR_ANY, &target, SDP_RETRY_IF_BUSY);
    session = sdp_connect(&source, &target, SDP_RETRY_IF_BUSY);

    if (!session) std::cerr << "no session resurned from sdp_connect" << std::endl;

    // specify the UUID of the application we're searching for
    sdp_uuid16_create(&svc_uuid, SERIAL_PORT_PROFILE_ID);
    search_list = sdp_list_append(NULL, &svc_uuid);

    // specify that we want a list of all the matching applications' attributes
    uint32_t range = 0x0000ffff;
    attrid_list = sdp_list_append(NULL, &range);

    // get a list of service records that have the serial port UUID
    sdp_service_search_attr_req(session, search_list, SDP_ATTR_REQ_RANGE, attrid_list, &response_list);
    sdp_list_t *r = response_list;

    // go through each of the service records
    for (; r; r = r->next)
    {
        sdp_record_t *rec = (sdp_record_t*)r->data;
        sdp_list_t *proto_list;

        // get a list of the protocol sequences
        if (sdp_get_access_protos(rec, &proto_list) == 0)
        {
            sdp_list_t *p = proto_list;

            // go through each protocol sequence
            for (; p; p = p->next)
            {
                sdp_list_t *pds = (sdp_list_t*)p->data;

                // go through each protocol list of the protocol sequence
                for (; pds; pds = pds->next)
                {

                    // check the protocol attributes
                    sdp_data_t *d = (sdp_data_t*)pds->data;
                    int proto = 0;

                    for (; d; d = d->next)
                    {
                        switch (d->dtd)
                        {
                        case SDP_UUID16:
                        case SDP_UUID32:
                        case SDP_UUID128:
                            proto = sdp_uuid_to_proto(&d->val.uuid);
                            break;
                        case SDP_UINT8:
                            if (proto == RFCOMM_UUID)
                            {
                                channelID = d->val.int8;
                                // TODO: cleanup
                                return channelID; // stop if channel is found
                            }
                            break;
                        }
                    }
                }

                sdp_list_free((sdp_list_t*)p->data, 0);
            }

            sdp_list_free(proto_list, 0);
        }

        sdp_record_free(rec);
    }

    sdp_close(session);
    return channelID;
}


std::string Device::Data::getDeviceClass()
{
    return getDeviceClass( device_class );
}

std::string Device::Data::getMajorDeviceClass()
{
    return getDeviceClass( major_device_class );
}
