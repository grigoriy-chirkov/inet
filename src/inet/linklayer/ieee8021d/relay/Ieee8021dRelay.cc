// Copyright (C) 2013 OpenSim Ltd.
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
//
// Author: Benjamin Martin Seregi

#include "inet/common/LayeredProtocolBase.h"
#include "inet/common/ModuleAccess.h"
#include "inet/common/IProtocolRegistrationListener.h"
#include "inet/common/ProtocolTag_m.h"
#include "inet/linklayer/common/Ieee802Ctrl.h"
#include "inet/linklayer/common/InterfaceTag_m.h"
#include "inet/linklayer/common/MACAddressTag_m.h"
#include "inet/linklayer/configurator/Ieee8021dInterfaceData.h"
#include "inet/linklayer/ethernet/EtherEncap.h"
#include "inet/linklayer/ieee8021d/relay/Ieee8021dRelay.h"
#include "inet/networklayer/common/InterfaceEntry.h"

namespace inet {

Define_Module(Ieee8021dRelay);

Ieee8021dRelay::Ieee8021dRelay()
{
}

void Ieee8021dRelay::initialize(int stage)
{
    if (stage == INITSTAGE_LOCAL) {
        // statistics
        numDispatchedBDPUFrames = numDispatchedNonBPDUFrames = numDeliveredBDPUsToSTP = 0;
        numReceivedBPDUsFromSTP = numReceivedNetworkFrames = numDroppedFrames = 0;
    }
    else if (stage == INITSTAGE_LINK_LAYER) {
        registerProtocol(Protocol::ethernet, gate("ifOut"));
    }
    else if (stage == INITSTAGE_LINK_LAYER_2) {
        NodeStatus *nodeStatus = dynamic_cast<NodeStatus *>(findContainingNode(this)->getSubmodule("status"));
        isOperational = (!nodeStatus) || nodeStatus->getState() == NodeStatus::UP;

        macTable = getModuleFromPar<IMACAddressTable>(par("macTableModule"), this);
        ifTable = getModuleFromPar<IInterfaceTable>(par("interfaceTableModule"), this);

        if (isOperational) {
            ie = chooseInterface();

            if (ie)
                bridgeAddress = ie->getMacAddress(); // get the bridge's MAC address
            else
                throw cRuntimeError("No non-loopback interface found!");
        }

        isStpAware = gate("stpIn")->isConnected();    // if the stpIn is not connected then the switch is STP/RSTP unaware

        WATCH(bridgeAddress);
        WATCH(numReceivedNetworkFrames);
        WATCH(numDroppedFrames);
        WATCH(numReceivedBPDUsFromSTP);
        WATCH(numDeliveredBDPUsToSTP);
        WATCH(numDispatchedNonBPDUFrames);
    }
}

void Ieee8021dRelay::handleMessage(cMessage *msg)
{
    if (!isOperational) {
        EV_ERROR << "Message '" << msg << "' arrived when module status is down, dropped it." << endl;
        delete msg;
        return;
    }

    if (!msg->isSelfMessage()) {
        // messages from STP process
        Packet *frame = check_and_cast<Packet *>(msg);
        if (msg->arrivedOn("stpIn")) {
            numReceivedBPDUsFromSTP++;
            EV_INFO << "Received " << msg << " from STP/RSTP module." << endl;
            dispatchBPDU(frame);
        }
        // messages from network
        else if (msg->arrivedOn("ifIn")) {
            numReceivedNetworkFrames++;
            EV_INFO << "Received " << msg << " from network." << endl;
            delete frame->removeTag<DispatchProtocolReq>();
            emit(LayeredProtocolBase::packetReceivedFromLowerSignal, frame);
            handleAndDispatchFrame(frame);
        }
    }
    else
        throw cRuntimeError("This module doesn't handle self-messages!");
}

bool Ieee8021dRelay::isForwardingInterface(InterfaceEntry *ie)
{
    if (isStpAware) {
        if (!ie->ieee8021dData())
            throw cRuntimeError("Ieee8021dInterfaceData not found for interface %s", ie->getFullName());
        return ie->ieee8021dData()->isForwarding();
    }
    return true;
}

void Ieee8021dRelay::broadcast(Packet *packet)
{
    EV_DETAIL << "Broadcast frame " << packet << endl;

    int inputInterfacceId = packet->getMandatoryTag<InterfaceInd>()->getInterfaceId();

    int numPorts = ifTable->getNumInterfaces();
    for (int i = 0; i < numPorts; i++) {
        InterfaceEntry *ie = ifTable->getInterface(i);
        if (ie->isLoopback() || !ie->isBroadcast())
            continue;
        if (ie->getInterfaceId() != inputInterfacceId && isForwardingInterface(ie)) {
            dispatch(packet->dup(), ie);
        }
    }
    delete packet;
}

void Ieee8021dRelay::handleAndDispatchFrame(Packet *packet)
{
    const auto& frame = packet->peekHeader<EtherFrame>();
    int arrivalInterfaceId = packet->getMandatoryTag<InterfaceInd>()->getInterfaceId();
    InterfaceEntry *arrivalInterface = ifTable->getInterfaceById(arrivalInterfaceId);
    Ieee8021dInterfaceData *arrivalPortData = arrivalInterface->ieee8021dData();
    if (isStpAware && arrivalPortData == nullptr)
        throw cRuntimeError("Ieee8021dInterfaceData not found for interface %s", arrivalInterface->getFullName());
    learn(frame->getSrc(), arrivalInterfaceId);

    // BPDU Handling
    if (isStpAware && (frame->getDest() == MACAddress::STP_MULTICAST_ADDRESS || frame->getDest() == bridgeAddress) && arrivalPortData->getRole() != Ieee8021dInterfaceData::DISABLED) {
        EV_DETAIL << "Deliver BPDU to the STP/RSTP module" << endl;
        deliverBPDU(packet);    // deliver to the STP/RSTP module
    }
    else if (isStpAware && !arrivalPortData->isForwarding()) {
        EV_INFO << "The arrival port is not forwarding! Discarding it!" << endl;
        numDroppedFrames++;
        delete packet;
    }
    else if (frame->getDest().isBroadcast()) {    // broadcast address
        broadcast(packet);
    }
    else {
        int outputInterfaceId = macTable->getPortForAddress(frame->getDest());
        // Not known -> broadcast
        if (outputInterfaceId == -1) {
            EV_DETAIL << "Destination address = " << frame->getDest() << " unknown, broadcasting frame " << frame << endl;
            broadcast(packet);
        }
        else {
            InterfaceEntry *outputInterface = ifTable->getInterfaceById(outputInterfaceId);
            if (outputInterfaceId != arrivalInterfaceId) {
                if (isForwardingInterface(outputInterface))
                    dispatch(packet, outputInterface);
                else {
                    EV_INFO << "Output interface " << outputInterface->getFullName() << " is not forwarding. Discarding!" << endl;
                    numDroppedFrames++;
                    delete packet;
                }
            }
            else {
                EV_DETAIL << "Output port is same as input port, " << packet->getFullName() << " destination = " << frame->getDest() << ", discarding frame " << frame << endl;
                numDroppedFrames++;
                delete packet;
            }
        }
    }
}

void Ieee8021dRelay::dispatch(Packet *packet, InterfaceEntry *ie)
{
    const auto& frame = packet->peekHeader<EtherFrame>();
    EV_INFO << "Sending frame " << packet << " on output interface " << ie->getFullName() << " with destination = " << frame->getDest() << endl;

    numDispatchedNonBPDUFrames++;
    packet->ensureTag<InterfaceReq>()->setInterfaceId(ie->getInterfaceId());
    emit(LayeredProtocolBase::packetSentToLowerSignal, packet);
    send(packet, "ifOut");
}

void Ieee8021dRelay::learn(MACAddress srcAddr, int arrivalInterfaceId)
{
    Ieee8021dInterfaceData *port = getPortInterfaceData(arrivalInterfaceId);

    if (!isStpAware || port->isLearning())
        macTable->updateTableWithAddress(arrivalInterfaceId, srcAddr);
}

void Ieee8021dRelay::dispatchBPDU(Packet *packet)
{
    const auto& bpdu = packet->peekHeader<BPDU>();
    (void)bpdu;       // unused variable
    unsigned int portNum = packet->getMandatoryTag<InterfaceReq>()->getInterfaceId();
    MACAddress address = packet->getMandatoryTag<MacAddressReq>()->getDestAddress();

    if (ifTable->getInterfaceById(portNum) == nullptr)
        throw cRuntimeError("Output port %d doesn't exist!", portNum);

    // TODO: use LLCFrame
    const auto& frame = std::make_shared<EthernetIIFrame>();
    packet->setKind(packet->getKind());
    frame->setSrc(bridgeAddress);
    frame->setDest(address);
    frame->setEtherType(-1);
    frame->markImmutable();
    packet->pushHeader(frame);

    EtherEncap::addPaddingAndFcs(packet);

    EV_INFO << "Sending BPDU frame " << packet << " with destination = " << frame->getDest() << ", port = " << portNum << endl;
    numDispatchedBDPUFrames++;
    emit(LayeredProtocolBase::packetSentToLowerSignal, packet);
    send(packet, "ifOut");
}

void Ieee8021dRelay::deliverBPDU(Packet *packet)
{
    const auto& eth = EtherEncap::decapsulate(packet);

    const auto& bpdu = packet->peekHeader<BPDU>();

    auto macAddressTag = packet->ensureTag<MacAddressInd>();
    macAddressTag->setSrcAddress(eth->getSrc());
    macAddressTag->setDestAddress(eth->getDest());

    EV_INFO << "Sending BPDU frame " << bpdu << " to the STP/RSTP module" << endl;
    numDeliveredBDPUsToSTP++;
    send(packet, "stpOut");
}

Ieee8021dInterfaceData *Ieee8021dRelay::getPortInterfaceData(unsigned int interfaceId)
{
    if (isStpAware) {
        InterfaceEntry *gateIfEntry = ifTable->getInterfaceById(interfaceId);
        Ieee8021dInterfaceData *portData = gateIfEntry ? gateIfEntry->ieee8021dData() : nullptr;

        if (!portData)
            throw cRuntimeError("Ieee8021dInterfaceData not found for port = %d", interfaceId);

        return portData;
    }
    return nullptr;
}

void Ieee8021dRelay::start()
{
    isOperational = true;

    ie = chooseInterface();
    if (ie)
        bridgeAddress = ie->getMacAddress(); // get the bridge's MAC address
    else
        throw cRuntimeError("No non-loopback interface found!");

    macTable->clearTable();
}

void Ieee8021dRelay::stop()
{
    isOperational = false;

    macTable->clearTable();
    ie = nullptr;
}

InterfaceEntry *Ieee8021dRelay::chooseInterface()
{
    // TODO: Currently, we assume that the first non-loopback interface is an Ethernet interface
    //       since relays work on EtherSwitches.
    //       NOTE that, we don't check if the returning interface is an Ethernet interface!
    for (int i = 0; i < ifTable->getNumInterfaces(); i++) {
        InterfaceEntry *current = ifTable->getInterface(i);
        if (!current->isLoopback())
            return current;
    }

    return nullptr;
}

void Ieee8021dRelay::finish()
{
    recordScalar("number of received BPDUs from STP module", numReceivedBPDUsFromSTP);
    recordScalar("number of received frames from network (including BPDUs)", numReceivedNetworkFrames);
    recordScalar("number of dropped frames (including BPDUs)", numDroppedFrames);
    recordScalar("number of delivered BPDUs to the STP module", numDeliveredBDPUsToSTP);
    recordScalar("number of dispatched BPDU frames to the network", numDispatchedBDPUFrames);
    recordScalar("number of dispatched non-BDPU frames to the network", numDispatchedNonBPDUFrames);
}

bool Ieee8021dRelay::handleOperationStage(LifecycleOperation *operation, int stage, IDoneCallback *doneCallback)
{
    Enter_Method_Silent();

    if (dynamic_cast<NodeStartOperation *>(operation)) {
        if ((NodeStartOperation::Stage)stage == NodeStartOperation::STAGE_LINK_LAYER) {
            start();
        }
    }
    else if (dynamic_cast<NodeShutdownOperation *>(operation)) {
        if ((NodeShutdownOperation::Stage)stage == NodeShutdownOperation::STAGE_LINK_LAYER) {
            stop();
        }
    }
    else if (dynamic_cast<NodeCrashOperation *>(operation)) {
        if ((NodeCrashOperation::Stage)stage == NodeCrashOperation::STAGE_CRASH) {
            stop();
        }
    }
    else {
        throw cRuntimeError("Unsupported operation '%s'", operation->getClassName());
    }

    return true;
}

} // namespace inet

