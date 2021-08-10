// Dark Souls 3 - Open Server

#pragma once

#include <vector>
#include <memory>

#include "Server/Streams/Frpg2UdpPacket.h"

class Cipher;
class NetConnection;

class Frpg2UdpPacketStream
{
public:
    Frpg2UdpPacketStream(std::shared_ptr<NetConnection> Connection, const std::vector<uint8_t>& CwcKey, uint64_t AuthToken);

    // Returns true if send was successful, if false is returned the send queue
    // is likely saturated or the packet is invalid.
    bool Send(const Frpg2UdpPacket& Packet);

    // Returns true if a packet was recieved and stores packet in OutputPacket.
    bool Recieve(Frpg2UdpPacket* Packet);

    // Sends and recieves packets in this stream. If this function
    // returns true the stream is considered to be in an error state
    // and the client this stream goes to should be disconnected.
    virtual bool Pump();

protected:

    bool BytesToPacket(const std::vector<uint8_t>& Buffer, Frpg2UdpPacket& Packet);
    bool PacketToBytes(const Frpg2UdpPacket& Packet, std::vector<uint8_t>& Buffer);

protected:

    std::shared_ptr<NetConnection> Connection;

    std::vector<uint8_t> CwcKey;
    uint64_t AuthToken;

private:
    
    bool InErrorState = false;

    std::vector<Frpg2UdpPacket> RecieveQueue;

    std::vector<uint8_t> RecieveBuffer;

    std::shared_ptr<Cipher> EncryptionCipher;
    std::shared_ptr<Cipher> DecryptionCipher;
};